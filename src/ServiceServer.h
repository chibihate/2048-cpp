#pragma once
#include "Tile.h"

#include <grpcpp/grpcpp.h>
#include "proto/score.grpc.pb.h"
#include "proto/score.pb.h"

struct UserInfo
{
    std::string api;
    int32_t score;
    int32_t totalScore;
    std::array<std::array<Tile, COLS>, ROWS> tiles;

    UserInfo()
    : api()
    , score(0)
    , totalScore(0)
    , tiles()
    {
    }
};

class ScoreServer final : public Score::Service 
{
    ::grpc::Status UpdateScore(::grpc::ServerContext* context, const ::ScoreRequest* request, ::ScoreReply* response) override;
public:
    const std::unordered_map<std::string, int32_t> GetScore() const;
private:
    int32_t Move(int32_t key, std::array<std::array<Tile, COLS>, ROWS>& tiles);
    int32_t GenerateRandom(std::array<std::array<Tile, COLS>, ROWS>& tiles);
    void ShiftTile(Tile& first, Tile& second, Tile& third, Tile& forth);
    int32_t MergeTile(Tile& first, Tile& second, Tile& third, Tile& forth);
    void ResetTiles(std::array<std::array<Tile, COLS>, ROWS>& tiles);
    
    std::unordered_map<std::string, int32_t> _mapNameScore;
    std::unordered_map<std::string, UserInfo> _mapNameUserInfo;
};
