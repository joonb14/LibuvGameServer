#ifndef __SESSION_H__
#define __SESSION_H__
#include "CorePch.h"
#include "Session.h"
#include "Service.h"

class ServerService;
class ClientMsg;

// Client!
class Session : public uv_tcp_t 
{
public:
  int               id;
  int               slot;
  ServerService*    service; // TODO ? weak_ptr<ServerService> 로 수정?
  void*             data;

public:
  ~Session()
  {
    // SessionRef의 ref count가 0이 되면 destructor를 호출하는지 확인하기 위함
    LOG_INFO("~Session(): Client id(%d) slot(%d) closed", id, slot);
  }
};

/*--------------------------------------------------------------------
						PacketSession
	TCP의 특성상 한번에 완전한 packet이 전달이 안될 수 있다
	그래서 PacketHeader를 확인해서 지금까지 받은 data의 size를
	확인하는 방법으로 data를 다 받았는지 확인하고 다 받으면 처리
---------------------------------------------------------------------*/

struct PacketHeader
{
	uint16 size;	// 가변적인 vector나 string 같은 data때문에 꼭 size가 필요하다
	uint16 id;		// 프로토콜 ID (ex. 1=로그인, 2=이동요청)
};

// [size(2)][id(2)][data...][size(2)][id(2)][data...]
// size : 헤더의 크기를 포함한 전체의 크기
class PacketSession : public Session
{
public:
  static void shutdownCB(uv_shutdown_t* req, int status);
  static void writeCB(uv_write_t* req, int status);
  static void closeCB(uv_handle_t* handle);
  static void clientShutdownCB(uv_shutdown_t* req, int status);
  static void allocCB(uv_handle_t *handle, size_t size, uv_buf_t *buf);
  static void readCB(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
  static void connectionCB(uv_stream_t *tcp, int status) ;

  static void disconnect(Session* client);
  // static void onRespond(ClientMsg* msg, char* response);
};

class ClientMsg 
{
public:
  char*       buf;
  size_t      len;
  Session*    session;
};

#endif