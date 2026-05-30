#ifndef CACHE_TPP
#define CACHE_TPP

template <typename KeyType, typename ValueType>
void Cache<KeyType, ValueType>::periodicClean()
{
    auto nextRun = std::chrono::steady_clock::now();

    while (running)
    {
        cleanExpired();

        nextRun += std::chrono::seconds(cleanerInterval);
        std::this_thread::sleep_until(nextRun);
    }
}

template <typename KeyType, typename ValueType>
Cache<KeyType, ValueType>::Cache(bool autoCleanup, int intervalSeconds)
{
    cleanerInterval = intervalSeconds;

    if (autoCleanup)
    {
        startCleaner();
    }
}

template <typename KeyType, typename ValueType>
void Cache<KeyType, ValueType>::startCleaner()
{
    if (running.exchange(true))
    {
        return;
    }

    cleanerThread = std::thread(&Cache::periodicClean, this);
}

template <typename KeyType, typename ValueType>
void Cache<KeyType, ValueType>::put(const KeyType &key, const ValueType &value, long long expiry_ms)
{
    std::lock_guard<std::mutex> lock(mtx);

    CacheItem<ValueType> item;
    item.value = value;
    item.version++;
    if (expiry_ms == -1)
    {
        item.expiry = std::chrono::steady_clock::time_point::max();
    }
    else
    {
        item.expiry = std::chrono::steady_clock::now() +
                      std::chrono::milliseconds(expiry_ms);

        pq.push({item.expiry, key});
    }
    m_cache_storage[key] = item;
}

template <typename KeyType, typename ValueType>
void Cache<KeyType, ValueType>::updateValue(const KeyType &key, const ValueType &value)
{
    std::lock_guard<std::mutex> lock(mtx);

    auto it = m_cache_storage.find(key);

    if (it != m_cache_storage.end())
    {
        it->second.version++;
        it->second.value = value;
    }
}
template <typename KeyType, typename ValueType>
uint64_t Cache<KeyType , ValueType>::getVersion(const KeyType &key){
    
    std::lock_guard<std::mutex> lock(mtx);
    auto it = m_cache_storage.find(key);

    if (it == m_cache_storage.end())
    {
        return 0;
    }
    return it->second.version;
}

template <typename KeyType, typename ValueType>
ValueType Cache<KeyType, ValueType>::get(const KeyType &key)
{
    std::lock_guard<std::mutex> lock(mtx);
    auto it = m_cache_storage.find(key);

    if (it == m_cache_storage.end())
    {
        return ValueType{};
    }

    if (std::chrono::steady_clock::now() >= it->second.expiry)
    {
        m_cache_storage.erase(it);
        return ValueType{};
    }
    return it->second.value;
}

template <typename KeyType, typename ValueType>
bool Cache<KeyType, ValueType>::exists(const KeyType &key)
{
    std::lock_guard<std::mutex> lock(mtx);
    auto it = m_cache_storage.find(key);

    if (it == m_cache_storage.end())
    {
        return false;
    }

    if (std::chrono::steady_clock::now() >= it->second.expiry)
    {
        m_cache_storage.erase(it);
        return false;
    }

    return true;
}

template <typename KeyType, typename ValueType>
void Cache<KeyType, ValueType>::cleanExpired()
{
    std::lock_guard<std::mutex> lock(mtx);
    while (!pq.empty())
    {
        std::pair<std::chrono::steady_clock::time_point, KeyType> minele = {pq.top().first, pq.top().second};
        if (minele.first > std::chrono::steady_clock::now())
        {
            break;
        }
        pq.pop();

        auto it = m_cache_storage.find(minele.second);

        // Lazy deletion check
        if (it != m_cache_storage.end() &&
            it->second.expiry == minele.first)
        {
            m_cache_storage.erase(it);
        }
    }
}

template <typename KeyType, typename ValueType>
void Cache<KeyType, ValueType>::stopCleaner()
{
    running = false;

    if (cleanerThread.joinable())
    {
        cleanerThread.join();
    }
}

template <typename KeyType, typename ValueType>
void Cache<KeyType, ValueType>::remove(const KeyType &key)
{
    std::lock_guard<std::mutex> lock(mtx);
    m_cache_storage.erase(key);
}

template <typename KeyType, typename ValueType>
Cache<KeyType, ValueType>::~Cache()
{
    stopCleaner();
}

#endif // CACHE_TPP
