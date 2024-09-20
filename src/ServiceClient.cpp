#include "ServiceClient.h"

ScoreClient::ScoreClient(std::shared_ptr<grpc::Channel> channel)
: stub(Score::NewStub(channel))
{
}

ScoreInfo ScoreClient::UpdateScore(const std::string& name, const std::string& api, int32_t key, bool reset)
{
    ScoreInfo scoreInfo;
    ScoreRequest request;
    request.set_name(name);
    request.set_api(api);
    request.set_key(key);
    request.set_reset(reset);

    ScoreReply reply;
    grpc::ClientContext context;
    grpc::Status status = stub->UpdateScore(&context, request, &reply);
    if (status.ok())
    {
        scoreInfo.score = reply.score();
        scoreInfo.totalScore = reply.total_score();
        int i = 0;
        for (int value : reply.values()) {
            scoreInfo.tiles[i] = value;
            ++i;
        }
    }
    else
    {
        std::cout << status.error_code() << ": " << status.error_message()
        << std::endl;
    }
    return scoreInfo;
}