#include"Window.h"

#include "Application.h"
#include "Logger.h"

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <random>

Window::Window()
: windowGL(nullptr)
{
}

Window::~Window()
{
}

bool Window::Init(const ApplicationConfiguration& config) {
    if (!glfwInit()) {
        LOG_CRITICAL("GLFW init failed");
        return false;
    }

    // Setup context for Opengl 4.3 Core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    windowGL = glfwCreateWindow(config.width, config.height, config.title, nullptr, nullptr);
    if (!windowGL) {
        LOG_CRITICAL("Window created failed");
        glfwTerminate();
        return false;
    }
    LOG_INFO("Window created success");

    glfwMakeContextCurrent(windowGL);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(windowGL, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        LOG_CRITICAL("Glad loaded failed");
        glfwTerminate();
        return false;
    }
    LOG_INFO("Glad loaded success");

    return true;
}

void Window::Shutdown() {
    LOG_INFO("Window closed");
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}

void Window::DearImGUI() {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    HandleScreen();

    // Rendering
    ImGui::Render();
    // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Window::Swapbuffers() {
    glfwSwapBuffers(windowGL);
}

void Window::PollsEvent() {
    glfwPollEvents();
}

bool Window::ShouldClose() {
    return glfwWindowShouldClose(windowGL);
}

void WindowServer::InitGRPC()
{
    LOG_INFO("Init gRPC server");
    _builder.AddListeningPort("0.0.0.0:9999", grpc::InsecureServerCredentials());
    _builder.RegisterService(&_scoreServer);
    _server = _builder.BuildAndStart();
}

void WindowServer::HandleScreen()
{
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::Begin("##2048", NULL, window_flags);

    if (!_scoreServer.GetScore().empty())
    {
        auto memberMap = _scoreServer.GetScore();
        // Create a vector of pairs to store the map's elements
        std::vector<std::pair<std::string, int>> memberVec(memberMap.begin(), memberMap.end());

        // Sort the vector by value in descending order
        std::sort(memberVec.begin(), memberVec.end(),
            [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
                return a.second > b.second; // Sort by value (second), descending order
        });

        if (ImGui::BeginTable("Rank",2))
        {
            ImGui::TableSetupColumn("Name");
            ImGui::TableSetupColumn("Score");
            ImGui::TableHeadersRow();
            for (const auto& member: memberVec)
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                auto name = member.first.c_str();
                ImGui::Text(name);
                ImGui::TableNextColumn();
                auto score = std::to_string(member.second).c_str();
                ImGui::Text(score);
            }
            ImGui::EndTable();
        }
    }

    if (ImGui::IsKeyPressed(ImGuiKey_Escape))
    {
        glfwSetWindowShouldClose(windowGL, true);
    }
    ImGui::End();
}

WindowClient::WindowClient()
    : _client(grpc::CreateChannel("0.0.0.0:9999", grpc::InsecureChannelCredentials()))
    , _screenPage()
    , _username()
    , _password()
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

const ImU32 BACKGROUND_COLOR = IM_COL32(205,193,180,255);
const ImU32 OUTLINE_THINKNESS = 10;
const ImU32 OUTLINE_COLOR = IM_COL32(187, 173, 160, 255);
const ImU32 FONT_COLOR = IM_COL32(119, 110, 101, 255);

const ImU32 COLORS[11] = {
    IM_COL32(238, 228, 218, 255),
    IM_COL32(237, 224, 200, 255),
    IM_COL32(242, 177, 121, 255),
    IM_COL32(245, 149, 99, 255),
    IM_COL32(246, 124, 95, 255),
    IM_COL32(246, 94, 59, 255),
    IM_COL32(237, 207, 114, 255),
    IM_COL32(237, 204, 97, 255),
    IM_COL32(237, 200, 80, 255),
    IM_COL32(237, 197, 63, 255),
    IM_COL32(237, 194, 46, 255),
};
const int32_t GAME_WIDTH = 480;
const int32_t GAME_HEIGHT = 480;
const int32_t RECT_WIDTH = GAME_WIDTH / ROWS;
const int32_t RECT_HEIGHT = GAME_HEIGHT / COLS;
const int32_t HEIGHT_TITLE_GAME = 60;

void WindowClient::HandleScreen()
{
    _screenPage = GAME;
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

    if (ImGui::IsKeyPressed(ImGuiKey_Escape))
    {
        glfwSetWindowShouldClose(windowGL, true);
    }
}

void WindowClient::LoginPage()
{
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, BACKGROUND_COLOR);
    ImGui::Begin("##Login", NULL, _defaultWindowFlags);

    auto windowWidth = ImGui::GetWindowSize().x;
    auto windowHeight = ImGui::GetWindowSize().y;
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
        // if ((strcmp(username, "1")==0) && (strcmp(password, "1")==0))
        // {
            LOG_INFO("Success");
            _screenPage = GAME;
        // }
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

void WindowClient::GenerateRandom()
{
    // Create a random device to seed the random number generator
    std::random_device rd;
    // Create a Mersenne Twister random number generator
    std::mt19937 gen(rd());
    // Create a distribution in the range [0, 3]
    std::uniform_int_distribution<> distrib(0, 3);

    int generateTimes = 0;
    for (const auto& row : _tiles) {
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
        if (_tiles[row][col].GetValue() == 0)
        {
            _tiles[row][col].SetValue(2);
            --generateTimes;
        }
    }
}

