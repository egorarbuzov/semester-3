#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <ctime>

static const int MAX_CITIES = 20;
static const int MAX_POP = 2048;
static const int INF = 1000000000;
static const int NAME_LEN = 32;

struct Graph {
    int n;
    int w[MAX_CITIES][MAX_CITIES];
    char name[MAX_CITIES][NAME_LEN];
};

struct Individual {
    int tour[MAX_CITIES];
    int length;
};

struct GAParams {
    int populationSize;
    int childrenPerGeneration;
    int generations;
    int mutationPercent;
};

static bool inputClosed = false;

static void readLine(char* buf, int size) {
    std::cin.getline(buf, size);
    int len = (int)std::strlen(buf);
    if (len > 0 && buf[len - 1] == '\r') {
        buf[--len] = '\0';
    }
    bool hasContent = (buf[0] != '\0');
    if (std::cin.eof()) {
        if (!hasContent) {
            inputClosed = true;
        }
        std::cin.clear();
        return;
    }
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
}

static int readInt(const char* prompt, int defaultValue) {
    char buf[64];
    std::cout << prompt;
    readLine(buf, sizeof(buf));
    if (buf[0] == '\0') {
        return defaultValue;
    }
    std::istringstream input(buf);
    int value = 0;
    if (!(input >> value)) {
        return defaultValue;
    }
    return value;
}

static void initGraph(Graph& g, int n) {
    g.n = n;
    for (int i = 0; i < MAX_CITIES; ++i) {
        g.name[i][0] = '\0';
        for (int j = 0; j < MAX_CITIES; ++j) {
            g.w[i][j] = (i == j) ? 0 : -1;
        }
    }
}

static void setCityName(Graph& g, int index, const char* text) {
    int i = 0;
    while (text[i] != '\0' && i < NAME_LEN - 1) {
        g.name[index][i] = text[i];
        ++i;
    }
    g.name[index][i] = '\0';
}

static void loadDemo5(Graph& g) {
    const int demoW[5][5] = {
        {  0, 25, 40, 31, 27 },
        {  5,  0, 17, 30, 25 },
        { 19, 15,  0,  6,  1 },
        {  9, 50, 24,  0,  6 },
        { 22,  8,  7, 10,  0 }
    };
    initGraph(g, 5);
    for (int i = 0; i < 5; ++i) {
        std::ostringstream nameStream;
        nameStream << (i + 1);
        setCityName(g, i, nameStream.str().c_str());
        for (int j = 0; j < 5; ++j) {
            g.w[i][j] = demoW[i][j];
        }
    }
}

static void loadGraph8(Graph& g) {
    const int m[8][8] = {
        {  0, 25, 40, 31, 27, 12, 35, 44 },
        {  5,  0, 17, 30, 25,  9, 28, 37 },
        { 19, 15,  0,  6,  1, 11, 23, 33 },
        {  9, 50, 24,  0,  6, 15, 27, 29 },
        { 22,  8,  7, 10,  0, 13, 18, 25 },
        { 18, 21, 13, 19, 14,  0, 17, 26 },
        { 20, 16, 17, 22, 12, 21,  0, 19 },
        { 14, 26, 24, 31, 23, 15, 15,  0 }
    };
    initGraph(g, 8);
    for (int i = 0; i < 8; ++i) {
        std::ostringstream nameStream;
        nameStream << (i + 1);
        setCityName(g, i, nameStream.str().c_str());
        for (int j = 0; j < 8; ++j) {
            g.w[i][j] = m[i][j];
        }
    }
}

static void printCityList(const Graph& g) {
    std::cout << "Города: ";
    for (int i = 0; i < g.n; ++i) {
        std::cout << (i + 1) << ")" << g.name[i] << " ";
    }
    std::cout << "\n";
}

