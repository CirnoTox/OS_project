//pcout类 保证cout输出不串行
#pragma once
#include<string>
#include<iostream>
#include<mutex>
using namespace std;
struct pcout : public stringstream {
    //static inline mutex cout_mutex;
    ~pcout() {
        //lock_guard<mutex> l{ cout_mutex };
        cout << rdbuf();
        //cout.flush();
    }
};
