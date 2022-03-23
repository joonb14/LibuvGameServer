#include "Session.h"
#include "CorePch.h"
#include "functional"
#include <memory>

class LibuvCore : public std::enable_shared_from_this<LibuvCore>
{
public:
    void idleRegister(uv_loop_t *, uv_idle_t *, uv_idle_cb);
    void run(uv_loop_t *);
    void close(uv_loop_t *);
};