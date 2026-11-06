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

std::vector<int> findPipesByName(const PipeMap& pipes, const std::string& q) {
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

std::string inputLine(const std::string& prompt) {
    std::cout << prompt;
    std::string s;
    std::getline(std::cin, s);
    return s;
}

std::vector<int> selectSubset(const std::vector<int>& found) {
    if (found.empty()) { std::cout << "Ничего не найдено\n"; return {}; }
    std::cout << "Найдено: ";
    for (int id : found) std::cout << id << " ";
    std::cout << "\n1 - все, 2 - выбрать вручную: ";
    int mode; std::cin >> mode;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (mode == 1) return found;
    std::vector<int> chosen;
    std::cout << "Введите id (0 - конец): ";
    while (true) {
        int id; std::cin >> id;
        if (!std::cin || id == 0) break;
        if (std::find(found.begin(), found.end(), id) != found.end())
            chosen.push_back(id);
        else std::cout << "id " << id << " не найден\n";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return chosen;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    g_log.open("app.log", std::ios::app);
    logAction("program started");

    PipeMap pipes;
    CSMap css;
    int nextPipeId = 1;
    int nextCSId = 1;

    int choice;
    do {
        std::cout << "\n Меню\n"
            << "1. Добавить трубу\n"
            << "2. Добавить КС\n"
            << "3. Показать все\n"
            << "4. Редактировать статус ремонта трубы\n"
            << "5. Запустить цех\n"
            << "6. Остановить цех\n"
            << "7. Сохранить данные в файл\n"
            << "8. Загрузить данные из файла\n"
            << "9. Поиск труб\n"
            << "10. Поиск КС\n"
            << "11. Пакетное редактирование труб\n"
            << "12. Удалить трубу\n"
            << "13. Удалить КС\n"
            << "0. Выход\n"
            << "Выбор: ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка! Введите неотрицательное число: ";
            continue;
            }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1) {
            Pipe p; inputPipe(p);
            pipes[nextPipeId++] = p;
            logAction("add pipe id=" + std::to_string(nextPipeId - 1));
        }
        else if (choice == 2) {
            CompressorStation c; inputCompressorStation(c);
            css[nextCSId++] = c;
            logAction("add cs id=" + std::to_string(nextCSId - 1));
        }
        else if (choice == 3) {
            if (pipes.empty() && css.empty()) {
                std::cout << "Данных пока нет\n";
            }
            else {
                for (const auto& [id, p] : pipes) {
                    std::cout << "id=" << id; printPipe(p);
                }
                for (const auto& [id, c] : css) {
                    std::cout << "id=" << id; printCS(c);
                }
            }
            logAction("show all");
        }
        else if (choice == 4) {
            if (pipes.empty()) {
                std::cout << "Сначала добавьте трубу (1)\n";
            }
            else {
                int id; std::cout << "id трубы: "; std::cin >> id;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                auto it = pipes.find(id);
                if (it != pipes.end()) {
                    editPipeRepair(it->second);
                    logAction("edit repair pipe id=" + std::to_string(id));
                } else std::cout << "Не найдено\n";
            }
        }
        else if (choice == 5) {
            if (css.empty()) {
                std::cout << "Сначала добавьте КС (2)\n";
            }
            else {
                int id; std::cout << "id КС: "; std::cin >> id;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                auto it = css.find(id);
                if (it != css.end()) {
                    startShop(it->second);
                    logAction("start shop cs id=" + std::to_string(id));
                } else std::cout << "Не найдено\n";
            }
        }
        else if (choice == 6) {
            if (css.empty()) {
                std::cout << "Сначала введите КС (2)\n";
            }
            else {
                int id; std::cout << "id КС: "; std::cin >> id;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                auto it = css.find(id);
                if (it != css.end()) {
                    stopShop(it->second);
                    logAction("stop shop cs id=" + std::to_string(id));
                }
                else std::cout << "Не найдено\n";
            }
        }
        else if (choice == 7) {
            std::string fn = inputLine("Имя файла для сохранения: "); // CHANGED
            saveToFile(fn, pipes, css);                               // CHANGED
        }
        else if (choice == 8) {
            std::string fn = inputLine("Имя файла для загрузки: "); // CHANGED
            loadFromFile(fn, pipes, css);                           // CHANGED
            nextPipeId = 1; nextCSId = 1;                           // NEW
            for (const auto& [id, _] : pipes) if (id >= nextPipeId) nextPipeId = id + 1; // NEW
            for (const auto& [id, _] : css)   if (id >= nextCSId)   nextCSId = id + 1; // NEW
        }
        else if (choice == 9) {                                     // NEW
            std::cout << "1 - по названию, 2 - по признаку 'в ремонте': ";
            int m; std::cin >> m;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::vector<int> found;
            if (m == 1) {
                std::string q = inputLine("Подстрока названия: ");
                found = findPipesByName(pipes, q);
            }
            else if (m == 2) {
                int r; std::cout << "1 - в ремонте, 0 - исправна: "; std::cin >> r;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                found = findPipesByRepair(pipes, r == 1);
            }
            logAction("search pipes, found=" + std::to_string(found.size())); // NEW
            for (int id : found) { std::cout << "id=" << id; printPipe(pipes[id]); } // NEW
        }
        else if (choice == 10) {                                    // NEW
            std::cout << "1 - по названию, 2 - по % незадействованных цехов: ";
            int m; std::cin >> m;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::vector<int> found;
            if (m == 1) {
                std::string q = inputLine("Подстрока названия: ");
                found = findCSByName(css, q);
            }
            else if (m == 2) {
                double p; std::cout << "Процент (0..100): "; std::cin >> p;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                found = findCSByIdlePercent(css, p);
            }
            logAction("search cs, found=" + std::to_string(found.size())); // NEW
            for (int id : found) { std::cout << "id=" << id; printCS(css[id]); } // NEW
        }
        else if (choice == 11) {                                    // NEW
            std::cout << "1 - по названию, 2 - по признаку 'в ремонте': ";
            int m; std::cin >> m;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::vector<int> found;
            if (m == 1) {
                std::string q = inputLine("Подстрока названия: ");
                found = findPipesByName(pipes, q);
            }
            else if (m == 2) {
                int r; std::cout << "1 - в ремонте, 0 - исправна: "; std::cin >> r;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                found = findPipesByRepair(pipes, r == 1);
            }
            auto subset = selectSubset(found);
            if (subset.empty()) break;
            std::cout << "1 - переключить ремонт, 2 - удалить: ";
            int act; std::cin >> act;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (act == 1) batchEditRepair(pipes, subset);
            else if (act == 2) batchDeletePipes(pipes, subset);
            logAction("batch pipe action=" + std::to_string(act) +
                " count=" + std::to_string(subset.size()));
        }
        else if (choice == 12) {                                    // NEW
            int id; std::cout << "id трубы для удаления: "; std::cin >> id;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (pipes.erase(id)) { std::cout << "Удалено\n"; logAction("delete pipe id=" + std::to_string(id)); }
            else std::cout << "Не найдено\n";
        }
        else if (choice == 13) {                                    // NEW
            int id; std::cout << "id КС для удаления: "; std::cin >> id;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (css.erase(id)) { std::cout << "Удалено\n"; logAction("delete cs id=" + std::to_string(id)); }
            else std::cout << "Не найдено\n";
        }
        else if (choice == 0) {
            std::cout << "Выход\n";
            logAction("exit");
        }
        else {
            std::cout << "Неверный пункт меню.\n";
        }
    } while (choice != 0);
    g_log.close();                                          // NEW
    return 0;
}