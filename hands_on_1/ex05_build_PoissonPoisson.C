// ***********************************************************************************************
//
//  - Build a model Poisson(N_SR | mu * S + B ) * Poisson (N_CR | tau * B)  with S=10, tau=10 constant
//    representing a signal region SR and a control region CR (that measures only the bkg)
//    The constant parameter tau represents the sideband scale factor
//
//  - Generate an observed dataset N_SR=25,N_CR=200 ;
//
//  - Construct a unique statistical problem definition (RooStats::ModelConfig) that specifies
//         model = model
//         observable = {N_SR, N_SR}
//         parameter of interest = mu
//         nuisance parameter = B
//
//  - Store everything in a root file for later use
//
// ***********************************************************************************************

void ex05_build_PoissonPoisson()
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
  w.factory("Poisson::model_CR(Nobs_CR[0,1000],Nexp_CR)") ;

  // *************************************************
  // *** Construct a joint model for the SR and CR ***
  // *************************************************

  w.factory("PROD::model(model_SR,model_CR)") ;

  // ********************************************************************
  // *** Construct a dataset containing a single observed event count ***
  // ********************************************************************

  // For convenience define a named set in the workspace obs = (Nobs_SR,Nobs_CR)
  w.defineSet("obs","Nobs_SR,Nobs_CR") ;

  // Construct an empty dataset for observable Nobs
  RooDataSet d("d","d",*w.set("obs")) ;

  // Set Nobs to desired observed event count
  w.var("Nobs_SR")->setVal(25) ;
  w.var("Nobs_CR")->setVal(200) ;
  
  // Add observation of 25 events to dataset
  d.add(*w.set("obs")) ;

  w.import(d,RooFit::Rename("observed_data")) ;
  

  // *****************************************************************************************
  // *** Construct a unique statistical interpretatation of the model to be used by RooStats *
  // *****************************************************************************************

  // Create an empty ModelConfig
  RooStats::ModelConfig mc("ModelConfig",&w);

  // Define the pdf, the parameter of interest and the observables
  mc.SetPdf(*w.pdf("model"));
  mc.SetParametersOfInterest(*w.var("mu"));
  mc.SetNuisanceParameters(*w.var("B"));
  mc.SetObservables(*w.set("obs"));

  // Define the current value mu (1) as an hypothesis 
  mc.SetSnapshot(*w.var("mu"));

  mc.Print();

  // import model in the workspace 
  w.import(mc);

  w.writeToFile("model.root") ;

  w.pdf("model")->fitTo(d) ;
}  

 
