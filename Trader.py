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
    tradeSize = 1
    marketPos = 0
    entryP = 0
    hhigh = 0
    llow = 0
    prevPeak = 0
    prevTrough = 0
    data = pd.DataFrame({})
    orderEntry = pd.DataFrame({})
    orderExit = pd.DataFrame({})
    orderOpen = pd.DataFrame({})
    
    def __init__(self, data, chnLen, stopPct):
        self.data = data
        self.chnLen = chnLen
        self.stopPct = stopPct
        self.orderEntry = pd.DataFrame(index = np.arange(0,100), columns=('datein','timein','qty','pricein','status'))
        self.orderExit = pd.DataFrame(index = np.arange(0,100), columns=('dateout','timeout','qty','priceout','position'))
        self.orderOpen = pd.DataFrame(columns=('date','time','qty','price','type'))
        
    def setParam(self, chnLen, stopPct):
        self.chnLen = chnLen
        self.stopPct = stopPct

    def setData(self, data):
        self.data = data
        self.orderEntry = pd.DataFrame(index = np.arange(0,100), columns=('datein','timein','qty','pricein','status'))
        self.orderExit = pd.DataFrame(index = np.arange(0,100), columns=('dateout','timeout','qty','priceout','position'))
        self.orderOpen = pd.DataFrame(columns=('date','time','qty','price','type'))
        
   
    def startTrade(self):
        i = 0
        entryPrice = 0
        self.hhigh = data.High[0]
        self.llow = data.Low[0]
        for temp, d in self.data[self.chnLen:].iterrows():
           print temp
           openBuyToExec = self.orderOpen[(self.orderOpen['qty'] > 0) & (self.orderOpen['price'] < d.Close)]
           openSellToExec = self.orderOpen[(self.orderOpen['qty'] < 0) & (self.orderOpen['price'] > d.Close)]
           netPos = openBuyToExec.qty.sum() + openSellToExec.qty.sum() 
           if netPos > 0:
               price = openBuyToExec.price.loc[openBuyToExec.index[-1]]
           if netPos < 0:
               price = openSellToExec.price.loc[openSellToExec.index[-1]]
               
           if (netPos != 0):
               #Close all positions and take net pos at next bar's open
               openPos = self.orderEntry[self.orderEntry['status']=='open']
               for temp1, d1 in openPos.iterrows():
                   self.orderExit.loc[temp1] = [data.Date[temp+1], data.Time[temp+1], -d1.qty, data.Open[temp+1], self.marketPos]
                   self.marketPos += -d1.qty
               self.orderEntry.append([[d.Date,d.Time,netPos,price,'open']])
               self.marketPos += netPos
               self.orderOpen = pd.DataFrame(columns=('date','time','qty','price','type'))
           
          
            
            #Create new orders
           if self.hhigh < d.High:
               self.hhigh = d.High
           if self.llow > d.Low:
               self.llow = d.Low
           
           if self.marketPos == 0:
               self.orderOpen.loc[i] = [d.Date, d.Time, self.tradeSize, self.hhigh, 'stop']
               i += 1
               self.orderOpen.loc[i] = [d.Date, d.Time, -self.tradeSize, self.llow, 'stop']
               i += 1
           elif(self.marketPos > 0):
               entryPrice = self.orderEntry.pricein[self.orderEntry['status'] == 'open'].mean()
               if entryPrice == entryPrice:
                   self.prevPeak = entryPrice
               if d.Close > self.prevPeak:
                   self.prevPeak = d.Close
               self.orderOpen.loc[i] = [d.Date, d.Time, -self.tradeSize, self.prevPeak * (1-self.stopPct), 'stop']
               i += 1
           else:
               entryPrice = self.orderEntry.pricein[self.orderEntry['status'] == 'open'].mean()
               if entryPrice == entryPrice:
                   self.prevTrough = entryPrice
               if d.Close < self.prevTrough:
                   self.prevTrough = d.Close
               self.orderOpen.loc[i] = [d.Date, d.Time, self.tradeSize, self.prevTrough * (1+self.stopPct), 'stop']
               i += 1
                
data = pd.read_csv(r'C:\Users\Akshat Sinha\Dropbox\Classes\4075P\WC-5min.asc')
x = Trade(data,500,0.02)
x.startTrade()
