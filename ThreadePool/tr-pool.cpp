#include <iostream>
#include <queue>
#include <thread>
#include <chrono>
#include <mutex>
#include <future>         
#include <unordered_set>  
#include <atomic>         
// ^^^ Helps with take info about treade or with send come massage.
//     Helps with swap two sets or check equelness of them. Just helps to working with sets.
//     Block accsess to threades to one variable at the same time.
#include <vector>
#include <chrono>


// Mutex locks acsess to commands for every threads without one.


// C++ 14
class thread_pool {
public:
// ћетод, создающий массив из num_threads потоков. ¬ качестве данного метода - конструктор. // ѕринимает число потоков.
    thread_pool(uint32_t num_threads) {
        threads.reserve(num_threads);                       // F() Make empty space to new vector object. But space * num of threades.
        for (uint32_t i = 0; i < num_threads; ++i) {         
            threads.emplace_back(&thread_pool::run, this);   // F() Add element to end of vector. "this" point on "threade"
        }
    }

    template <typename Func, typename ...Args>
    int64_t add_task(const Func& task_func, Args&&... args) {
        int64_t task_idx = last_idx++;
        
        // Ќе блокирующий метод добавлени€ новой задачи. ѕринимает функцию task_func
        // и аргументы данной функции args и возвращает task_id (уникальный номер задачи).
        std::lock_guard<std::mutex> q_lock(q_mtx);                                  // Mutex a threade.
        q.emplace(std::async(std::launch::deferred, task_func, args...), task_idx); // F() Place elem in vector to specified position.
        q_cv.notify_one();                                                          // Unlock one threade after mutex. (For u_l make recheck predicat(bool) value). 
        return task_idx;
    }

    // wait(), wait_all(), calculate() = checks for thread pool working. (complete by id?, compelte all?, complete by id? without stop pool).

    // Ѕлокирующий метод, ожидающий выполнени€ задачи с указанным task_id.
    // If this task (by id) in "completed_task" != end task in "completed_task" so stop wait.
    void wait(int64_t task_id) {
        std::unique_lock<std::mutex> lock(completed_task_ids_mtx);              // Mutex for "completed_tasks" locks by u_l named "lock".
        completed_task_ids_cv.wait(lock, [this, task_id]()->bool {   // Condition_variable for "completed_tasks" wait while "lock" mtx will unlocked 
                                                                                // by predicate "[this, task_id]()->bool" that neans after complite task for thread "task id").
                                                                                // When "task id" false then wait is working. 
            return completed_task_ids.find(task_id) != completed_task_ids.end(); 
            });
    }
    // Ѕлокирующий метод, дожидающийс€ завершени€ всех задач.
    void wait_all() {
        std::unique_lock<std::mutex> lock(q_mtx);
        completed_task_ids_cv.wait(lock, [this]()->bool {
            std::lock_guard<std::mutex> task_lock(completed_task_ids_mtx);
            return q.empty() && last_idx == completed_task_ids.size();
            });
    }
   // Ќе блокирующий метод, провер€ющий была ли выполнена задача с номером task_id
    bool calculated(int64_t task_id) {
        std::lock_guard<std::mutex> lock(completed_task_ids_mtx);
        if (completed_task_ids.find(task_id) != completed_task_ids.end()) {
            return true;
        }
        return false;
    }

    // Ѕлокирующий метод, дожидающийс€ завершени€ всех задач и завершающий работу Thread Pool.
    ~thread_pool() {
        quite = true;
        for (uint32_t i = 0; i < threads.size(); ++i) {
            q_cv.notify_all();
            threads[i].join();
        }
    }

private:



