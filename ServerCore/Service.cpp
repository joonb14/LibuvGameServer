#include "../include/Service.h"

Service::Service(ServiceType type, LibuvCoreRef core, SessionFactory factory, uv_loop_t *loop, const char* host, int port)
    : ids(0), _libuvCore(core), _sessionFactory(factory), _type(type), _loop(loop), _host(host), _port(port)
{
    LOG_INFO("Service Constructor");

}

Service::~Service()
{

}

void Service::closeService()
{
	// TODO
}

SessionRef Service::createSession()
{
    return _sessionFactory();
}

void Service::moveSession(SessionRef session)
{
    _sessions.insert(make_pair(session.get(), std::move(session)));
    ids++;
}

void Service::releaseSession(Session* session)
{
    _sessions.erase(session);
}

ServerService::ServerService(LibuvCoreRef core, SessionFactory factory, uv_loop_t *loop, const char* host, int port, OnConnected onConnected, OnDisconnected onDisconnected, OnRecv onRecv)
    : Service(ServiceType::SERVER, core, factory, loop, host, port), onConnected(onConnected), onDisconnected(onDisconnected), onRecv(onRecv)
{
    
}

bool ServerService::start()
{
	if (canStart() == false)
		return false;

    LOG_INFO("Creating server");
    _listener = std::make_shared<Listener>();
	if (_listener == nullptr)
		return false;

    ServerServiceRef service = std::static_pointer_cast<ServerService>(shared_from_this());
    if (_listener->startAccept(service) == false)
		return false;
    
    return true;
}

void ServerService::closeServerService()
{
	// TODO
	Service::closeService();
}

