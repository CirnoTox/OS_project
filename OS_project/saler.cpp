//������Ա�߳�
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
	while (1) {//��������
		/******************���ۿ�ʼ*******************************/
		cout << "Saler" << number << " Begin." << endl;
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<> dist(1, 2000);
		auto sleepTime = dist(gen);
		this_thread::sleep_for(chrono::milliseconds(sleepTime));
		//wait()����������ȵȴ�sleepTime��ʱ��
		//ģ�����ۻ��ѵ�ʱ��
		unique_lock<mutex> salerLock(salerMutex);
		/******************�ж����ݶ������*******************************/
		if (dataQ.empty()) {
			cout << "No data salerThread" << number << " End." << endl;
			break;//��������
		}
		/******************�ж����ݶ������*******************************/

		/********************���ݲ���***********************************/
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
		/********************���ݲ���***********************************/

		/******************���ѹ˿��뿪***************************/
		customerIdSaleFinished = customerId;
		customerSaleFinishCV.notify_all();
		/******************���ѹ˿��뿪***************************/
		cout << "Saler" << number << " End." << endl;
		salerLock.unlock();
		/******************���۽���*******************************/
	}
}