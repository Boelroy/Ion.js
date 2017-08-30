#ifdef _WIN32

typedef wchar_t Char;
#define CONSOLE(...) w_printf(__VA_ARGS__)
#else

#include "unicode/uchar.h"
#include "unicode/uchar.h"
#include "unicode/ustdio.h"

typedef UChar Char;

#define CONSOLE(...) u_printf(__VA_ARGS__)
#endif