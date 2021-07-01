/* 主线程
1. 标准输入获取数据
2. 创建运行n个销售人员线程
3. 创建运行c个顾客线程
4. 标准输出数据
*/
#pragma once
#include"saler.cpp"
#include"customer.cpp"
#include"pcout.h"
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <queue>
using namespace std;

//初始化mutex, condition_variable
static size_t bread, cake;
//vector[0] for id,[1] for bread,[2] for cake
static queue<vector<size_t>> dataQ;

static mutex customerDataMutex;
static mutex customerSaleFinishMutex;
static condition_variable customerSaleFinishCV;
static int customerIdSaleFinished=-1;

static mutex salerMutex;

int main()
{
    size_t saler, customer;
    cout << "总共的面包："; cin >> bread;
    cout << "总共的蛋糕："; cin >> cake;
    cout << "销售人员个数："; cin >> saler;
    cout << "顾客个数："; cin >> customer;
    vector<thread>salerThreadPool;
    vector<thread> customerThreadPool;
    //线程池 方便管理线程
    for (auto i = 0; i < customer; ++i) {
        customerThreadPool.emplace_back(customerThread,i);
        this_thread::sleep_for(10ms);
    }
    while (dataQ.size() != customer) { ; }
    //忙等待顾客生成好数据
    for (size_t i{ 0 }; i < saler; ++i) {
        salerThreadPool.emplace_back(salerThread,i);
    }
    for (thread& t : customerThreadPool) {
        t.join();
    }
    for (thread& t : salerThreadPool) {
        t.join();
    }
    
    pcout{} << bread << " " << cake << "\n";
}
