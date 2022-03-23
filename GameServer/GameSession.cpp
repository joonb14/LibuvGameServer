#include "GameSession.h"
#include "GenPackets.h"
#include "PacketHandler.h"

void GameSession::onConnected(Session* client, int totalConnections)
{
    ServerService *service = client->service;

    LOG_INFO("New player, %d total now.", totalConnections);

    // 새로운 Client에게 모든 플레이어 목록 전송
    S_PLAYERLIST* players = new S_PLAYERLIST();
    for (auto kv : service->getSessions())
    {
        GameSessionRef session = static_pointer_cast<GameSession>(kv.second);
        S_PlayerRef player = make_shared<S_Player>();
        player->isSelf = (kv.first == client);
        player->playerId = session->slot;
        if(player->isSelf){
            // 새로운 session이기 때문에 초기 생성 좌표를 집어넣어준다
            player->posX = 0;
            player->posY = 0;
            player->posZ = 0;
        }
        else{
            // 원래 존재하던 session이기에 그 좌표를 받아온다
            player->posX = session->posX;
            player->posY = session->posY;
            player->posZ = session->posZ;
        }

        players->players.push_back(player);
    }
    
    BYTE * msg = new BYTE[4096];
	uint16_t size = players->write(msg);
	// PacketHeader* header = reinterpret_cast<PacketHeader*>(msg);
    // GSessionManager.uvServerSend(service,client,(char *)msg, header->size);
    GSessionManager.uvServerSend(service,client,(char *)msg, size);
    
    // 현재 접속중인 모든 client들에게 새로운 client가 들어왔다는 것을 알림
    S_BROADCASTENTERGAME* pkt = new S_BROADCASTENTERGAME(client->slot, 0, 0, 0);
    
    BYTE * broadcastMsg = new BYTE[4096];
	size = pkt->write(broadcastMsg);
    GSessionManager.broadcast(client->service, broadcastMsg);
    
    // TODO : writeCB이 call될때가 이때보다 늦으면 nullptr error 가능...
    // Free memory
    delete players;
    delete pkt;

    delete [] msg;
    delete [] broadcastMsg;
}

// PacketSession::readCB에서 호출
void GameSession::onRecv(Session* client, BYTE* msg, int offset, int len) 
{
    PacketHeader* header = reinterpret_cast<PacketHeader*>(msg[offset]);

	// TODO : packetId 대역 체크
	PacketHandler::handlePacket(client, msg, len);
}

void GameSession::onDisconnected(Session* client) 
{
    int totalConnections = client->service->getCount();
    LOG_INFO("Player %d quit, %d total now.", client->id, totalConnections-1);
    GSessionManager.uvServerBroadcast(client->service,
        "Player quit %d :(\nWe have %d players left.\n",
        client->id, totalConnections);
}