static void printGraph(const Graph& g) {
    std::cout << "\nГраф, городов: " << g.n << "\n";
    std::cout << "Матрица смежности (0 — дуги нет, '-' — диагональ):\n";
    std::cout << "       ";
    for (int j = 0; j < g.n; ++j) {
        std::cout << std::setw(6) << g.name[j];
    }
    std::cout << "\n";
    for (int i = 0; i < g.n; ++i) {
        std::cout << std::setw(6) << g.name[i] << " ";
        for (int j = 0; j < g.n; ++j) {
            if (i == j) {
                std::cout << std::setw(6) << "-";
            } else if (g.w[i][j] < 0) {
                std::cout << std::setw(6) << "0";
            } else {
                std::cout << std::setw(6) << g.w[i][j];
            }
        }
        std::cout << "\n";
    }
    std::cout << "Список дуг:\n";
    for (int i = 0; i < g.n; ++i) {
        for (int j = 0; j < g.n; ++j) {
            if (i != j && g.w[i][j] >= 0) {
                std::cout << "  " << g.name[i] << " -> " << g.name[j]
                          << " : " << g.w[i][j] << "\n";
            }
        }
    }
}

static void addCity(Graph& g) {
    if (g.n >= MAX_CITIES) {
        std::cout << "Достигнут предел " << MAX_CITIES << " городов.\n";
        return;
    }
    char nameBuf[NAME_LEN];
    std::cout << "Имя нового города: ";
    readLine(nameBuf, sizeof(nameBuf));
    if (nameBuf[0] == '\0') {
        std::cout << "Имя не задано, город не добавлен.\n";
        return;
    }

    int newIndex = g.n;
    printCityList(g);
    std::cout << "Для каждой пары дорог введите вес или -1 (дороги нет).\n";
    for (int j = 0; j < newIndex; ++j) {
        std::ostringstream outPrompt;
        outPrompt << "Дорога " << g.name[j] << " -> новый город: ";
        int wOut = readInt(outPrompt.str().c_str(), -1);
        std::ostringstream inPrompt;
        inPrompt << "Дорога новый город -> " << g.name[j] << ": ";
        int wIn = readInt(inPrompt.str().c_str(), -1);
        g.w[j][newIndex] = (wOut > 0) ? wOut : -1;
        g.w[newIndex][j] = (wIn > 0) ? wIn : -1;
    }
    g.w[newIndex][newIndex] = 0;
    setCityName(g, newIndex, nameBuf);
    g.n = newIndex + 1;
    std::cout << "Город " << nameBuf << " добавлен. Теперь городов: " << g.n << ".\n";
}

static void removeCity(Graph& g) {
    if (g.n <= 2) {
        std::cout << "Нельзя удалить город: должно остаться хотя бы 2 города.\n";
        return;
    }
    printCityList(g);
    int index = readInt("Номер города для удаления: ", -1) - 1;
    if (index < 0 || index >= g.n) {
        std::cout << "Неверный номер города.\n";
        return;
    }
    char removedName[NAME_LEN];
    std::strcpy(removedName, g.name[index]);

    for (int i = index; i < g.n - 1; ++i) {
        for (int j = 0; j < g.n; ++j) {
            g.w[i][j] = g.w[i + 1][j];
        }
        setCityName(g, i, g.name[i + 1]);
    }
    for (int j = index; j < g.n - 1; ++j) {
        for (int i = 0; i < g.n - 1; ++i) {
            g.w[i][j] = g.w[i][j + 1];
        }
    }
    --g.n;
    for (int i = 0; i < g.n; ++i) {
        g.w[i][i] = 0;
    }
    std::cout << "Город " << removedName << " удалён. Теперь городов: " << g.n << ".\n";
}

static void addRoad(Graph& g) {
    printCityList(g);
    int from = readInt("От какого города (номер): ", -1) - 1;
    int to = readInt("До какого города (номер): ", -1) - 1;
    if (from < 0 || from >= g.n || to < 0 || to >= g.n) {
        std::cout << "Неверный номер города.\n";
        return;
    }
    if (from == to) {
        std::cout << "Дорога города в самого себя не добавляется.\n";
        return;
    }
    int weight = readInt("Вес дороги (положительное число): ", -1);
    if (weight <= 0) {
        std::cout << "Вес должен быть положительным.\n";
        return;
    }
    bool existed = (g.w[from][to] > 0);
    g.w[from][to] = weight;
    if (existed) {
        std::cout << "Дорога " << g.name[from] << " -> " << g.name[to]
                  << " обновлена, вес " << weight << ".\n";
    } else {
        std::cout << "Дорога " << g.name[from] << " -> " << g.name[to]
                  << " добавлена, вес " << weight << ".\n";
    }
}

