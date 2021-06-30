//������Ա�߳�
#pragma once
#include"pcout.h"
#include<iostream>
#include<vector>
#include<queue>
#include<random>
#include <mutex>
using namespace std;

extern size_t cake;
extern size_t bread;
extern mutex salerMutex;
extern condition_variable customerSaleFinishCV;
extern int customerIdSaleFinished;
extern queue<vector<size_t>> dataQ;
//vector[0] for id,[1] for bread,[2] for cake

static void salerThread(size_t number)
{
	while (1) {//��������
		/******************���ۿ�ʼ*******************************/
		pcout{} << "Saler " << number << " Begin.\n";
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
			pcout{} << "No customer, Saler " << number << " End.\n";
			break;//��������
		}
		/******************�ж����ݶ������*******************************/

		/********************���ݲ���***********************************/
		auto serveCustomer = dataQ.front();
		dataQ.pop();
		auto customerId = serveCustomer[0];
		if(serveCustomer[1]>bread|| serveCustomer[2]>cake){
			customerIdSaleFinished = customerId;
			customerSaleFinishCV.notify_all();//���ѹ˿��뿪
			pcout{} << "Customer " << customerId <<
				" need "<< serveCustomer[1] << " breads " <<
				serveCustomer[2] << " cakes"
				", rejected by Saler "<< number<<".\n";
			salerLock.unlock();
			continue;
		}
		bread -= serveCustomer[1];
		cake -= serveCustomer[2];
		/********************���ݲ���***********************************/

		pcout{} << "Saler" << number << " sold Customer " 
			<< customerId << " with " <<serveCustomer[1]<<" breads "<< 
			serveCustomer[2]<<" cakes" << " End.\n";
		/******************���ѹ˿��뿪***************************/
		customerIdSaleFinished = customerId;
		customerSaleFinishCV.notify_all();
		/******************���ѹ˿��뿪***************************/
		salerLock.unlock();
		/******************���۽���*******************************/
	}
}