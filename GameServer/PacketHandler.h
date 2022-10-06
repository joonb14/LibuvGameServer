#ifndef __PACKET_HANDLER_H__
#define __PACKET_HANDLER_H__
#include "GenPackets.h"
#include "../include/Session.h"
#include <functional>

using PacketHandlerFunc = std::function<bool(Session*, BYTE*, int32)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];

// Custom Handlers
bool handleINVALID(Session* session, BYTE* buffer, int32 len);
bool handleC_LEAVEGAME(Session* session, C_LEAVEGAME& pkt);
bool handleC_MOVE(Session* session, C_MOVE& pkt);
bool handleC_ANIMATION(Session* session, C_ANIMATION& pkt);


class PacketHandler
{
public:
	static void Init()
	{
		for (uint16 i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = handleINVALID;
		GPacketHandler[(uint16)PacketID::C_LEAVEGAME] = [](Session* session, BYTE* buffer, int32 len) { return handlePacket<C_LEAVEGAME>(handleC_LEAVEGAME, session, buffer, len); };
		GPacketHandler[(uint16)PacketID::C_MOVE] = [](Session* session, BYTE* buffer, int32 len) { return handlePacket<C_MOVE>(handleC_MOVE, session, buffer, len); };
		GPacketHandler[(uint16)PacketID::C_ANIMATION] = [](Session* session, BYTE* buffer, int32 len) { return handlePacket<C_ANIMATION>(handleC_ANIMATION, session, buffer, len); };
	}

	static bool handlePacket(Session* session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}
	
private:
	template<typename PacketType, typename ProcessFunc>
	static bool handlePacket(ProcessFunc func, Session* session, BYTE* buffer, int32 len)
	{
		PacketType pkt;
        pkt.read(buffer);

		return func(session, pkt);
    }
};

#endif