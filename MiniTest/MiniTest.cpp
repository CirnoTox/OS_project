
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <shared_mutex>
using namespace std;
queue<size_t>q;
mutex m;
mutex m2;
condition_variable cv;

void threadfun(size_t num)
{
    unique_lock<mutex>ul(m);
    //cv.wait(ul, [&] {return !q.empty(); });
    q.pop();
    q.push(num);
    cout << num;
    ul.unlock();
    //cv.notify_all();
}
int main()
{
    vector<thread>v;
    q.push(0);
    for (int i = 1; i < 10; ++i) {
        v.emplace_back(threadfun, i);
    }
    for (thread& t : v) {
        t.join();
    }
    return 0;
}
