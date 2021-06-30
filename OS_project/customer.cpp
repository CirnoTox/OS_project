//顾客线程
#pragma once
#include"pcout.h"
#include<iostream>
#include<vector>
#include<queue>
#include<random>
#include <mutex>
using namespace std;

extern queue<vector<size_t>> dataQ;
extern size_t cake;
extern size_t bread;
extern mutex customerDataMutex;
extern mutex customerSaleFinishMutex;
extern condition_variable customerSaleFinishCV;
extern int customerIdSaleFinished;

static void customerThread(size_t number)
{
    /*******************顾客线程生成数据*******************************/
    unique_lock<mutex> dataLock(customerDataMutex);
    pcout{} << "Customer " << number << " come.\n";
    random_device rd;   
    mt19937 gen(rd());  
    gamma_distribution<> dist(1, 4);
    //生成从0到cake三分之一总量的随机数
    size_t needCake = size_t(dist(gen));
    size_t needBread;
    if (needCake == 0) {//
        gamma_distribution<> newDist(1, 4);
        needBread = size_t(newDist(gen))+1;
    }
    else {
        needBread= size_t(dist(gen));
    }
	dataQ.push(vector<size_t>{number,needBread,needCake});
    dataLock.unlock();
    /*******************顾客线程生成数据*******************************/

    /*******************顾客线程等待离开*******************************/
    unique_lock<mutex> saleFinishLock(customerSaleFinishMutex);
    customerSaleFinishCV.wait(saleFinishLock,
        [&]() { return customerIdSaleFinished==number; });

    pcout{} << "Customer " << number << " leave.\n";
    /*******************顾客线程等待离开*******************************/
}