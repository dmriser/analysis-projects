#!/usr/bin/env python 

import glob 
import numpy as np 
import pandas as pd 
import ROOT 

def build_histogram_title(base_name, sector, slice_index):
    return '{}_wByQQ_s{}_slice{}'.format(base_name, sector, slice_index)

def root_histo_to_numpy(histogram):
    ''' Accept a TH1 object and return three 
    numpy arrays which indicate the bin content, 
    the error, and the axis values. '''

    n_bins = histogram.GetNbinsX()
    axis_values = np.zeros(n_bins)
    content = np.zeros(n_bins)
    errors = np.zeros(n_bins)

    for idx in range(n_bins):
        axis_values[idx] = histogram.GetBinCenter(idx + 1)
        content[idx] = histogram.GetBinContent(idx + 1)
        errors[idx] = histogram.GetBinError(idx + 1)

    return axis_values, content, errors



if __name__ == '__main__':

    n_sectors = 7 
    n_w_bins = 40
    n_q2_bins = 10 
    w_bins = np.linspace(1.1, 2.1, n_w_bins + 1)
    q2_bins = np.linspace(1.7, 4.2, n_q2_bins + 1)

    input_directory = '/volatile/clas12/dmriser/rootFiles/inclusive/xs'

    input_files = ['xsBugFix.root', 'xsBugFixNoES.root']
    root_files = {}

    for input_filename in input_files:
        print('Opening {}'.format(input_filename))

        root_files[input_filename] = ROOT.TFile.Open(input_directory + '/' + input_filename)    

    # See which one is better. 
    for sector_index in range(n_sectors):        
        for q_index in range(n_q2_bins):

            chi2_dict = {}
            for root_name, root_file in root_files.iteritems():

                # build the title for the cross section
                # histogram and load it into numpy arrays 
                title = build_histogram_title(
                    base_name = 'radCorrCrossSection',
                    sector = sector_index, 
                    slice_index = q_index
                )
                
                _, xs_value, xs_error = root_histo_to_numpy(
                    root_file.Get(title)
                )
            
                # build the title for the model
                # and load it into arrays 
                title = build_histogram_title(
                    base_name = 'modelCrossSection',
                    sector = sector_index, 
                    slice_index = q_index
                )
                
                _, model_value, model_error = root_histo_to_numpy(
                    root_file.Get(title)
                )
            
                chi2_dict[root_name] = np.sum( (model_value - xs_value)**2 / xs_error**2 )

            best_chi2 = np.inf 
            best_file = None
            for root_name, chi2 in chi2_dict.iteritems():
                if chi2 < best_chi2:
                    best_file = root_name

            print(sector_index, q_index, best_file, best_chi2)
