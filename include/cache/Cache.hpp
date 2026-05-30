#ifndef CACHE_HPP
#define CACHE_HPP

#include <mutex>
#include <unordered_map>
#include <queue>
#include <chrono>
#include <thread>
#include <atomic>

template <typename Value>
struct CacheItem
{
    Value value;
    std::chrono::steady_clock::time_point expiry;
    uint64_t version;
};

template <typename KeyType, typename ValueType>
class Cache
{
private:
    std::mutex mtx;
    std::thread cleanerThread;
    std::atomic<bool> running{false};
    int cleanerInterval = 5;

private:
    std::priority_queue<
        std::pair<std::chrono::steady_clock::time_point, KeyType>,
        std::vector<std::pair<std::chrono::steady_clock::time_point, KeyType>>,
        std::greater<std::pair<std::chrono::steady_clock::time_point, KeyType>>>
        pq;
    std::unordered_map<KeyType, CacheItem<ValueType>> m_cache_storage;

private:
    void periodicClean();

public:
    Cache(bool autoCleanup = false, int intervalSeconds = 5);
    void startCleaner();
    void put(const KeyType &key, const ValueType &value, long long expiry_ms = -1);
    void updateValue(const KeyType &key, const ValueType &value);
    ValueType get(const KeyType &key);
    bool exists(const KeyType &key);
    void cleanExpired();
    void stopCleaner();
    void remove(const KeyType &key);
    uint64_t getVersion(const KeyType &key)
    ~Cache();
};

#include "Cache.tpp"

#endif // CACHE_HPP
