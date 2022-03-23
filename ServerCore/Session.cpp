#include "../include/Session.h"

void PacketSession::shutdownCB(uv_shutdown_t* req, int status) 
{
    uv_close((uv_handle_t*) req->handle, closeCB);
    free(req);
}

void PacketSession::writeCB(uv_write_t* req, int status) 
{
    CHECK(status, "writeCB");
    free(req->bufs);
    free(req);
}

void PacketSession::closeCB(uv_handle_t* handle) 
{
    free(handle->data); 
    Session * session = (Session *) handle;
    free(session->data); 
    session->service->releaseSession(session);
}

void PacketSession::clientShutdownCB(uv_shutdown_t* req, int status) 
{
    uv_close((uv_handle_t*) req->handle, closeCB);
    /* todo: clean client (req->data) and remove it from it's server */
    free(req);
}

void PacketSession::allocCB(uv_handle_t *handle, size_t size, uv_buf_t *buf) 
{
    buf->base = (char *)malloc(size); 
    buf->len = size; 
    if (buf->base == NULL) LOG_ERROR("allocCB buffer didn't properly initialize");
}

void PacketSession::readCB(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) 
{
    Session *client = (Session*) stream;
    ServerService* service = client->service;

    /* Errors or EOF */
    if (nread < 0) {
        if (nread != UV_EOF) CHECK(nread, "readCB");
        free(buf->base);
        disconnect(client);
        return;
    }

    if (nread == 0) {
        free(buf->base);
        return;
    }

	int processLen = 0;

    // 클라이언트나 서버나 tcp 통신이기에 stream으로 data가 들어옴
	while (true)
	{
		int32 dataSize = nread - processLen;
		// 최소한 헤더는 파싱할 수 있어야한다
		if (dataSize < sizeof(PacketHeader))
			break;

		PacketHeader* header = reinterpret_cast<PacketHeader*>(&buf->base[processLen]);
		//헤더에 기록된 패킷 크기를 파싱할 수 있어야 한다
		if (dataSize < header->size)
			break;
        
        // 여기까지 왔으면 stream을 message로 파싱 가능(패킷 조립 가능)
        // GameSession::onRecv()가 호출된다. GameServer.cpp에서 callback으로 등록을 했기 때문.
        service->onRecv(client, reinterpret_cast<BYTE*>(buf->base), processLen, header->size);

		processLen += header->size;
	}

    // Free memory
    free(buf->base);
}

void PacketSession::connectionCB(uv_stream_t *tcp, int status) 
{
    CHECK(status, "connectionCB");
    ServerService* service = (ServerService*)tcp;

    /* Accept client connection */
    LOG_INFO("Accepting Connection");

    if (service->getCount() == MAX_CLIENTS) {
        LOG_INFO("exceeded allowed number of clients");
        /* todo: Ideally we'd allow the client to connect,  tell it and disconnect */
        return;
    }

    SessionRef client = service->createSession(); // shared_ptr로 Session 생성
    
    int r = uv_tcp_init(tcp->loop, (uv_tcp_t*)client.get());
    CHECK(r, "uv_tcp_init");

    r = uv_accept(tcp, (uv_stream_t*) client.get());
    if (r) {
        LOG_ERROR("trying to accept connection %d", r);
        disconnect(client.get()); //  LibuvUtils::closeCB()을 호출하는데 여기서 session->service->releaseSession(session)로 client의 ref_count 줄여서 삭제
        return;
    }

    // client 초기화
    client->service = service;
    client->slot   = service->getCount();
    client->id     = service->ids;

    // moveSession
    service->moveSession(client); // SessionRef client의 소유권을 service로 이전
    
    service->onConnected(client.get(), service->getCount());

    /* Start reading data from client */
    r = uv_read_start((uv_stream_t*) client.get(), allocCB, readCB);
    CHECK(r, "uv_read_start");
}

void PacketSession::disconnect(Session* client) 
{
    ServerService* service = client->service;

    service->onDisconnected(client);

    uv_shutdown_t* shutdown_req = (uv_shutdown_t*)malloc(sizeof(uv_shutdown_t));
    shutdown_req->data = client;
    int r = uv_shutdown(shutdown_req, (uv_stream_t*) client, clientShutdownCB);
    CHECK(r, "uv_shutdown");
}