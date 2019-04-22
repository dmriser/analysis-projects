#!/usr/bin/env python 

''' 

David Riser
April 22, 2019

verify_nathan_files_have_charge.py 

The purpose of this python script is to open 
Nathan's good run list as well as my list of 
runs with Faraday cup accumulated charge and 
ensure that all of his files exist in my list 
and have nonzero charge. 

'''

import os

def read_run_charge(input_dir, file_name):
    ''' The table schema is:
    [run number, charge, num. events, daq scale, beam energy]
    '''

    # k: run number, v: charge 
    data = {} 
    with open('{}/{}'.format(input_dir, file_name)) as input_file:
        for line in input_file.readlines():
            tokens = line.strip().split()
            data[int(tokens[0])] = int(tokens[1])

    return data

def read_nathans_runs(input_dir, file_name):

    files = []
    with open('{}/{}'.format(input_dir, file_name)) as input_file:

        for line in input_file.readlines():
            files.append(int(line.strip()))

    return set(files)

def create_charged_set(data):

    runs = [] 

    for run, charge in data.items():
        if charge != 0:
            runs.append(run)

    return set(runs)
            
if __name__ == '__main__':
    
    print('Starting verification...')
    
    base_path = os.path.abspath('../')
    nathans_runs = read_nathans_runs(base_path + '/data', 'nathan.dat')
    run_charge = read_run_charge(base_path + '/data', 'run_charge.dat')
    charged_runs = create_charged_set(run_charge)

    have_files = nathans_runs.issubset(charged_runs)

    if have_files:
        print('All files are accounted for!')
    else:
        print('Missing {} files'.format(nathans_runs.difference(charged_runs)))

    print('Missing {} files'.format(charged_runs.difference(nathans_runs)))
