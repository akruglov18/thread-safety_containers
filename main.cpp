#include "vector.h"
#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include "queue.h"

enum class TaskDesc {
    SUM, SUB, END
};

std::vector<TaskDesc> tasks_types = {TaskDesc::SUM, TaskDesc::SUB, TaskDesc::END};

class Task {
public:
    TaskDesc task;
    int ind = 0;
    Task(bool end = false) {
        if (end) {
            task = tasks_types[2];
        } else {
            std::random_device device;
            std::mt19937 gen(device());
            int key = gen() % 2;
            task = tasks_types[key];
            ind = gen() % 5;
        }
    }
};

int main() {
    AtomicsVector<int> vec(5);
    std::vector<int> ref(5);
    std::size_t count = 3;
    std::vector<Queue<Task>> queues(count, Queue<Task>(1000000));
    auto process = [&](int owned_pos) {
        while(true) {
            if(queues[owned_pos].get_size() == 0)
                continue;
            auto cur_task = queues[owned_pos].front();
            switch (cur_task.task)
            {
            case TaskDesc::SUM:
                vec[cur_task.ind]++;
                break;
            case TaskDesc::SUB:
                vec[cur_task.ind]--;
                break;
            case TaskDesc::END:
            default:
                return;
            }
            
        }
    };
    std::size_t start_count = 100000;
    std::random_device device;
    std::mt19937 gen(device());
    for (std::size_t i = 0; i < 3 *start_count; i++) {
        auto task = Task();
        if(task.task == TaskDesc::SUM)
            ref[task.ind]++;
        else
            ref[task.ind]--;;
        queues[gen() % count].push(task);
    }

    std::vector<std::thread> thread_group(count);
    for (std::size_t i = 0; i < count; i++) {
        thread_group[i] = std::thread(process, i);
    }

    for (std::size_t i = 0; i < 7 * start_count; i++) {
        auto task = Task();
        if(task.task == TaskDesc::SUM)
            ref[task.ind]++;
        else
            ref[task.ind]--;;
        queues[gen() % count].push(task);
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
    for (std::size_t i = 0; i < vec.get_size(); i++) {
        std::cout << ref[i] << " ";
    }
}
