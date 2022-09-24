/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef FS_FAT32_INCLUDE_OPENFSL_FAT32_INTERNAL_CLUSTERCHAIN_H_
#define FS_FAT32_INCLUDE_OPENFSL_FAT32_INTERNAL_CLUSTERCHAIN_H_

#ifndef __cplusplus
#error C++ compiler required.
#endif

#include <cstdint>
#include <cstddef>

#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include <condition_variable>
#include <utility>
#include <map>

#include "openfsl/blockdevice.h"
#include "openfsl/sector.h"
#include "openfsl/filesystem.h"
#include "openfsl/file.h"
#include "openfsl/fsltime.h"
#include "openfsl/endian.h"

namespace openfsl {
namespace fat32 {

class FAT32;
typedef uint32_t cluster_t;

/**
 * @brief FAT32 cluster chain
 */
class ClusterChain {
 private:
    /// File system class pointer
    FAT32* fileSystem;

    /// Sector cursor cache
    Sector* fetchedSectorCache = nullptr;
    cluster_t currentCacheClusterIndex = 0;
    cluster_t currentCacheClusterSize = 0;

    /// Cluster chain start point
    cluster_t startCluster;

    /// List of cluster chain members
    std::vector<cluster_t> clusterList;

    /// List of modified clusters
    std::map<cluster_t, Sector*> modifiedClusters;

    // Internal functions /////////////////////////////////////////////////
    int updateClusterList();
    cluster_t getNextCluster(const cluster_t cluster);

    lba48_t clusterToLBA(const cluster_t cluster);
    ///////////////////////////////////////////////////////////////////////
 public:
    // General class functions /////////////////////////////////////////////
    ClusterChain(FAT32* fileSystem_, const cluster_t startCluster_);
    ~ClusterChain();
    ///////////////////////////////////////////////////////////////////////


    // Data controls //////////////////////////////////////////////////////
    error_t __check_result fetch(
        const cluster_t cluster, const cluster_t clusterSize);
    error_t __check_result flush();
    error_t __check_result read(
        void* buf, const size_t pos, const size_t size);
    error_t __check_result write(
        const void* buf, const size_t pos, const size_t size);
    ///////////////////////////////////////////////////////////////////////


    // Cluster controls ///////////////////////////////////////////////////
    error_t __check_result addCluster(const cluster_t addCount);
    error_t __check_result removeCluster(const cluster_t removeCount);
    ///////////////////////////////////////////////////////////////////////


    // Getter / Setter ////////////////////////////////////////////////////
    cluster_t calculateLinkLength();
    inline std::vector<cluster_t>* getClusterList() { return &clusterList; }
    ///////////////////////////////////////////////////////////////////////


    // Cache controls /////////////////////////////////////////////////////
    error_t __check_result shrinkCache(const cluster_t clusterSize);
    ///////////////////////////////////////////////////////////////////////
};

}  // namespace fat32
}  // namespace openfsl

#endif  // FS_FAT32_INCLUDE_OPENFSL_FAT32_INTERNAL_CLUSTERCHAIN_H_
