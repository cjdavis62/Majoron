{

  // Get the file to generate from
  //TFile *MC_file = new TFile("_filename_");

  // The normalization factor
  double normalization_factor = __normalization_factor__;
  // The livetime factor
  double livetime_factor = __livetime_factor__;
  // number of events to generate
  double events = normalization_factor * livetime_factor;

  std::cout << normalization_factor << std::endl;
  
}
