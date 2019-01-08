# Overview

These scripts are used to generate Toy MC for a sensitivity study on the Majoron decay rate

The sensitivity is done by the following process:
1. Sample MC according to the H<sub>0</sub> hypothesis (no Majoron signal)
    * Proportionally scale the amount of data sampled by the desired livetime
2. Run the fit on the MC according to the H<sub>1</sub> hypothesis (one of the Majoron signals)
3. Extract the 90% limit
4. Plot the distributions of the 90% limit with respect to the livetime

# Details

To Generate the MC, run `Generate_N_sets()` from GenerateToyMC.py. This script samples the Monte Carlo listed in FileList.txt based on their normalization. This is done by altering the GenerateToyMC_template.root file. The files are then placed in directories hard coded into the python script.
