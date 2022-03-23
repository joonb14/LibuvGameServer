#include "GameSessionManager.h"

GameSessionManager GSessionManager;

void GameSessionManager::uvServerSend(ServerService* self, Session* client, char* msg, int len)
{
    int r;
    uv_write_t* write_req;

    if (client == nullptr) {
        LOG_WARNING("Client was not properly initialized, cannot send message to it.");
        return;
    }

    uv_buf_t buf = uv_buf_init(msg, len);
    // LOG_INFO("buf.base == msg : %d",buf.base == msg); // this returns true
    write_req = (uv_write_t *)malloc(sizeof(uv_write_t));
    r = uv_write(write_req, (uv_stream_t*) client, &buf, 1, PacketSession::writeCB);
    CHECK(r, "uv_write");
    
}