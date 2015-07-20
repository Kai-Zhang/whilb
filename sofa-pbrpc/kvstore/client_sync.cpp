#include <cstdio>
#include <iostream>
#include <string>
#include <sofa/pbrpc/pbrpc.h>
#include "kvstore_proto.pb.h"

namespace test {
namespace kvstore_service {
class KVStore_client_sync {
public:
    KVStore_client_sync() { this->_stub = NULL; }
    ~KVStore_client_sync() { if (this->_stub != NULL) delete this->_stub; }

    void initialize(char *ip, char *port);

    int put(const std::string &key, const std::string &value);

    int get(const std::string &key, std::string &value);

private:
    sofa::pbrpc::RpcClient *_rpc_client;
    sofa::pbrpc::RpcChannel *_rpc_channel;
    KVStoreInterface_Stub *_stub;
};

void KVStore_client_sync::initialize(char *ip, char *port) {
    SOFA_PBRPC_SET_LOG_LEVEL(NOTICE);

    sofa::pbrpc::RpcClientOptions options;
    options.work_thread_num = 8;
    this->_rpc_client = new sofa::pbrpc::RpcClient(options);

    std::string address = ip;
    address += ":";
    address += port;
    this->_rpc_channel = new sofa::pbrpc::RpcChannel(this->_rpc_client, address.c_str());

    this->_stub = new KVStoreInterface_Stub(this->_rpc_channel);
}

int KVStore_client_sync::put(const std::string &key, const std::string &value) {
    StoreRequest request;
    request.set_key(key);
    request.set_value(value);

    StoreResponse response;

    sofa::pbrpc::RpcController controller;

    this->_stub->Store(&controller, &request, &response, NULL);

    if (controller.Failed()) {
        SLOG(ERROR, "Store request failed: %s", controller.ErrorText().c_str());
        return StoreResponse::STORE_FAILURE;
    }

    return response.ret_value();
}

int KVStore_client_sync::get(const std::string &key, std::string &value) {
    ReadRequest request;
    request.set_key(key);

    ReadResponse response;

    sofa::pbrpc::RpcController controller;

    this->_stub->Read(&controller, &request, &response, NULL);

    if (controller.Failed()) {
        SLOG(ERROR, "Read request failed: %s", controller.ErrorText().c_str());
        return ReadResponse::READ_FAILURE;
    }

    if (response.status() == ReadResponse::READ_SUCCESS) {
        value = response.value();
    }

    return response.status();
}
}
}

static inline void print_tips() {
    puts("Enter command:");
    puts("  store(s) - store (key, value) to memory");
    puts("  read(r) - read the value to the key");
    puts("  exit(e) - exit program");
    printf(" > ");
}

int main(int argc, char **argv) {
    if (argc != 3) {
        puts("Usage: [client] [ip address] [port]");
        return EXIT_SUCCESS;
    }

    test::kvstore_service::KVStore_client_sync client;
    client.initialize(argv[1], argv[2]);

    std::string choice, key, value;
    print_tips();
    std::getline(std::cin, choice);
    while (choice.compare("exit") && choice.compare("e")) {
        if (!choice.compare("store") || !choice.compare("s")) {
            printf("Please input the key\n > ");
            std::getline(std::cin, key);
            printf("Now the value\n > ");
            std::getline(std::cin, value);
            if (client.put(key, value) == test::kvstore_service::StoreResponse::STORE_SUCCESS) {
                puts("Successfully stored");
            } else {
                puts("Something wrong during storing");
            }
        } else if (!choice.compare("read") || !choice.compare("r")) {
            printf("Please input the key\n > ");
            std::getline(std::cin, key);
            int ret = client.get(key, value);
            if (ret == test::kvstore_service::ReadResponse::READ_SUCCESS) {
                std::cout << "Value: " << value << std::endl;
            } else if (ret == test::kvstore_service::ReadResponse::READ_INEXISTS) {
                std::cout << "No value for key: " << key << std::endl;
            } else {
                puts("Something wrong during reading");
            }
        } else {
            puts("Unfamiliar command");
        }
        print_tips();
        std::getline(std::cin, choice);
    }
}

