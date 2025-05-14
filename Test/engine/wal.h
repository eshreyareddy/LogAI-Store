// engine/wal.h

#ifndef WAL_H
#define WAL_H

#include <string>
#include <fstream>

class MemTable; // forward declaration

class WAL {
public:
    WAL(const std::string& filename = "data/wal.log");
    ~WAL();

    void logPut(const std::string& key, const std::string& value);
    void logDelete(const std::string& key);
    void replay(MemTable& memtable);

private:
    std::ofstream walStream;
    std::string filePath;
};

#endif // WAL_H