void WindowClient::ShiftTile(Tile& first, Tile& second, Tile& third, Tile& forth)
{
    std::array<uint16_t, 4> tiles = {first.GetValue(), second.GetValue(), third.GetValue(), forth.GetValue()};
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

void WindowClient::MergeTile(Tile& first, Tile& second, Tile& third, Tile& forth)
{
    std::array<uint16_t, 4> tiles = {first.GetValue(), second.GetValue(), third.GetValue(), forth.GetValue()};
    for (int i = 0; i < 3; ++i)
    {
        if (tiles[i] == tiles[i+1])
        {
            tiles[i] += tiles[i+1];
            tiles[i+1] = 0;
        }
    }
    first.SetValue(tiles[0]);
    second.SetValue(tiles[1]);
    third.SetValue(tiles[2]);
    forth.SetValue(tiles[3]);
}

void WindowClient::GamePage()
{
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(GAME_WIDTH, GAME_HEIGHT));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, BACKGROUND_COLOR);
    ImGui::Begin("##Game", NULL, _defaultWindowFlags);
    ImGui::SetWindowFontScale(2.0f);
    
    if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow, false))
    {
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 2; ++j)
            {
                ShiftTile(_tiles[i][0], _tiles[i][1], _tiles[i][2], _tiles[i][3]);
                MergeTile(_tiles[i][0], _tiles[i][1], _tiles[i][2], _tiles[i][3]);
            }
        }
        GenerateRandom();
    }
    if (ImGui::IsKeyPressed(ImGuiKey_RightArrow, false))
    {
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 2; ++j)
            {
                ShiftTile(_tiles[i][3], _tiles[i][2], _tiles[i][1], _tiles[i][0]);
                MergeTile(_tiles[i][3], _tiles[i][2], _tiles[i][1], _tiles[i][0]);
            }
        }
        GenerateRandom();
    }
    if (ImGui::IsKeyPressed(ImGuiKey_UpArrow, false))
    {
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 2; ++j)
            {
                ShiftTile(_tiles[0][i], _tiles[1][i], _tiles[2][i], _tiles[3][i]);
                MergeTile(_tiles[0][i], _tiles[1][i], _tiles[2][i], _tiles[3][i]);
            }
        }
        GenerateRandom();
    }
    if (ImGui::IsKeyPressed(ImGuiKey_DownArrow, false))
    {
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 2; ++j)
            {
                ShiftTile(_tiles[3][i], _tiles[2][i], _tiles[1][i], _tiles[0][i]);
                MergeTile(_tiles[3][i], _tiles[2][i], _tiles[1][i], _tiles[0][i]);
            }
        }
        GenerateRandom();
    }

    uint16_t totalScore = 0;
    uint16_t countOfTile = 0;
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    for (auto& row : _tiles) {
        for (auto& tile : row) {
            tile.Draw(drawList);
            totalScore += tile.GetValue();
            if (tile.GetValue() != 0)
            {
                ++countOfTile;
            }
        }
    }
    DrawGrid(drawList);
    ImGui::End();
    ImGui::PopStyleColor();

    ImGui::SetNextWindowPos(ImVec2(0, GAME_HEIGHT));
    ImGui::SetNextWindowSize(ImVec2(GAME_WIDTH, HEIGHT_TITLE_GAME));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, BACKGROUND_COLOR);
    ImGui::Begin("##Title game", NULL, _defaultWindowFlags);
    ImGui::Text("Score: %d", totalScore);
    ImGui::End();
    ImGui::PopStyleColor();
}

Tile::Tile()
: _value(0)
, _x(0)
, _y(0)
{
}

Tile::Tile(uint16_t row, uint16_t col)
: _value(0)
, _x(col*RECT_WIDTH)
, _y(row*RECT_HEIGHT)
{
}

const ImU32 Tile::GetColor() const
{
    if (_value == 0)
    {
        return BACKGROUND_COLOR;
    }
    ImU32 color;
    int32_t colorIndex = int(log2(double(_value))) - 1;
    color = COLORS[colorIndex];
    return color;
}

void Tile::Draw(ImDrawList* drawList)
{
    ImU32 color = GetColor();
    drawList->AddRectFilled(ImVec2(_x,_y),ImVec2(_x+RECT_WIDTH, _y+RECT_HEIGHT), color);

    if (_value != 0)
    {
        const char* text = std::to_string(_value).c_str();
        ImVec2 text_size = ImGui::CalcTextSize(text);
        ImVec2 text_pos = ImVec2(
            _x + RECT_WIDTH / 2 - text_size.x / 2,
            _y + RECT_HEIGHT / 2 - text_size.y / 2
        );
        drawList->AddText(text_pos, IM_COL32_BLACK, text);
    }
}

void Tile::SetValue(uint16_t value)
{
    _value = value;
}

const uint16_t Tile::GetValue() const
{
    return _value;
}

const uint16_t Tile::GetX() const
{
    return _x;
}

const uint16_t Tile::GetY() const
{
    return _y;
}