#ifndef functions_HPP
#define functions_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <omp.h>
#include <vector>
#include <sstream>
#include <sys/time.h>
#include <algorithm>
#include <sstream>
#include <iomanip>      // std::setiosflags

using namespace std;

void importData(vector<double>& open,
	vector<double>& high,
	vector<double>& low,
	vector<double>& close,
	vector<int>& volume,
	vector<int>& tickCount);

void printVector(const vector<double>& v);

void print2DVector(const vector< vector<double> > &vec2D);

double round(double val, int precision);

double vectorMin(const vector<double>& v);

double vectorMax(const vector<double>& v);

double netProfitToMaxDD(const vector<double>& currentEquity,
	const vector<double>& drawDown,
	const int& currentBar);

double highestHigh(const vector<double>& high,
	int& chnLen, int& barNumber);

double lowestLow(const vector<double>& low,
	int& chnLen, int& barNumber);

void printPosition(int& currentBar,
	vector<double>& currentCash,
	vector<double>& isEquity,
	vector<double>& currentPosition,
	vector<double>& currentHoldings,
	vector<double>& transactionCosts,
	vector<double>& totalTransactionCosts,
	vector<double>& highWaterMark,
	vector<double>& drawDown,
	vector<int>& drawDownDuration,
	vector<double>& totalReturn);

void printBar(int& currentBar,
	vector<double>& open,
	vector<double>& high,
	vector<double>& low,
	vector<double>& close);

void printHighLow(vector<double>& high,
	vector<double>& low,
	int& currentBar,
	int& chnLen);

vector<double> updatePosition(const string& orderSignal,
	vector<double>& currentPosition,
	const int& lastBar,
	const int& tradeSize);

vector<double> updateTransCosts(const string& orderSignal,
	const int& tradeSize,
	vector<double>& transactionCosts,
	const double& roundTripSlippage);

vector<double> updateTotalTransCosts(const int& lastBar,
	const int& currentBar,
	const vector<double>& transactionCosts,
	vector<double>& totalTransactionCosts);

vector<double> updateCash(const string& orderSignal,
	const double& orderPrice,
	vector<double>& currentCash,
	const int& lastBar,
	const int& currentBar,
	const vector<double>& transactionCosts,
	const int& chnLen);

double updateEntryPrice(double& entryPrice,
	const vector<double>& currentPosition,
	const string& orderSignal,
	const vector<double>& high,
	const vector<double>& low,
	int& currentBar,
	int& chnLen);

double updateEntryPrice(double& entryPrice,
	double& orderPrice,
	const string& orderSignal);

vector<double> updateHoldings(vector<double>& currentHoldings,
	const vector<double>& currentPosition,
	const vector<double>& close,
	int& currentBar,
	const double& entryPrice);

vector<double> updateEquity(vector<double>& isEquity,
	const vector<double>& currentCash,
	const vector<double>& totalTransactionCosts,
	const vector<double>& currentPosition,
	const vector<double>& close,
	const int& currentBar,
	const int& lastBar,
	const double& entryPrice);

vector<double> updateHWM(vector<double>& highWaterMark,
	const vector<double>& isEquity,
	const int& lastBar,
	const int& currentBar);

vector<double> updateReturn(vector<double>& totalReturn,
	const vector<double>& isEquity,
	const int& currentBar);

vector<double> updateDrawDown(vector<double>& drawDown,
	const vector<double>& highWaterMark,
	const vector<double>& isEquity,
	const int& currentBar);

vector<int> updateDrawDownDuration(vector<int>& drawDownDuration,
	const vector<double>& drawDown,
	const int& currentBar,
	const int& lastBar);




#endif