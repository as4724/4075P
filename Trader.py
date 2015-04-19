# -*- coding: utf-8 -*-
"""
Created on Sat Apr 18 20:20:37 2015

@author: Akshat Sinha
"""
import pandas as pd
import numpy as np
class Trade:
    #Parameter set:
    chnLen = 0
    stopPct = 0
    tradeSize = 0
    marketPos = 0
    entryP = 0
    hhigh = 0
    llow = 0
    prevPeak = 0
    prevTrough = 0
    data = pd.DataFrame({})
    orderEntry = pd.DataFrame({})
    orderExit = pd.DataFrame({})
    openOrder = pd.DataFrame({})
    
    def __init__(self, data, chnLen, stopPct):
        self.data = data
        self.chnLen = chnLen
        self.stopPct = stopPct
        self.orderEntry = pd.DataFrame(index = np.arange(0,data.size), columns=('datein','timein','qty','pricein','status'))
        self.orderExit = pd.DataFrame(index = np.arange(0,data.size), columns=('dateout','timeout','qty','priceout','position'))
        self.openOrder = pd.DataFrame(index = np.arange(0,data.size), columns=('date','time','qty','price','type'))
        
    def setParam(self, chnLen, stopPct):
        self.chnLen = chnLen
        self.stopPct = stopPct

    def setData(self, data):
        self.data = data
        self.orderEntry = pd.DataFrame(index = np.arange(0,data.size), columns=('datein','timein','qty','pricein','status'))
        self.orderExit = pd.DataFrame(index = np.arange(0,data.size), columns=('dateout','timeout','qty','priceout','position'))
        self.openOrder = pd.DataFrame(index = np.arange(0,data.size), columns=('date','time','qty','price','type'))
        
    def startTrade(self):
        index = self.chnLen
        i = 0
        entryPrice = 0
        for d in self.data[self.chnLen:]:
            #Process open orders            
            openBuy = self.openOrder[self.openOrder['qty'] > 0]
            openSell = self.openOrder[self.openOrder['qty'] < 0]
            
            #Create new orders
            hhigh = self.data.High[self.data.High[(index-self.chnLen):index]]
            llow = self.data.Low[self.data.Low[(index-self.chnLen):index]]
            if self.marketPos == 0:
                self.openOrder[i] = [d.Date, d.Time, self.tradeSize, hhigh, 'stop']
                i += 1
                self.openOrder[i] = [d.Date, d.Time, -self.tradeSize, llow, 'stop']
                i += 1
            elif(self.marketPos > 0):
                prevPeak = entryPrice
                if d.Close > prevPeak:
                    prevPeak = d.Close
                self.openOrder[i] = [d.Date, d.Time, -self.tradeSize, prevPeak * (1-self.stopPct), 'stop']
            else:
                prevTrough = entryPrice
                if d.Close < prevTrough:
                    prevTrough = d.Close
                self.openOrder[i] = [d.Date, d.Time, self.tradeSize, prevTrough * (1+self.stopPct), 'stop']