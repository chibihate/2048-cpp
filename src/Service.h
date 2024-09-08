#pragma once
#include <grpcpp/grpcpp.h>
#include "proto/score.grpc.pb.h"
#include "proto/score.pb.h"

class ScoreServer final : public Score::Service 
{
    ::grpc::Status UpdateScore(::grpc::ServerContext* context, const ::ScoreRequest* request, ::ScoreReply* response) override;
public:
    const std::unordered_map<std::string, int32_t> GetScore() const;
private:
    std::unordered_map<std::string, int32_t> _mapNameScore;
};

class ScoreClient {
public:
    ScoreClient(std::shared_ptr<grpc::Channel> channel);
    int32_t UpdateScore(const std::string& name, const std::string& api, int32_t key);
private:
    std::unique_ptr<Score::Stub> stub;
};