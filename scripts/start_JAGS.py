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
    parser.add_argument("-n", "--spectral_index", type=int, help="the spectral index, i.e. 1, 2, 3, 7")
    parser.add_argument("-D", "--data_name", type=str, help="the name of the data file")
    parser.add_argument("-p", "--setup_path", type=str, help="The path to the setup files, e.g. '/home/cjdavis/JAGS_setup'")
    parser.add_argument("-M", "--multiplicities", type=str, help="The multiplicities to consider, viz. M1, M2, and/or M2sum. Input as '-M M1 -M M2 -M M2sum'")
    parser.add_argument("-s", "--systematic", type=str, help="The name of the systematic, e.g. OddTowers or NoT12")
    args = parser.parse_args()

    # check that inputs point to existing places
    # note that data input test comes later as it requires a file to be sourced
    if not(os.path.isfile("{setup_path}/n{spectral_index}/setup_{systematic}_n{spectral_index}.sh".format(setup_path=args.setup_path, spectral_index=args.spectral_index, systematic=args.systematic))):
        print("Error: setup file not found at path: {setup_path}/n{spectral_index}/setup_{systematic}_n{spectral_index}.sh".format(setup_path=args.setup_path, spectral_index=args.spectral_index, systematic=args.systematic))
        sys.exit(1)
    return args

def PrepareData_script_gen(data_name, multiplicities, systematic):

    # Start the PrepareData script
    PrepareData_start = "PrepareDataMult ${{JAGS_DATADIR}}/{data_name} ListMC.txt ${{JAGS_MCDIR}} ".format(data_name=data_name)
    
    # Add each of the multiplicities
    Multiplicities_output = ""
    print(multiplicities)
    while (len(multiplicities) != 0):
        Multiplicities_output = Multiplicities_output + multiplicities.pop(0) + "_" + systematic + " "
    PrepareData_script = PrepareData_start + Multiplicities_output

    return PrepareData_script

# run the scripts
def main():

    # start with a clean slate
    os.system("clear")
    
    # get the command line arguments
    args = get_arguments()

    # source the setup file
    os.system("source {setup_path}/n{spectral_index}/setup_{systematic}_n{spectral_index}.sh".format(setup_path=args.setup_path, spectral_index=args.spectral_index, systematic=args.systematic))

    # check that the data name points to an actual place
    # this is broken for some reason. needs more testing


    #if not(os.path.isfile("${JAGS_DATADIR}/"+args.data_name)):
    #    print("Error: data file not found at path: ${{JAGS_DATADIR}}/{data_name}".format(data_name=args.data_name))
    #    sys.exit(1)

    # cd to the starting dir for Prepare Data
    os.system("cd $JAGS_WORKDIR")

    # Create the PrepareData script
    prepareDataScript = PrepareData_script_gen(args.data_name, args.multiplicities, args.systematic)
    
    os.system(prepareDataScript)

    os.system("cd JAGS_{DATA}".format(DATA=args.data_name))

    os.system("jags LaunchJags.cmd")


main()
