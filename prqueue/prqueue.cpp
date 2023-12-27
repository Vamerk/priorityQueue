#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

template <typename T>
class PriorityQueue {
private:
    struct Triple {
        int priority;
        T item;
        int executionTime;

        Triple(int p, T i, int time) : priority(p), item(i), executionTime(time) {}

        // сортировка очереди
        bool operator<(const Triple& other) const {
            if (priority < other.priority) return true;
            if (priority > other.priority) return false;
            return item < other.item;
        }
    };

    std::vector<Triple> list;

public:
    int Count() const {
        return static_cast<int>(list.size());
    }

    void Enqueue(T item, int priority, int executionTime) {
        list.emplace_back(priority, item, executionTime);
        std::push_heap(list.begin(), list.end());
    }

    void ChangePriority(const T& item, int newPriority) {
        auto it = std::find_if(list.begin(), list.end(), [&](const Triple& task) {
            return task.item == item;
            });

        if (it != list.end()) {
            it->priority = newPriority;
            std::make_heap(list.begin(), list.end());
        }
        else {
            std::cerr << "Задача не найдена." << std::endl;
        }
    }

    Triple Dequeue() {
        if (list.empty()) {
            throw std::out_of_range("Очередь пустая");
        }

        std::pop_heap(list.begin(), list.end());
        Triple task = list.back();
        list.pop_back();

        return task;
    }

    void PrintQueue() const {
        std::cout << "Queue contents:" << std::endl;
        for (const auto& task : list) {
            std::cout << "Название задачи: " << task.item << ", Приоритет: " << task.priority << ", Время выполнения: " << task.executionTime << std::endl;
        }
        std::cout << std::endl;
    }

    void LoadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Ошибка открытия файла " + filename);
        }

        T item;
        int priority, executionTime;
        while (file >> item >> priority >> executionTime) {
            Enqueue(item, priority, executionTime);
        }

        file.close();
    }
};

int main() {
    std::setlocale(0, "");
    PriorityQueue<std::string> priorityQueue;

    try {
        priorityQueue.LoadFromFile("заявки.txt");
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    

    int choice;
    do {
        std::cout << "Выберите действие:" << std::endl;
        std::cout << "1. Обработать заявки" << std::endl;
        std::cout << "2. Добавить заявку" << std::endl;
        std::cout << "3. Вывести содержимое очередей" << std::endl;
        std::cout << "4. Выход" << std::endl;
        std::cout << "Ваш выбор: ";
        std::cin >> choice;

        switch (choice) {
        case 1: {
            // Обработка заявок
            while (priorityQueue.Count() > 0) {
                auto task = priorityQueue.Dequeue();
                std::cout << "Обработана задача: " << task.item << ", Приоритет: " << task.priority << ", Время выполнения: " << task.executionTime << std::endl;
            }
            break;
        }
        case 2: {
            // Добавление заявки
            std::string task;
            int priority, executionTime;
            std::cout << "Введите задачу: ";
            std::cin >> task;
            std::cout << "Введите приоритет: ";
            std::cin >> priority;
            std::cout << "Введите время выполнения: ";
            std::cin >> executionTime;
            priorityQueue.Enqueue(task, priority, executionTime);
            break;
        }
        case 3: {
            // Вывод содержимого очередей
            priorityQueue.PrintQueue();
            break;
        }
        case 4: {
            std::cout << "Программа завершена." << std::endl;
            break;
        }
        default: {
            std::cout << "Некорректный ввод. Попробуйте еще раз." << std::endl;
        }
        }
    } while (choice != 4);

    return 0;
}
