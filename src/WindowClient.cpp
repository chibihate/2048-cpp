#include "Logger.h"
#include "WindowClient.h"

WindowClient::WindowClient()
    : _client(grpc::CreateChannel("0.0.0.0:9999", grpc::InsecureChannelCredentials()))
    , _screenPage()
    , _username()
    , _password()
    , _api()
    , _totalScore(0)
    , _defaultWindowFlags(0)
{
    for (int row = 0; row < ROWS; ++row)
    {
        for (int col = 0; col < COLS; ++col)
        {
            _tiles[row][col] = Tile(row,col);
        }
    }
}

void WindowClient::InitGRPC()
{
    _defaultWindowFlags |= ImGuiWindowFlags_NoTitleBar;
    _defaultWindowFlags |= ImGuiWindowFlags_NoMove;
    _defaultWindowFlags |= ImGuiWindowFlags_NoResize;
}

void WindowClient::HandleScreen()
{
    switch (_screenPage)
    {
    case LOGIN:
        LoginPage();
        break;
    case GAME:
        GamePage();
        break;
    default:
        break;
    }
}

void WindowClient::LoginPage()
{
    static LoginInfo loginInfo;
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, BACKGROUND_COLOR);
    ImGui::Begin("##Login", NULL, _defaultWindowFlags);

    float windowWidth = ImGui::GetWindowSize().x;
    float windowHeight = ImGui::GetWindowSize().y;
    const uint32_t TEXT_WIDTH = 200;

    ImGui::SetNextItemWidth(TEXT_WIDTH);
    ImGui::SetCursorPosX((windowWidth - TEXT_WIDTH) * 0.5f);
    ImGui::SetCursorPosY((windowHeight - 100) * 0.5f);
    ImGui::InputTextWithHint("##username", "Username", _username, IM_ARRAYSIZE(_username));

    ImGui::SetNextItemWidth(TEXT_WIDTH);
    ImGui::SetCursorPosX((windowWidth - TEXT_WIDTH) * 0.5f);
    ImGui::InputTextWithHint("##password", "Password", _password, IM_ARRAYSIZE(_password), ImGuiInputTextFlags_Password);

    ImVec2 button_size = ImVec2(50, 20);
    ImGui::SetCursorPosX((windowWidth - 50) * 0.5f);
    if (ImGui::Button("Login", button_size))
    {
        loginInfo = _client.LoginGame(_username, _password);
        if (loginInfo.valid)
        {
            _api = loginInfo.api;
            _totalScore = loginInfo.totalScore;
            _screenPage = GAME;
        }
    }
    if (!loginInfo.valid)
    {
        ImGui::SetCursorPosX((windowWidth - TEXT_WIDTH) * 0.5f);
        ImGui::Text("Password is incorrect");
    }

    ImGui::End();
    ImGui::PopStyleColor();
}

void WindowClient::DrawGrid(ImDrawList* drawList)
{
    for (int32_t row = 1; row < ROWS; ++row)
    {
        int32_t y = row * RECT_WIDTH;
        drawList->AddRect(ImVec2(0,y),ImVec2(GAME_WIDTH,y), OUTLINE_COLOR, 0, 0, 5);
    }
    for (int32_t col = 1; col < COLS; ++col)
    {
        int32_t x = col * RECT_HEIGHT;
        drawList->AddRect(ImVec2(x,0),ImVec2(x,GAME_HEIGHT), OUTLINE_COLOR, 0, 0, 5);
    }
    drawList->AddRect(ImVec2(0,0),ImVec2(GAME_WIDTH,GAME_HEIGHT), OUTLINE_COLOR, 0, 0, 10);
}

bool WindowClient::IsGameOver()
{
    bool isGameOver = true;
    for (int i = 0; i < 4; ++i)
    {
        if (isGameOver)
        {
            for (int j = 0; j < 3; ++j)
            {
                if (_tiles[i][j].GetValue() == _tiles[i][j+1].GetValue())
                {
                    isGameOver = false;
                    break;
                }
                if (_tiles[j][i].GetValue() == _tiles[j+1][i].GetValue())
                {
                    isGameOver = false;
                    break;
                }
            }
        }
    }
    return isGameOver;
}

