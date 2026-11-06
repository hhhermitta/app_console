#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cctype>
#include <unordered_map>
#include <ctime>
#include <vector>
#include <algorithm>

static std::ofstream g_log;
void logAction(const std::string& msg) {
    if (g_log) {
        std::time_t t = std::time(nullptr);
        char buf[32];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
        g_log << "[" << buf << "]" << msg << "\n";
    }
}

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
        if (std::isalnum(c) || c == ' ' || c == ',' || c == '.' || c == '_' || c == '№')
            continue;
        return false;
    }
    return true;

}
void inputPipeName(Pipe& pipe) {
    std::cin.ignore();
    std::cout << "Отметка: ";
    std::getline(std::cin, pipe.name);
    while (!isValidName(pipe.name)) {
        std::cout << "Ошибка! Недопустимые символы. Разрешены буквы, цифры.\n";
        std::cout << "Отметка: ";
        std::getline(std::cin, pipe.name);
    }
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

void inputPipe(Pipe& pipe) {
    inputPipeName(pipe);
    inputPipeLength(pipe);
    inputPipeDiameter(pipe);
    inputPipeRepair(pipe);
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
    while (!isValidName(cs.name)) {
        std::cout << "Ошибка! Недопустимые символы. Разрешены буквы, цифры.\n";
        std::cout << "Название КС: ";
        std::getline(std::cin, cs.name);
    }
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

void inputCompressorStation(CompressorStation& cs) {
    inputCSName(cs);
    inputCSTotalShops(cs);
    inputCSActiveShops(cs);
    inputCSClass(cs);
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

using PipeMap = std::unordered_map<int, Pipe>;
using CSMap = std::unordered_map<int, CompressorStation>;

std::vector<int> findPipeByName(const PipeMap& pipes, const std::string& q) {
    std::vector<int> res;
    for (const auto& [id, p] : pipes)
        if (p.name.find(q) != std::string::npos) res.push_back(id);
    return res;
}

std::vector<int> findPipesByRepair(const PipeMap& pipes, bool inRepair) {
    std::vector<int> res;
    for (const auto& [id, p] : pipes)
        if (p.inRepair == inRepair) res.push_back(id);
    return res;
}

std::vector<int> findCSByName(const CSMap& css, const std::string& q) {
    std::vector<int> res;
    for (const auto& [id, c] : css)
        if (c.name.find(q) != std::string::npos) res.push_back(id);
    return res;
}

std::vector<int> findCSByIdlePercent(const CSMap& css, double idlePercent) {
    std::vector<int> res;
    for (const auto& [id, c] : css) {
        double idle = (c.totalShops - c.activeShops) * 100.0 / c.totalShops;
        if (std::abs(idle - idlePercent) < 1e-9) res.push_back(id);
    }
    return res;
}

void batchEditRepair(PipeMap& pipes, const std::vector<int>& ids) {
    for (int id : ids) {
        auto it = pipes.find(id);
        if (it != pipes.end()) {
            it->second.inRepair = !it->second.inRepair;
            logAction("batch edit pipe id=" + std::to_string(id));
        }
    }
}

void batchDeletePipes(PipeMap& pipes, const std::vector<int>& ids) {
    for (int id : ids) {
        pipes.erase(id);
        logAction("batch delete pipe id=" + std::to_string(id));
    }
}

void savePipeToFile(std::ostream& out, const PipeMap& pipes) {
    out << "PIPES\n";               
    out << pipes.size() << "\n";
    for (const auto& [id, pipe] : pipes) {
        out << id << "\n";
        out << pipe.name << "\n";
        out << pipe.length << "\n";
        out << pipe.diameter << "\n";
        out << pipe.inRepair << "\n";
    }
}

void saveCSToFile(std::ostream& out, const CSMap& css) {
    out << "CSS\n";
    out << css.size() << "\n";
    for (const auto& [id, cs] : css) {
        out << id << "\n";
        out << cs.name << "\n";
        out << cs.totalShops << "\n";
        out << cs.activeShops << "\n";
        out << cs.stationClass << "\n";
    }
}

void saveToFile(const std::string& filename, const PipeMap& pipes, const CSMap& css) {
    std::ofstream out(filename);
    if (!out) {
        std::cout << "Не удалось открыть файл для записи\n";
        logAction("save failed: " + filename);
        return;
    }
    savePipeToFile(out, pipes);
    saveCSToFile(out, css);
    logAction("saved to " + filename);
    std::cout << "Данные сохранены в " << filename << "\n";
}

bool loadPipeFromFile(std::istream& in, PipeMap& pipes) {
    std::string objType;
    std::getline(in, objType);
    if (objType != "PIPES") {
        std::cout << "Ошибка файла: данные труб не найдены\n";
        return false;
    }
    size_t n = 0;
    in >> n;
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    pipes.clear();
    for (size_t i = 0; i < n; ++i) {
        int id; Pipe p;
        in >> id;
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(in, p.name);
        in >> p.length >> p.diameter >> p.inRepair;
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        pipes[id] = p;
    }
    return true;
}

bool loadCSFromFile(std::istream& in, CSMap& css) {
    std::string objType;
    std::getline(in, objType);
    if (objType != "CSS") {
        std::cout << "Ошибка файла: данные КС не найдены\n";
        return false;
    }
    size_t n = 0;
    in >> n;
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    css.clear();
    for (size_t i = 0; i < n; ++i) {
        int id; CompressorStation c;
        in >> id;
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(in, c.name);
        in >> c.totalShops >> c.activeShops >> c.stationClass;
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        css[id] = c;
    }
    return true;
}

void loadFromFile(const std::string& filename, PipeMap& pipes, CSMap& css) {
    std::ifstream in(filename);

    if (!in) {
        std::cout << "Не удалось открыть файл для чтения\n";
        logAction("load failed: " + filename);
        return;
    }
    if (!loadPipeFromFile(in, pipes)) return;
    if (!loadCSFromFile(in, css)) return;
    logAction("loaded from " + filename);
    std::cout << "Данные загружены из " << filename << "\n";
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
            << "7. Сохранить данные в файл\n"
            << "8. Загрузить данные из файла\n"
            << "0. Выход\n"
            << "Выбор: ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка! Введите неотрицательное число: ";
            continue;
            }

        if (choice == 1) {
            inputPipe(myPipe);
            pipeEntered = true;
        }
        else if (choice == 2) {
            inputCompressorStation(myCS);
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
            std::ofstream fout("data.txt");
            if (!fout) {
                std::cout << "Не удалось открыть файл для записи\n";
            }
            else {
                saveToFile(fout, myPipe, pipeEntered, myCS, csEntered);
                fout.close();
                std::cout << "Данные сохранены в data.txt\n";
            }
        }
        else if (choice == 8) {
            std::ifstream fin("data.txt");
            if (!fin) {
                std::cout << "Не удалось открыть файл для чтения\n";
            }
            else {
                loadFromFile(fin, myPipe, pipeEntered, myCS, csEntered);
                fin.close();
            }
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