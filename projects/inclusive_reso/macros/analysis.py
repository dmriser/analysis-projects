#!/usr/bin/env python 

import ROOT 
import argparse 

def init_histos():
    histos = {}

    histos['w'] = {}
    histos['w']['reso'] = ROOT.TH1F('w_reso', 'w_reso', 200, -1, 1)

    histos['q2'] = {}
    histos['q2']['reso'] = ROOT.TH1F('q2_reso', 'q2_reso', 200, -1, 1)

    return histos

def fill_histos(event, histos):
    histos['w']['reso'].Fill(event.w-event.w_gen)
    histos['q2']['reso'].Fill(event.q2-event.q2_gen)

def save_histos(output_file, histos):
    of = ROOT.TFile.Open(output_file, 'recreate')

    for k1, v1 in histos.iteritems():
        for key, val in histos[k1].iteritems():
            val.Write()

    of.Close()

def good_event(event):
    return event.w > 0

def main(input_file, output_file):
    f = ROOT.TFile.Open(input_file)
    h = init_histos()

    for event in f.events:
        if good_event(event):
            fill_histos(event,h)

    save_histos(output_file, h)

if __name__ == "__main__":
    arg_parser = argparse.ArgumentParser()
    arg_parser.add_argument('-i', '--input_file', required=True)
    arg_parser.add_argument('-o', '--output_file', required=True)
    args = arg_parser.parse_args()

    main(args.input_file, args.output_file) 