static void removeRoad(Graph& g) {
    
    printCityList(g);
    int from = readInt("От какого города (номер): ", -1) - 1;
    int to = readInt("До какого города (номер): ", -1) - 1;
    if (from < 0 || from >= g.n || to < 0 || to >= g.n) {
        std::cout << "Неверный номер города.\n";
        return;
    }
    if (from == to || g.w[from][to] < 0) {
        std::cout << "Дороги " << g.name[from] << " -> " << g.name[to] << " нет.\n";
        return;
    }
    g.w[from][to] = -1;
    std::cout << "Дорога " << g.name[from] << " -> " << g.name[to] << " удалена.\n";
}

static int tourLength(const Graph& g, const int* tour) {
    int total = 0;
    for (int i = 0; i < g.n; ++i) {
        int from = tour[i];
        int to = tour[(i + 1) % g.n];
        int weight = g.w[from][to];
        if (weight <= 0) {
            return INF;
        }
        total += weight;
    }
    return total;
}

static void printTour(const Graph& g, const int* tour) {
    for (int i = 0; i < g.n; ++i) {
        std::cout << g.name[tour[i]] << " -> ";
    }
    std::cout << g.name[tour[0]];
}

static void shuffleTour(int* tour, int n) {
    for (int i = n - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int tmp = tour[i];
        tour[i] = tour[j];
        tour[j] = tmp;
    }
}

static void initPopulation(Individual* pop, int size, const Graph& g) {
    for (int k = 0; k < size; ++k) {
        for (int i = 0; i < g.n; ++i) {
            pop[k].tour[i] = i;
        }
        shuffleTour(pop[k].tour, g.n);
        pop[k].length = tourLength(g, pop[k].tour);
    }
}

static int tournamentSelect(const Individual* pop, int size) {
    int best = rand() % size;
    for (int k = 0; k < 2; ++k) {
        int candidate = rand() % size;
        if (pop[candidate].length < pop[best].length) {
            best = candidate;
        }
    }
    return best;
}

static void oxChild(const int* base, const int* other, int n, int a, int b, int* child) {
    bool used[MAX_CITIES];
    for (int i = 0; i < n; ++i) {
        used[i] = false;
    }
    for (int i = a; i <= b; ++i) {
        child[i] = base[i];
        used[base[i]] = true;
    }
    int pos = (b + 1) % n;
    for (int k = 0; k < n; ++k) {
        int city = other[(b + 1 + k) % n];
        if (!used[city]) {
            child[pos] = city;
            used[city] = true;
            pos = (pos + 1) % n;
        }
    }
}

static void orderCrossover(const int* first, const int* second, int n, int* child1, int* child2) {
    int a = rand() % n;
    int b = rand() % n;
    if (a > b) {
        int tmp = a;
        a = b;
        b = tmp;
    }
    oxChild(first, second, n, a, b, child1);
    oxChild(second, first, n, a, b, child2);
}

static void mutate(int* tour, int n, int mutationPercent) {
    if (mutationPercent <= 0) {
        return;
    }
    for (int i = 0; i < n; ++i) {
        if (rand() % 100 < mutationPercent) {
            int j = rand() % n;
            if (i != j) {
                int tmp = tour[i];
                tour[i] = tour[j];
                tour[j] = tmp;
            }
        }
    }
}

static void sortPopulation(Individual* pop, int size) {
    for (int i = 0; i < size - 1; ++i) {
        int best = i;
        for (int j = i + 1; j < size; ++j) {
            if (pop[j].length < pop[best].length) {
                best = j;
            }
        }
        if (best != i) {
            Individual tmp = pop[i];
            pop[i] = pop[best];
            pop[best] = tmp;
        }
    }
}

