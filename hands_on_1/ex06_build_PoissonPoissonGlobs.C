// ***********************************************************************************************
//
//  This macro illustrates the concept of 'Global Observables' - which moves an observable from the
//  explicitly defined dataset to an implicitly defined observable defined in the pdf itself.
//
//  - Build a model Poisson(N_SR | mu * S + B ) * Poisson (N_CR | tau * B)  with N_CR=200, S=10, tau=10 constant
//    representing a signal region SR and a control region CR (that measures only the bkg)
//    The constant parameter tau represents the sideband scale factor
//   
//    In this model a different technical approach is taken to describing the measured
//    event count in the control region N_CR. Here it is described as a 'Global Observable',
//    which hides it as an explicit member of the dataset, but retains it as an implicit member
//    of the data that is defined in the model, rather than in the data
//
//  - Generate an observed dataset N_SR=25 (N_CR=200 is not part of the data, but implicitly defined in the model)
//
//  - Construct a unique statistical problem definition (RooStats::ModelConfig) that specifies
//         model = model
//         observable = N_SR
//         global observable = N_CR
//         parameter of interest = mu
//         nuisance parameter = B
//
//  - Store everything in a root file for later use
//
// The concept of global observables is mostly useful for measurements where the observed
// value is always the same (unlike the N_CR of this example, which could genuinely be different
// in every measurement). It is commonly used in the description of subsidiary measurements
// that represent systematic uncertainties, where the observed value is by construction always exactly
// zero (for additive uncertainties) or one (for multiplicative uncertainties)
//
// ***********************************************************************************************

void ex06_build_PoissonPoissonGlobs()
{

  // Construct an empy workspace that will contain the model
  RooWorkspace w("w") ;

  // ***********************************************
  // *** Construct a Poisson signal region model ***
  // **********************************************

  // Construct expression for expected event count Nexp_SR = mu * S + B, with S fixed and B floating
  w.factory("expr::Nexp_SR('mu*S+B',mu[1,-1,10],S[10],B[20,0,200])") ;

  // Construct Poisson probability model for Nobs_SR given Nexp_SR
  w.factory("Poisson::model_SR(Nobs_SR[0,100],Nexp_SR)") ;

  // ************************************************
  // *** Construct a Poisson control region model ***
  // ************************************************

  // Construct expression for expected event count Nexp_CR = tau * B, with tau fixed
  // The factor tau expresses the ratio of the CR size w.r.t. the SR size
  w.factory("expr::Nexp_CR('tau*B',tau[10],B)") ;

  // Construct Poisson probability model for Nobs given Nexp
  // NB: If we treat Nobs_CR as a global observable, we make it constant in model
  //     This is implicitly done here by not not specifying a range, 
  //     but just a single value x[lo,hi] --> x[val]
  w.factory("Poisson::model_CR(Nobs_CR[200],Nexp_CR)") ;

  // *************************************************
  // *** Construct a joint model for the SR and CR ***
  // *************************************************

  w.factory("PROD::model(model_SR,model_CR)") ;

  // ********************************************************************
  // *** Construct a dataset containing a single observed event count ***
  // ********************************************************************


  // Construct an empty dataset for observable Nobs
  RooDataSet d("d","d",*w.var("Nobs_SR")) ;

  // Set Nobs to desired observed event count
  w.var("Nobs_SR")->setVal(25) ;
  
  // Add observation of 25 events to dataset
  d.add(*w.var("Nobs_SR")) ;

  w.import(d,RooFit::Rename("observed_data")) ;
  
  // NB: When treating Nobs_CR as a global observable, it does _not_ appear in the dataset
  //     it is implicitly part of the observed data (as 'global observable') but not
  //     _explicitly_ part of the dataset anymore

  // *****************************************************************************************
  // *** Construct a unique statistical interpretatation of the model to be used by RooStats *
  // *****************************************************************************************

  // Create an empty ModelConfig
  RooStats::ModelConfig mc("ModelConfig",&w);

  // Define the pdf, the parameter of interest and the observables
  mc.SetPdf(*w.pdf("model"));
  mc.SetParametersOfInterest(*w.var("mu"));
  mc.SetNuisanceParameters(*w.var("B"));
  mc.SetObservables(*w.var("Nobs_SR"));
  mc.SetGlobalObservables(*w.var("Nobs_CR")) ;

  // NB: When treating Nobs_CR as a global observable, it must be marked as such in the ModelConfig

  // Define the current value mu (1) as an hypothesis 
  mc.SetSnapshot(*w.var("mu"));

  mc.Print();

  // import model in the workspace 
  w.import(mc);

  w.writeToFile("model.root") ;

  // ***********************************************************************
  // *** Fit model to the data (after the model has been written to file ***
  // ***********************************************************************

  // NB: Global Observables that occur in the model must be explicitly identified
  //     when using the pdf

  w.pdf("model")->fitTo(d,RooFit::GlobalObservables(*w.var("Nobs_CR"))) ;
}  

 
