#include "vector.h"
#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include "queue.h"

enum class TaskDesc {
    CALC, END
};

std::vector<TaskDesc> tasks_types = {TaskDesc::CALC, TaskDesc::END};

class Task {
public:
    TaskDesc task;
    int ind = 0;
    int difficulty = 0;
    Task(bool end = false) {
        if (end) {
            task = tasks_types[1];
        } else {
            std::random_device rd{};
            std::mt19937 gen{rd()};
            std::normal_distribution<> d{500, 100};
            task = tasks_types[0];
            ind = gen() % 5;
            difficulty = std::round(d(gen));
        }
    }
};

int main() {
    AtomicsVector<int> vec(5);
    std::vector<int> ref(5);
    std::size_t count = 5;
    AtomicsVector<int> workload(5);
    std::vector<Queue<Task>> queues(count, Queue<Task>(1000000));
    auto process = [&](int owned_pos) {
        while(true) {
            if(queues[owned_pos].get_size() == 0)
                continue;
            auto cur_task = queues[owned_pos].front();
            workload[cur_task.ind] -= cur_task.difficulty;
            if (cur_task.task == TaskDesc::CALC) {
                while(cur_task.difficulty--) {
                    auto cur = std::sin(cur_task.ind / 10.) * std::sin(cur_task.ind / 10.) + std::cos(cur_task.ind / 10.) * std::cos(cur_task.ind / 10.);
                    auto res = static_cast<int>(cur);
                    if (res == 0)
                        res++;
                    vec[cur_task.ind] += res;
                }
            } else {
                return;
            }

        }
    };
    std::size_t start_count = 100000;
    std::random_device device;
    std::mt19937 gen(device());

    std::vector<std::thread> thread_group(count);
    for (std::size_t i = 0; i < count; i++) {
        thread_group[i] = std::thread(process, i);
    }

    for (std::size_t i = 0; i < start_count; i++) {
        auto task = Task();
        ref[task.ind] += task.difficulty;
        workload[task.ind] += task.difficulty;
        int num = 0, min = workload[0].load();
        for (std::size_t i = 1; i < workload.get_size(); i++) {
            if (workload[i].load() < min) {
                min = workload[i].load();
                num = i;
            }
        }
        queues[num].push(task);
    }

    for (std::size_t i = 0; i < count; i++) {
        queues[i].push(Task(true));
    }
    for (std::size_t i = 0; i < count; i++) {
        thread_group[i].join();
    }
    for (std::size_t i = 0; i < vec.get_size(); i++) {
        std::cout << vec[i] << " ";
    }
    std::cout << '\n';
    for (std::size_t i = 0; i < ref.size(); i++) {
        std::cout << ref[i] << " ";
    }
    for (std::size_t i = 0; i < workload.get_size(); i++) {
        std::cout << workload[i] << " ";
    }
}
