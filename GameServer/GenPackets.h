#ifndef __GEN_PACKETS__
#define __GEN_PACKETS__
#include "../include/CorePch.h"
#include <vector>
using namespace std;

using S_PlayerRef = std::shared_ptr<class S_Player>;

enum class PacketID
{
    S_BROADCASTENTERGAME = 1000, 
	C_LEAVEGAME = 1001, 
	S_BROADCASTLEAVEGAME = 1002, 
	S_PLAYERLIST = 1003, 
	C_MOVE = 1004, 
	S_BROADCASTMOVE = 1005, 
};

class IPacket
{
public:
	uint16 protocol;

public:
	void read(BYTE* data);
	uint16 write(BYTE* buffer);
};

class C_MOVE : public IPacket
{
public:
    float posX;
	float posY;
	float posZ;

public:
	C_MOVE() 
	{
		protocol = (uint16)PacketID::C_MOVE;
	}

	~C_MOVE()
	{
		// LOG_INFO("~C_MOVE()");
	}

    void read(BYTE* data)
    {
        uint16 count = 0;
        count += sizeof(uint16);
        count += sizeof(uint16);
        posX = *reinterpret_cast<float*>(data + count);
		count += sizeof(float);
		posY = *reinterpret_cast<float*>(data + count);
		count += sizeof(float);
		posZ = *reinterpret_cast<float*>(data + count);
		// count += sizeof(float);
    }

    uint16 write(BYTE* buffer)
    {
        uint16 count = 0;

        count += sizeof(uint16);
		memcpy(buffer + count, &protocol, sizeof(uint16));
		count += sizeof(uint16);
		memcpy(buffer + count, &posX, sizeof(float));
        count += sizeof(float);
		memcpy(buffer + count, &posY, sizeof(float));
		count += sizeof(float);
		memcpy(buffer + count, &posZ, sizeof(float));
		count += sizeof(float);

        memcpy(buffer, &count, sizeof(uint16));

        return count;
    }
};

class S_BROADCASTENTERGAME : public IPacket
{
private:
	int32 _playerId;
	float _posX;
	float _posY;
	float _posZ;

public:
	~S_BROADCASTENTERGAME()
	{
		// LOG_INFO("~S_BROADCASTENTERGAME()");
	}

	S_BROADCASTENTERGAME(int32 playerId, float posX, float posY, float posZ) 
	{
		protocol = (uint16)PacketID::S_BROADCASTENTERGAME;
		_playerId = playerId;
		_posX = posX;
		_posY = posY;
		_posZ = posZ;
	}

    void read(BYTE* data)
    {
        uint16 count = 0;
        count += sizeof(uint16);
        count += sizeof(uint16);
        _playerId = *reinterpret_cast<int32*>(data + count);
		count += sizeof(int32);
        _posX = *reinterpret_cast<float*>(data + count);
		count += sizeof(float);
		_posY = *reinterpret_cast<float*>(data + count);
		count += sizeof(float);
		_posZ = *reinterpret_cast<float*>(data + count);
		// count += sizeof(float);
    }

    uint16 write(BYTE* buffer)
    {
        uint16 count = 0;

        count += sizeof(uint16);
		memcpy(buffer + count, &protocol, sizeof(uint16));
		count += sizeof(uint16);
        memcpy(buffer + count, &_playerId, sizeof(int32));
        count += sizeof(int32);
		memcpy(buffer + count, &_posX, sizeof(float));
        count += sizeof(float);
		memcpy(buffer + count, &_posY, sizeof(float));
		count += sizeof(float);
		memcpy(buffer + count, &_posZ, sizeof(float));
		count += sizeof(float);

        memcpy(buffer, &count, sizeof(uint16));

        return count;
    }
};


class S_Player
{
public:
	bool isSelf;
	int32 playerId;
	float posX;
	float posY;
	float posZ;

	~S_Player() {
		// LOG_INFO("~S_Player() : Destroying player %d", playerId);
	}

