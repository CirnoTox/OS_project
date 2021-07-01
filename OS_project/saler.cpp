//������Ա�߳�
#pragma once
#include"pcout.h"
#include<iostream>
#include<vector>
#include<queue>
#include<random>
#include <mutex>
#include<condition_variable>
#include<thread>
using namespace std;

extern size_t cake;
extern size_t bread;
extern mutex salerMutex;
extern condition_variable customerSaleFinishCV;
extern int customerIdSaleFinished;
extern queue<vector<size_t>> dataQ;
//vector[0] for id,[1] for bread,[2] for cake
extern vector<int> testSaleQueue;

static void salerThread(size_t number)
{
	while (1) {//持续运行
		/******************销售开始*******************************/
		pcout{} << "Saler " << number << " Begin.\n";
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
			pcout{} << "No customer, Saler " << number << " End.\n";
			return;//结束运行
		}
		/******************判断数据队列情况*******************************/

		/********************数据操作***********************************/
		auto serveCustomer = dataQ.front();
		dataQ.pop();
		auto customerId = serveCustomer[0];
		if(serveCustomer[1]>bread|| serveCustomer[2]>cake){
			customerIdSaleFinished = customerId;
			customerSaleFinishCV.notify_all();//提醒顾客离开
			pcout{} << "Customer " << customerId <<
				" need "<< serveCustomer[1] << " breads " <<
				serveCustomer[2] << " cakes"
				", rejected by Saler "<< number<<".\n";
			salerLock.unlock();
			continue;
		}
		bread -= serveCustomer[1];
		cake -= serveCustomer[2];
		/********************数据操作***********************************/

		pcout{} << "Saler" << number << " sold Customer " 
			<< customerId << " with " <<serveCustomer[1]<<" breads "<< 
			serveCustomer[2]<<" cakes" << " End.\n";
		/******************提醒顾客离开***************************/
		customerIdSaleFinished = customerId;
		customerSaleFinishCV.notify_all();
		/******************提醒顾客离开***************************/
		testSaleQueue.push_back(customerId);
		salerLock.unlock();
		/******************销售结束*******************************/
	}
	pcout{} << "return";
	return;
}