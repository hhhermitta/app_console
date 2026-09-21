#include <iostream>
#include <fstream>
#include <string>
#include <limits>

struct Pipe {
    std::string name;
    double length;
    double diameter;//!! в мм
    bool inRepair;
};

struct CompressorStation {
    std::string name;
    int totalShops;
    int activeShops;
    char stationClass;
};

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

void saveToFile(const Pipe& pipe, const CompressorStation& cs, bool pipeEntered, bool csEntered) {
    std::ofstream fout("data.txt");
    if (!fout) {
        std::cout << "Не удалось открыть файл для записи\n";
        return;
    }
    fout << pipeEntered << "\n";
    if (pipeEntered) {//!!
        fout << pipe.name << "\n";
        fout << pipe.length << "\n";
        fout << pipe.diameter << "\n";
        fout << pipe.inRepair << "\n";
    }
    fout << csEntered << "\n";
    if (csEntered) {//!!!
        fout << cs.name << "\n";
        fout << cs.totalShops << "\n";
        fout << cs.activeShops << "\n";
        fout << cs.stationClass << "\n";
    }
    fout.close();
    std::cout << "Данные сохранены в data.txt\n";
}

void loadFromFile(Pipe& pipe, CompressorStation& cs, bool& pipeEntered, bool& csEntered) {
    std::ifstream fin("data.txt");
    if (!fin) {
        std::cout << "Не удалось открыть файл для чтения\n";
        return;
    }
    fin >> pipeEntered;
    if (pipeEntered) {//!!
        std::getline(fin>>std::ws, pipe.name);
        fin >> pipe.length;
        fin >> pipe.diameter;
        fin >> pipe.inRepair;
    }
    fin >> csEntered;
    if (csEntered) {//!!!
        std::getline(fin>>std::ws, cs.name);
        fin >> cs.totalShops;
        fin >> cs.activeShops;
        fin >> cs.stationClass;
    }
    fin.close();
    std::cout << "Данные загружены из data.txt\n";
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
            << "7. Сохранить в файл\n"
            << "8. Загрузить данные из файла\n"
            << "0. Выход\n"
            << "Выбор: ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка! Введите неотрицательное число: ";
            continue;
            }

        switch (choice) {
            //!! для каждой структуры (pipe и cs) свои функции.
        case 1: {
            std::cin.ignore();
            std::cout << "Отметка: ";
            std::getline(std::cin, myPipe.name);

            std::cout << "Длина (км): ";
            while (!(std::cin >> myPipe.length) || myPipe.length <= 0) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Ошибка! Введите неотрицательное число: ";
            }

            std::cout << "Диаметр (мм): ";
            while (!(std::cin >> myPipe.diameter) || myPipe.diameter <= 0) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Ошибка! Введите неотрицательное число: ";
            }

            int flag;
            std::cout << "Труба в ремонте? (1 - да, 0 - нет): ";
            while (!(std::cin >> flag) || (flag != 0 && flag != 1)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Ошибка! Введите 1 или 0: ";
            }
            myPipe.inRepair = flag;
            pipeEntered = true;
            break;
        }

        case 2: {
            std::cin.ignore();
            std::cout << "Название КС: ";
            std::getline(std::cin, myCS.name);

            std::cout << "Количество цехов: ";
            while (!(std::cin >> myCS.totalShops) || myCS.totalShops <= 0) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Ошибка! Введите целое неотрицательное число: ";
            }

            std::cout << "Количество цехов в работе: ";
            while (!(std::cin >> myCS.activeShops) || myCS.activeShops < 0 || myCS.activeShops > myCS.totalShops) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Ошибка! Введите число от 0 до " << myCS.totalShops << ": ";
            }

            std::cout << "Класс станции (от 1 до 4): ";
            while (!(std::cin >> myCS.stationClass) || myCS.stationClass < '1' || myCS.stationClass > '4') {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Ошибка! Введите число от 1 до 4: ";
            }

            csEntered = true;
            break;
        }

        case 3:
            if (!pipeEntered && !csEntered) {
                std::cout << "Данных пока нет\n";
                break;
            }
            if (pipeEntered) {
                std::cout << "\nТруба\n"
                    << "Отметка: " << myPipe.name << "\n"
                    << "Длина: " << myPipe.length << "\n"
                    << "Диаметр: " << myPipe.diameter << "\n"
                    << "В ремонте: "
                    << (myPipe.inRepair ? "да" : "нет") << "\n";
            }
            if (csEntered) {
                std::cout << "\nКС\n"
                    << "Название: " << myCS.name << "\n"
                    << "Всего цехов: " << myCS.totalShops << "\n"
                    << "В работе: " << myCS.activeShops << "\n"
                    << "Класс: " << myCS.stationClass << "\n";
            }
            break;

        case 4:
            if (!pipeEntered) {
                std::cout << "Сначала введите трубу (1)\n";
                break;
            }
            myPipe.inRepair = !myPipe.inRepair;
            std::cout << "Новый статус: "
                << (myPipe.inRepair ? "в ремонте" : "исправна") << "\n";
            break;

        case 5:
            if (!csEntered) {
                std::cout << "Сначала введите КС (2)\n";
                break;
            }
            startShop(myCS);
            break;

        case 6:
            if (!csEntered) {
                std::cout << "Сначала введите КС (2)\n";
                break;
            }
            stopShop(myCS);
            break;

        case 7:
            saveToFile(myPipe, myCS, pipeEntered, csEntered);
            break;

        case 8:
            loadFromFile(myPipe, myCS, pipeEntered, csEntered);
            break;

        case 0:
            std::cout << "Выход\n";
            break;

        default:
            std::cout << "Неверный пункт меню.\n";
            break;
        }
    } while (choice != 0);
    return 0;
}