    void run() {
        while (!quite) {
            std::unique_lock<std::mutex> lock(q_mtx);                                     // Only one threade got acsess to this commands.

            // если есть задачи, то берЄм задачу, иначе - засыпаем
            // если мы зашли в деструктор, то quite будет true и мы не будем 
            // ждать завершени€ всех задач и выйдем из цикла
    //           Not wait!                                       True    or    True
            q_cv.wait(lock, [this]()->bool { return !q.empty() || quite; });   // While q is not empty or pool is not working so "don't wait!".
                                                                                          // While q have some tasks then --------------------- ^^^.

            if (!q.empty()) {
                auto elem = std::move(q.front()); // f"front"() returns first value of elements of q.
                q.pop();                                                       // delite first value of q.
                lock.unlock();

                // вычисл€ем объект типа std::future (вычисл€ем функцию) 
                elem.first.get(); // pair have two value "lvalue and rvalue" (first and second). f"get"() comlete a task in thread.

                std::lock_guard<std::mutex> lock(completed_task_ids_mtx); // after and {} lock guard will be unlocked.

                // добавл€ем номер выполненой задачи в список завершЄнных
                completed_task_ids.insert(elem.second); // ID comes to c_t. 

                completed_task_ids_cv.notify_all(); // Dont wait to threads.
            }
        }
    }
                    //2 ---------------------------        
    std::queue<std::pair<std::future<void>, int64_t>> q; // очередь задач - хранит функцию(задачу), которую нужно исполнить и номер данной задачи
    std::mutex q_mtx;
    std::condition_variable q_cv;

    std::unordered_set<int64_t> completed_task_ids;      // помещаем в данный контейнер исполненные задачи
    std::condition_variable completed_task_ids_cv;
    std::mutex completed_task_ids_mtx;

    std::vector<std::thread> threads;


    std::atomic<bool> quite{ false };                    // флаг завершени€ работы thread_pool
    std::atomic<int64_t> last_idx = 0;                   // переменна€ хран€ща€ id, который будет выдан следующей задаче
};


void test_func(int& res, const std::vector<int>& arr) {
    using namespace std::chrono_literals;
    res = 0;
    for (int i = arr.size() - 1; i >= 0; --i) {
        for (int j = 0; j < arr.size(); ++j) {
            res += arr[i] + arr[j];
        }
    }
}


void thread_pool_test(std::vector<int> ans, std::vector<int> arr) {
    auto begin = std::chrono::high_resolution_clock::now();

    thread_pool t(6);
    for (int i = 0; i < ans.size(); ++i) {
        t.add_task(test_func, std::ref(ans[i]), std::ref(arr));
    }
    t.wait_all();

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
}

void without_thread_test(std::vector<int> ans, std::vector<int> arr) {

    auto begin = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ans.size(); ++i) {
        test_func(std::ref(ans[i]), std::ref(arr));
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
}

void raw_thread_test(std::vector<int> ans, std::vector<int> arr) {
    auto begin = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < ans.size() / 6; ++i) {
        std::thread t1(test_func, std::ref(ans[i]), std::ref(arr));
        std::thread t2(test_func, std::ref(ans[i]), std::ref(arr));
        std::thread t3(test_func, std::ref(ans[i]), std::ref(arr));
        std::thread t4(test_func, std::ref(ans[i]), std::ref(arr));
        std::thread t5(test_func, std::ref(ans[i]), std::ref(arr));
        std::thread t6(test_func, std::ref(ans[i]), std::ref(arr));

        t1.join(); t2.join(); t3.join(); t4.join(); t5.join(); t6.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
}

void run_test() {
    std::vector<int> ans(24);
    std::vector<int> arr1(10000);
    thread_pool_test(ans, arr1);//  52474
    without_thread_test(ans, arr1); // 83954
    raw_thread_test(ans, arr1); // 62386
}

class Test {
public:
    void operator() () {
        std::cout << "Working with functors!\n";
    }
};

void sum(int a, int b) {
    std::cout << a + b << std::endl;
}

int main() {
    //run_test();

    Test test;
    auto res = std::bind(sum, 2, 3);

    thread_pool t(3);
    t.add_task(test); // Got tasks for threade pool.
    t.add_task(res);
    t.wait_all();

    return 0;
}