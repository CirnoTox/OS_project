//销售人员线程
#pragma once
#include<iostream>
#include<vector>
#include<queue>
#include<random>
#include <mutex>
using namespace std;

extern int cake;
extern int bread;
extern mutex salerMutex;
extern condition_variable salerCV;
extern condition_variable customerSaleFinishCV;
extern int customerIdSaleFinished;
extern queue<vector<int>> dataQ;
//vector[0] for id,[1] for bread,[2] for cake
extern mutex salerDataQueueMutex;
extern condition_variable salerDataQueueCV;
extern bool salerQueueBool;

static void salerThread(size_t number)
{
	while (1) {//持续运行
		/******************销售开始*******************************/
		cout << "Saler" << number << " Begin." << endl;
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<> dist(1, 2000);
		auto sleepTime = dist(gen);
		this_thread::sleep_for(chrono::milliseconds(sleepTime));
		//wait()解除阻塞后先等待sleepTime的时间
		//模拟销售花费的时间
		unique_lock<mutex> salerLock(salerMutex);
		/******************判断数据队列情况*******************************/
		if (dataQ.empty()) {
			cout << "No data salerThread" << number << " End." << endl;
			break;//结束运行
		}
		/******************判断数据队列情况*******************************/

		/********************数据操作***********************************/
		auto serveCustomer = dataQ.front();
		dataQ.pop();
		auto customerId = serveCustomer[0];
		if(serveCustomer[1]>bread|| serveCustomer[2]>cake){
			customerIdSaleFinished = customerId;
			customerSaleFinishCV.notify_all();
			cout << "Saler" << number << " rejected." << endl;
			salerLock.unlock();
			continue;
		}
		bread -= serveCustomer[1];
		cake -= serveCustomer[2];
		/********************数据操作***********************************/

		/******************提醒顾客离开***************************/
		customerIdSaleFinished = customerId;
		customerSaleFinishCV.notify_all();
		/******************提醒顾客离开***************************/
		cout << "Saler" << number << " End." << endl;
		salerLock.unlock();
		/******************销售结束*******************************/
	}
}