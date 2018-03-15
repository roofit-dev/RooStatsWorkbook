// *******************************************************
// *** Construct a Bayesian interval                   ***
// *** using the RooStats::BayesianCalculator          ***
// *** from the model and data stored in model.root    ***
// *******************************************************

void ex04_roostats_bayes_interval()
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

  // Instantiate a Bayesian interval calculator
  RooStats::BayesianCalculator bayesianCalc(*data,*mc);

  // ******************************************************************
  // *** Statistical configuration of Bayesian interval calculation ***
  // ******************************************************************
  
  // Calculate the 90% C.L. central interval
  bayesianCalc.SetConfidenceLevel(0.90);

  // Choose central interval
  bayesianCalc.SetLeftSideTailFraction(0.5);
  //bayesianCalc.SetLeftSideTailFraction(0);  // alternative for upper limit
  //bayesianCalc.SetShortestInterval();       // alternative for shortest intervals

  // Optionally introduce a prior pdf on mu
  bool use_prior=false ;
  if (use_prior) {
    w->factory("EXPR::prior('1/sqrt(mu)',mu)") ;
    bayesianCalc.SetPriorPdf(*w->pdf("prior")); 
  }

  // ********************************************
  // Technical parameter of Bayesian calculator *
  // ********************************************

  // Choose best numeric integration algorithm 
  RooAbsReal::defaultIntegratorConfig()->method1D().setLabel("RooAdaptiveGaussKronrodIntegrator1D");

  // Select number of scan points
  bayesianCalc.SetScanOfPosterior(500);

  // Calculate interval
  RooStats::SimpleInterval* interval = bayesianCalc.GetInterval();

  // Print the result
  RooRealVar* poi = (RooRealVar*) mc->GetParametersOfInterest()->first();
  double lowerLimit = interval->LowerLimit();
  double upperLimit = interval->UpperLimit();
  cout << "RESULT: " << 100*bayesianCalc.ConfidenceLevel() << "% interval is : ["<< lowerLimit << ", "<< upperLimit <<"] "<<endl;

  // Use the visualization tool of the Bayesian calculator to show how the interval was calculated
  RooPlot* plot = bayesianCalc.GetPosteriorPlot();
  plot->Draw() ;
}  

 
