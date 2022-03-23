#ifndef __SERVICE_H__
#define __SERVICE_H__
#include "CorePch.h"
#include "Session.h"
#include "Listener.h"
#include <map>
#include <functional>

class ClientMsg;

// 함수 포인터
typedef void (*OnRecv)(Session*, BYTE*, int, int);
typedef void (*OnConnected)(Session*, int);
typedef void (*OnDisconnected)(Session*);

enum class ServiceType : uint8_t
{
	SERVER,
	CLIENT
};

using SessionFactory = function<SessionRef(void)>;

class Service : public uv_tcp_t, public enable_shared_from_this<Service>
{
public:
    int         ids;

private:
    LibuvCoreRef                _libuvCore;
	SessionFactory	            _sessionFactory;
    ServiceType                 _type;
    uv_loop_t*                  _loop;
    const char*                 _host;
    int                         _port;
    map<Session*, SessionRef>   _sessions; // Session* 를 key로 (보통 session.get()이 key) 사용해서 SessionRef를 찾기위해 map 사용
    
public:
    Service(ServiceType type, LibuvCoreRef core, SessionFactory factory, uv_loop_t *loop, const char* host, int port);
    ~Service();

    bool            canStart() {return  _sessionFactory != nullptr;}
	void            closeService();
	SessionRef		createSession();
	void			moveSession(SessionRef session); // session의 소유권을 _session으로 이전해주는 함수 
	void			releaseSession(Session* session); // map에서 key가 session인 SessionRef 제거하여 delete(여기선 ~Session())를 호출하게함
    int             getCount() { return _sessions.size(); } // 현재 연결된 Session의 수를 return

    map<Session*, SessionRef> getSessions() { return _sessions; }

public:
	LibuvCoreRef&	getLibuvCore() { return _libuvCore; }
    ServiceType     getServiceType() { return _type; }
    uv_loop_t*      getLoop() { return _loop; }
    const char*     getHost() { return _host; }
    int             getPort() { return _port; }

};

class ServerService: public Service
{
public:
    /* callbacks in GameSession */
    OnConnected     onConnected;
    OnDisconnected  onDisconnected;
    OnRecv          onRecv;

private:
    ListenerRef     _listener = nullptr;

public:
	ServerService(LibuvCoreRef core, SessionFactory factory, uv_loop_t *loop, const char* host, int port, OnConnected onConnected, OnDisconnected onDisconnected, OnRecv onRecv);
	~ServerService() {}

	bool            start();
	void            closeServerService();
};

#endif