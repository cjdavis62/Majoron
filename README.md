# Majoron
Collection of codes for Majoron Search

## Overview
This is the set of codes for editing and plotting files for a Majoron search. The procedure is as follows

1. Create the MC files needed
2. Run g4cuore 
3. Run addRejected_DS.C to add rejected channels and assign datasets to the MC
4. Create a reduced data file
5. Run addRejected_Data.C to add rejected channels to the data
6. on ULITE, run PrepareDataMult with ```PrepareDataMult ${JAGS/DATADIR}/Data.root ListMC.txt ${JAGS_MCDIR} M1 M2 M2sum```
7. Then run ```jags LaunchJags.cmd```
8. Finish with ```back2ground $JAGS_DATA/Data.root ListMC_opt.txt SpectraM1.root SpectraM2.root```

