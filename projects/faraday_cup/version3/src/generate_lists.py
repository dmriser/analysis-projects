#!/usr/bin/env python 


''' 

This file reads in multiple files of lists and charges 
and then outputs files that need charges from mss and 
files that can be gotten from the lists. 

'''

import os
import pandas as pd 

def read_nathans_runs(input_dir, file_name):

    files = []
    with open('{}/{}'.format(input_dir, file_name)) as input_file:

        for line in input_file.readlines():
            files.append(int(line.strip()))

    return files

def read_mss_runs(input_dir, file_name):

    files = [] 
    with open('{}/{}'.format(input_dir, file_name)) as input_file:
        
        for line in input_file.readlines():
            if 'clas_' in line:
                files.append(line.strip())

    # Create a dataframe 
    data = pd.DataFrame({'file':files})
    data['run'] = data['file'].apply(lambda x: x.split('_')[-1].split('.')[0]).astype(int)

    return data['run'].unique()

def read_file_charge(input_dir, file_name):
    return pd.read_csv(input_dir + '/' + file_name)

def read_run_charge(input_dir, file_name):
    
    ''' The table schema is:
    [run number, charge, num. events, daq scale, beam energy]
    '''

    runs = []
    charges = [] 
    with open('{}/{}'.format(input_dir, file_name)) as input_file:
        for line in input_file.readlines():
            tokens = line.strip().split()
            
            runs.append(int(tokens[0]))
            charges.append(int(tokens[1]))

    return pd.DataFrame({'run' : runs, 'charge' : charges})

if __name__ == '__main__':

    # Setup input folder 
    input_dir = os.path.abspath('../data')
    print('Reading lists from {}'.format(input_dir))

    nathans_runs = read_nathans_runs(input_dir, 'nathan.dat')
    mss_runs = read_mss_runs(input_dir, 'mss.dat')
    #    print(nathans_runs)

    print('Nathan has {} runs'.format(len(nathans_runs)))
    print('MSS has {} runs'.format(len(mss_runs)))

    file_charges = read_file_charge(input_dir, 'files_charge.csv')
    run_charges = read_run_charge(input_dir, 'run_charge.dat')

    #    print(file_charges.head())
    #    print(run_charges.head())

    zero_charge_runs = list(run_charges[run_charges['charge'] == 0]['run'])
    #    print(zero_charge_runs)

    nonzero_charge_runs = list(run_charges[run_charges['charge'] != 0]['run'])
    #    print(nonzero_charge_runs)
    
    # big problem files are those that we need that dont 
    # exist in mss for some reason, this should be none 
    big_problem_runs = [f for f in nathans_runs if f not in mss_runs]
    #    print(big_problem_runs)

    # these are files we need to fix 
    runs_to_fix = [r for r in nathans_runs if r not in nonzero_charge_runs]
    #    print(runs_to_fix)

    # runs that are not in need of fixing 
    runs_not_to_fix = [r for r in nathans_runs if r in nonzero_charge_runs]
    #    print(runs_not_to_fix)
    
    print('Runs to fix {}'.format(len(runs_to_fix)))
    print('Runs not to fix {}'.format(len(runs_not_to_fix)))
    print('Runs to fix + runs not to fix {}'.format(len(runs_to_fix) + len(runs_not_to_fix)))

    run_charges['status'] = run_charges['run'].apply(lambda x: x in nathans_runs)
    run_charges[run_charges['status'] == True].to_csv('charges_for_analysis.csv', index = False)
