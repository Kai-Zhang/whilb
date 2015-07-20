#include <cstdio>
#include <iostream>
#include <string>
#include <unistd.h>
#include <sofa/pbrpc/pbrpc.h>
#include "kvstore_proto.pb.h"

namespace test {
namespace kvstore_service {
class KVStore_client_async {
public:
    KVStore_client_async() {
        this->_rpc_client = NULL;
        this->_rpc_channel = NULL;
        this->_stub = NULL;
    }
    ~KVStore_client_async() {
           if (this->_stub != NULL) {
               delete this->_stub;
           }
           if (this->_rpc_channel != NULL) {
               delete this->_rpc_channel;
           }
           if (this->_rpc_client != NULL) {
               delete this->_rpc_client;
           }
    }

    void initialize(char *ip, char *port);

    bool *put(const std::string &key, const std::string &value, StoreResponse **response);

    bool *get(const std::string &key, ReadResponse **response);

    static void readcallback(sofa::pbrpc::RpcController *cntl,
                             ReadRequest *request,
                             ReadResponse *response,
                             bool *done);

    static void storecallback(sofa::pbrpc::RpcController *cntl,
                             StoreRequest *request,
                             StoreResponse *response,
                             bool *done);

private:
    sofa::pbrpc::RpcClient *_rpc_client;
    sofa::pbrpc::RpcChannel *_rpc_channel;
    KVStoreInterface_Stub *_stub;
};

void KVStore_client_async::initialize(char *ip, char *port) {
    SOFA_PBRPC_SET_LOG_LEVEL(NOTICE);

    sofa::pbrpc::RpcClientOptions options;
    this->_rpc_client = new sofa::pbrpc::RpcClient(options);

    std::string address = ip;
    address += ":";
    address += port;
    this->_rpc_channel = new sofa::pbrpc::RpcChannel(this->_rpc_client, address.c_str());

    this->_stub = new KVStoreInterface_Stub(this->_rpc_channel);
}

void KVStore_client_async::readcallback(sofa::pbrpc::RpcController *cntl,
                         ReadRequest *request,
                         ReadResponse* /*response*/,
                         bool *done) {
    SLOG(NOTICE, "RemoteAddress=%s", cntl->RemoteAddress().c_str());
    SLOG(NOTICE, "IsRequestSent=%s", cntl->IsRequestSent() ? "true" : "false");
    if (cntl->IsRequestSent())
    {
        SLOG(NOTICE, "LocalAddress=%s", cntl->LocalAddress().c_str());
        SLOG(NOTICE, "SentBytes=%ld", cntl->SentBytes());
    }

    if (cntl->Failed()) {
        SLOG(ERROR, "read request failed: %s", cntl->ErrorText().c_str());
    }
    else {
        SLOG(NOTICE, "read request succeed");
    }

    delete cntl;
    delete request;

    *done = true;
}

void KVStore_client_async::storecallback(sofa::pbrpc::RpcController *cntl,
                         StoreRequest *request,
                         StoreResponse* /*response*/,
                         bool *done) {
    SLOG(NOTICE, "RemoteAddress=%s", cntl->RemoteAddress().c_str());
    SLOG(NOTICE, "IsRequestSent=%s", cntl->IsRequestSent() ? "true" : "false");
    if (cntl->IsRequestSent())
    {
        SLOG(NOTICE, "LocalAddress=%s", cntl->LocalAddress().c_str());
        SLOG(NOTICE, "SentBytes=%ld", cntl->SentBytes());
    }

    if (cntl->Failed()) {
        SLOG(ERROR, "store request failed: %s", cntl->ErrorText().c_str());
    }
    else {
        SLOG(NOTICE, "store request succeed");
    }

    delete cntl;
    delete request;

    *done = true;
}

bool *KVStore_client_async::put(const std::string &key,
                                const std::string &value,
                                StoreResponse **response) {
    StoreRequest *request = new StoreRequest();
    request->set_key(key);
    request->set_value(value);

    *response = new StoreResponse();

    sofa::pbrpc::RpcController *controller = new sofa::pbrpc::RpcController();

    bool *result = new bool;
    *result = false;
    google::protobuf::Closure *done = sofa::pbrpc::NewClosure(
            &KVStore_client_async::storecallback, controller, request, *response, result);

    this->_stub->Store(controller, request, *response, done);

    return result;
}

bool *KVStore_client_async::get(const std::string &key, ReadResponse **response) {
    ReadRequest *request = new ReadRequest();
    request->set_key(key);

    *response = new ReadResponse();

    sofa::pbrpc::RpcController *controller = new sofa::pbrpc::RpcController();

    bool *result = new bool;
    *result = false;
    google::protobuf::Closure *done = sofa::pbrpc::NewClosure(
            &KVStore_client_async::readcallback, controller, request, *response, result);

    this->_stub->Read(controller, request, *response, done);

    return result;
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

    test::kvstore_service::KVStore_client_async client;
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
            test::kvstore_service::StoreResponse *response;
            bool *result = client.put(key, value, &response);
            while (!(*result)) {
                usleep(1000);
            }
            if (response->ret_value() == (int)test::kvstore_service::StoreResponse::STORE_SUCCESS) {
                puts("Successfully stored");
            } else {
                puts("Something wrong during storing");
            }
            delete response;
            delete result;
        } else if (!choice.compare("read") || !choice.compare("r")) {
            printf("Please input the key\n > ");
            std::getline(std::cin, key);
            test::kvstore_service::ReadResponse *response;
            bool *result = client.get(key, &response);
            while (!(*result)) {
                usleep(1000);
            }
            if (response->status() == test::kvstore_service::ReadResponse::READ_SUCCESS) {
                std::cout << "Value: " << response->value() << std::endl;
            } else if (response->status() == test::kvstore_service::ReadResponse::READ_INEXISTS) {
                std::cout << "No value for key: " << key << std::endl;
            } else {
                puts("Something wrong during reading");
            }
            delete response;
            delete result;
        } else {
            puts("Unfamiliar command");
        }
        print_tips();
        std::getline(std::cin, choice);
    }
}

