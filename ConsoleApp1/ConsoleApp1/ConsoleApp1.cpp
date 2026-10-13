#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cctype>

struct Pipe {
    std::string name;
    double length;
    double diameter;
    bool inRepair;
};

struct CompressorStation {
    std::string name;
    int totalShops;
    int activeShops;
    char stationClass;
};

bool isValidName(const std::string& s) {
    if (s.empty()) return false;
    for (unsigned char c : s) {
        if (std::isalnum(c)) continue;
        return false;
    }
    return true;
}
void inputPipeName(Pipe& pipe) {
    std::cin.ignore();
    std::cout << "Отметка: ";
    std::getline(std::cin, pipe.name);
}

void inputPipeLength(Pipe& pipe) {
    std::cout << "Длина (км): ";
    while (!(std::cin >> pipe.length) || pipe.length <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Ошибка! Введите положительное число: ";
    }
}

void inputPipeDiameter(Pipe& pipe) {
    std::cout << "Диаметр (мм): ";
    while (!(std::cin >> pipe.diameter) || pipe.diameter <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Ошибка! Введите положительное число: ";
    }
}

void inputPipeRepair(Pipe& pipe) {
    int flag;
    std::cout << "Труба в ремонте? (1 - да, 0 - нет): ";
    while (!(std::cin >> flag) || (flag != 0 && flag != 1)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Ошибка! Введите 1 или 0: ";
    }
    pipe.inRepair = flag;
}

void editPipeRepair(Pipe& pipe) {
    pipe.inRepair = !pipe.inRepair;
    std::cout << "Новый статус: "
        << (pipe.inRepair ? "в ремонте" : "исправна") << "\n";
}

void printPipe(const Pipe& pipe) {
    std::cout << "\nТруба\n"
        << "Отметка: " << pipe.name << "\n"
        << "Длина: " << pipe.length << "\n"
        << "Диаметр: " << pipe.diameter << "\n"
        << "В ремонте: "
        << (pipe.inRepair ? "да" : "нет") << "\n";
}

void inputCSName(CompressorStation& cs) {
    std::cin.ignore();
    std::cout << "Название КС: ";
    std::getline(std::cin, cs.name);
}

void inputCSTotalShops(CompressorStation& cs) {
    std::cout << "Количество цехов: ";
    while (!(std::cin >> cs.totalShops) || cs.totalShops <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Ошибка! Введите целое положительное число: ";
    }
}

void inputCSActiveShops(CompressorStation& cs) {
    std::cout << "Количество цехов в работе: ";
    while (!(std::cin >> cs.activeShops)
        || cs.activeShops < 0
        || cs.activeShops > cs.totalShops) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Ошибка! Введите число от 0 до " << cs.totalShops << ": ";
    }
}

void inputCSClass(CompressorStation& cs) {
    std::cout << "Класс станции (от 1 до 4): ";
    while (!(std::cin >> cs.stationClass)
        || cs.stationClass < '1'
        || cs.stationClass > '4') {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Ошибка! Введите число от 1 до 4: ";
    }
}

void printCS(const CompressorStation& cs) {
    std::cout << "\nКС\n"
        << "Название: " << cs.name << "\n"
        << "Всего цехов: " << cs.totalShops << "\n"
        << "В работе: " << cs.activeShops << "\n"
        << "Класс: " << cs.stationClass << "\n";
}
bool startShop(CompressorStation& cs) {
    if (cs.activeShops >= cs.totalShops) {
        std::cout << "Все цеха работают " << cs.activeShops << "/" << cs.totalShops << "\n";
        return false;
    }
    ++cs.activeShops;
    std::cout << "Цех запущен. Работает " << cs.activeShops << "/" << cs.totalShops << "\n";
    return true;
}

bool stopShop(CompressorStation& cs) {
    if (cs.activeShops <= 0) {
        std::cout << "Нет работающих цехов для остановки (" << cs.activeShops << "/" << cs.totalShops << ")\n";
        return false;
    }
    --cs.activeShops;
    std::cout << "Цех остановлен. Работает " << cs.activeShops << "/" << cs.totalShops << "\n";
    return true;
}


void saveToFileP(const Pipe& pipe, bool pipeEntered) {
    std::ofstream fout("dataP.txt");
    if (!fout) {
        std::cout << "Не удалось открыть файл для записи\n";
        return;
    }
    fout << pipeEntered << "\n";
    if (pipeEntered) {
        fout << pipe.name << "\n";
        fout << pipe.length << "\n";
        fout << pipe.diameter << "\n";
        fout << pipe.inRepair << "\n";
    }
    fout.close();
    std::cout << "Данные трубы сохранены в dataP.txt\n";
}

