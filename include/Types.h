#include <mutex>
#include <atomic>
#include <memory>
#include <array>
#include <vector>

using BYTE = unsigned char;
using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;
using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

template<typename T>
using Atomic = std::atomic<T>;
using Mutex = std::mutex;
using UniqueLock = std::unique_lock<std::mutex>;
using LockGuard = std::lock_guard<std::mutex>;

// shared_ptr

#define USING_SHARED_PTR(name)	using name##Ref = std::shared_ptr<class name>;
USING_SHARED_PTR(ServerService);
USING_SHARED_PTR(Listener);
USING_SHARED_PTR(LibuvCore);
USING_SHARED_PTR(Session);
USING_SHARED_PTR(PacketSession);

#define size16(val)		static_cast<int16>(sizeof(val))
#define size32(val)		static_cast<int32>(sizeof(val))
#define len16(arr)		static_cast<int16>(sizeof(arr)/sizeof(arr[0]))
#define len32(arr)		static_cast<int32>(sizeof(arr)/sizeof(arr[0]))

#define _STOMP