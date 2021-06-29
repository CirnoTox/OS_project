//�˿��߳�
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
extern int customerIdSaleFinished;

static void customerThread(size_t number)
{
    /*******************�˿��߳���������*******************************/
    unique_lock<mutex> dataLock(customerDataMutex);
    cout << "customer" << number << " come" << endl;
    random_device rd;   
    mt19937 gen(rd());  
    uniform_int_distribution<> dist(0, cake/30); 
    //���ɴ�0��cake����֮һ�����������
    int needCake=dist(gen);
    int needBread;
    if (needCake == 0) {//
        uniform_int_distribution<> newDist(1, bread /30);
        needBread = newDist(gen);
    }
    else {
        needBread=dist(gen);
    }
	dataQ.push(vector<int>{int(number),needBread,needCake});
    dataLock.unlock();
    /*******************�˿��߳���������*******************************/

    /*******************�˿��̵߳ȴ��뿪*******************************/
    unique_lock<mutex> saleFinishLock(customerSaleFinishMutex);
    customerSaleFinishCV.wait(saleFinishLock,
        [&]() { return customerIdSaleFinished==number; });

	cout << "customer"<<number<<" leave" << endl;
    /*******************�˿��̵߳ȴ��뿪*******************************/
}