void savePipeToFile(const Pipe& pipe, bool pipeEntered) {
    std::ofstream fout("data.txt", std::ios::app);
    if (!fout) {
        std::cout << "Не удалось открыть файл для записи\n";
        return;
    }
    fout << "PIPE\n";
    fout << pipeEntered << "\n";
    if (pipeEntered) {
        fout << pipe.name << "\n";
        fout << pipe.length << "\n";
        fout << pipe.diameter << "\n";
        fout << pipe.inRepair << "\n";
    }
    fout.close();
    std::cout << "Данные трубы сохранены в data.txt\n";
}

void saveCSToFile(const CompressorStation& cs, bool csEntered) {
    std::ofstream fout("data.txt", std::ios::app);
    if (!fout) {
        std::cout << "Не удалось открыть файл для записи\n";
        return;
    }
    fout << "CS\n";
    fout << csEntered << "\n";
    if (csEntered) {
        fout << cs.name << "\n";
        fout << cs.totalShops << "\n";
        fout << cs.activeShops << "\n";
        fout << cs.stationClass << "\n";
    }
    fout.close();
    std::cout << "Данные КС сохранены в data.txt\n";
}

void loadFromFile(Pipe& pipe, bool& pipeEntered,
    CompressorStation& cs, bool& csEntered) {
    std::ifstream fin("data.txt");
    if (!fin) {
        std::cout << "Не удалось открыть файл для чтения\n";
        return;
    }

    std::string tag;
    while (fin >> tag) {
        if (tag == "PIPE") {
            fin >> pipeEntered;
            if (pipeEntered) {
                std::getline(fin >> std::ws, pipe.name);
                fin >> pipe.length;
                fin >> pipe.diameter;
                fin >> pipe.inRepair;
            }
            std::cout << "Данные трубы загружены из data.txt\n";
        }
        else if (tag == "CS") {
            fin >> csEntered;
            if (csEntered) {
                std::getline(fin >> std::ws, cs.name);
                fin >> cs.totalShops;
                fin >> cs.activeShops;
                fin >> cs.stationClass;
            }
            std::cout << "Данные КС загружены из data.txt\n";
        }
        else {
            std::cout << "Неизвестный тег в файле: " << tag << "\n";
        }
    }
    fin.close();
}

int main()
{
    setlocale(LC_ALL, "Russian");

    Pipe myPipe{};
    CompressorStation myCS{};
    bool pipeEntered = false;
    bool csEntered = false;

    int choice;
    do {
        std::cout << "\n Меню\n"
            << "1. Ввести трубу\n"
            << "2. Ввести КС\n"
            << "3. Показать все\n"
            << "4. Редактировать статус ремонта трубы\n"
            << "5. Запустить цех\n"
            << "6. Остановить цех\n"
            << "7. Сохранить данные трубы в файл\n"
            << "8. Сохранить данные КС в файл\n"
            << "9. Загрузить данные трубы из файла\n"
            << "10. Загрузить данные КС из файла\n"
            << "0. Выход\n"
            << "Выбор: ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка! Введите неотрицательное число: ";
            continue;
            }

        if (choice == 1) {
            inputPipeName(myPipe);
            inputPipeLength(myPipe);
            inputPipeDiameter(myPipe);
            inputPipeRepair(myPipe);
            pipeEntered = true;
        }
        else if (choice == 2) {
            inputCSName(myCS);
            inputCSTotalShops(myCS);
            inputCSActiveShops(myCS);
            inputCSClass(myCS);
            csEntered = true;
        }
        else if (choice == 3) {
            if (!pipeEntered && !csEntered) {
                std::cout << "Данных пока нет\n";
            }
            else {
                if (pipeEntered) printPipe(myPipe);
                if (csEntered) printCS(myCS);
            }
        }
        else if (choice == 4) {
            if (!pipeEntered) {
                std::cout << "Сначала введите трубу (1)\n";
            }
            else {
                editPipeRepair(myPipe);
            }
        }
        else if (choice == 5) {
            if (!csEntered) {
                std::cout << "Сначала введите КС (2)\n";
            }
            else {
                startShop(myCS);
            }
        }
        else if (choice == 6) {
            if (!csEntered) {
                std::cout << "Сначала введите КС (2)\n";
            }
            else {
                stopShop(myCS);
            }
        }
        else if (choice == 7) {
            savePipeToFile(myPipe, pipeEntered);
        }
        else if (choice == 8) {
            saveCSToFile(myCS, csEntered);
        }
        else if (choice == 9) {
            loadFromFile(myPipe, pipeEntered, myCS, csEntered);
        }
        else if (choice == 0) {
            std::cout << "Выход\n";
        }
        else {
            std::cout << "Неверный пункт меню.\n";
        }
    } while (choice != 0);
    return 0;
}