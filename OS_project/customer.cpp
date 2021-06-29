//顾客线程
#pragma once
#include<iostream>
#include<vector>
#include<queue>
#include<random>
#include <mutex>
using namespace std;

extern queue<vector<int>> dataQ;
extern int cake;
extern int bread;
extern mutex customerDataMutex;
extern condition_variable customerDataCV;
extern bool customerDataBool;
extern mutex customerSaleFinishMutex;
extern condition_variable customerSaleFinishCV;
extern vector<int> customerSaleFinishBoolVector;
extern unique_lock<mutex> dataLock;
extern unique_lock<mutex> saleFinishLock;

static void customerThread(size_t number)
{
    customerDataCV.wait(dataLock, 
        [&]() {return customerDataBool; });
    customerDataBool = false;
    cout << "customer" << number << " come" << endl;

    random_device rd;   
    mt19937 gen(rd());  
    uniform_int_distribution<> dist(0, cake/3); 
    //生成从0到cake三分之一总量的随机数
    int needCake=dist(gen);
    int needBread;
    if (needCake == 0) {//
        uniform_int_distribution<> newDist(1, bread /3);
        needBread = newDist(gen);
    }
    else {
        needBread=dist(gen);
    }
	dataQ.push(vector<int>{needBread,needCake});
    customerDataBool = true;
    customerDataCV.notify_one();

    customerSaleFinishCV.wait(saleFinishLock,
        [&]() { return customerSaleFinishBoolVector[number]==1; });

	cout << "customer"<<number<<" leave" << endl;
}