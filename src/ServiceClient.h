#pragma once
#include "imgui.h"

#include <grpcpp/grpcpp.h>
#include "proto/score.grpc.pb.h"
#include "proto/score.pb.h"

struct ScoreInfo
{
    int32_t score;
    int32_t totalScore;
    std::array<int32_t, 16> tiles;

    ScoreInfo()
    : score(0)
    , totalScore(0)
    , tiles()
    {
    }

    ScoreInfo(int32_t total_score)
    : score(0)
    , totalScore(total_score)
    , tiles()
    {
    }
};

struct LoginInfo
{
    bool valid;
    std::string name;
    std::string api;
    int32_t totalScore;

    LoginInfo()
    : valid(true)
    , name()
    , api()
    , totalScore(0)
    {
    }
};

class ScoreClient {
public:
    ScoreClient(std::shared_ptr<grpc::Channel> channel);
    LoginInfo LoginGame(const std::string& username, const std::string& password);
    ScoreInfo UpdateScore(const std::string& name, const std::string& api, int32_t key, bool reset);
private:
    std::unique_ptr<Score::Stub> stub;
};