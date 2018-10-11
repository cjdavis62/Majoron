# Calculates the half-life determined by a JAGS fit

# takes as input the exposure in kg-yr, the normalization constant from JAGS, and the number of MC chains generated (grab from g4cuore output in the infoTree directory)

# * The exposure
# * The normalization constant
# * Number of generated chains in MC

import sys
if sys.version_info[0] != 3 or (sys.version_info[0] == 3 and sys.version_info[1] < 2):
    print("Sorry, the code requires Python 3.2 or greater")
    print("Exiting...")
    sys.exit(1)

import argparse
import math

# read arguments
def get_arguments():
    parser = argparse.ArgumentParser()
    parser.add_argument("-E", "--exposure", type=float, help="The exposure in kilogram-years with the mass as the mass of Te (not Te130)")
    parser.add_argument("-N", "--normalization", type=float, help="The normalization constant from JAGS")
    parser.add_argument("-C", "--nchains", type=float, help="The number of chains generated in the MC")

    args = parser.parse_args()

    if not (args.exposure or args.normalization or args.nchains):
        print("Need to include all arguments")
        sys.exit(1)

    if (args.exposure <= 0 or args.normalization <= 0 or args.nchains <=0):
        print("Need all arguments positive and nonzero")
        sys.exit(1)
        
    return args

# calculation of the half-life
def calculate_halflife(exposure, nchains, normalization):
    navogadro=6.022140857 * pow(10,23) # Avogadro's number
    isotopicAbundance = 0.34167 # isotopic abundance as a %
    molarMassTe = 0.1276 # molar mass in kg
    molarMassO2 = 0.032 # molar mass in kg
    dataefficiency = 0.9103 # pm 0.0016
    mcefficiency = 0.95
    
    # do all the math as logs to save precision
    halflife_log = math.log(math.log(2)) + math.log(exposure) + math.log(navogadro) + math.log(isotopicAbundance) + math.log(dataefficiency)- (math.log(molarMassTe + molarMassO2) + math.log(nchains) + math.log(normalization) + math.log(mcefficiency))

    
    halflife = math.exp(halflife_log) # unlog the halflife

    return halflife
    
arguments = get_arguments()

halflife = calculate_halflife(arguments.exposure, arguments.nchains, arguments.normalization)

print("half-life: %s yr" %(halflife))



