#ifndef __TEST_KVSTORESERVICE_MEMORYPOOL_H__
#define __TEST_KVSTORESERVICE_MEMORYPOOL_H__
#include <map>
#include <string>

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
        TypeName(const TypeName&); \
        TypeName& operator=(const TypeName&)

namespace test {
namespace kvstore_service {
enum MemoryPoolAccessReturn {
    STORE_SUCCESS = 1,
    STORE_FAILURE = -1,

    READ_SUCCESS = 1,
    READ_INEXISTS = 0,
    READ_FAILURE = -1
};

class MemoryPool {
public:
    MemoryPool() { }
    virtual ~MemoryPool() { }

    virtual int store(const std::string &key, const std::string &value) {
        this->_map[key] = value;
        return STORE_SUCCESS;
    }
    
    virtual int read(const std::string &key, std::string &value) const {
		std::map<std::string, std::string>::const_iterator dst_iter = this->_map.find(key);
        if (dst_iter == this->_map.end()) {
            return READ_INEXISTS;
        }
        value = dst_iter->second;
        return READ_SUCCESS;
    }

private:
	std::map<std::string, std::string> _map;

    DISALLOW_COPY_AND_ASSIGN(MemoryPool);
};
}
}

#endif

