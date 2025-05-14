#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <conio.h>
#include "MyData.h"
#include "CMyDataFile.h"

bool is_int(std::string str, int& res) {
    if (str.empty()) return false;

    size_t start = 0;
    if (str[0] == '-' || str[0] == '+') start = 1;
    if (start == str.size()) return false; // только знак без цифр

    for (size_t i = start; i < str.size(); ++i) {
        if (!std::isdigit(str[i])) return false;
    }

    res = atoi(str.c_str());
    return true;
}

bool is_float(const std::string& str, float& res) {
    if (str.empty()) return false;

    size_t i = 0;
    bool hasDecimal = false;
    bool hasDigits = false;

    // Знак в начале
    if (str[i] == '+' || str[i] == '-') i++;

    for (; i < str.size(); ++i) {
        if (std::isdigit(str[i])) {
            hasDigits = true;
        }
        else if (str[i] == ',' && !hasDecimal) {
            hasDecimal = true;
        }
        else {
            return false; // Лишний символ
        }
    }

    if (hasDigits) {
        res = atof(str.c_str());
        return true;
    }
    else {
        return false;
    }
}
int GetInt(const std::string& prompt) {
    std::string value;
    int res = 0;
    while (true) {
        std::cout << prompt;
        getline(std::cin, value);

        if (is_int(value, res)) {
            return res;
        }
        else {
            std::cout << "\n\nНеверный ввод! Повторите попытку.\n\n";
        }
    }
}

float GetFloat(const std::string& prompt) {
    std::string value;
    float res;
    while (true) {
        std::cout << prompt;
        getline(std::cin, value);

        if (is_float(value, res)) {
            return res;
        }
        else {
            std::cout << "\n\nНеверный ввод! Повторите попытку.\n\n";
        }
    }
}

std::string GetPath(const std::string& prompt) {
    while (true) {
        std::string s;
        std::cout << prompt;
        std::getline(std::cin, s);

        if (s.find_first_of(".<>{}\\|/\"\'*$") != s.npos) {
            std::cout << "\n\nНеверный ввод! Повторите попытку.\n\n";
        }
        else {
            return s;
        }
    }
}

std::string GetString(const std::string& prompt) {
    std::string s;
    std::cout << prompt;
    std::getline(std::cin, s);
    return s;
}

std::vector<float> GetFloatArray() {
    int count = GetInt("Введите количество элементов массива: ");
    std::vector<float> arr(count);
    for (int i = 0; i < count; ++i) {
        arr[i] = GetFloat("Элемент " + std::to_string(i + 1) + ": ");
    }
    return arr;
}

void Menu() {
    CMyDataFile file;
    MyData data;
    std::string filename;

    while (true) {
        std::cout << "\n=== Меню ===\n"
            << "1. Записать файл\n"
            << "2. Прочитать файл\n"
            << "3. Изменить данные и сохранить\n"
            << "4. Выход\n\n";

        int choice = _getch();

        switch (choice) {
        case '1':
            filename = GetPath("Введите имя файла: ");
            data.intValue = GetInt("Введите целое число: ");
            data.floatValue = GetFloat("Введите число с плавающей точкой: ");
            data.strValue = GetString("Введите строку: ");
            data.floatArray = GetFloatArray();
            if (file.WriteData(filename, data))
                std::cout << "Файл успешно записан.\n";
            else
                std::cout << "Ошибка записи файла!\n";
            break;

        case '2':
            filename = GetPath("Введите имя файла: ");
            if (file.ReadData(filename, data)) {
                std::cout << "Целое число: " << data.intValue << "\n";
                std::cout << "Число с плавающей точкой: " << data.floatValue << "\n";
                std::cout << "Строка: " << data.strValue << "\n";
                std::cout << "Массив: ";
                for (float f : data.floatArray)
                    std::cout << f << " ";
                std::cout << "\n";
            }
            else {
                std::cout << "Не удалось прочитать файл или файл поврежден.\n";
            }
            break;

        case '3':
            filename = GetPath("Введите имя файла: ");
            if (!file.ReadData(filename, data)) {
                std::cout << "Файл не найден или поврежден. Сначала запишите файл.\n";
                break;
            }

            std::cout << "Редактирование данных:\n";
            data.intValue = GetInt("Новое целое число: ");
            data.floatValue = GetFloat("Новое число с плавающей точкой: ");
            data.strValue = GetString("Новая строка: ");
            data.floatArray = GetFloatArray();

            if (file.WriteData(filename, data))
                std::cout << "Файл успешно обновлён.\n";
            else
                std::cout << "Ошибка записи файла!\n";
            break;

        case '4':
            std::cout << "\n\nВыход из программы.\n";
            return;

        default:
            std::cout << "\n\nНеверный выбор. Попробуйте снова.\n\n";
        }
    }
}