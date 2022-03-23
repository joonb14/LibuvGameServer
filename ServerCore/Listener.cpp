#include "../include/Listener.h"

Listener::~Listener()
{
    int r;
    uv_shutdown_t *shutdown_req;

    for (auto& kv : _service->getSessions()) {
        shutdown_req = (uv_shutdown_t *)malloc(sizeof(uv_shutdown_t));
        shutdown_req->data = kv.first;
        r = uv_shutdown(shutdown_req, (uv_stream_t*) kv.first, PacketSession::shutdownCB);
        CHECK(r, "uv_shutdown");
    }

    uv_close((uv_handle_t*) _service.get(), NULL);
}

bool Listener::startAccept(ServerServiceRef service)
{
    _service = service;
	if (_service == nullptr)
		return false;

    int r;
    /* Initialize TCP server */
    r = uv_tcp_init(_service->getLoop(), _service.get());
    CHECK(r, "uv_tcp_init");

    /* Bind to localhost:7000 */
    struct sockaddr_in addr;
    r = uv_ip4_addr(_service->getHost(), _service->getPort(), &addr);
    CHECK(r, "uv_ip4_addr");

    r = uv_tcp_bind(_service.get(), (struct sockaddr*) &addr, AF_INET);
    CHECK(r, "uv_tcp_bind");

    LOG_INFO("Starting server");

    r = uv_listen((uv_stream_t*) _service.get(), SOMAXCONN, PacketSession::connectionCB);
    CHECK(r, "uv_listen");
    LOG_INFO("Listening on %s:%d", _service->getHost(), _service->getPort());

    return true;
}
