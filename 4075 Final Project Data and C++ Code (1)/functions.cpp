#ifndef functions_cpp
#define functions_cpp

#include "functions.hpp"

void importData(vector<double>& open,
	vector<double>& high,
	vector<double>& low,
	vector<double>& close,
	vector<int>& volume,
	vector<int>& tickCount){

	unsigned int count = 0;
	ifstream myReadFile;
	myReadFile.open("WC5min.txt");
	string line;
	if(myReadFile.is_open()){
    	cout << "\nFile Opened successfully!!!. Reading data from file into vectors\n" << endl;
		while (getline(myReadFile,line)){
			++count;
			stringstream linestream(line);
			double o, h, l, c;
			int i, v, t;
			linestream >> i >> o >> h >> l >> c >> v >> t;
			open.push_back(o);
			high.push_back(h);
			low.push_back(l);
			close.push_back(c);
			volume.push_back(v);
			tickCount.push_back(t);
		}
	}
	else {
		cout << "error opening file" << endl;
	}
	// cout << open[0] << endl;
	open.erase(open.begin());
	// cout << open[0] << endl;
	high.erase(high.begin());
	low.erase(low.begin());
	close.erase(close.begin());
	volume.erase(volume.begin());
	tickCount.erase(tickCount.begin());

	myReadFile.close();
}

double round(double val, int precision){
    std::stringstream s;
    s << std::setprecision(precision) << std::setiosflags(std::ios_base::fixed) << val;
    s >> val;
    return val;
}

double vectorMin(const vector<double>& v){

  vector<double>::const_iterator it = min_element(v.begin(), v.end());
  return *it;
}

double vectorMax(const vector<double>& v){

  vector<double>::const_iterator it = max_element(v.begin(), v.end());
  return *it;
}

int vectorMax(const vector<int>& v){

  vector<int>::const_iterator it = max_element(v.begin(), v.end());
  return *it;
}

void printVector(const vector<double>& v){
	cout << "( ";
    for (vector<double>::const_iterator it = v.begin(); it != v.end(); ++it) {
        cout  << *it  << " ";}
        cout << ")" << endl; 
}

void print2DVector(const vector< vector<double> > &vec2D){
	for (int i = 0 ; i < vec2D.size(); i++){ // loop over rows
		for (int j = 0; j < vec2D[0].size(); j++){ // loop over cols
            cout << vec2D[i][j] << " | ";
         }
         cout << endl;
    }
}

double netProfitToMaxDD(const vector<double>& isEquity,
	const vector<double>& drawDown,
	const int& currentBar){
	// cout << "begEquity: " << isEquity[0] << endl;
	// cout << "endEquity: " << isEquity[currentBar] << endl;
	// cout << "MaxDD: " << vectorMax(drawDown) << endl;
	if (vectorMax(drawDown) == 0){
		return isEquity[currentBar] - isEquity[0];
	}
	else{
		return (isEquity[currentBar] - isEquity[0]) / vectorMax(drawDown);
	}
}

double highestHigh(const vector<double>& high,
	int& chnLen, int& barNumber){
	int end = min(chnLen,barNumber+1);
	vector<double>::const_iterator it = max_element(high.begin(), high.begin() + end);
  	return *it;
}

double lowestLow(const vector<double>& low,
	int& chnLen, int& barNumber){
	int end = min(chnLen,barNumber+1);
	vector<double>::const_iterator it = min_element(low.begin(), low.begin() + end);
  	return *it;
}

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
	vector<double>& totalReturn){

	printf("cCash: %.2f cPos: %.0f cHold: %.2f tCost: %.2f equity: %.2f hwm: %.2f\n",
        currentCash[currentBar],
		currentPosition[currentBar],
		currentHoldings[currentBar],
		totalTransactionCosts[currentBar],
		isEquity[currentBar],
		highWaterMark[currentBar]);

	printf("Net Profit: %.2f ret: %.2f cDD: %.2f maxDD: %.2f DDdur: %d maxDDDur: %d\n",
		isEquity[currentBar] - isEquity[0],
		totalReturn[currentBar],
		drawDown[currentBar],
		vectorMax(drawDown),
		drawDownDuration[currentBar],
		vectorMax(drawDownDuration));
}

void printBar(int& currentBar,
	vector<double>& open,
	vector<double>& high,
	vector<double>& low,
	vector<double>& close){
	
	printf("Bar: %d Open: %.2f High: %.2f Low: %.2f Close: %.2f\n",
        currentBar,
		open[currentBar],
		high[currentBar],
		low[currentBar],
		close[currentBar]);
}

void printHighLow(vector<double>& high,
	vector<double>& low,
	int& currentBar,
	int& chnLen){
	cout << "HH: " << highestHigh(high,chnLen,currentBar) << "  ";
	cout << "LL: " << lowestLow(low,chnLen,currentBar) << endl;
}

vector<double> updatePosition(const string& orderSignal,
	vector<double>& currentPosition,
	const int& lastBar,
	const int& tradeSize){
	if (orderSignal == "buy"){
		currentPosition.push_back(currentPosition[lastBar] + tradeSize);
	}
	else if (orderSignal == "sell"){
		currentPosition.push_back(currentPosition[lastBar] - tradeSize);
	}
	else{
		currentPosition.push_back(currentPosition[lastBar]);
	}
	return currentPosition;
}

