//pcout�� ��֤cout���������
#pragma once
#include<string>
#include<iostream>
#include<mutex>
using namespace std;
class pcout : public stringstream {
public:
    static inline mutex cout_mutex;
    ~pcout() {
        lock_guard<mutex> l{ cout_mutex };
        cout << rdbuf();
        cout.flush();
    }
};
