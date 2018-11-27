#!/usr/bin/env python 

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

def append_simple(output_dict, root_file, 
                  base_name, sector_index, 
                  slice_index, count_name, 
                  error_name):
    ''' Simply add to the existing dict. '''

    title = build_histogram_title(base_name, sector_index, slice_index)
    _, values, errors = root_histo_to_numpy(
        root_file.Get(title)
    )
    
    for value, error in zip(values, errors):
        output_dict[count_name].append(value)
        output_dict[error_name].append(error)

def build_output_dict(cols_with_errors, cols_without_errors):
    ''' Setup structure of dataframe '''
    data = {}

    for col in cols_without_errors:
        data[col] = []

    for col in cols_with_errors:
        data[col + '_value'] = []
        data[col + '_error'] = []

    return data

def add_basic_cols(output_data, sector_index, w_index, q_index,
                   w_value, q2_value):
    ''' Really quite simple things are added like this. ''' 
    output_data['sector'].append(sector_index)
    output_data['w_bin'].append(w_index)
    output_data['w_value'].append(w_value)
    output_data['q2_bin'].append(q_index)
    output_data['q2_value'].append(q2_value)


if __name__ == '__main__':

    input_filename = '/volatile/clas12/dmriser/rootFiles/inclusive/xs/xsBugFix.root'
    root_file = ROOT.TFile.Open(input_filename)    

    n_sectors = 7 
    n_w_bins = 40
    n_q2_bins = 10 
    w_bins = np.linspace(1.1, 2.1, n_w_bins + 1)
    q2_bins = np.linspace(1.7, 4.2, n_q2_bins + 1)

    # Setup Output Structure 
    cols_with_errors = ['data', 'rec', 'gen', 
                        'cross_section', 
                        'radiative_correction', 
                        'acceptance', 
                        'model_cross_section',
                        'bin_center_correction']
    cols_without_errors = ['sector', 'w_bin', 'q2_bin',
                           'w_value', 'q2_value']

    naming_lookup_dict = {}
    naming_lookup_dict['dataEvents'] = 'data'
    naming_lookup_dict['recEventsRad'] = 'rec'
    naming_lookup_dict['genEventsRad'] = 'gen'
    naming_lookup_dict['crossSection'] = 'cross_section'
    naming_lookup_dict['radCorr'] = 'radiative_correction'
    naming_lookup_dict['acceptance'] = 'acceptance'
    naming_lookup_dict['modelCrossSection'] = 'model_cross_section'
    naming_lookup_dict['binCenterCorrection'] = 'bin_center_correction'

    output_data = build_output_dict(cols_with_errors, 
                                    cols_without_errors
    )

    for sector_index in range(n_sectors):
        
        for q_index in range(n_q2_bins):
            q2_value = 0.5 * (q2_bins[q_index + 1] - q2_bins[q_index]) + q2_bins[q_index] 

            for key, value in naming_lookup_dict.iteritems():
                append_simple(
                    output_data, 
                    root_file,
                    key, 
                    sector_index,
                    q_index, 
                    value + '_value',
                    value + '_error'
                )

            for w_index in range(n_w_bins):

                # Calculate value for this bin 
                w_value = 0.5 * (w_bins[w_index + 1] - w_bins[w_index]) + w_bins[w_index] 

                add_basic_cols(output_data, sector_index, w_index, q_index, w_value, q2_value)


    df = pd.DataFrame(output_data)
    df.to_csv('inclusive_cross_section.csv', index = False)
