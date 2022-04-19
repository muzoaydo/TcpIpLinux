#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main (int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "ERROR, usage %s {portNo}\n", argv[0]);
        exit(1);
    }
    int port = atoi(argv[1]);
    close(port);
}