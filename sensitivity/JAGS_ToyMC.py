# Script to run JAGS on a particular MC on ULITE
# Run this script with python3.4 or greater

# This script will create a batch job to run jags on a MC file generated from GenerateToyMC.py

import sys
import os
import argparse

# Get the arguments for the script to be generated
def get_args():
    parser = argparse.ArgumentParser()

    parser.add_argument("-i", "--iterations", type=int, help="The number of iterations of MC completed")
    parser.add_argument("-l", "--livetime_dir", type=str, help="The directory of the data for a particular livetime, e.g. livetimeA")
    parser.add_argument("-q", "--queue", type=str, help="The queue to be used")
    parser.add_argument("-n", "--index", type=int, help="The Majoron spectral index to be considered, viz. 1, 2, 3, or 7")
    
    args = parser.parse_args()
    
    return args

def write_batch_script(spectral_index, livetime_dir, queue, iterations):

    # edit this to change which directory for the data files
    livetime_dir="livetimeA"
    
    #make the file to be used with pbs
    qsub_file = open("/nfs/cuore1/scratch/cjdavis/MajoronSensitivity/batchScripts/%s_%s.pbs" %(spectral_index, livetime_dir), "w")
    job_name = "n" + str(spectral_index) + "_" + str(livetime_dir)
    walltime = "24:00:00" #24 hour walltime, 12 is too short
    
    log_file_dir = "localhost:/nfs/cuore1/scratch/cjdavis/MajoronSensitivity/log/"
    # Write the frontmatter for the PBS job
    qsub_file.write("#PBS -N %s\n" %(job_name))
    qsub_file.write("#PBS -q %s\n" %(queue))
    qsub_file.write("#PBS -l walltime={walltime} nodes=1:ppn=1\n".format(walltime=walltime))
    qsub_file.write("#PBS -M christopher.davis@yale.edu\n")
    qsub_file.write("#PBS -m ae\n")
    qsub_file.write("#PBS -o %s\n" %(log_file_dir))
    qsub_file.write("#PBS -e %s\n" %(log_file_dir))
    qsub_file.write("#PBS -t 0-%s%%%s\n" %(iterations-1, iterations))
    qsub_file.write("taskID=$PBS_ARRAYID\n")

    # setups for the script
    qsub_file.write("source /cuore/soft/root_v5.34.34/bin/thisroot.sh\n")
    qsub_file.write("export PATH=$PATH:/nfs/cuore1/scratch/spozzi/simulations/JAGS-4.3.0/install/bin:/nfs/cuore1/scratch/spozzi/simulations/JagsBkgAnalysis/Codes/bin/\n")
    qsub_file.write("export PATH=$PATH:/nfs/cuore1/scratch/cjdavis/MajoronSensitivity/bin/\n")
    qsub_file.write("export BOOT_DIR=/nfs/cuore1/scratch/cjdavis/MajoronSensitivity/n%s/%s/$PBS_ARRAYID\n" %(spectral_index, livetime_dir))
    qsub_file.write("export JAGS_WORKDIR=${BOOT_DIR}\n")
    qsub_file.write("export JAGS_TXTDIR=${BOOT_DIR}\n")
    
    # needs to be updated for each time this is run
    qsub_file.write("export JAGS_DATADIR=../../../MC_iterations/%s/\n" %(livetime_dir))
    
    qsub_file.write("export JAGS_MCDIR=/nfs/cuore1/data/simulation/CUORE/2017/ntp/\n")

    # start in the boot directory
    qsub_file.write("mkdir -p ${BOOT_DIR}\n")
    qsub_file.write("cd ${BOOT_DIR}\n")
    # copy things over to the new directory that will be needed
    qsub_file.write("cp ../../../stash/Parameters*.txt .\n")
    qsub_file.write("cp ../../../stash/PeakList*.txt .\n")
    qsub_file.write("cp ../../../stash/ListMC_n%s.txt ListMC.txt\n" %(spectral_index))
    qsub_file.write("cp ../../../stash/AlphaBinning-*.txt .\n")
    # The actual job
    qsub_file.write("python3.5 ../../../start_JAGS.py -n %s -d ${JAGS_DATADIR} -D MC_data_${taskID} -m ${JAGS_MCDIR} -M M1 -M M2 -M M2sum -s standard -w ${BOOT_DIR}\n" %(spectral_index)) 
    qsub_file.write("cp b2g-MC_data_${task_ID}.txt /nfs/cuore1/scratch/cjdavis/MajoronSensitivity/n%s/%s/b2g_files/." %(spectral_index, livetime_dir))

# create the directory where the output b2g files will go. Allows for easily seeing which jobs failed or had other shenanigans
def create_output_file(spectral_index, livetime_name):
    # use makedirs because it makes directories in a recursive and safe way (no craziness from \rm -rf *\
    os.makedirs("/home/nfs/cuore1/scratch/cjdavis/MajoronSensitivity/n%s/%s/b2g_files/" %(spectral_index, livetime_name))

# Start the program
args = get_args()

write_batch_script(args.index, args.livetime_dir, args.queue, args.iterations)
