#include <random>
#include "ServiceServer.h"

::grpc::Status ScoreServer::UpdateScore(::grpc::ServerContext* context, const ::ScoreRequest* request, ::ScoreReply* response)
{
    int32_t score = 0;
    if (auto search = _mapNameUserInfo.find(request->name()); search != _mapNameUserInfo.end())
    {
        if (request->reset())
        {
            ResetTiles(_mapNameUserInfo[request->name()].tiles);
            _mapNameUserInfo[request->name()].score = 0;
            score += GenerateRandom(_mapNameUserInfo[request->name()].tiles);
        }
        else
        {
            score += Move(request->key(), _mapNameUserInfo[request->name()].tiles);
        }
    }
    else
    {
        UserInfo userInfo;
        _mapNameUserInfo.insert(std::pair{request->name(), userInfo});
        _mapNameScore.insert(std::pair{request->name(), 0});
        score += GenerateRandom(_mapNameUserInfo[request->name()].tiles);
    }
    _mapNameUserInfo[request->name()].score += score;
    _mapNameUserInfo[request->name()].totalScore += score;    
    _mapNameScore[request->name()] = _mapNameUserInfo[request->name()].totalScore;

    response->set_score(_mapNameUserInfo[request->name()].score);
    response->set_total_score(_mapNameUserInfo[request->name()].totalScore);

    for (const auto& row : _mapNameUserInfo[request->name()].tiles) {
        for (const auto& tile : row) {
            response->add_values(tile.GetValue());
        }
    }
    return grpc::Status::OK;
}

const std::unordered_map<std::string, int32_t> ScoreServer::GetScore() const
{
    return _mapNameScore;
}

void ScoreServer::ResetTiles(std::array<std::array<Tile, COLS>, ROWS>& tiles)
{
    for (auto& row : tiles) {
        for (auto& tile : row) {
            tile.SetValue(0);
        }
    }
}

int32_t ScoreServer::Move(int32_t key, std::array<std::array<Tile, COLS>, ROWS>& tiles)
{
    int32_t score = 0;
    if (key == static_cast<int32_t>(ImGuiKey_LeftArrow))
    {
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 2; ++j)
            {
                ShiftTile(tiles[i][0], tiles[i][1], tiles[i][2], tiles[i][3]);
                score += MergeTile(tiles[i][0], tiles[i][1], tiles[i][2], tiles[i][3]);
            }
        }
        score += GenerateRandom(tiles);
    } 
    else if (key == static_cast<int32_t>(ImGuiKey_RightArrow))
    {
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 2; ++j)
            {
                ShiftTile(tiles[i][3], tiles[i][2], tiles[i][1], tiles[i][0]);
                score += MergeTile(tiles[i][3], tiles[i][2], tiles[i][1], tiles[i][0]);
            }
        }
        score += GenerateRandom(tiles);
    }
    else if (key == static_cast<int32_t>(ImGuiKey_UpArrow))
    {
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 2; ++j)
            {
                ShiftTile(tiles[0][i], tiles[1][i], tiles[2][i], tiles[3][i]);
                score += MergeTile(tiles[0][i], tiles[1][i], tiles[2][i], tiles[3][i]);
            }
        }
        score += GenerateRandom(tiles);
    }
    else if (key = static_cast<int32_t>(ImGuiKey_DownArrow))
    {
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 2; ++j)
            {
                ShiftTile(tiles[3][i], tiles[2][i], tiles[1][i], tiles[0][i]);
                score += MergeTile(tiles[3][i], tiles[2][i], tiles[1][i], tiles[0][i]);
            }
        }
        score += GenerateRandom(tiles);
    }
    return score;
}

int32_t ScoreServer::GenerateRandom(std::array<std::array<Tile, COLS>, ROWS>& tiles)
{
    int32_t score = 0;
    // Create a random device to seed the random number generator
    std::random_device rd;
    // Create a Mersenne Twister random number generator
    std::mt19937 gen(rd());
    // Create a distribution in the range [0, 3]
    std::uniform_int_distribution<> distrib(0, 3);

    int generateTimes = 0;
    for (const auto& row : tiles) {
        for (const auto& tile : row) {
            if (tile.GetValue() == 0)
            {
                ++generateTimes;
            }
        }
    }
    generateTimes = generateTimes >= 2 ? 2 : generateTimes;
    while (generateTimes != 0)
    {
        int row = distrib(gen);
        int col = distrib(gen);
        if (tiles[row][col].GetValue() == 0)
        {
            tiles[row][col].SetValue(2);
            score += 2;
            --generateTimes;
        }
    }
    return score;
}

void ScoreServer::ShiftTile(Tile& first, Tile& second, Tile& third, Tile& forth)
{
    std::array<int32_t, 4> tiles = {first.GetValue(), second.GetValue(), third.GetValue(), forth.GetValue()};
    for (int i = 0; i < 3; ++i)
    {
        if (tiles[i] == 0)
        {
            for (int j = i + 1; j < 4; ++j)
            {
                if (tiles[j] != 0)
                {
                    tiles[i] = tiles[j];
                    tiles[j] = 0;
                    break;
                }
                else
                {
                    continue;
                }
            }
        }
    }
    first.SetValue(tiles[0]);
    second.SetValue(tiles[1]);
    third.SetValue(tiles[2]);
    forth.SetValue(tiles[3]);
}

int32_t ScoreServer::MergeTile(Tile& first, Tile& second, Tile& third, Tile& forth)
{
    int32_t score = 0;
    std::array<int32_t, 4> tiles = {first.GetValue(), second.GetValue(), third.GetValue(), forth.GetValue()};
    for (int i = 0; i < 3; ++i)
    {
        if (tiles[i] == tiles[i+1])
        {
            tiles[i] += tiles[i+1];
            score += tiles[i]; 
            tiles[i+1] = 0;
        }
    }
    first.SetValue(tiles[0]);
    second.SetValue(tiles[1]);
    third.SetValue(tiles[2]);
    forth.SetValue(tiles[3]);
    return score;
}