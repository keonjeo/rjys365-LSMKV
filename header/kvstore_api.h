#pragma once

#include <cstdint>
#include <string>
#include <list>

class KVStoreAPI {

public:
	KVStoreAPI(const std::string &dir) { }
	KVStoreAPI() = delete;

	virtual void scan(uint64_t key1, uint64_t key2, std::list<std::pair<uint64_t, std::string> > &list) = 0;
	virtual bool del(uint64_t key) = 0;
	virtual std::string get(uint64_t key) = 0;
	virtual void reset() = 0;
	virtual void put(uint64_t key, const std::string &s) = 0;
};

