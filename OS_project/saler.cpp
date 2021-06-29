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
extern vector<int> customerSaleFinishBoolVector;
extern queue<vector<int>> dataQ;
//vector[0] for id,[1] for bread,[2] for cake
extern mutex salerDataQueueMutex;
extern condition_variable salerDataQueueCV;
extern unique_lock<mutex>dataAvailableLock;
extern unique_lock<mutex> salerLock;

static void salerThread(size_t number)
{
	while (1) {//��������
		auto now = std::chrono::system_clock::now();
		salerDataQueueCV.wait_until(dataAvailableLock, now+10000ms,
			[&]() { return !dataQ.empty(); });
		//�ȴ�10���������ݶ��ж�Ϊ��ʱ��ֱ�ӽ������
		if (dataQ.empty()) {
			cout << "No data salerThread"<<number<<" End." << endl;
			break;//��������
		}
		cout << "Saler" << number << " Begin." << endl;
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<> dist(1, 2000);
		auto sleepTime = dist(gen);
		salerCV.wait_for(salerLock,chrono::milliseconds(sleepTime),
			[&]() { return true; });
		//wait_for()����������ȵȴ�sleepTime��ʱ��
		//ģ�����ۻ��ѵ�ʱ��
		auto serveCustomer = dataQ.front();
		dataQ.pop();
		auto customerId = serveCustomer[0];
		bread -= serveCustomer[1];
		cake -= serveCustomer[2];
		cout << "Saler" << number << " End." << endl;
		//���ѹ˿��뿪
		customerSaleFinishBoolVector[customerId] = 1;
		customerSaleFinishCV.notify_all();
	}
}