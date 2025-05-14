// engine/sstable.cpp

#include "sstable.h"
#include <fstream>
#include <iostream>
#include <set>
#include <filesystem>
#include <chrono>
#include <ctime>
#include "metrics.h"



void SSTable::flushToDisk(
    const std::map<std::string, std::string>& data,
    const std::set<std::string>& tombstones,
    int fileIndex)
{
    std::string filename = "data/sstable_" + std::to_string(fileIndex) + ".txt";
    std::ofstream outFile(filename);

    if (!outFile.is_open()) {
        std::cerr << "❌ Failed to open " << filename << " for writing.\n";
        return;
    }

    // Write all valid key-value pairs
    for (const auto& [key, value] : data) {
        outFile << key << " " << value << "\n";
    }

    // 🆕 Write tombstones
    for (const auto& key : tombstones) {
        outFile << key << " __TOMBSTONE__\n";
    }

    outFile.close();
    std::cout << "✅ Flushed MemTable to " << filename << "\n";
}


std::string SSTable::search(const std::vector<int>& fileIndices, const std::string& key) {
    for (int i : fileIndices) {
        std::string filename = "data/sstable_" + std::to_string(i) + ".txt";
        std::ifstream file(filename);

        if (!file.is_open()) continue;

        std::string k, v;
        while (file >> k >> v) {
            if (k == key) {
                if (v == "__TOMBSTONE__") return "NOT_FOUND";  // ✅ Skip tombstones
                return v;  // ✅ Return actual value
            }
        }        
    }
    return "NOT_FOUND";
}

void SSTable::compact(const std::vector<int>& fileIndices) {
    std::map<std::string, std::string> mergedData;

    for (int i : fileIndices) {
        std::string filename = "data/sstable_" + std::to_string(i) + ".txt";
        std::ifstream file(filename);
        if (!file.is_open()) continue;

        std::string key, value;
        while (file >> key >> value) {
            if (value == "__TOMBSTONE__") {
                mergedData.erase(key);  // ✅ Skip tombstone
            } else {
                mergedData[key] = value;  // ✅ Keep valid values
            }
        }

        file.close();
    }

    std::string output = "data/sstable_compacted.txt";
    std::ofstream outFile(output);
    if (!outFile.is_open()) {
        std::cerr << "Failed to write compacted SSTable.\n";
        return;
    }

    for (const auto& [key, value] : mergedData) {
        outFile << key << " " << value << "\n";
    }

    outFile.close();
    std::cout << "✅ Compacted SSTables to " << output << "\n";

    compactionCount++;

        // Track the current time for "last_compaction_time"
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    lastCompactionTime = std::ctime(&now_c);
    lastCompactionTime.pop_back(); // Remove the newline at the end

}


void SSTable::cleanupAndReset(const std::vector<int>& fileIndices, int& sstableIndex) {
    namespace fs = std::filesystem;

    // Step 1: Delete old SSTables
    for (int i : fileIndices) {
        std::string filename = "data/sstable_" + std::to_string(i) + ".txt";
        if (fs::exists(filename)) {
            fs::remove(filename);
            std::cout << "🗑 Deleted " << filename << "\n";
        }
    }

    // Step 2: Rename compacted file to sstable_0.txt
    std::string oldName = "data/sstable_compacted.txt";
    std::string newName = "data/sstable_0.txt";

    if (fs::exists(oldName)) {
        fs::rename(oldName, newName);
        std::cout << "🔁 Renamed " << oldName << " to " << newName << "\n";
    }

    // Step 3: Reset index to 1 (since 0 is now taken)
    sstableIndex = 1;
}
