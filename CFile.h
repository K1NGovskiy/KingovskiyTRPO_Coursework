// CFile.h
#pragma once
#include <fstream>

class CFile {
protected:
    std::fstream file;

public:
    bool Open(const std::string& filename, std::ios::openmode mode) {   // Открытие файла с заданным именем и режимом 
        file.open(filename, mode | std::ios::binary);
        return file.is_open();  // Возвращает true, если файл успешно открыт
    }

    void Close() {  // Закрытие файла, если он открыт
        if (file.is_open())
            file.close();
    }

    void Seek(std::streampos pos) { // Установка позиции указателя чтения и записи
        file.seekg(pos);    // Установка позиции чтения
        file.seekp(pos);    // Установка позиции записи
    }

    std::streampos GetPosition() {  // Получение текущей позиции указателя чтения
        return file.tellg();
    }

    void Write(const char* data, std::streamsize size) {    // Запись данных в файл
        file.write(data, size);
    }

    void Read(char* data, std::streamsize size) {   // Чтение данных из файла
        file.read(data, size);
    }

    std::streamsize GetLength() {   // Получение длины файла в байтах
        auto current = file.tellg();    // Сохраняем текущую позицию
        file.seekg(0, std::ios::end);   // Перемещаемся в конец файла
        auto length = file.tellg(); // Получаем позицию конца (размер файла)
        file.seekg(current);  // восстановим позицию
        return length;
    }

    virtual ~CFile() {
        Close();
    }
};
