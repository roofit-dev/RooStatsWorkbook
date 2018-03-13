
void ex09_roostats_cls_limit_toys()
{
  // Open the ROOT file
  TFile* f = TFile::Open("model.root") ;

  // Retrieve the workspace
  RooWorkspace* w = (RooWorkspace*) f->Get("w") ;
  w->Print() ;

  // *******************************************************
  // *** Retrieve the ModelConfig for the S+B hypothesis ***
  // *******************************************************

  // Retrieve the ModelConfig and the observed data
  // Together these uniquely define the statistical problem
  RooAbsData* data = w->data("observed_data") ;
  RooStats::ModelConfig* sbModel = (RooStats::ModelConfig*) w->obj("ModelConfig") ;

  // *********************************************************
  // *** Construct a ModelConfig for the B-only hypothesis ***
  // *********************************************************

  // For a CLS-style limit calculation (hypothesis test inversion)
  // we need an explicit specification of the background-only
  // hypothesis == another RooStats::ModelConfig that describe the B-only scenario
  RooStats::ModelConfig* bModel = (RooStats::ModelConfig*) sbModel->Clone("BonlyModel") ;

  // Here we take a little shortcut from universality by assuming
  // that the POI=0 scenario corresponds to the background-only scenario
  // Set value POI parameter to zero
  RooRealVar* poi = (RooRealVar*) bModel->GetParametersOfInterest()->first();
  poi->setVal(0) ;
  // Configure bModel to encode current poi=0 scenario as its hypothesis
  bModel->SetSnapshot( *poi  );
  
  // NB: To make CLS-style hypothesis calculation macros truly universal
  // workspace files should contain both ModelConfigs upfront


  // *****************************************************************************
  // *** Construct an hypothesis p-value calculator,                           ***
  // *** i.e the calculation of p(sbModel) and p(bModel) for the observed data ***
  // *****************************************************************************

  // Instantiate hypothesis testing calculator that generates toy datasets
  // to obtain the distribution of the profile likelihood ratio test statistic.
  // This calculator is much more time consuming than the asymptotic calculator
  // but is also valid in the low statistics regime
  RooStats::FrequentistCalculator freqCalc(*data, *bModel, *sbModel);

  // *** Statistical configuration of calculator ***


  // The frequentist calculator is more general than the asymptotic calculator:
  // it can calculate distributions for _any_ test statistic. So here we
  // have to tell it that we want the profile likelihood ratio test statistic
  RooStats::ProfileLikelihoodTestStat* plr = new RooStats::ProfileLikelihoodTestStat(*sbModel->GetPdf());

  // Configure calculator for a limit (=one-sided interval)
  plr->SetOneSided(true);  

  // Specifically we have to tell the Toy MC sampler part of the calculator what
  // the relevant test statistic is
  RooStats::ToyMCSampler* toymcs = (RooStats::ToyMCSampler*) freqCalc.GetTestStatSampler();
  toymcs->SetTestStatistic(plr);

  // If we use the frequentist calculator for counting experiments (instead of models of distributions)
  // we should instruct the sampler to generate one event for each toy. [ This is the case because
  // we model counting experiments in RooFit as a single observation in distribution of event counts  
  if (!sbModel->GetPdf()->canBeExtended()) {
    toymcs->SetNEventsPerToy(1);
  }

  // *** Technical configuration of calculator ***

  // Sample 1000 toys for SB and B hypothesis respectively to model their distributions
  // (Here you can trade speed vs accuracy)
  freqCalc.SetToys(1000,1000) ;

  // *****************************************************************************
  // *** Construct an hypothesis test inverter                                 ***
  // *** i.e. a tool that can calculate the POI value for which (in this case) ***
  // *** CLS==p(sbModel)/(1-pModel) takes a certain value                      ***
  // *** This inversion requires a scan over possible values of mu             ***               
  // *****************************************************************************

  RooStats::HypoTestInverter inverter(freqCalc);

  // Statistical configuration of hypothesis test inverter
  inverter->SetConfidenceLevel(0.90);
  inverter->UseCLs(true); 

  // Technical configuration of hypothesis test inverter
  inverter->SetVerbose(false);
  inverter->SetFixedScan(10,0.0,6.0); // set number of points , xmin and xmax 

  // Perform calculation of limit
  RooStats::HypoTestInverterResult* result =  inverter->GetInterval();

  // Print observed limit 
  cout << 100*inverter.ConfidenceLevel() << "%  upper limit : " << result->UpperLimit() << endl;
  
  // compute expected limit
  std::cout << "Expected upper limits, using the B (alternate) model : " << std::endl;
  std::cout << " expected limit (median) " << result->GetExpectedUpperLimit(0) << std::endl;
  std::cout << " expected limit (-1 sig) " << result->GetExpectedUpperLimit(-1) << std::endl;
  std::cout << " expected limit (+1 sig) " << result->GetExpectedUpperLimit(1) << std::endl;
  std::cout << " expected limit (-2 sig) " << result->GetExpectedUpperLimit(-2) << std::endl;
  std::cout << " expected limit (+2 sig) " << result->GetExpectedUpperLimit(2) << std::endl;

  // Use the visualization tool of the PLC to show how the interval was calculated
  RooStats::HypoTestInverterPlot* plot = new RooStats::HypoTestInverterPlot("HTI_Result_Plot","HypoTest Scan Result",result);
  plot->Draw("CLb 2CL");  // plot also CLb and CLs+b 
  
}
