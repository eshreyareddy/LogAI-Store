#include "memtable.h"
#include <iostream>

void MemTable::put(const std::string& key, const std::string& value) {
    table[key] = value;
}

std::string MemTable::get(const std::string& key) {
    if (table.find(key) != table.end())
        return table[key];
    else
        return "NOT_FOUND";
}

void MemTable::del(const std::string& key) {
    table.erase(key);               // Remove from memory
    tombstones.insert(key);        // Track deletion
}

int MemTable::size() {
    return table.size();
}

void MemTable::printAll() {
    for (const auto& pair : table) {
        std::cout << pair.first << " => " << pair.second << std::endl;
    }
}

std::map<std::string, std::string> MemTable::getTable() {
    return table;
}

void MemTable::clear() {
    table.clear();
}

std::set<std::string> MemTable::getTombstones() {
    return tombstones;
}

void MemTable::clearTombstones() {
    tombstones.clear();
}

