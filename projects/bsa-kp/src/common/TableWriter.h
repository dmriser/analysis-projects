#ifndef table_writer_h
#define table_writer_h 


enum axes {
  x, 
  z, 
  q2, 
  pt
};

class TableWriter {

 public:
  TableWriter(){ 
    for(int iaxis=0; iaxis<NAXES; iaxis++)
      for(int ihel=0; ihel<NHEL; ihel++)
	for(int ibin=0; ibin<NBINS; ibin++)
	  for(int iphi=0; iphi<NAXES; iphi++)
	    counts[iaxis][ihel][ibin][iphi] = 0;
  }

  ~TableWriter();

 protected:
  const static int NBINS    = 10; 
  const static int NPHIBINS = 12; 
  const static int NAXES    = 4;
  const static int NHEL     = 2; 

  // to hold the information 
  int counts[NAXES][NHEL][NBINS][NPHIBINS];

  // the binning information 
  // x, z, q2, pt
  float binLimits[NAXES][NBINS+1] = 
    {
      {
	0.106729, 0.143282, 0.166495, 0.188544, 0.210731, 0.233939, 0.259004, 0.287564, 0.322765, 0.375026, 0.601938
      }, 
      {
	0.121260, 0.239688, 0.298012, 0.345193, 0.388443, 0.431311, 0.475792, 0.524436, 0.581625, 0.657274, 0.893084
      },
      {
	1.000001, 1.173185, 1.299205, 1.416112, 1.539892, 1.679987, 1.845621, 2.051025, 2.321366, 2.734286, 4.736822
      },
      {
	0.000284, 0.202108, 0.274544, 0.328453, 0.376169, 0.422123, 0.469541, 0.521940, 0.585257, 0.674587, 1.193084
      }
    };

  float phiLimits[NPHIBINS+1] = {-180.0 };
  
};

#endif 
