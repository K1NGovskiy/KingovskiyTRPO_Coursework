#pragma once
#include <iostream>
#include <vector>
#include "CFile.h"
#include "MyData.h"

class CMyDataFile : public CFile {  // Класс для сохранения и загрузки структуры MyData в/из бинарного файла
    const std::string magic = "MYBIN";  // Уникальная сигнатура (magic string), используемая для проверки формата файла

public:
    bool WriteData(const std::string& filename, const MyData& data) {   // Метод записи данных в бинарный файл
        if (!Open(filename, std::ios::out | std::ios::trunc)) return false; // Открываем файл для записи в бинарном режиме, предварительно очищая его

        // Записываем заголовок
        Write(magic.c_str(), 5);

        uint32_t strSize = data.strValue.size();    // Получаем размеры строки и массива для последующей записи
        uint32_t arrSize = data.floatArray.size();

        Write(reinterpret_cast<char*>(&strSize), sizeof(strSize));  // Записываем размеры строки и массива
        Write(reinterpret_cast<char*>(&arrSize), sizeof(arrSize));

        // Записываем целочисленное и вещественное значения
        Write(reinterpret_cast<const char*>(&data.intValue), sizeof(data.intValue));
        Write(reinterpret_cast<const char*>(&data.floatValue), sizeof(data.floatValue));

        Write(data.strValue.c_str(), strSize); // Записываем строку как массив символов (без завершающего нуля '\0')

        for (float f : data.floatArray) {   // Записываем каждый элемент массива float
            Write(reinterpret_cast<const char*>(&f), sizeof(f));
        }

        Close();
        return true;
    }

    bool ReadData(const std::string& filename, MyData& data) {  // Метод чтения данных из бинарного файла
        if (!Open(filename, std::ios::in)) return false;    // Открываем файл для чтения в бинарном режиме

        // Чтение заголовка
        char buffer[5];
        Read(buffer, 5);
        if (std::string(buffer, 5) != magic) return false;

        uint32_t strSize, arrSize;
        Read(reinterpret_cast<char*>(&strSize), sizeof(strSize)); // Читаем размеры строки и массива
        Read(reinterpret_cast<char*>(&arrSize), sizeof(arrSize));

        // Читаем целочисленное и вещественное значения
        Read(reinterpret_cast<char*>(&data.intValue), sizeof(data.intValue));
        Read(reinterpret_cast<char*>(&data.floatValue), sizeof(data.floatValue));

        data.strValue.resize(strSize);// Читаем строку: сначала выделяем нужное количество памяти, затем читаем данные
        Read(data.strValue.data(), strSize);

        data.floatArray.resize(arrSize); // Читаем массив float: выделяем память, затем поэлементно читаем
        for (uint32_t i = 0; i < arrSize; ++i) {
            Read(reinterpret_cast<char*>(&data.floatArray[i]), sizeof(float));
        }

        Close();
        return true;
    }
};
