#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <filesystem>
#include <format>
#include <ranges>

struct Task {
    std::string title;
    bool done = false;
};

// Load tasks from file
std::vector<Task> loadTasks(const std::string &filename) {
    std::vector<Task> tasks;
    std::ifstream file(filename);
    std::string title;
    bool done;

    while (file >> std::quoted(title) >> done)
        tasks.push_back({title, done});

    return tasks;
}

// Save tasks to file
void saveTasks(const std::vector<Task> &tasks, const std::string &filename) {
    std::ofstream file(filename);
    for (const auto &t : tasks)
        file << std::quoted(t.title) << " " << t.done << "\n";
}

// Display tasks
void listTasks(const std::vector<Task> &tasks) {
    std::cout << "\n" << std::format("{:=^30}\n", " To-Do List ");
    if (tasks.empty()) {
        std::cout << "(No tasks yet!)\n";
        return;
    }

    for (auto &&[i, task] : std::views::enumerate(tasks)) {
        char status = task.done ? 'x' : '0';
        std::cout << std::format("{:2}. [{}] {}\n", i + 1, status, task.title);
    }
}

// Colored output helper (ANSI escape codes)
enum class Color { Default, Green, Yellow, Blue };

void printColor(const std::string &text, Color color) {
    std::string code;
    switch (color) {
        case Color::Green:  code = "\033[32m"; break;
        case Color::Yellow: code = "\033[33m"; break;
        case Color::Blue:   code = "\033[34m"; break;
        default: code = "\033[0m"; break;
    }
    std::cout << code << text << "\033[0m";
}

int main() {
    const std::string filename = "tasks.txt";
    std::vector<Task> tasks = loadTasks(filename);

    int choice;
    while (true) {
        printColor("\n1. Add Task\n2. List Tasks\n3. Mark Done\n4. Delete Task\n5. Exit\n> ", Color::Blue);
        std::cin >> choice;

        if (choice == 1) {
            std::cin.ignore();
            std::string title;
            printColor("Enter task: ", Color::Yellow);
            std::getline(std::cin, title);
            tasks.push_back({title, false});
            saveTasks(tasks, filename);
        } 
        else if (choice == 2) {
            listTasks(tasks);
        } 
        else if (choice == 3) {
            listTasks(tasks);
            printColor("Enter task number to mark done: ", Color::Yellow);
            size_t n; std::cin >> n;
            if (n >= 1 && n <= tasks.size()) {
                tasks[n - 1].done = true;
                saveTasks(tasks, filename);
                printColor("Task marked done!\n", Color::Green);
            }
        } 
        else if (choice == 4) {
            listTasks(tasks);
            printColor("Enter task number to delete: ", Color::Yellow);
            size_t n; std::cin >> n;
            if (n >= 1 && n <= tasks.size()) {
                tasks.erase(tasks.begin() + (n - 1));
                saveTasks(tasks, filename);
                printColor("Task deleted!\n", Color::Green);
            }
        } 
        else if (choice == 5) {
            break;
        } 
        else {
            printColor("Invalid option.\n", Color::Yellow);
        }
    }

    return 0;
}
