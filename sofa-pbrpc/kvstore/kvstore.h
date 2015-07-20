#ifndef __TEST_KVSTORESERVICE_KVSTORE_H__
#define __TEST_KVSTORESERVICE_KVSTORE_H__

#include <sofa/pbrpc/pbrpc.h>
#include "kvstore_proto.pb.h"
#include "memory_pool.h"

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
        TypeName(const TypeName&); \
        TypeName& operator=(const TypeName&)

namespace test {
namespace kvstore_service {
class KVStore : public KVStoreInterface {
public:
    KVStore() { }
    virtual ~KVStore() { }

    virtual void Read(google::protobuf::RpcController* /*controller*/,
                      const test::kvstore_service::ReadRequest *request,
                      test::kvstore_service::ReadResponse *response,
                      google::protobuf::Closure *done) {
        // Access memory pool for reading
        std::string read_rst;
        int ret_value = this->_memory_pool.read(request->key(), read_rst);
        response->set_status((ReadResponse_ReadStatus)ret_value);
        if (ret_value == ReadResponse::READ_SUCCESS) {
            response->set_value(read_rst);
        }

        done->Run();
    }

    virtual void Store(google::protobuf::RpcController* /*controller*/,
                      const test::kvstore_service::StoreRequest *request,
                      test::kvstore_service::StoreResponse *response,
                      google::protobuf::Closure *done) {
        // Access memory pool for writing
        response->set_ret_value((StoreResponse_StoreStatus)
                (this->_memory_pool.store(request->key(), request->value())));
        
        done->Run();
    }

private:
    MemoryPool _memory_pool;

    DISALLOW_COPY_AND_ASSIGN(KVStore);
};
}
}

#endif

