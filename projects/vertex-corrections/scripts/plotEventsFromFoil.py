#!/usr/bin/env python

import argparse 
import glob 
import matplotlib.pyplot as plt 
import numpy as np 
import ROOT 
import sys 
import time 

def process_file(input_folder):

    chain = ROOT.TChain('h22')

    for input_filename in glob.glob(input_folder + '*.root'):
        chain.AddFile(input_filename)

    print('File loaded with %d entries' % chain.GetEntries())

    data = {}
    data['vertex'] = []

    t_start = time.time() 
    for index, event in enumerate(chain):
        if index%1000 == 0:
            sys.stdout.write('\rProcessing (%d/%d)' % (index, chain.GetEntries()))
            sys.stdout.flush() 

        if event.id[0] is 11 and event.ec_sect[0] > 0:
            if event.vz[0] < 0.5 and event.vz[0] > -3.0:
                vertex = np.array([event.vx[0], 
                                   event.vy[0],
                                   event.vz[0]])
                data['vertex'].append(vertex)
        


    t_run = float(time.time() - t_start) 
    events_per_second = chain.GetEntries()/t_run 
    
    print('\nProcessed in %.2f seconds with %.2e events/sec.' % (t_run, events_per_second))

    data['vertex'] = np.array(data['vertex'])

    plt.subplot(1,3,1)
    plt.hist2d(data['vertex'][:,0], data['vertex'][:,1], bins=100, 
               range=[[-1.0, 1.0], [-1.0, 1.0]]);

    plt.subplot(1,3,2)
    plt.hist(data['vertex'][:,0], bins=np.linspace(-3, 3, 100));

    plt.subplot(1,3,3)
    plt.hist(data['vertex'][:,1], bins=np.linspace(-3, 3, 100));

    plt.show() 

if __name__ == '__main__':

    argparser = argparse.ArgumentParser()
    argparser.add_argument('-i', '--input_folder', required=True)
    argparser.add_argument('-o', '--output_file', default='vertex.root')
    args = argparser.parse_args()

    # go go go 
    process_file(args.input_folder)
