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
        

    def find_nan(self,vec):
        """return the index of the first occurence of nan in vec"""
        r = np.arange(0, len(vec), 500)
        temp=0
        t=0
        for i in range(0,len(r)-1):
            temp = vec[r[i]:r[i+1]]
            t = np.where(temp!=temp)[0]
            if len(t)!=0:
                return t[0]+r[i] 
        temp = vec[r[i+1]:]
        t = np.where(temp!=temp)[0]
        if len(t)!=0:
            return t[0] + len(t)
        return -1
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
        #hhigh = self.data.High[0:self.chnLen].max()
        #llow = self.data.Low[0:self.chnLen].min()
        openBuy = np.empty([5000,1])
        openSell = np.empty([5000,1])
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
        closeP = np.array(self.data.Close)
        openP= np.array(self.data.Open)
        highP = np.array(self.data.High)
        lowP = np.array(self.data.Low)
        
        for flag, close in np.ndenumerate(closeP[self.chnLen:-1]):
          # print flag[0]
           flag = flag[0]+self.chnLen
           openBuyExec = openBuy[close > openBuy]
           openSellExec = openSell[close < openSell]
           pos = (len(openBuyExec) - len(openSellExec)) * self.tradeSize
           
           if pos != 0:
               openBuy[close > openBuy] = np.NAN
               openSell[close < openSell] = np.NAN
               
               if (pos > 0) == (netPos[flag] > 0):
                       netPos[flag + 1] = netPos[flag] + pos
               else:
                       netPos[flag + 1] = pos
           else:
               netPos[flag+1] = netPos[flag]
               
           
               
           #Create new orders
           
           hhigh = highP[flag-self.chnLen:flag].max()
           llow = lowP[flag-self.chnLen:flag].min()
           
           if netPos[flag] == 0:
               try:
                   openBuy[np.where(openBuy!=openBuy)[0][0]] = hhigh
                   #buyCounter += 1
                   openSell[np.where(openSell!=openSell)[0][0]] = llow
                   #sellCounter += 1    
               except:
                   print 'Exception'
           elif(netPos[flag] > 0):
               entryPrice = openP[flag]
               prevPeak = entryPrice
               if close > prevPeak:
                   prevPeak = close
               openSell[np.where(openSell!=openSell)[0][0]] = prevPeak*(1-self.stopPct)
               #sellCounter += 1
           else:
               entryPrice = openP[flag]
               prevTrough = entryPrice
               if close < prevTrough:
                   prevTrough = close
               
               openBuy[np.where(openBuy!=openBuy)[0][0]] = prevTrough*(1+self.stopPct)
               
        return netPos

#Do these two lines first before running code the first time (expensive)
#data = (pd.read_csv(r'C:\Users\Akshat Sinha\Dropbox\Classes\4075P\WC-5min.asc'))
#data.Date = pd.to_datetime(data.Date)
#x = Trade(data,500,0.005)
#y=data.Close
#y[16555]=np.nan
#print x.find_nan(y)

import cProfile, pstats, StringIO
pr = cProfile.Profile()
pr.enable()
data = data[data.Date<'1986-01-01']
print len(data)
x = Trade(data,500,0.005)
nP=x.startTrade_lite()
pr.disable()
s = StringIO.StringIO()
sortby = 'cumulative'
ps = pstats.Stats(pr, stream=s).sort_stats(sortby)
ps.print_stats()
print s.getvalue() 
                
#returnP = (data.Open-data.Open.shift(1))
#pnl = np.zeros([len(data),1])
#pnl[0]= data.Open[0]*nP[0]
#i=0
#for d in pnl:
#    if i==0:
#        continue
#    pnl[i]=pnl[i-1]+returnP[i]*nP[i]
#    i += 1