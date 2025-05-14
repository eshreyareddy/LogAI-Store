#ifndef MEMTABLE_H
#define MEMTABLE_H

#include <string>
#include <map>
#include <set> // 🆕 Tombstone support

class MemTable {
public:
    void put(const std::string& key, const std::string& value);
    std::string get(const std::string& key);
    void del(const std::string& key);
    void printAll();
    int size();

    std::map<std::string, std::string> getTable();
    void clear();

    // 🆕 Tombstone support
    std::set<std::string> getTombstones();
    void clearTombstones();

private:
    std::map<std::string, std::string> table;
    std::set<std::string> tombstones;  // 🆕 Tracks deleted keys
};

#endif // MEMTABLE_H
