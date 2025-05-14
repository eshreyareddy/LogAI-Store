#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "engine/memtable.h"
#include "engine/wal.h"
#include "engine/sstable.h"
#include "engine/json.hpp"
#include "engine/metrics.h"
#include <ctime>

using json = nlohmann::json;

void appendMetricSnapshot(int memSize, int tombstoneCount, int sstableCount, int compactionCount) {
    std::ifstream inFile("data/metrics.json");
    json metrics = json::array();

    if (inFile.good()) {
        inFile >> metrics;
        inFile.close();
    }

    // ⏰ Timestamp
    time_t now = time(0);
    char* dt = ctime(&now);
    std::string timestamp(dt);
    timestamp.erase(timestamp.find_last_not_of(" \n\r\t")+1); // trim newline

    metrics.push_back({
        {"timestamp", timestamp},
        {"memtable_size", memSize},
        {"tombstone_count", tombstoneCount},
        {"sstable_count", sstableCount},
        {"compaction_count", compactionCount}
    });
    
    std::ofstream outFile("data/metrics.json");
    std::cout << "📊 Writing metrics snapshot...\n";
    outFile << metrics.dump(4);
    outFile.close();
}

void writeStatusJson(int memtableSize, int tombstoneCount, int compactionCount, const std::string& lastCompactionTime) {
    json j;
    j["memtable_size"] = memtableSize;
    j["tombstone_count"] = tombstoneCount;
    j["compaction_count"] = compactionCount;
    j["last_compaction_time"] = lastCompactionTime;

    std::ofstream file("data/status.json");
    file << j.dump(4);
    file.close();
}

int main() {
    MemTable mem;
    WAL wal;
    int sstableIndex = 0;
    std::vector<int> flushedSSTables;

    // Step 1: Replay WAL
    wal.replay(mem);

    // Step 2: Read command from commands.txt
    std::ifstream cmdFile("commands.txt");
    std::string command;
    if (cmdFile.is_open()) {
        std::getline(cmdFile, command);
        cmdFile.close();
    }

    std::istringstream iss(command);
    std::string action, key, value;
    iss >> action >> key;

    if (action == "put") {
        iss >> value;
        wal.logPut(key, value);
        mem.put(key, value);
        std::cout << "Inserted: " << key << " = " << value << "\n";
        appendMetricSnapshot(mem.size(), mem.getTombstones().size(), flushedSSTables.size(), compactionCount);
    } else if (action == "delete") {
        wal.logDelete(key);
        mem.del(key);
        std::cout << "Deleted: " << key << "\n";
        appendMetricSnapshot(mem.size(), mem.getTombstones().size(), flushedSSTables.size(), compactionCount);
    } else {
        std::cout << "No valid command found.\n";
    }

    // Step 3: Flush if needed
    if (mem.size() >= 3) {
        SSTable::flushToDisk(mem.getTable(), mem.getTombstones(), sstableIndex);
        flushedSSTables.push_back(sstableIndex++);
        mem.clear();
        mem.clearTombstones();
        appendMetricSnapshot(mem.size(), mem.getTombstones().size(), flushedSSTables.size(), compactionCount);
    }

    // Step 4: Search for the affected key (optional debug)
    std::string result = mem.get(key);
    if (result == "NOT_FOUND") {
        result = SSTable::search(flushedSSTables, key);
        std::cout << "[SSTable Fallback] " << key << ": " << result << "\n";
    } else {
        std::cout << "[MemTable Hit] " << key << ": " << result << "\n";
    }

    // Step 5: Compaction and metrics update
    SSTable::compact(flushedSSTables);
    appendMetricSnapshot(mem.size(), mem.getTombstones().size(), flushedSSTables.size(), compactionCount);

    SSTable::cleanupAndReset(flushedSSTables, sstableIndex);
    writeStatusJson(mem.size(), mem.getTombstones().size(), compactionCount, lastCompactionTime);

    // ✅ Final snapshot
    appendMetricSnapshot(mem.size(), mem.getTombstones().size(), flushedSSTables.size(), compactionCount);

    return 0;
}
