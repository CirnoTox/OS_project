//¹Ë¿ÍÏß³Ì
#pragma once
#include<iostream>
#include<vector>
#include<queue>
using namespace std;
extern queue<vector<int>> dataQ;
static void customerThread()
{
	
	dataQ.push(vector<int>{1, 2});
	cout << "customer" << endl;
}