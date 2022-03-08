/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include <chrono>

#include "openfsl/fat32/fs_fat32.h"

error_t openfsl::FAT32::__insertCache(const std::string key,
    const CacheType type, const void* data, const size_t dataSize) {
    CacheEntry cache;
    cache.cacheType = type;
    cache.cacheCreated = cacheOpCount;

    cacheOpCountMutex.lock();
    cacheOpCount++;
    cacheOpCountMutex.unlock();

    switch (type) {
    case CacheType::PathClusterCache:
        cache.cacheData = new cluster_t;
        if (sizeof(cluster_t) != dataSize)
            return OPENFSL_ERROR_CACHE;
        memcpy(cache.cacheData, data, dataSize);
        break;
    default:
        return OPENFSL_ERROR_CACHE;
    }

    cacheMutex.lock();
    fsCache.insert(std::make_pair(key, cache));
    cacheMutex.unlock();

    cacheCvMutex.lock();
    cacheCv.notify_all();
    cacheCvMutex.unlock();

    return OPENFSL_SUCCESS;
}

std::pair<openfsl::FAT32::CacheType, void*>
    openfsl::FAT32::__getCacheValue(
            const std::string key, const CacheType cacheType) {
    cacheOpCountMutex.lock();
    size_t opcnt = cacheOpCount++;
    cacheOpCountMutex.unlock();

    cacheMutex.lock();
    if (fsCache.find(key) != fsCache.end()) {
        fsCache[key].cacheLastUsed = opcnt;
        fsCache[key].cacheReadCount++;

        std::pair<FAT32::CacheType, void*> ret =
            std::make_pair(fsCache[key].cacheType, fsCache[key].cacheData);
        if (fsCache[key].cacheType != CacheType::PathClusterCache) {
            goto cacheTypeNotMatch;
        }

        cacheMutex.unlock();
        return ret;
    } else {
cacheTypeNotMatch:
        cacheMutex.unlock();
        return std::make_pair(CacheType::None, nullptr);
    }
}

error_t openfsl::FAT32::__deleteCache(const std::string key) {
    cacheOpCountMutex.lock();
    cacheOpCount++;
    cacheOpCountMutex.unlock();

    cacheMutex.lock();

    if (fsCache.find(key) != fsCache.end()) {
        switch (fsCache[key].cacheType) {
        case CacheType::PathClusterCache:
            delete reinterpret_cast<cluster_t*>(fsCache[key].cacheData);
            break;
        default:
            return OPENFSL_ERROR_CACHE;
        }

        fsCache.erase(key);
    }
    cacheMutex.unlock();

    return OPENFSL_SUCCESS;
}

void openfsl::FAT32::__cacheMgrThreadRunner() {
    while (cacheEnabled) {
        std::unique_lock<std::mutex> lock(cacheCvMutex);
        cacheCv.wait(lock);
        lock.unlock();

        cacheMutex.lock();

        // If cache size is bigger than given maximum entry size
        if (fsCache.size() > cacheMaxEntry) {
            std::pair<size_t, std::map<std::string, CacheEntry>::iterator>
                oldestCache;  // first: cacheCreated
            cacheOpCountMutex.lock();
            for (std::map<std::string, CacheEntry>::iterator
                 i = fsCache.begin();
                 i != fsCache.end(); ++i) {

                if (i->second.cacheCreated < oldestCache.first) {
                    oldestCache = std::make_pair(i->second.cacheCreated, i);
                }

                if (i->second.cacheLastUsed < cacheOpCount - 40) {
                    // Remove cache which has been unused for a long time
                    fsCache.erase(i);
                    break;
                } else if (i->second.cacheReadCount < 20) {
                    // Remove cache which is rarely used
                    fsCache.erase(i);
                    break;
                }
            }
            cacheOpCountMutex.unlock();

            if (fsCache.size() > cacheMaxEntry) {
                // Remove old cache
                fsCache.erase(oldestCache.second);
            }
        }
        cacheMutex.unlock();
    }
}

error_t openfsl::FAT32::enableCache(const size_t cacheMaxEntry_) {
    if (!cacheEnabled) {
        fsOperationMutex.lock();
        cacheEnabled = true;

        cacheMgrThread =
            std::thread(&openfsl::FAT32::__cacheMgrThreadRunner, this);
        cacheMaxEntry = cacheMaxEntry_;
        cacheOpCount = 0;

        fsOperationMutex.unlock();
    }

    return OPENFSL_SUCCESS;
}

void openfsl::FAT32::disableCache() {
    if (cacheEnabled) {
        fsOperationMutex.lock();
        cacheEnabled = false;

        // Stop cache manager thread
        if (cacheMgrThread.joinable()) {
            cacheCvMutex.lock();
            cacheCv.notify_all();
            cacheCvMutex.unlock();

            cacheMgrThread.join();
        }

        // Unalloc all cache data
        for (std::map<std::string, CacheEntry>::iterator i = fsCache.begin();
                    i != fsCache.end();)
            __deleteCache((i++)->first);
        cacheMutex.lock();
        fsCache.clear();
        cacheMutex.unlock();

        cacheMaxEntry = 0;

        fsOperationMutex.unlock();
    }
}