static void runGA(const Graph& g, const GAParams& input) {
    if (g.n < 3) {
        std::cout << "Для маршрута нужно минимум 3 города.\n";
        return;
    }

    int n = g.n;
    int populationSize = input.populationSize;
    int children = input.childrenPerGeneration;
    int generations = input.generations;
    int mutationPercent = input.mutationPercent;

    static Individual population[MAX_POP];
    static Individual nextPopulation[MAX_POP];

    initPopulation(population, populationSize, g);
    sortPopulation(population, populationSize);
    Individual best = population[0];

    std::cout << "\n=== Запуск генетического алгоритма ===\n";
    std::cout << "Городов: " << n << "; популяция: " << populationSize
              << "; потомков за поколение: " << children
              << "; эволюций: " << generations
              << "; мутации: " << mutationPercent << "%\n";
    std::cout << "Популяция 0: ";
    printTour(g, best.tour);
    if (best.length >= INF) {
        std::cout << ", маршрут невозможен\n";
    } else {
        std::cout << ", длина " << best.length << "\n";
    }

    for (int gen = 1; gen <= generations; ++gen) {
        int newCount = 0;
        nextPopulation[newCount++] = best;

        int produced = 0;
        while (produced < children && newCount < populationSize) {
            int parent1 = tournamentSelect(population, populationSize);
            int parent2 = tournamentSelect(population, populationSize);

            int child1[MAX_CITIES];
            int child2[MAX_CITIES];
            orderCrossover(population[parent1].tour, population[parent2].tour,
                           n, child1, child2);

            mutate(child1, n, mutationPercent);
            for (int k = 0; k < n; ++k) {
                nextPopulation[newCount].tour[k] = child1[k];
            }
            nextPopulation[newCount].length = tourLength(g, child1);
            ++newCount;
            ++produced;

            if (produced < children && newCount < populationSize) {
                mutate(child2, n, mutationPercent);
                for (int k = 0; k < n; ++k) {
                    nextPopulation[newCount].tour[k] = child2[k];
                }
                nextPopulation[newCount].length = tourLength(g, child2);
                ++newCount;
                ++produced;
            }
        }

        int oldIndex = 0;
        while (newCount < populationSize) {
            nextPopulation[newCount] = population[oldIndex];
            ++newCount;
            ++oldIndex;
            if (oldIndex >= populationSize) {
                oldIndex = 0;
            }
        }

        for (int k = 0; k < populationSize; ++k) {
            population[k] = nextPopulation[k];
        }
        sortPopulation(population, populationSize);
        if (population[0].length < best.length) {
            best = population[0];
        }

        std::cout << "Популяция " << gen << ": ";
        printTour(g, population[0].tour);
        if (population[0].length >= INF) {
            std::cout << ", маршрут невозможен\n";
        } else {
            std::cout << ", длина " << population[0].length << "\n";
        }
    }

    std::cout << "\nЛучший найденный маршрут: ";
    printTour(g, best.tour);
    if (best.length >= INF) {
        std::cout << ", маршрут невозможен (не хватает дорог)\n";
    } else {
        std::cout << ", длина " << best.length << "\n";
    }
}

static void bruteForceRec(const Graph& g, int depth, int* tour, bool* used,
                          int currentLength, int* bestLength, int* bestTour) {
    int n = g.n;
    if (depth == n) {
        int last = tour[n - 1];
        int weight = g.w[last][tour[0]];
        if (weight > 0) {
            int total = currentLength + weight;
            if (total < *bestLength) {
                *bestLength = total;
                for (int i = 0; i < n; ++i) {
                    bestTour[i] = tour[i];
                }
            }
        }
        return;
    }
    int previous = tour[depth - 1];
    for (int city = 1; city < n; ++city) {
        if (!used[city] && g.w[previous][city] > 0) {
            used[city] = true;
            tour[depth] = city;
            bruteForceRec(g, depth + 1, tour, used,
                          currentLength + g.w[previous][city],
                          bestLength, bestTour);
            used[city] = false;
        }
    }
}

static void bruteForce(const Graph& g) {
    if (g.n < 3) {
        std::cout << "Для маршрута нужно минимум 3 города.\n";
        return;
    }
    if (g.n > 10) {
        std::cout << "Полный перебор доступен только для n <= 10.\n";
        return;
    }

    int tour[MAX_CITIES];
    int bestTour[MAX_CITIES];
    bool used[MAX_CITIES];
    for (int i = 0; i < g.n; ++i) {
        used[i] = false;
    }
    tour[0] = 0;
    used[0] = true;
    int bestLength = INF;

    bruteForceRec(g, 1, tour, used, 0, &bestLength, bestTour);

    std::cout << "\nПолный перебор (эталон): ";
    if (bestLength >= INF) {
        std::cout << "гамильтонов цикл не существует\n";
    } else {
        printTour(g, bestTour);
        std::cout << ", длина " << bestLength << "\n";
    }
}

