#include <cstdio>
#include "kvstore_service.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        puts("Usage: [server] [port]\n");
        return EXIT_SUCCESS;
    }
    test::kvstore_service::KVStore_service server;
    if (server.connect(argv[1]) < 0) {
        return EXIT_FAILURE;
    }

    server.run();

    server.stop();

    return EXIT_SUCCESS;
}

