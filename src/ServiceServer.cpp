#include <random>
#include "ServiceServer.h"

#define FILE_NAME "user.db"

int ScoreServer::CreateTable(sqlite3* db)
{
    const char * CREATE_TABLE = "CREATE TABLE IF NOT EXISTS User("
                                "Name TEXT PRIMARY KEY NOT NULL, "
                                "Password TEXT NOT NULL, "
                                "Api TEXT NOT NULL, "
                                "TotalScore INT NOT NULL DEFAULT 0 "
                                ");";
    char* errorMessage = nullptr;
    int exit = sqlite3_exec(db, CREATE_TABLE, nullptr, 0, &errorMessage);

    if (exit != SQLITE_OK) {
        std::cerr << "CreateTable - Error creating table: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
        return exit;
    }
    return SQLITE_OK;
}

int ScoreServer::InsertData(sqlite3* db,  std::string username, std::string password, std::string api)
{
    const char* INSERT_DATA = "INSERT INTO User (Name, Password, Api) "
                              "VALUES($name, $password, $api);";
    sqlite3_stmt* stmt;
    // Prepare the SQL statement
    int rc = sqlite3_prepare_v2(db, INSERT_DATA, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "InsertData - Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return rc;
    }

    sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, "$name"), username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, "$password"), password.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, "$api"), api.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "InsertData - Error updating data: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return rc;
    }

    // Finalize the statement
    sqlite3_finalize(stmt);
    return SQLITE_OK;
}

int ScoreServer::UpdateData(sqlite3* db, std::string username)
{
    const char* UPDATE_DATA = "UPDATE User SET "
                              "TotalScore = $totalScore "
                              "WHERE Name = $name;";
    sqlite3_stmt* stmt;
    // Prepare the SQL statement
    int rc = sqlite3_prepare_v2(db, UPDATE_DATA, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "UpdateData - Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return rc;
    }

    UserInfo user = _mapNameUserInfo[username];
    sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, "$totalScore"), user.totalScore);
    sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, "$name"), username.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "UpdateData - Error updating data: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return rc;
    }

    sqlite3_finalize(stmt);
    return SQLITE_OK;
}

int ScoreServer::UpdateTotalScore(sqlite3* db, std::string username)
{
    const char* SELECT_DATA = "SELECT TotalScore FROM User WHERE Name = $name;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, SELECT_DATA, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "UpdateTotalScore - Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return rc;
    }

    sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, "$name"), username.c_str(), -1, SQLITE_STATIC);
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        _totalScore = sqlite3_column_int(stmt, 0);
    }

    if (rc != SQLITE_DONE) {
        std::cerr << "UpdateTotalScore - Error while fetching data: " << sqlite3_errmsg(db) << std::endl;
    }
    // Finalize the statement
    sqlite3_finalize(stmt);

    return SQLITE_OK;
}

bool ScoreServer::IsUserNameAvailable(sqlite3* db, std::string username)
{
    const char* sqlSelect = "SELECT COUNT(*) FROM User WHERE Name = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sqlSelect, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "IsUserNameAvailable - Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Bind the name parameter to the SQL query
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    // Execute the query and get the result
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, 0);  // Get the COUNT(*) result
        sqlite3_finalize(stmt);

        // If count > 0, the member exists
        return count > 0;
    }

    sqlite3_finalize(stmt);
    return false;
}

bool ScoreServer::IsValidUser(sqlite3* db, std::string username, std::string password)
{
    const char* sqlSelect = "SELECT COUNT(*) FROM User WHERE Name = ? AND Password = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sqlSelect, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "IsValidUser - Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    // Execute the query and get the result
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, 0);  // Get the COUNT(*) result
        sqlite3_finalize(stmt);

        // If count > 0, the member exists
        return count > 0;
    }

    sqlite3_finalize(stmt);
    return false;
}

::grpc::Status ScoreServer::Login(::grpc::ServerContext* context, const ::LoginRequest* request, ::LoginResponse* response)
{
    sqlite3* db;
    int isSuccess = sqlite3_open(FILE_NAME, &db);
    if (isSuccess) {
        std::cerr << "Login - Error opening DB: " << sqlite3_errmsg(db) << std::endl;
    }
    isSuccess = CreateTable(db);
    if (isSuccess == SQLITE_OK)
    {
        std::string api = request->username() + request->password() + "2048";
        if (IsUserNameAvailable(db, request->username()))
        {
            if (IsValidUser(db, request->username(), request->password()))
            {
                isSuccess == SQLITE_OK;
                if (UpdateTotalScore(db, request->username()) != SQLITE_OK)
                {
                    _totalScore = 0;
                }
            }
            else
            {
                isSuccess = SQLITE_ERROR;
            }
        }
        else
        {
            isSuccess = InsertData(db, request->username(), request->password(), api);
            _totalScore = 0;
        }
        if (isSuccess == SQLITE_OK)
        {
            response->set_valid(true);
            response->set_name(request->username());
            response->set_api(api);
            response->set_total_score(_totalScore);
        }
        else
        {
            response->set_valid(false);
        }
    }
    else
    {
        response->set_valid(false);
    }
    sqlite3_close(db);
    return grpc::Status::OK;
}

bool ScoreServer::ValidRequest(sqlite3* db, std::string name, std::string api)
{
    const char* sqlSelect = "SELECT Api FROM User WHERE Name = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sqlSelect, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "ValidRequest - Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Bind the name parameter to the SQL query
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);

    // Execute the query and get the result
    std::string apiFromUser = "";
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        const unsigned char* apiText = sqlite3_column_text(stmt, 0);  // Get the COUNT(*) result
        if (apiText != nullptr) {
            apiFromUser = reinterpret_cast<const char*>(apiText);
        }
        if (api == apiFromUser)
        {
            sqlite3_finalize(stmt);
            return true;
        }
    }
    else
    {
        std::cerr << "No user found with the name: " << name << std::endl;
    }

    sqlite3_finalize(stmt);
    return false;
}

::grpc::Status ScoreServer::UpdateScore(::grpc::ServerContext* context, const ::ScoreRequest* request, ::ScoreResponse* response)
{
    sqlite3* db;
    int isSuccess = sqlite3_open(FILE_NAME, &db);
    if (isSuccess) {
        std::cerr << "UpdateScore - Error opening DB: " << sqlite3_errmsg(db) << std::endl;
    }
    if (ValidRequest(db, request->name(), request->api()))
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
            userInfo.totalScore =_totalScore;
            _mapNameUserInfo.insert(std::pair{request->name(), userInfo});
            _mapNameScore.insert(std::pair{request->name(), _totalScore});
            score += GenerateRandom(_mapNameUserInfo[request->name()].tiles);
        }

        _mapNameUserInfo[request->name()].score += score;
        _mapNameUserInfo[request->name()].totalScore += score;
        _mapNameScore[request->name()] = _mapNameUserInfo[request->name()].totalScore;
        (void)UpdateData(db, request->name());

        response->set_score(_mapNameUserInfo[request->name()].score);
        response->set_total_score(_mapNameUserInfo[request->name()].totalScore);
        for (const auto& row : _mapNameUserInfo[request->name()].tiles) {
            for (const auto& tile : row) {
                response->add_values(tile.GetValue());
            }
        }
    }
    else
    {
        response->set_score(0);
        response->set_total_score(0);
        for (int i = 0; i < 16; ++i) {
            response->add_values(0);
        }
    }

    sqlite3_close(db);
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