#!/usr/bin/python 

import KeppelModel, BostedModel  

# beam energy 
beam = 5.75

# QQ Binning Parametesr 
numberQQBins = 40
qqMin = 1.0
qqMax = 4.8 
qqWidth = (qqMax-qqMin)/numberQQBins

# W Binning Parameters 
numberWBins = 40
wMin = 1.1
wMax = 2.7 
wWidth = (wMax-wMin)/numberWBins

keppel = []

for i in range(1,numberQQBins):
    for j in range(1, numberWBins):
        qq = qqMin + i*qqWidth
        w = wMin + j*wWidth 

        xsKeppel = KeppelModel.tkeppel(beam,qq,w)
        keppel.append(xsKeppel)
        print "{0} {1} {2}".format(i, j, xsKeppel)

