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

//初始化mutex, condition_variable
static queue<vector<int>> dataQ;
//vector[0] for id,[1] for bread,[2] for cake
static int bread, cake;

static mutex customerDataMutex;
static condition_variable customerDataCV;
static bool customerDataBool = true;
static mutex customerSaleFinishMutex;
static condition_variable customerSaleFinishCV;
static unique_lock<mutex> dataLock(customerDataMutex);
static unique_lock<mutex> saleFinishLock(customerSaleFinishMutex);
static vector<int> customerSaleFinishBoolVector;

static mutex salerMutex;
static condition_variable salerCV;
static mutex salerDataQueueMutex;
static condition_variable salerDataQueueCV;
static unique_lock<mutex>dataAvailableLock(salerDataQueueMutex, defer_lock);
static unique_lock<mutex> salerLock(salerMutex, defer_lock);

int main()
{
    int saler, customer;
    cin >> bread >> cake >> saler >> customer;
    customerSaleFinishBoolVector = vector<int>(customer, 0);
    vector<thread>salerThreadP;
    static deque<thread> customerThreadQ;
    //新建一个队列存储顾客线程，在销售进程中从队列里取出进程处理完后结束线程
    //线程池 方便管理线程
    for (auto i = 0; i < customer; ++i) {
        customerThreadQ.emplace_back(customerThread,i);
    }
    while (dataQ.size() != customer) { ; }
    //忙等待顾客生成好数据
    for (size_t i{ 0 }; i < saler; ++i) {
        salerThreadP.emplace_back(salerThread,i);
    }
    for (thread& t : salerThreadP) {
        t.join();
    }
    for (thread& t : customerThreadQ) {
        t.join();
    }
    
    cout << bread << cake;
}
