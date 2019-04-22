#!/usr/bin/env python 

import pandas as pd 

if __name__ == '__main__':

    data = pd.read_csv('/u/home/dmriser/clas/analysis-main/lists/info/files_df.csv')
    
    files_in_data = data['run'].unique()
    print(files_in_data)

    miss = [] 

    for rn in open('/u/home/dmriser/clas/analysis-main/lists/files/data/nathan_goodrun.dat').readlines():
        
        run = int(rn.strip())
        if run not in files_in_data:
            miss.append(run)

    print(miss)
