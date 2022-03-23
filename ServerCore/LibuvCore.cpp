#include "../include/LibuvCore.h"

void LibuvCore::idleRegister(uv_loop_t * loop, uv_idle_t * handle, uv_idle_cb callback)
{
    int r = uv_idle_init(loop, handle);
    CHECK(r,"uv_idle_init(loop, handle)")
    r = uv_idle_start(handle, callback);
    CHECK(r,"uv_idle_start(handle, handle)")
}

void LibuvCore::run(uv_loop_t * loop)
{
    int r = uv_run(loop, UV_RUN_DEFAULT);
    CHECK(r,"uv_run(loop, UV_RUN_DEFAULT)")
}

void LibuvCore::close(uv_loop_t * loop)
{
    int r;
    do 
    {
        r = uv_loop_close(loop);
        CHECK(r,"uv_loop_close(loop)")
    } while (0);
}