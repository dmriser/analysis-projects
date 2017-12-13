#!/usr/bin/python 

import glob
import numpy as np
import re 

def get_runno(filename):
    m = re.search('[3-4][0-9][0-9][0-9][0-9]', filename)
    return m.group(0)

def get_filenumber(filename):
    m = re.search('[a?A][0-9][0-9]', filename)
    msub = re.search('[0-9][0-9]', m.group(0))
    return msub.group(0)

def get_info(filename):
    return get_runno(filename), get_filenumber(filename)

runs = []
all_h22files = glob.glob('/volatile/clas12/dmriser/analysis/ef1_analysis/root_files/*.root')
all_h10files = glob.glob('/volatile/clas12/dmriser/analysis/e1f_analysis/pass2/*.root')

list_22 = []
for file in all_h22files:
    r, s = get_info(file)
    list_22.append(r + '.A' + s)
    
list_10 = []
for file in all_h10files:
    r, s = get_info(file)
    list_10.append(r + '.A' + s)
    

common_files = list(set(list_10) & set(list_22))

for f in common_files:
    print f
