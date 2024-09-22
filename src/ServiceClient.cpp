#include "ServiceClient.h"

ScoreClient::ScoreClient(std::shared_ptr<grpc::Channel> channel)
    : stub(Score::NewStub(channel))
{
}

LoginInfo ScoreClient::LoginGame(const std::string& username, const std::string& password)
{
    LoginInfo loginInfo;
    LoginRequest request;
    request.set_username(username);
    request.set_password(password);

    LoginResponse response;
    grpc::ClientContext context;
    grpc::Status status = stub->Login(&context, request, &response);
    if (status.ok())
    {
        loginInfo.valid = response.valid();
        loginInfo.name = response.name();
        loginInfo.api = response.api();
        loginInfo.totalScore = response.total_score();
    }
    else
    {
        loginInfo.valid = false;
        std::cout << status.error_code() << ": " << status.error_message()
        << std::endl;
    }
    return loginInfo;
}

ScoreInfo ScoreClient::UpdateScore(const std::string& name, const std::string& api, int32_t key, bool reset)
{
    ScoreInfo scoreInfo;
    ScoreRequest request;
    request.set_name(name);
    request.set_api(api);
    request.set_key(key);
    request.set_reset(reset);

    ScoreResponse response;
    grpc::ClientContext context;
    grpc::Status status = stub->UpdateScore(&context, request, &response);
    if (status.ok())
    {
        if (response.total_score() != 0)
        {
            scoreInfo.score = response.score();
            scoreInfo.totalScore = response.total_score();
            int i = 0;
            for (int value : response.values()) {
                scoreInfo.tiles[i] = value;
                ++i;
            }
        }
    }
    else
    {
        std::cout << status.error_code() << ": " << status.error_message()
        << std::endl;
    }
    return scoreInfo;
}