# This script reads in all the completed b2g-MC_data_*.txt files from a completed set of batch jobs
# It then outputs the information for the Majoron_n*_g4cuore spectrum into a .dat file that can be easily read by ROOT to plot

import sys
import os
import argparse
sys.path.append("..")
import halflifeCalculator_module as halflife_calculator

def get_arguments():
    parser = argparse.ArgumentParser()

    parser.add_argument("-i", "--number_of_iterations", type=int, help="The number of iterations of ToyMC to go over")
    parser.add_argument("-n", "--spectral_index", type=int, help="The spectral index to be considered, e.g., '-n 1' for spectral index 1")
    parser.add_argument("-l", "--livetime", type=float, help="The livetime ratio to be considered")
    parser.add_argument("-d", "--livetime_dir", type=str, help="The name of the directory for the livetime")

    args = parser.parse_args()

    # Make sure got all the args we need
    if (args.spectral_index is not None  and args.number_of_iterations is not None and args.livetime is not None):
    
        return args
    else:
        print("Need to have all the args!")
        parser.print_help()
        sys.exit(1)
    

# find a substring in a line and return the entire line
def find_substring_line(substring, infile_name):
    infile = open(infile_name)
    occurrences = 0       # use this to grab the 2nd occurrence of the substring
    for line in infile:
        if substring in line:
            occurrences += 1
            if occurrences == 2:
                outstring = line
                out_substrings = outstring.split(" ")
                out_substrings[:] = [substring for substring in out_substrings if (substring != "" and substring != "\n")]
                return out_substrings
    print("Broken input file")
    sys.exit(1)
            
def calculate_halflife(livetime, normalization):

    standard_exposure = 86.3       # Standard Exposure is 86.3 kg
    exposure = livetime * standard_exposure

    nchains = 5e8
    
    return halflife_calculator.calculate_halflife(exposure, nchains, normalization)
    
def iterate_over_files(number_of_iterations, livetime, livetime_dir, spectral_index):
    Majoron_substring = "Majoron_n%s_g4cuore" %(spectral_index)

    outfile = open("b2g_files/n%s/n%s_%s.dat" %(spectral_index, spectral_index, livetime_dir), "w")
    
    for i in range(number_of_iterations):
        infile_name = "b2g_files/n%s/%s/b2g-MC_data_%s.txt" %(spectral_index, livetime_dir, i)
        print(infile_name)
        Majoron_fit_results = find_substring_line(Majoron_substring, infile_name)
        Majoron_90 = float(Majoron_fit_results[7]) # 90% limit
        Majoron_mean = float(Majoron_fit_results[2]) # Gaussian mean
        Majoron_error = float(Majoron_fit_results[3]) # Gaussian error

        Majoron_relative_error = Majoron_error / Majoron_mean
        
        halflife_90 = calculate_halflife(livetime, Majoron_90)
        halflife_mean = calculate_halflife(livetime, Majoron_mean)
        halflife_error = halflife_mean * Majoron_relative_error
        
        outfile.write("%s\t%s\t%s\t%s\n" %(livetime, halflife_90, halflife_mean, halflife_error))

    outfile.close()
            
args = get_arguments()


Majoron_substring = "Majoron_n%s_g4cuore" %(args.spectral_index)
iterate_over_files(args.number_of_iterations, args.livetime, args.livetime_dir, args.spectral_index)
