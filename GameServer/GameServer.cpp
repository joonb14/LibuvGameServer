#ifndef __GAME_SERVER_H__
#define __GAME_SERVER_H__
#include "../include/CorePch.h"
#include "GameSession.h"
#include "PacketHandler.h"
#include "../include/Listener.h"
#include "../include/Service.h"
#include "../include/LibuvCore.h"

int main(void) {
  
  uv_loop_t *loop = uv_default_loop();

  /* Ensure that each horse gets its own thread, the default libuv threadpool size is 4 */
  setenv("UV_THREADPOOL_SIZE", THREADS, 1);

  srand(time(NULL));

  // PacketHandler 초기화
  PacketHandler::Init();

  // ServerService를 만들어 Listener 호출을 통해 startAccept(내부에서 uv_listen)
  // 실제 Accept는 uv_listen에 등록된 connectionCB(내부에서 uv_accept)에서 수행
  // connection이 수립되면 client에 uv_read_start로 readCB을 등록한다
  // client가 message를 전송하여 readCB이 실행되면 message를 처리한다음 luv_server_t->onclient_msg에 등록된 GameSession::onclient_msg 호출
  ServerServiceRef service= std::make_shared<ServerService>(
    std::make_shared<LibuvCore>(), 
    std::make_shared<GameSession>,
    loop, 
    HOST, 
    PORT, 
    GameSession::onConnected, 
    GameSession::onDisconnected, 
    GameSession::onRecv);
  
  if(!service->start()) {
    fprintf(stderr,"Error: service->start()");
    return -1;
  }

  LOG_INFO("Initializing game loop");
  // Room* room = new Room();
  
  // room->service =  service.get();
  // room->delay = DELAY;
  // service->data = room;

  // -------------------------------------------------------------------------
  // TODO : Replace the idle with Async IO
  // (learnuv의 08_horse_race.c 를 보고 수정)
  // int uv_async_init(uv_loop_t *loop, uv_async_t *async, uv_async_cb async_cb)
  // int uv_queue_work(uv_loop_t *loop, uv_work_t *req, uv_work_cb work_cb, uv_after_work_cb after_work_cb)
  // EM이 worker(Thread)를 생성하고 callback을 등록하는 부분에 해당
  
  // uv_idle_t questionHandle;
  // questionHandle.data = room;
  // service->getLibuvCore()->idleRegister(
  //   loop, 
  //   &questionHandle, 
  //   GameSession::questionHandler);

  // uv_idle_t scoreHandle;
  // scoreHandle.data = room;
  // service->getLibuvCore()->idleRegister(
  //   loop, 
  //   &scoreHandle, 
  //   GameSession::scoreHandler);
  
  // -------------------------------------------------------------------------

  service->getLibuvCore()->run(loop);

  service->getLibuvCore()->close(loop);

  // Free Memory
  // delete room;
  
  return 0;
}

#endif