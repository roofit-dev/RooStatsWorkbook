// *******************************************************
// *** Construct a profile likelihood ratio interval   ***
// *** using the RooStats::ProfileLikelihoodCalculator ***
// *** from the model and data stored in model.root    ***
// *******************************************************

void ex03_roostats_plr_interval()
{
  // Open the ROOT file
  TFile* f = TFile::Open("model.root") ;

  // Retrieve the workspace
  RooWorkspace* w = (RooWorkspace*) f->Get("w") ;
  w->Print() ;

  // Retrieve the ModelConfig and the observed data
  // Together these uniquely define the statistical problem
  RooAbsData* data = w->data("observed_data") ;
  RooStats::ModelConfig* mc = (RooStats::ModelConfig*) w->obj("ModelConfig") ;

  // Instantiate a Profile Likelihood interval calculator
  RooStats::ProfileLikelihoodCalculator plCalc(*data,*mc);
  
  // Calculate the 90% C.L. interval
  // Note that Profile Likelihood Ratio is always a two-sided interval
  // where the definition of the interval is always uniquely defined by the technique
  // hence we only need to define the CL.
  plCalc.SetConfidenceLevel(0.90);
  RooStats::LikelihoodInterval* interval = plCalc.GetInterval();

  // Print the result
  RooRealVar* poi = (RooRealVar*) mc->GetParametersOfInterest()->first();
  double lowerLimit = interval->LowerLimit(*poi);
  double upperLimit = interval->UpperLimit(*poi);
  cout << "RESULT: " << 100*plCalc.ConfidenceLevel() << "% interval is : ["<< lowerLimit << ", "<< upperLimit <<"] "<<endl;

  // Use the visualization tool of the PLC to show how the interval was calculated
  RooStats::LikelihoodIntervalPlot *plot = new RooStats::LikelihoodIntervalPlot(interval);
  //plot->SetNPoints(50);   // Use this to reduce sampling granularity (trades speed for precision)
  plot->Draw("TF1"); gPad->Draw();

}  

 
