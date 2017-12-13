#!/bin/tcsh 

set runs = `cat superGoodRuns.dat`

foreach run ($runs) 
    echo "/volatile/clas12/dmriser/analysis/e1f_analysis/skim/elastic/$run.root"
end
