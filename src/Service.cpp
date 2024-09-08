#include "Service.h"

::grpc::Status ScoreServer::UpdateScore(::grpc::ServerContext* context, const ::ScoreRequest* request, ::ScoreReply* response)
{
    std::cout << "Received from client: " << request->name().c_str() << std::endl;
    if (auto search = _mapNameScore.find(request->name()); search != _mapNameScore.end())
    {
        _mapNameScore[request->name()]++;
    }
    else
    {
        _mapNameScore.insert(std::pair{request->name(), 1});
    }
    response->set_score(_mapNameScore[request->name()]);
    return grpc::Status::OK;
}

const std::unordered_map<std::string, int32_t> ScoreServer::GetScore() const
{
    return _mapNameScore;
}

ScoreClient::ScoreClient(std::shared_ptr<grpc::Channel> channel)
: stub(Score::NewStub(channel))
{
}

int32_t ScoreClient::UpdateScore(const std::string& name, const std::string& api, int32_t key)
{
    ScoreRequest request;
    request.set_name(name);
    request.set_api(api);
    request.set_key(key);

    ScoreReply reply;
    grpc::ClientContext context;
    grpc::Status status = stub->UpdateScore(&context, request, &reply);
    if (status.ok())
    {
        return reply.score();
    }
    else
    {
        std::cout << status.error_code() << ": " << status.error_message()
        << std::endl;
        return 0;
    }
}