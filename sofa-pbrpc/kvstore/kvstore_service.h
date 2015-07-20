#ifndef __TEST_KVSTORESERVICE_KVSTORESERVICE_H__
#define __TEST_KVSTORESERVICE_KVSTORESERVICE_H__

#include "kvstore_proto.pb.h"
#include "kvstore.h"

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
        TypeName(const TypeName&); \
        TypeName& operator=(const TypeName&)

namespace test {
namespace kvstore_service {
class KVStore_service {
public:
    KVStore_service() { }
    virtual ~KVStore_service() { }

    int connect(char *port);

    void run() { this->_rpc_server->Run(); }

    void stop() { this->_rpc_server->Stop(); delete this->_rpc_server; }

private:
    sofa::pbrpc::RpcServer *_rpc_server;

    DISALLOW_COPY_AND_ASSIGN(KVStore_service);
};
}
}

#endif