vector<double> updateTransCosts(const string& orderSignal,
	const int& tradeSize,
	vector<double>& transactionCosts,
	const double& roundTripSlippage){

	if (orderSignal == "buy"){
		transactionCosts.push_back(roundTripSlippage/2);
	}
	else if (orderSignal == "sell"){
		transactionCosts.push_back(roundTripSlippage/2);
	}
	else{
		transactionCosts.push_back(0.0);
	}

	return transactionCosts;
}

vector<double> updateTotalTransCosts(const int& lastBar,
	const int& currentBar,
	const vector<double>& transactionCosts,
	vector<double>& totalTransactionCosts){
	// cout << "totalTransactionCosts: " << totalTransactionCosts[lastBar] << endl;
	// cout << "transactionCosts: " << transactionCosts[currentBar] << endl;
	totalTransactionCosts.push_back(totalTransactionCosts[lastBar]+
		transactionCosts[currentBar]);
	// cout << "totalTransactionCosts: " << totalTransactionCosts[currentBar] << endl;

	return transactionCosts;
}

vector<double> updateCash(const string& orderSignal,
	const double& orderPrice,
	vector<double>& currentCash,
	const int& lastBar,
	const int& currentBar,
	const vector<double>& transactionCosts,
	const int& chnLen){

	if (orderSignal == "buy"){
		currentCash.push_back(currentCash[lastBar] - 
			transactionCosts[currentBar] - orderPrice);
	}
	else if (orderSignal == "sell"){
		currentCash.push_back(currentCash[lastBar] - 
			transactionCosts[currentBar] + orderPrice);
	}
	else{
		currentCash.push_back(currentCash[lastBar]);
	}
	return currentCash;		
}

double updateEntryPrice(double& entryPrice,
	const vector<double>& currentPosition,
	const string& orderSignal,
	const vector<double>& high,
	const vector<double>& low,
	int& currentBar,
	int& chnLen){

	if (orderSignal == "buy"){
		entryPrice = (entryPrice + highestHigh(high,chnLen,currentBar)) /
			currentPosition[currentBar];
	}
	else if (orderSignal == "sell"){
		entryPrice = (entryPrice - lowestLow(low,chnLen,currentBar)) /
			currentPosition[currentBar];
	}
	else{
		entryPrice = entryPrice;
	}
	return entryPrice;
}

double updateEntryPrice(double& entryPrice,
	double& orderPrice,
	const string& orderSignal){

	// cout << "Inside entryPrice: " << entryPrice << endl;
	// cout << "orderPrice: " << orderPrice << endl;
	if (orderSignal == "buy"){
		entryPrice = orderPrice;
	}
	else if (orderSignal == "sell"){
		entryPrice = orderPrice;
	}
	else{
		entryPrice = entryPrice;
	}
	return entryPrice;
}

vector<double> updateHoldings(vector<double>& currentHoldings,
	const vector<double>& currentPosition,
	const vector<double>& close,
	int& currentBar,
	const double& entryPrice){

	double priceChange = close[currentBar] - entryPrice;
	double valueChange = priceChange * 4 * 12.5 * currentPosition[currentBar];

	if(currentPosition[currentBar] > 0){
		currentHoldings.push_back(entryPrice + currentPosition[currentBar] * priceChange);
	}
	else if(currentPosition[currentBar] < 0){
		currentHoldings.push_back(entryPrice + currentPosition[currentBar] * priceChange);
	}
	else{
		currentHoldings.push_back(0);
	}
	return currentHoldings;
}

vector<double> updateEquity(vector<double>& isEquity,
	const vector<double>& currentCash,
	const vector<double>& totalTransactionCosts,
	const vector<double>& currentPosition,
	const vector<double>& close,
	const int& currentBar,
	const int& lastBar,
	const double& entryPrice){

	double priceChange = close[currentBar] - entryPrice;
	double valueChange = priceChange * 4 * 12.5 * currentPosition[currentBar];
	// cout << "Price change: " << priceChange << endl;
	// cout << "Tick Change: " << priceChange * 4 << endl;
	// cout << "Value change: " << valueChange << endl;
	double changeInCash = currentCash[currentBar] - currentCash[lastBar];
	cout << "change cash: " << changeInCash << endl;
	isEquity.push_back(isEquity[0] + valueChange - totalTransactionCosts[currentBar]);
	
	return isEquity;		
}

vector<double> updateHWM(vector<double>& highWaterMark,
	const vector<double>& isEquity,
	const int& lastBar,
	const int& currentBar){

	highWaterMark.push_back(max(highWaterMark[lastBar],isEquity[currentBar]));
	
	return highWaterMark;		
}

vector<double> updateReturn(vector<double>& totalReturn,
	const vector<double>& isEquity,
	const int& currentBar){

	totalReturn.push_back(isEquity[currentBar] / isEquity[0] - 1);

	return totalReturn;		
}

vector<double> updateDrawDown(vector<double>& drawDown,
	const vector<double>& highWaterMark,
	const vector<double>& isEquity,
	const int& currentBar){

	drawDown.push_back(highWaterMark[currentBar] - isEquity[currentBar]);

	return drawDown;
}

vector<int> updateDrawDownDuration(vector<int>& drawDownDuration,
	const vector<double>& drawDown,
	const int& currentBar,
	const int& lastBar){

	// cout << "INside cDD: " << drawDown[currentBar] << endl;
	if (drawDown[currentBar] <= 0){
		drawDownDuration.push_back(0);
	}
	else {
		drawDownDuration.push_back(drawDownDuration[lastBar] + 1);
	}
	return drawDownDuration;
}



#endif