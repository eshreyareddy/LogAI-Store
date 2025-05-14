// engine/wal.cpp

#include "wal.h"
#include "memtable.h"
#include <fstream>
#include <iostream>

WAL::WAL(const std::string& filename) : filePath(filename) {
    walStream.open(filePath, std::ios::app); // append mode
}

WAL::~WAL() {
    walStream.close();
}

void WAL::logPut(const std::string& key, const std::string& value) {
    walStream << "PUT " << key << " " << value << "\n";
    walStream.flush();
}

void WAL::logDelete(const std::string& key) {
    walStream << "DEL " << key << "\n";
    walStream.flush();
}

void WAL::replay(MemTable& memtable) {
    std::ifstream infile(filePath);
    std::string operation, key, value;

    while (infile >> operation >> key) {
        if (operation == "PUT") {
            infile >> value;
            memtable.put(key, value);
        } else if (operation == "DEL") {
            memtable.del(key);
        }
    }
}
