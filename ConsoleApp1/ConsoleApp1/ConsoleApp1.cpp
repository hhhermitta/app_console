#include <iostream>
#include <fstream>
#include <string>
#include <limits>

struct Pipe {
    std::string name;
    double length;
    double diameter;
    bool inRepair;
};

int main()
{
    Pipe myPipe;
    std::cout << "Введите километровую отметку (name): ";
    std::cin.ignore();
    std::getline(std::cin, myPipe.name);

    std::cout << "Введите длину трубы в км (length): ";
    std::cin >> myPipe.length;

    std::cout << "Введите диаметр трубы в мм (diameter): ";
    std::cin >> myPipe.diameter;

    int repairFlag;
    std::cout << "Труба в ремонте? (inRepair, 1 - да, 0 - нет): ";
    std::cin >> repairFlag;
    myPipe.inRepair = repairFlag;

    std::cout << "\nВведённые данные:\n";
    std::cout << "name = " << myPipe.name<< "\n";
    std::cout << "length = " << myPipe.length << "\n";
    std::cout << "diameter = " << myPipe.diameter << "\n";
    std::cout << "inRepair = " << myPipe.inRepair << "\n";

    return 0;
}


