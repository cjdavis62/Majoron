### Python script to run GenerateToyMC_tempate.cxx

# Reads in a set of MC files and their normalization factors to sample for ToyMC

import numpy as np
import os

def do_stuff():
    pass

def add_MC_dir(MC_file):
    MC_dir = "/projects/cuore/simulation/"
    return MC_dir+MC_file

# Read in MC files and normalization factors
def Read_MC_files(filename):
    # give the datatype for the elements
    dtype_MC_files = np.dtype([('filename', np.unicode_,64), ('norm_factor', np.float64)])
    MC_file = np.loadtxt(filename, dtype = dtype_MC_files)

    return MC_file

# Use sed on the template script to change the files
def generate_from_template(template_filename, normalization_factor, livetime_factor):

    temp_name_A = "GeneratorScripts/" + template_filename + "_tempA"
    temp_name_B = "GeneratorScripts/" + template_filename + "_tempB"
    os.system("sed 's/__normalization_factor__/%s/' < GenerateToyMC_template.cxx  > %s" %(normalization_factor, temp_name_A))
    os.system("sed 's/__livetime_factor__/%s/' < %s > %s" %(livetime_factor, temp_name_A, temp_name_B))
    os.system("mv %s GeneratorScripts/%s; rm %s" %(temp_name_B, template_filename, temp_name_A))
            
# loop over all the needed MC files
def Loop_over_MC_files(read_file_array, livetime_factor):
    for file_name, normalization in read_file_array:
        # Create template filename
        template_file_name = "GenerateToyMC_" + file_name[:-5]
        generate_from_template(template_file_name, normalization, livetime_factor)

# Generate N ToyMC
def Generate_N_sets():
    do_stuff()

# main stuffs
livetime_factor = 1
read_file_array = Read_MC_files("FileList.txt")

Loop_over_MC_files(read_file_array, livetime_factor)

#edit_template("GenerateToyMC_template.cxx", )
