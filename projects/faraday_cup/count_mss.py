#!/usr/bin/env python 

import pandas as pd 
import numpy as np 

if __name__ == '__main__':

    input_file = 'mssfiles.dat'

    name = [] 

    for f in open(input_file, 'r').readlines():
        if 'clas_' in f:
            name.append(f.strip())


    # create df 
    data = pd.DataFrame({'name' : name})
    data['run'] = data['name'].apply(lambda x: x.split('_')[-1].split('.')[0])
    data['stub'] = data['name'].apply(lambda x: x.split('A')[-1])
    
    print(data['run'].nunique())
    