void WindowClient::GamePage()
{
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(GAME_WIDTH, GAME_HEIGHT));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, BACKGROUND_COLOR);
    ImGui::Begin("##Game", NULL, _defaultWindowFlags);
    ImGui::SetWindowFontScale(2.0f);
    uint16_t countOfTile = 0;
    static bool triggerNotification = false;
    static bool reset = false;
    static int32_t score = 0;
    static int32_t totalScore = _totalScore;
    static ScoreInfo scoreInfo(_totalScore);

    if (!triggerNotification)
    {
        if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow, false))
        {
            scoreInfo = _client.UpdateScore(_username, _api, ImGuiKey_LeftArrow, reset);
            reset = false;
        }
        if (ImGui::IsKeyPressed(ImGuiKey_RightArrow, false))
        {
            scoreInfo = _client.UpdateScore(_username, _api, ImGuiKey_RightArrow, reset);
            reset = false;
        }
        if (ImGui::IsKeyPressed(ImGuiKey_UpArrow, false))
        {
            scoreInfo = _client.UpdateScore(_username, _api, ImGuiKey_UpArrow, reset);
            reset = false;
        }
        if (ImGui::IsKeyPressed(ImGuiKey_DownArrow, false))
        {
            scoreInfo = _client.UpdateScore(_username, _api, ImGuiKey_DownArrow, reset);
            reset = false;
        }
    }
    if (totalScore != scoreInfo.totalScore)
    {
        score = scoreInfo.score;
        totalScore = scoreInfo.totalScore;
        uint16_t indexOfTile = 0;
        for (auto& row : _tiles) {
            for (auto& tile : row) {
                tile.SetValue(scoreInfo.tiles[indexOfTile]);
                ++indexOfTile;
            }
        }
    }

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    for (auto& row : _tiles) {
        for (auto& tile : row) {
            tile.Draw(drawList);
            if (tile.GetValue() != 0)
            {
                ++countOfTile;
            }
        }
    }
    DrawGrid(drawList);

    if (countOfTile == 16 && IsGameOver())
    {
        triggerNotification = true;
    }

    if (triggerNotification) {
        ImGui::OpenPopup("##GameOver");
        float buttonWidth = ImGui::CalcTextSize("Game over").x;
        float buttonHeight = ImGui::CalcTextSize("Game over").y;
        ImGui::SetNextWindowPos(ImVec2((GAME_WIDTH - buttonWidth) * 0.5f, (GAME_HEIGHT - buttonHeight*2) * 0.5f));
        if (ImGui::BeginPopup("##GameOver")) {
            ImGui::Text("Game over");
            float windowWidth = ImGui::GetWindowSize().x;
            buttonWidth = ImGui::CalcTextSize("OK").x + ImGui::GetStyle().FramePadding.x * 2.0f;
            ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);
            if (ImGui::Button("OK")) {
                triggerNotification = false;
                reset = true;
                score = 0;
                ResetTiles();
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }
    ImGui::End();
    ImGui::PopStyleColor();

    ImGui::SetNextWindowPos(ImVec2(0, GAME_HEIGHT));
    ImGui::SetNextWindowSize(ImVec2(GAME_WIDTH, HEIGHT_TITLE_GAME));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, BACKGROUND_COLOR);
    ImGui::Begin("##Title game", NULL, _defaultWindowFlags);
    ImGui::Text("Score: %d", score);
    ImGui::Text("Total score: %d", totalScore);
    ImGui::End();
    ImGui::PopStyleColor();
}

void WindowClient::ResetTiles()
{
    for (auto& row : _tiles) {
        for (auto& tile : row) {
            tile.SetValue(0);
        }
    }
}