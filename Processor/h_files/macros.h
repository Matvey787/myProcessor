#ifndef MACROS2
#define MACROS2

#ifdef STEPBYSTEP

    #define MYSBS(...) __VA_ARGS__

#else

    #define MYSBS(...)

#endif

#endif