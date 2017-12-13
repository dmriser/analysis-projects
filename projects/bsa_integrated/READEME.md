# Project Structure 

- This project is configuration file based, all the information about what happened in some run of the code should be contained in the configuration file.  Each 
  configuration file specifies one and only one way to go through the set of options and parameters for the codes.  One should be able to find out exactly what 
  went into an analysis file just by looking at the config.
- Because each config file has all the options, one root file is written out for each config file.  In the case that the farm scripts are used the output file has 
  to be moved to the destination, see below.

## Directory Setup 

The structure of the directories is important for the codes to run properly and the project to stay organized.  After the filling codes run the 
output root file will be placed in the /volatile/clas12/dmriser/farm_out/ folder with appropriate sub-folders for each config.  In this case the 
user has to move the file to its final home, this is done when running the mergeOutput script found in the farm folder.  This script setup your 
directory tree in /volatile/clas12/dmriser/analysis/asymmetry/ under the configuration name.  The rest of the codes will look in this location
as specified in your config file under the option "analysis_home" in the system section.