//�˿��߳�
#pragma once
#include<iostream>
#include<vector>
#include<queue>
#include<random>
using namespace std;

extern queue<vector<int>> dataQ;
extern int cake;

static void customerThread()
{
    random_device rd;   
    mt19937 gen(rd());  
    uniform_int_distribution<> dist(0, cake/3); 
    //���ɴ�0��cake����֮һ�����������
    int needCake=dist(gen);
    int needBread;
    if (needCake == 0) {//
        uniform_int_distribution<> newDist(1, cake/3);
        needBread = newDist(gen);
    }
    else {
        needBread=dist(gen);
    }
	dataQ.push(vector<int>{needCake, needBread});
	cout << "customer" << endl;
}