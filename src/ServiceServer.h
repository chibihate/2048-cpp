#pragma once
#include "Tile.h"
#include <sqlite3.h>

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
    ::grpc::Status Login(::grpc::ServerContext* context, const ::LoginRequest* request, ::LoginResponse* response) override;
    ::grpc::Status UpdateScore(::grpc::ServerContext* context, const ::ScoreRequest* request, ::ScoreResponse* response) override;
public:
    const std::unordered_map<std::string, int32_t> GetScore() const;
private:
    int CreateTable(sqlite3* db);
    int InsertData(sqlite3* db, std::string username, std::string password, std::string api);
    int UpdateData(sqlite3* db, std::string username);
    int UpdateTotalScore(sqlite3* db, std::string username);
    bool IsUserNameAvailable(sqlite3* db, std::string name);
    bool IsValidUser(sqlite3* db, std::string username, std::string password);
    int32_t Move(int32_t key, std::array<std::array<Tile, COLS>, ROWS>& tiles);
    int32_t GenerateRandom(std::array<std::array<Tile, COLS>, ROWS>& tiles);
    void ShiftTile(Tile& first, Tile& second, Tile& third, Tile& forth);
    int32_t MergeTile(Tile& first, Tile& second, Tile& third, Tile& forth);
    void ResetTiles(std::array<std::array<Tile, COLS>, ROWS>& tiles);
    bool ValidRequest(sqlite3* db, std::string name, std::string api);

    std::unordered_map<std::string, int32_t> _mapNameScore;
    std::unordered_map<std::string, UserInfo> _mapNameUserInfo;
    int32_t _totalScore;
};
