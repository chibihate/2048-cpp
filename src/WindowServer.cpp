#include "Logger.h"
#include "WindowServer.h"

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
    ImGui::End();
}