	void read(BYTE* data, uint16 &count)
	{
		isSelf = *reinterpret_cast<bool*>(data + count);
		count += sizeof(bool);
		playerId = *reinterpret_cast<int32*>(data + count);
		count += sizeof(int32);
		posX = *reinterpret_cast<float*>(data + count);
		count += sizeof(float);
		posY = *reinterpret_cast<float*>(data + count);
		count += sizeof(float);
		posZ = *reinterpret_cast<float*>(data + count);
		count += sizeof(float);
	}

	bool write(BYTE* data, uint16 &count)
	{
		bool success = true;
		memcpy(data + count, &isSelf, sizeof(bool));
		count += sizeof(bool);
		memcpy(data + count, &playerId, sizeof(int32));
		count += sizeof(int32);
		memcpy(data + count, &posX, sizeof(float));
		count += sizeof(float);
		memcpy(data + count, &posY, sizeof(float));
		count += sizeof(float);
		memcpy(data + count, &posZ, sizeof(float));
		count += sizeof(float);
		return success;
	}
};


class S_PLAYERLIST : public IPacket
{
public:
	vector<S_PlayerRef> players = vector<S_PlayerRef>();

public:
	S_PLAYERLIST() {
		protocol = (uint16)PacketID::S_PLAYERLIST;
	}
	
	~S_PLAYERLIST() {
		players.clear();
		// LOG_INFO("~S_PLAYERLIST()");
	}

    void read(BYTE* data)
    {
        uint16 count = 0;
        count += sizeof(uint16);
        count += sizeof(uint16);
        players.clear();
		uint16 playerLen = *reinterpret_cast<uint16*>(data + count);
		count += sizeof(uint16);
		for (int i = 0; i< playerLen; i++)
		{
		    S_PlayerRef player = make_shared<S_Player>();
		    player->read(data, count);
		    players.push_back(player);
		}
    }

    uint16 write(BYTE* buffer)
    {
        uint16 count = 0;

        count += sizeof(uint16);
		memcpy(buffer + count, &protocol, sizeof(uint16));
		count += sizeof(uint16);
		int playerCount = players.size();
		memcpy(buffer + count, &playerCount, sizeof(uint16));
        count += sizeof(uint16);
		for(S_PlayerRef player : players) 
		    player->write(buffer, count);

		memcpy(buffer, &count, sizeof(uint16));
        
        return count;
    }
};


class S_BROADCASTMOVE : public IPacket
{
private:
	int _playerId;
	float _posX;
	float _posY;
	float _posZ;

public:
	~S_BROADCASTMOVE()
	{
		// LOG_INFO("~S_BROADCASTMOVE()");
	}

	S_BROADCASTMOVE(int32 playerId, float posX, float posY, float posZ)
	{
		protocol = (uint16)PacketID::S_BROADCASTMOVE;
		_playerId = playerId;
		_posX = posX;
		_posY = posY;
		_posZ = posZ;
	}

    void read(BYTE* data)
    {
        uint16 count = 0;
        count += sizeof(uint16);
        count += sizeof(uint16);
		_playerId = *reinterpret_cast<int32*>(data + count);
		count += sizeof(int32);
		_posX = *reinterpret_cast<float*>(data + count);
		count += sizeof(float);
		_posY = *reinterpret_cast<float*>(data + count);
		count += sizeof(float);
		_posZ = *reinterpret_cast<float*>(data + count);
		// count += sizeof(float);
    }

    uint16 write(BYTE* buffer)
    {
        uint16 count = 0;

        count += sizeof(uint16);
		memcpy(buffer + count, &protocol, sizeof(uint16));
		count += sizeof(uint16);
        memcpy(buffer + count, &_playerId, sizeof(int32));
        count += sizeof(int32);
		memcpy(buffer + count, &_posX, sizeof(float));
        count += sizeof(float);
		memcpy(buffer + count, &_posY, sizeof(float));
		count += sizeof(float);
		memcpy(buffer + count, &_posZ, sizeof(float));
		count += sizeof(float);

        memcpy(buffer, &count, sizeof(uint16));

        return count;
    }
};

#endif