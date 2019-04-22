#!/usr/bin/env python 



''' 

File:   write_files_dataframe.py 
Author: David Riser 
Date:   April 16, 2019 

Purpose: This file is used to read in the files.info and output and 
dataframe that can be analyzed easier. 


'''

import pandas as pd 

def add_information(data):

    data['run'] = data['filename'].apply(lambda x: x.split('_')[-1].split('.')[0])
    data['stub'] = data['filename'].apply(lambda x: x.split('A')[-1])


if __name__ == '__main__':

    input_file = '/u/home/dmriser/clas/analysis-main/lists/info/files.info'
    output_file = '/u/home/dmriser/clas/analysis-main/lists/info/files_df.csv'

    name = [] 
    charge = [] 
    for line in open(input_file, 'r').readlines():
        tokens = line.split()
        name.append(tokens[0])
        charge.append(tokens[1])


    data = pd.DataFrame({'filename' : name, 'charge' : charge})
    add_information(data)

    data.to_csv(output_file)
