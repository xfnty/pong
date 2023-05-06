#include <stdio.h>
#include <assert.h>
#include <system/logging.h>


int main(int argc, char const *argv[])
{
    assert(logging_init());
    LOG("Hello, World");
    logging_shutdown();
    return 0;
}
