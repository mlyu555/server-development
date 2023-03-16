#include <iostream>
#include <thread>
#include <chrono>

using namespace std;
using std::count;
using std::endl;

void thread_entry(int n) {
    std::thread::id tid = std::this_thread::get_id();

    for (int i = 0; i < 5; i++) {
        cout << "child function thread【" << tid << "】running: " << i + 1 << endl;
        std::this_thread::sleep_for(std::chrono::seconds(n));                           // 睡眠n秒
    }
}


// 仿函数类，类似Python中__call__()
class ThreadFunctor {
public:
    // 通过类重载()运算符——实现函数式创建类对象
    void operator()(int n) {
        std::thread::id tid = std::this_thread::get_id();
        
        for (int i = 0; i < 5; i++) {
            cout << "child function thread【" << tid << "】running: " << i + 1 << endl;
            std::this_thread::sleep_for(std::chrono::seconds(n));                           // 睡眠n秒
        }
    }
};


int main() {
    std::thread t1(thread_entry, 1);
    if (t1.joinable()) {
        t1.join();
    }

    ThreadFunctor tf;
    std::thread t2(tf, 2);
    if (t2.joinable()) {
        t2.detach();
    }

    return 0;
}