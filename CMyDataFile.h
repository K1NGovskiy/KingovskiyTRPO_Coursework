#pragma once
#include <iostream>
#include <vector>
#include "CFile.h"
#include "MyData.h"

class CMyDataFile : public CFile {
    const std::string magic = "MYBIN";

public:
    bool WriteData(const std::string& filename, const MyData& data) {
        if (!Open(filename, std::ios::out | std::ios::trunc)) return false;

        // Записываем заголовок
        Write(magic.c_str(), 5);

        uint32_t strSize = data.strValue.size();
        uint32_t arrSize = data.floatArray.size();

        Write(reinterpret_cast<char*>(&strSize), sizeof(strSize));
        Write(reinterpret_cast<char*>(&arrSize), sizeof(arrSize));

        // Данные
        Write(reinterpret_cast<const char*>(&data.intValue), sizeof(data.intValue));
        Write(reinterpret_cast<const char*>(&data.floatValue), sizeof(data.floatValue));

        Write(data.strValue.c_str(), strSize);

        for (float f : data.floatArray) {
            Write(reinterpret_cast<const char*>(&f), sizeof(f));
        }

        Close();
        return true;
    }

    bool ReadData(const std::string& filename, MyData& data) {
        if (!Open(filename, std::ios::in)) return false;

        // Чтение заголовка
        char buffer[5];
        Read(buffer, 5);
        if (std::string(buffer, 5) != magic) return false;

        uint32_t strSize, arrSize;
        Read(reinterpret_cast<char*>(&strSize), sizeof(strSize));
        Read(reinterpret_cast<char*>(&arrSize), sizeof(arrSize));

        // Данные
        Read(reinterpret_cast<char*>(&data.intValue), sizeof(data.intValue));
        Read(reinterpret_cast<char*>(&data.floatValue), sizeof(data.floatValue));

        data.strValue.resize(strSize);
        Read(data.strValue.data(), strSize);

        data.floatArray.resize(arrSize);
        for (uint32_t i = 0; i < arrSize; ++i) {
            Read(reinterpret_cast<char*>(&data.floatArray[i]), sizeof(float));
        }

        Close();
        return true;
    }
};
