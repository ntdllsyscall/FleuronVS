#include <debug.h>
#include <engine.h>

void fl_error(const char* msg, enum FL_ERROR type)
{
    switch (type)
    {
    case FL_NOTICE:
        printf("[*] %s", msg);
        return;
    case FL_WARNING:
        printf("[?] Warning: %s", msg);
        return;
    case FL_FATAL:
        printf("[!] Fatal error: %s", msg);
        cleanUp();
        system("pause");
        exit(FL_FATAL);
        return;
    default:
        return;
    }

    return;
}
