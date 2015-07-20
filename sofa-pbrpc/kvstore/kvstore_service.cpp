#include "kvstore_service.h"
#include <sofa/pbrpc/pbrpc.h>
#include <string>
#include "kvstore.h"

int test::kvstore_service::KVStore_service::connect(char *port) {
    SOFA_PBRPC_SET_LOG_LEVEL(NOTICE);
    sofa::pbrpc::RpcServerOptions options;
    options.work_thread_num = 8;
    this->_rpc_server = new sofa::pbrpc::RpcServer(options);

    std::string address = "0.0.0.0:";
    address += port;
    if (!this->_rpc_server->Start(address.c_str())) {
        SLOG(ERROR, "Start server failed");
        return -1;
    }

    KVStoreInterface *kvservice = new KVStore();
    if (!this->_rpc_server->RegisterService(kvservice)) {
        SLOG(ERROR, "Register service failed");
        return -1;
    }

    return 0;
}