static GAParams readGAParams(const Graph& g) {
    GAParams p;

    int defaultPopulation = 10 * g.n;
    if (defaultPopulation < 4) {
        defaultPopulation = 4;
    }
    if (defaultPopulation > MAX_POP) {
        defaultPopulation = MAX_POP;
    }
    std::ostringstream populationPrompt;
    populationPrompt << "Начальный размер популяции (Enter = " << defaultPopulation
                     << ", рассчитывается от " << g.n << " городов): ";
    p.populationSize = readInt(populationPrompt.str().c_str(), defaultPopulation);
    if (p.populationSize < 4) {
        p.populationSize = 4;
        std::cout << "Размер популяции увеличен до 4.\n";
    }
    if (p.populationSize > MAX_POP) {
        p.populationSize = MAX_POP;
        std::cout << "Размер популяции ограничен " << MAX_POP << ".\n";
    }

    int maxChildren = p.populationSize - 1;
    if (maxChildren % 2 != 0) {
        --maxChildren;
    }
    if (maxChildren < 2) {
        maxChildren = 2;
    }
    std::ostringstream childrenPrompt;
    childrenPrompt << "Количество потомков за поколение, чётное (Enter = "
                   << maxChildren << "): ";
    p.childrenPerGeneration = readInt(childrenPrompt.str().c_str(), maxChildren);
    if (p.childrenPerGeneration < 2) {
        p.childrenPerGeneration = 2;
    }
    if (p.childrenPerGeneration % 2 != 0) {
        ++p.childrenPerGeneration;
        std::cout << "Количество потомков скорректировано до чётного: "
                  << p.childrenPerGeneration << ".\n";
    }
    if (p.childrenPerGeneration > maxChildren) {
        p.childrenPerGeneration = maxChildren;
        std::cout << "Количество потомков скорректировано до " << maxChildren << ".\n";
    }

    p.generations = readInt("Количество эволюций (Enter = 100): ", 100);
    if (p.generations < 1) {
        p.generations = 1;
        std::cout << "Количество эволюций увеличено до 1.\n";
    }
    if (p.generations > 100000) {
        p.generations = 100000;
        std::cout << "Количество эволюций ограничено 100000.\n";
    }

    p.mutationPercent = readInt("Процент мутаций, 0-100 (Enter = 10): ", 10);
    if (p.mutationPercent < 0) {
        p.mutationPercent = 0;
        std::cout << "Процент мутаций ограничен 0.\n";
    }
    if (p.mutationPercent > 100) {
        p.mutationPercent = 100;
        std::cout << "Процент мутаций ограничен 100.\n";
    }

    return p;
}

int main() {
    srand((unsigned)time(NULL));

    Graph g;
    loadDemo5(g);

    std::cout << "Лабораторная работа № 9: генетический алгоритм (задача коммивояжёра).\n";
    std::cout << "По умолчанию загружен демонстрационный граф из задания (5 городов).\n";

    while (!inputClosed) {
        std::cout << "\n--- Меню ---\n";
        std::cout << "1. Загрузить демонстрационный граф (5 городов из задания)\n";
        std::cout << "2. Загрузить основной граф (8 городов)\n";
        std::cout << "3. Показать граф\n";
        std::cout << "4. Добавить город\n";
        std::cout << "5. Удалить город\n";
        std::cout << "6. Добавить дорогу\n";
        std::cout << "7. Удалить дорогу\n";
        std::cout << "8. Запустить генетический алгоритм\n";
        std::cout << "9. Проверить полным перебором (n <= 10)\n";
        std::cout << "0. Выход\n";

        int choice = readInt("Выбор: ", -1);
        if (inputClosed) {
            break;
        }

        if (choice == 1) {
            loadDemo5(g);
            std::cout << "Загружен демонстрационный граф: 5 городов.\n";
        } else if (choice == 2) {
            loadGraph8(g);
            std::cout << "Загружен основной граф: 8 городов.\n";
        } else if (choice == 3) {
            printGraph(g);
        } else if (choice == 4) {
            addCity(g);
        } else if (choice == 5) {
            removeCity(g);
        } else if (choice == 6) {
            addRoad(g);
        } else if (choice == 7) {
            removeRoad(g);
        } else if (choice == 8) {
            GAParams params = readGAParams(g);
            runGA(g, params);
        } else if (choice == 9) {
            bruteForce(g);
        } else if (choice == 0) {
            std::cout << "Работа завершена.\n";
            break;
        } else {
            std::cout << "Неверный пункт меню.\n";
        }
    }

    return 0;
}
