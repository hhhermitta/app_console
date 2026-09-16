#include <iostream>
#include <fstream>
#include <string>
#include <limits>

#include <clocale> 

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
    int stationClass;
};

int main()
{
    setlocale(LC_ALL, "Russian");

    Pipe myPipe;
    std::cout << "Введите километровую отметку (name): ";
    std::cin.ignore();
    std::getline(std::cin, myPipe.name);

    std::cout << "Введите длину трубы в км (length): ";
    while(!(std::cin>>myPipe.length) || (myPipe.length <= 0)) {
        std::cout << "Ошибка! Введите число: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cout << "Введите диаметр трубы в мм (diameter): ";
    while (!(std::cin >> myPipe.diameter) || (myPipe.diameter <= 0)) {
        std::cout << "Ошибка! Введите число: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    int repairFlag;
    std::cout << "Труба в ремонте? (inRepair, 1 - да, 0 - нет): ";
    while (!(std::cin >> repairFlag) || (repairFlag != 0 && repairFlag != 1)) {
        std::cout << "Ошибка! Введите 1 или 0: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    myPipe.inRepair = repairFlag;

    CompressorStation myCS;
    std::cout << "Введите название КС: ";
    std::cin.ignore();
    std::getline(std::cin, myCS.name);

    std::cout << "Введите количество цехов: ";
    while (!(std::cin >> myCS.totalShops) || (myCS.totalShops <= 0)) {
        std::cout << "Ошибка! Введите целое число: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cout << "Введите количество цехов в работе: ";
    while (!(std::cin >> myCS.activeShops) || (myCS.activeShops <= 0)) {
        std::cout << "Ошибка! Введите целое число: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cout << "\nВведённые данные:\n";
    std::cout << "name = " << myPipe.name<< "\n";
    std::cout << "length = " << myPipe.length << "\n";
    std::cout << "diameter = " << myPipe.diameter << "\n";
    std::cout << "inRepair = " << myPipe.inRepair << "\n";

    return 0;
}


