# -*- coding: utf-8 -*-
"""
Created on Sat Apr 18 20:20:37 2015

@author: Akshat Sinha
"""
import pandas as pd
import numpy as np
import matplotlib.pyplot as mp

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
        
   
#    def startTrade(self):
#        i = 0
#        entryPrice = 0
#        self.hhigh = data.High[0]
#        self.llow = data.Low[0]
#        for temp, d in self.data[self.chnLen:].iterrows():
#           print temp
#           openBuyToExec = self.orderOpen[(self.orderOpen['qty'] > 0) & (self.orderOpen['price'] < d.Close)]
#           openSellToExec = self.orderOpen[(self.orderOpen['qty'] < 0) & (self.orderOpen['price'] > d.Close)]
#           netPos = openBuyToExec.qty.sum() + openSellToExec.qty.sum() 
#           if netPos > 0:
#               price = openBuyToExec.price.loc[openBuyToExec.index[-1]]
#           if netPos < 0:
#               price = openSellToExec.price.loc[openSellToExec.index[-1]]
#               
#           if (netPos != 0):
#               #Close all positions and take net pos at next bar's open
#               openPos = self.orderEntry[self.orderEntry['status']=='open']
#               for temp1, d1 in openPos.iterrows():
#                   self.orderExit.loc[temp1] = [data.Date[temp+1], data.Time[temp+1], -d1.qty, data.Open[temp+1], self.marketPos]
#                   self.marketPos += -d1.qty
#               self.orderEntry.append([[d.Date,d.Time,netPos,price,'open']])
#               self.marketPos += netPos
#               self.orderOpen = pd.DataFrame(columns=('date','time','qty','price','type'))
#           
#          
#            
#            #Create new orders
#           if self.hhigh < d.High:
#               self.hhigh = d.High
#           if self.llow > d.Low:
#               self.llow = d.Low
#           
#           if self.marketPos == 0:
#               self.orderOpen.loc[i] = [d.Date, d.Time, self.tradeSize, self.hhigh, 'stop']
#               i += 1
#               self.orderOpen.loc[i] = [d.Date, d.Time, -self.tradeSize, self.llow, 'stop']
#               i += 1
#           elif(self.marketPos > 0):
#               entryPrice = self.orderEntry.pricein[self.orderEntry['status'] == 'open'].mean()
#               if entryPrice == entryPrice:
#                   self.prevPeak = entryPrice
#               if d.Close > self.prevPeak:
#                   self.prevPeak = d.Close
#               self.orderOpen.loc[i] = [d.Date, d.Time, -self.tradeSize, self.prevPeak * (1-self.stopPct), 'stop']
#               i += 1
#           else:
#               entryPrice = self.orderEntry.pricein[self.orderEntry['status'] == 'open'].mean()
#               if entryPrice == entryPrice:
#                   self.prevTrough = entryPrice
#               if d.Close < self.prevTrough:
#                   self.prevTrough = d.Close
#               self.orderOpen.loc[i] = [d.Date, d.Time, self.tradeSize, self.prevTrough * (1+self.stopPct), 'stop']
#               i += 1
               
    def startTrade_lite(self):
        entryPrice = 0
        hhigh = self.data.High[0:self.chnLen].max()
        llow = self.data.Low[0:self.chnLen].min()
        entryPrice = 0
        openBuy = np.empty([len(self.data),1])
        openSell = np.empty([len(self.data),1])
        netPos = np.zeros([len(self.data),1])
        openBuy[:] = np.NAN
        openSell[:] = np.NAN
        #buyCounter = 0
        #sellCounter = 0
        prevPeak = 0
        prevTrough = 0
        openBuyExec = 0
        openSellExec = 0
        prevPeak = 0
        prevTrough = 0
        pos = 0
        for flag, data in self.data[self.chnLen:-1].iterrows():
           print flag
           openBuyExec = openBuy[data.Close > openBuy]
           openSellExec = openSell[data.Close < openSell]
           pos = (len(openBuyExec) - len(openSellExec)) * self.tradeSize
           
           if pos != 0:
               openBuy[data.Close > openBuy] = np.NAN
               openSell[data.Close < openSell] = np.NAN         
               if (pos > 0) == (netPos[flag] > 0):
                       netPos[flag + 1] = netPos[flag] + pos
               else:
                       netPos[flag + 1] = pos
           else:
               netPos[flag+1] = netPos[flag]
               
           
               
           #Create new orders
           
           hhigh = self.data.High[flag-self.chnLen:flag].max()
           llow = self.data.Low[flag-self.chnLen:flag].min()
           
           if netPos[flag] == 0:
               try:
                   openBuy[np.where(openBuy!=openBuy)[0][0]] = hhigh
                   #buyCounter += 1
                   openSell[np.where(openSell!=openSell)[0][0]] = llow
                   #sellCounter += 1    
               except:
                   print 'Exception'
           elif(netPos[flag] > 0):
               entryPrice = data.Open
               prevPeak = entryPrice
               if data.Close > self.prevPeak:
                   prevPeak = data.Close
               openSell[np.where(openSell!=openSell)[0][0]] = prevPeak*(1-self.stopPct)
               #sellCounter += 1
           else:
               entryPrice = data.Open
               prevTrough = entryPrice
               if data.Close < prevTrough:
                   prevTrough = data.Close
               try:
                   openBuy[np.where(openBuy!=openBuy)[0][0]] = prevTrough*(1+self.stopPct)
               except:
                   
                   print np.where(openBuy!=openBuy) 
                   print np.where(openBuy!=openBuy)[0]
                   print np.where(openBuy!=openBuy)[0][0]
                   print  openBuy[np.where(openBuy!=openBuy)[0][0]]
        return netPos
      
       
      
                
data = (pd.read_csv(r'C:\Users\Akshat Sinha\Dropbox\Classes\4075P\WC-5min.asc'))
data.Date = pd.to_datetime(data.Date)
data = data[data.Date<'1986-01-01']
print len(data)
x = Trade(data,500,0.005)
nP=x.startTrade_lite()
returnP = (data.Open-data.Open.shift(1))/data.Open