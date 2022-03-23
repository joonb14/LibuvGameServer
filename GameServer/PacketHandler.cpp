#include "PacketHandler.h"
#include "GameSession.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool handleINVALID(Session* session, BYTE* buffer, int32 len) 
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO : Log
	return true;
}

// bool Handle_C_LEAVEGAME(PacketSession* session, C_LEAVEGAME& pkt)
// {
// 	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

// 	GRoom->DoAsync(&Room::Leave, gameSession->_currentPlayer);
// 	return true;
// }

bool handleC_MOVE(Session* session, C_MOVE& pkt)
{
	GameSession* client = reinterpret_cast<GameSession*>(session);

	// gameSession->_room = GRoom;

	LOG_INFO("Client(ID:%d) C_MOVE Packet: %f,%f.%f", client->id, pkt.posX, pkt.posY, pkt.posZ);
	// 좌표바꿔주고
	client->posX = pkt.posX;
	client->posY = pkt.posY;
	client->posZ = pkt.posZ;

	// 모두에게 알린다
	S_BROADCASTMOVE* broadcastPkt = new S_BROADCASTMOVE(client->id, client->posX, client->posY, client->posZ);
    BYTE * msg = new BYTE[4096];
	uint16 size = broadcastPkt->write(msg);
	GSessionManager.broadcast(client->service, msg);

    // Free memory
    delete broadcastPkt;
	delete [] msg;

	return true;
}