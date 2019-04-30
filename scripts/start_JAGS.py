# Script to start a jags fit on ulite

# Run this script with python3.4 or greater

# Can be run either from an interactive session or from a batch job

import sys
import os

# Check version
if sys.version_info[0] != 3 or (sys.version_info[0] == 3 and sys.version_info[1] < 2):
    print("Sorry, the code requires Python 3.2 or greater")
    print("Exiting...")
    sys.exit(1)

import argparse
import math

# read arguments
def get_arguments():
    parser = argparse.ArgumentParser()
    parser.add_argument("-d", "--data_path", type=str, help="The path to the data directory")
    parser.add_argument("-D", "--data_name", type=str, help="The name of the data file. Do not include the '.root'")
    parser.add_argument("-m", "--mc_path", type=str, help="The path the MC directory")
    parser.add_argument("-w", "--work_dir", type=str, help="The path to the working directory")
    parser.add_argument("-M", "--multiplicities", type=str, action="append", help="The multiplicities to consider, viz. M1, M2, and/or M2sum. Input as '-M M1 -M M2 -M M2sum'")
    args = parser.parse_args()

    # check that inputs point to existing places
    if not(os.path.isfile(args.data_path + "/" + args.data_name + ".root")):
        print("Error: data file not found at path: {data_path}/{data_name}.root".format(data_path=args.data_path, data_name=args.data_name))
        sys.exit(1)
    if not(os.path.isdir(args.mc_path)):
        print("Error: MC directory does not exist at path: {mc_path}".format(mc_path=args.mc_path))
        sys.exit(1)
    if not(os.path.isdir(args.work_dir)):
        print("Error: Work directory not found at path: {work_dir}".format(work_dir=args.work_dir))
        sys.exit(1)
    return args

def PrepareData_script_gen(data_path, data_name, mc_path, multiplicities):

    # Start the PrepareData script
    PrepareData_start = "PrepareDataMult {data_path}/{data_name}.root ListMC.txt {mc_path} ".format(data_path=data_path, data_name=data_name, mc_path=mc_path)
    
    # pipe the multiplicity info into a new list as we don't want to change it here
    multiplicities_dummy = []
    for i in multiplicities:
        multiplicities_dummy.append(i)

    # Add each of the multiplicities
    Multiplicities_output = ""
    while (len(multiplicities_dummy) != 0):
        Multiplicities_output = Multiplicities_output + multiplicities_dummy.pop(0) + " "
    PrepareData_script = PrepareData_start + Multiplicities_output

    return PrepareData_script

def Back2ground_script_gen(data_path, data_name, multiplicities):

    # Start the back2ground script
    Back2ground_start = "back2ground_nopress {data_path}/{data_name}.root ListMC_opt.txt ".format(data_path=data_path, data_name=data_name)

    # multiplicities is mutable, so don't change it
    multiplicities_dummy = []
    for i in multiplicities:
        multiplicities_dummy.append(i)

    # Add each of the multiplicities for SpectraM*
    Multiplicities_output = ""
    while (len(multiplicities_dummy) != 0):
        Multiplicities_output = Multiplicities_output + "Spectra" + multiplicities_dummy.pop(0) + ".root "
    Back2ground_script = Back2ground_start + Multiplicities_output

    return Back2ground_script

# run the scripts
def start_jags():

    # start with a clean slate
    os.system("clear")
    
    # get the command line arguments
    args = get_arguments()

    # cd to the starting dir for Prepare Data
    os.chdir(args.work_dir)

    # Create the PrepareData script
    prepareDataScript = PrepareData_script_gen(args.data_path, args.data_name, args.mc_path, args.multiplicities)

    # Create the back2ground script
    back2groundScript = Back2ground_script_gen(args.data_path, args.data_name, args.multiplicities)

    # For double-checking back2ground script
    print(back2groundScript)
    # For double-checking preparedata script
    print(prepareDataScript)

    os.system(prepareDataScript)

    os.system("mkdir -p JAGS_{DATA}".format(DATA=args.data_name))
    os.chdir("JAGS_{DATA}".format(DATA=args.data_name))

    os.system("jags LaunchJags.cmd")

    os.system(back2groundScript)

start_jags()
