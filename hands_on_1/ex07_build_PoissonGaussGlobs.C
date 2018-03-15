// ***********************************************************************************************
//
//  - Build a model Poisson(N_SR | mu * S + (B_nom+alpha_B*B_systerr) ) * Gaussian ( 0 | alpha_B, 1)
//    representing a signal region SR that includes an uncertain background component B
//    that is known with a systematic uncertainty B = Bnom +/- B_systerr 
//
//    Compare to the model of ex3 the constant B is replaced by a response function B(alpha_B) = Bnom + alpha_B * B_systerr
//    where B_nom is known nominal value of B and B_systerr is the associated systematic uncertainty. 
//    
//    The response function B(alpha_B) is defined such that if nuisance parameter alpha_B is measured
//    with unit Gaussian uncertainty the uncertainty on B is modelled as Gaussian centered at B_nom
//    with a error of B_systerr.
//
//    A subdiary Gaussian measurement is included that measured alpha_B according to a unit Gaussian
//    distribuion. Since the observed value of the systematic uncertainty is by construction zero,
//    that observable is included as a Global Observable in the model
//
// ***********************************************************************************************

void ex07_build_PoissonGaussGlobs()
{

  // Construct an empy workspace that will contain the model
  RooWorkspace w("w") ;

  // ***********************************************
  // *** Construct a Poisson signal region model ***
  // **********************************************

  // Construct the response function for the background B = Bnom + alphaB * Bsysterr
  w.factory("expr::B('B_nom+alpha_B*B_systerr',B_nom[20],alpha_B[-5,5],B_systerr[4])") ;

  // Construct expression for expected event count Nexp_SR = mu * S + B, with S fixed and B previously defined
  w.factory("expr::Nexp_SR('mu*S+B',mu[1,-1,10],S[10],B)") ;

  // Construct Poisson probability model for Nobs_SR given Nexp_SR
  w.factory("Poisson::model_SR(Nobs_SR[0,100],Nexp_SR)") ;

  // ************************************************
  // *** Construct a Gaussian subsidiary measurement
  // ************************************************

  // Construct expression for expected event count Nexp_CR = tau * B, with tau fixed
  // The factor tau expresses the ratio of the CR size w.r.t. the SR size
  w.factory("Gaussian::model_alphaB(alphaB_nom[0],alpha_B,1)") ;

  // *************************************************
  // *** Construct a joint model for the SR and CR ***
  // *************************************************

  w.factory("PROD::model(model_SR,model_alphaB)") ;

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
  
  // *****************************************************************************************
  // *** Construct a unique statistical interpretatation of the model to be used by RooStats *
  // *****************************************************************************************

  // Create an empty ModelConfig
  RooStats::ModelConfig mc("ModelConfig",&w);

  // Define the pdf, the parameter of interest and the observables
  mc.SetPdf(*w.pdf("model"));
  mc.SetParametersOfInterest(*w.var("mu"));
  mc.SetNuisanceParameters(*w.var("alpha_B"));
  mc.SetObservables(*w.var("Nobs_SR"));
  mc.SetGlobalObservables(*w.var("alphaB_nom")) ;

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

  w.pdf("model")->fitTo(d,RooFit::GlobalObservables(*w.var("alphaB_nom"))) ;
}  

 
