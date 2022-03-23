#ifndef __GAME_SESSION_MANAGER_H__
#define __GAME_SESSION_MANAGER_H__
#include <memory>
#include <set>
#include "GameSession.h"
#include "../include/Listener.h"
#include "../include/Session.h"

class GameSession;
using GameSessionRef = std::shared_ptr<GameSession>;

/*
 * Config
 */

#define MAX_MSG     1024
#define DELAY       1E6

class GameSessionManager
{
public:
    static void uvServerSend(ServerService* self, Session* client, char* msg, int len);

    template <typename S, typename FMT, typename... ARGS>
    void uvServerBroadcast(S server, FMT fmt, ARGS... args)
    {
        do {
            int msgLen;
            char msg[MAX_MSG]; 
            snprintf(msg, MAX_MSG, fmt, args...);
            msgLen = strlen(msg);
            for (auto& kv : server->getSessions()) {
                uvServerSend(server, kv.first, msg, msgLen);
            }
        } while(0);
    }

    void broadcast(ServerService* server, BYTE* msg)
    {
		PacketHeader* header = reinterpret_cast<PacketHeader*>(msg);
        // LOG_INFO("header size : %d", header->size);
        for (auto& kv : server->getSessions()) {
            uvServerSend(server, kv.first, (char *)msg, header->size);
        }
    }
};

extern GameSessionManager GSessionManager;

#endif