/* 主线程
1. 标准输入获取数据
2. 创建运行n个销售人员线程
3. 创建运行c个顾客线程
4. 标准输出数据
*/
#pragma once
#include"saler.cpp"
#include"customer.cpp"
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <queue>
using namespace std;

//初始化mutex,unique_lock,condition_variable
static mutex salermutex;
static mutex customermutex;
static unique_lock<mutex>salerLock;
static unique_lock<mutex>customerLock;
static condition_variable salerCV;
static condition_variable customerCV;
static queue<vector<int>> dataQ;
static int bread, cake;


int main()
{
    int saler, customer;
    cin >> bread >> cake >> saler >> customer;
    vector<thread>salerThreadP;
    vector<thread>customerThreadP;
    //线程池 方便管理线程
    for (auto i = 0; i < customer; ++i) {
        customerThreadP.push_back(thread(*customerThread));
    }
    for (auto i = 0; i < saler; ++i) {
        salerThreadP.push_back(thread(*salerThread));
    }
    for (thread& t : customerThreadP) {
        t.join();
    }
    for (thread& t : salerThreadP) {
        t.join();
    }
    cout << bread << cake;
}
