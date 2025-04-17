// CFile.h
#pragma once
#include <fstream>

class CFile {
protected:
    std::fstream file;

public:
    bool Open(const std::string& filename, std::ios::openmode mode) {
        file.open(filename, mode | std::ios::binary);
        return file.is_open();
    }

    void Close() {
        if (file.is_open())
            file.close();
    }

    void Seek(std::streampos pos) {
        file.seekg(pos);
        file.seekp(pos);
    }

    std::streampos GetPosition() {
        return file.tellg();
    }

    void Write(const char* data, std::streamsize size) {
        file.write(data, size);
    }

    void Read(char* data, std::streamsize size) {
        file.read(data, size);
    }

    std::streamsize GetLength() {
        auto current = file.tellg();
        file.seekg(0, std::ios::end);
        auto length = file.tellg();
        file.seekg(current);  // восстановим позицию
        return length;
    }

    virtual ~CFile() {
        Close();
    }
};
