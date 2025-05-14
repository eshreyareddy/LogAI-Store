// engine/sstable.h

#ifndef SSTABLE_H
#define SSTABLE_H

#include <map>
#include <set>
#include <string>
#include <vector>

class SSTable {
public:
static void flushToDisk(
    const std::map<std::string, std::string>& data,
    const std::set<std::string>& tombstones,
    int fileIndex);
    static std::string search(const std::vector<int>& fileIndices, const std::string& key);
    static void compact(const std::vector<int>& fileIndices);  // 🆕
    static void cleanupAndReset(const std::vector<int>& fileIndices, int& sstableIndex);

};

#endif // SSTABLE_H
