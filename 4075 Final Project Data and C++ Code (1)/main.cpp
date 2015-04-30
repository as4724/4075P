
#include "functions.hpp"

int main(int argc, char* argv[]){

	struct timeval startTime, endTime;
    long seconds, useconds;
    double mtime;
	gettimeofday(&startTime, NULL);
	//////////////////////////////////////////////////////
	double tickValue = 12.5;
	double PointValue = 50.0;
	double roundTripSlippage = 0.31;  // 0.31 or 31?
	int tradeSize = 1;

	int numBarsFourYears = 45540;
	int numBarsThreeMonths = 2880;
	int numBarsTotal = 371337;

	vector<double> oosEquity;

	vector<double> open;
	vector<double> high;
	vector<double> low;
	vector<double> close;
	vector<int> volume;
	vector<int> tickCount;

	importData(open, high, low, close, volume, tickCount);

	int numInSamplePeriod = 0;

	// only testing one 4 year period right now
	for (int bar = 0; bar + numBarsFourYears-1 < numBarsFourYears; bar = bar + numBarsThreeMonths-1){
	// for (int bar = 0; bar + numBarsFourYears-1 < numBarsTotal; bar = bar + numBarsThreeMonths-1){
		numInSamplePeriod++;
		cout << "In sample period: " << numInSamplePeriod << endl;
		cout << "Bar start: " << bar << "  ";
		cout << "Bar end: " << bar + numBarsFourYears-1 << endl;

		vector<double> optimalParams(2);
		vector< vector<double> > heatMap(1, vector<double>(1));;

		int row = 0;
		int col = 0;

		double maxObjective = -10000000.0;
		double newObjective;

		// for (int chnLen = 500; chnLen <= 10000; chnLen = chnLen + 10){ // use this to test more paramters
		for (int chnLen = 500; chnLen <= 10000; chnLen = chnLen + 100){
			cout << "chnLen: " << chnLen << endl;
			for (double stpPct = 0.005; stpPct <= 0.10; stpPct = stpPct + 0.001){
				cout << "stpPct: " << stpPct << endl;

				double initialCash = 100000.0;
				double objective = 0.0;
				vector<double> currentCash;
				vector<double> isEquity;
				vector<double> currentPosition;
				vector<double> currentHoldings;
				vector<double> transactionCosts;
				vector<double> totalTransactionCosts;
				vector<double> highWaterMark;
				vector<double> drawDown;
				vector<int> drawDownDuration;
				vector<double> totalReturn;

				double entryPrice = 0.0;
				vector<double> highHigh;
				vector<double> lowLow;
				double prevPeak = 0.0;
				double prevTrough = 0.0;

				string orderSignal = "none";

				int currentBar = bar;
				int endBar = bar + numBarsFourYears - 1;
				cout << "start: " << currentBar << "  ";
				cout << "end: " << endBar << endl;

				
				double orderPrice = 0.0;
				int numTrades = 0;
				double priceChange = 0.0;
				double valueChange = 0.0;

				for (currentBar; currentBar <= endBar; currentBar++){

					int lastBar = currentBar-1;
					if (currentBar == 0){
						printBar(currentBar,open,high,low,close);

						currentCash.push_back(initialCash);
						currentPosition.push_back(0.0);
						currentHoldings.push_back(currentPosition[currentBar]*close[currentBar]);
						transactionCosts.push_back(0.0);
						totalTransactionCosts.push_back(0.0);
						isEquity.push_back(currentCash[currentBar]+currentHoldings[currentBar]);
						highWaterMark.push_back(isEquity[currentBar]);
						totalReturn.push_back(0.0);
						highHigh.push_back(high[currentBar]);
						lowLow.push_back(low[currentBar]);
						drawDown.push_back(highWaterMark[currentBar]-isEquity[currentBar]);
						drawDownDuration.push_back(0);

						printPosition(currentBar,currentCash,isEquity,currentPosition,
							currentHoldings,transactionCosts,totalTransactionCosts,
							highWaterMark,drawDown,drawDownDuration,totalReturn);

						printHighLow(high,low,currentBar,chnLen);

						if(highestHigh(high,chnLen,currentBar) >= 
								low[currentBar] && highestHigh(high,chnLen,currentBar) <= 
								high[currentBar]){

								orderSignal = "buy";
								orderPrice = highestHigh(high,chnLen,currentBar);
								cout << "Buy signal generated at $" << orderPrice << endl;
						}
						else if(lowestLow(high,chnLen,currentBar) >= 
								low[currentBar] && highestHigh(high,chnLen,currentBar) <= 
								high[currentBar]){

								orderSignal = "sell";
								orderPrice = lowestLow(low,chnLen,currentBar);
								cout << "Sell signal generated at $" << orderPrice << endl;
						}
						// cin.get();
					}
					if (currentBar > 0 && currentBar < endBar){
						printBar(currentBar,open,high,low,close);

						printPosition(lastBar,currentCash,isEquity,currentPosition,
							currentHoldings,transactionCosts,totalTransactionCosts,
							highWaterMark,drawDown,drawDownDuration,totalReturn);

						printHighLow(high,low,currentBar,chnLen);

						cout << "Signal: " << orderSignal << " at $" << orderPrice << endl;
						if(orderSignal != "none"){
							numTrades++;
						}
						cout << "Updating Position..." << endl;
						updatePosition(orderSignal,currentPosition,lastBar,tradeSize);
						
						updateTransCosts(orderSignal,tradeSize,transactionCosts,
							roundTripSlippage);
						
						updateTotalTransCosts(lastBar,currentBar,transactionCosts,
							totalTransactionCosts);
						
						updateCash(orderSignal,orderPrice,currentCash,lastBar,currentBar,
							transactionCosts,chnLen);

						updateEntryPrice(entryPrice,orderPrice,orderSignal);

						updateHoldings(currentHoldings,currentPosition,close,currentBar,
							entryPrice);

						updateEquity(isEquity,currentCash,totalTransactionCosts,
							currentPosition,close,currentBar,lastBar,entryPrice);

						updateHWM(highWaterMark,isEquity,lastBar,currentBar);

						updateReturn(totalReturn,isEquity,currentBar);

						updateDrawDown(drawDown,highWaterMark,isEquity,currentBar);

						updateDrawDownDuration(drawDownDuration,drawDown,currentBar,lastBar);

						printPosition(currentBar,currentCash,isEquity,currentPosition,
							currentHoldings,transactionCosts, totalTransactionCosts,
							highWaterMark,drawDown,drawDownDuration,totalReturn);

						priceChange = close[currentBar] - entryPrice;
						valueChange = priceChange * 4 * tickValue;
						cout << "entryPrice: " << entryPrice << endl;
						cout << "Price change: " << priceChange << endl;
						cout << "Tick Change: " << priceChange * 4 << endl;
						if(currentPosition[currentBar] < 0){
							cout << "Value change: " << -1*valueChange << endl;
						}
						else if(currentPosition[currentBar] > 0){
							cout << "Value change: " << valueChange << endl;
						}

						cout << "OBJECTIVE: " << netProfitToMaxDD(isEquity,drawDown,currentBar) << endl;
						
						if(round(currentPosition[currentBar],1) == 0){
							cout << "currentPosition = 0" << endl;

							printBar(currentBar,open,high,low,close);
							printHighLow(high,low,currentBar,chnLen);
							
							if(high[currentBar] >= highestHigh(high,chnLen,currentBar)){

								orderSignal = "buy";
								orderPrice = highestHigh(high,chnLen,currentBar);
								cout << "Buy signal generated at $" << orderPrice << endl;
							}
							else if(low[currentBar] <= lowestLow(low,chnLen,currentBar)){

								orderSignal = "sell";
								orderPrice = lowestLow(low,chnLen,currentBar);
								cout << "Sell signal generated at $" << orderPrice << endl;
							}
							else {
								orderSignal = "none"; // not sure if necessary
								orderPrice = 0;
							}
						}
						else if(currentPosition[currentBar] > 0){
							cout << "currentPosition > 0" << endl;
							// cout << "entryPrice: " << entryPrice << endl;
							// cout << "prevPeak before: " << prevPeak << endl;
							prevPeak = entryPrice;
							// cout << "prevPeak after: " << prevPeak << endl;
							// cout << "close: " << close[currentBar] << endl;
							if (close[currentBar] > prevPeak){
								prevPeak = close[currentBar];
							}
							// cout << "prevPeak after: " << prevPeak << endl;
							cout << "stoploss: " << prevPeak * (1 - stpPct) << endl;
							cout << "stoploss orderPrice: " << round(prevPeak * (1 - stpPct)*4,0)/4 << endl;
							// cout << "low: " << low[currentBar] << endl;
							// cout << "high: " << high[currentBar] << endl;

							if (low[currentBar] <= prevPeak * (1 - stpPct)) {

								orderSignal = "sell";
								// "stoploss" = prevPeak * (1 - stpPct);
								orderPrice = round(prevPeak*(1-stpPct)*4,0)/4;
								cout << "Sell signal generated at $" << orderPrice << endl;
							}
							else{
								orderSignal = "none";
								orderPrice = 0;
								cout << "stoploss not hit" << endl;
							}
						}
						else if(currentPosition[currentBar] < 0){
							cout << "currentPosition < 0" << endl;
							// cout << "entryPrice: " << entryPrice << endl;
							// cout << "prevTrough before: " << prevTrough << endl;
							prevTrough = entryPrice;
							// cout << "prevTrough after: " << prevTrough << endl;
							// cout << "close: " << close[currentBar] << endl;
							if (close[currentBar] < prevTrough){
								prevTrough = close[currentBar];
							}
							// cout << "prevTrough after: " << prevTrough << endl;
							cout << "stoploss: " << prevTrough * (1 + stpPct) << endl;
							cout << "stoploss orderPrice: " << round(prevTrough * (1 + stpPct)*4,0)/4 << endl;
							// cout << "low: " << low[currentBar] << endl;
							// cout << "high: " << high[currentBar] << endl;
							
							if (high[currentBar] >= prevTrough * (1 + stpPct)) {
								
								orderSignal = "buy";
								orderPrice = round(prevTrough * (1 + stpPct)*4,0)/4;
								cout << "Buy signal generated at $" << orderPrice << endl;
							}
							else{
								orderSignal = "none";
								orderPrice = 0;
								cout << "stoploss not hit" << endl;
							}
						}
					// cin.get();
					}
				}
				// cout << "Trades: " << numTrades << endl;
				newObjective = netProfitToMaxDD(isEquity,drawDown,currentBar);
				heatMap[row][col] = newObjective;
				print2DVector(heatMap); 
				cout << "netProfitToMaxDD: " << newObjective << endl;
				col++;
				if (newObjective > maxObjective){
					maxObjective = newObjective;
					optimalParams[0] = chnLen;
					optimalParams[1] = stpPct;
				}
				// cin.get();
			}	
			row++;
			// cin.get();
		}
		// cin.get();
	}
	////////////////////////////////////////////////////
	gettimeofday(&endTime, NULL);
    seconds = endTime.tv_sec - startTime.tv_sec;
    useconds = endTime.tv_usec - startTime.tv_usec;
    mtime = ((seconds) * 1000.0 + useconds/1000.0); 
    cout << "\nTime elapsed was: " << mtime << " (milliseconds)" << endl;
    cout << endl;

	return 0;
}