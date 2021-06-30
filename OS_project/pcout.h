//pcout�� ��֤cout���������
#pragma once
#include<string>
#include<iostream>
#include <sstream>
#include<mutex>
using namespace std;
struct pcout : public stringstream {
    static inline mutex cout_mutex;
    ~pcout() {
        lock_guard<mutex> l{ cout_mutex };
        cout << rdbuf();
        cout.flush();
    }
};
