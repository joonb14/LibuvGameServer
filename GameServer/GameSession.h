#ifndef __GAME_SESSION_H__
#define __GAME_SESSION_H__
#include "../include/Service.h"
#include "GameSessionManager.h"

class GameSession : public PacketSession
{
public:
  float posX;
  float posY;
  float posZ;
  // Room  room;

public:
  static void onConnected(Session* client, int totalConnections) ;
  // TODO : Packet을 보고 알맞은 ClientPacketHandler.cpp에 있는 Handler를 호출하도록 수정
  // static void onRecv(ClientMsg* msg, OnRespond respond); 
  static void onRecv(Session* client, BYTE* msg, int offset, int len); 
  static void onDisconnected(Session* client);
};

#endif