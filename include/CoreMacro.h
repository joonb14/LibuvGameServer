#include <cstdio>

#define TIME_TO_ANSWER 1E7 * 4
#define TRACKS MAX_CLIENTS
#define to_s(x) #x
#define THREADS to_s(TRACKS)

#define HOST  "0.0.0.0" /* localhost */
#define PORT  7000

#define MAX_CLIENTS 100

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
/* The error code set by various library functions.  */
extern int *__errno_location (void) __THROW __attribute_const__;
# define errno (*__errno_location ())
/* safe readable version of errno */
#define clean_errno() (errno == 0 ? "None" : strerror(errno))
#define LOG_ERROR(M, ...) fprintf(stderr,  "\33[31mERR\33[39m   " M "  \33[90m at %s (%s:%d) \33[94merrno: %s\33[39m\n", ##__VA_ARGS__, __func__, __FILE__, __LINE__, clean_errno())
#define LOG_WARNING(M, ...) fprintf(stderr, "\33[91mWARN\33[39m  " M "  \33[90m at %s (%s:%d) \33[94merrno: %s\33[39m\n", ##__VA_ARGS__, __func__, __FILE__, __LINE__, clean_errno())
#define LOG_INFO(M, ...) fprintf(stderr, "\33[32mINFO\33[39m  " M "  \33[90m at %s (%s:%d) \33[39m\n", ##__VA_ARGS__, __func__, __FILENAME__, __LINE__)
#define CHECK(code, msg) if(code) { fprintf(stderr, "%s: [%s(%d): %s]\n", msg, uv_err_name((code)), (int) code, uv_strerror((code))); assert(0); } 
