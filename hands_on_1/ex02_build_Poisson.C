// ***********************************************************************************************
//
//  - Build a model Poisson(N | mu * S + B )  with S=10, B=20 constant
//
//  - Generate an observed dataset N=25 ;
//
//  - Construct a unique statistical problem definition (RooStats::ModelConfig) that specifies
//         model = model
//         observable = N
//         parameter of interest = mu
//
//  - Store everything in a root file for later use
//
// ***********************************************************************************************

void ex02_build_Poisson()
{
  // Straightforward exploration of the Poisson distribution using RooFit

  // Construct an empy workspace that will contain the model
  RooWorkspace w("w") ;

  // ****************************************
  // *** Construct a single Poisson model ***
  // ****************************************

  // Construct expression for expected event count Nexp = mu * S + B, with S and B fixed
  w.factory("expr::Nexp('mu*S+B',mu[1,-1,10],S[10],B[20])") ;

  // Construct Poisson probability model for Nobs given Nexp
  w.factory("Poisson::model(Nobs[0,100],Nexp)") ;


  // ********************************************************************
  // *** Construct a dataset containing a single observed event count ***
  // ********************************************************************

  // Construct an empty dataset for observable Nobs
  RooDataSet d("d","d",*w.var("Nobs")) ;

  // Set Nobs to desired observed event count
  w.var("Nobs")->setVal(25) ;

  // Add observation of 25 events to dataset
  d.add(*w.var("Nobs")) ;

  w.import(d,RooFit::Rename("observed_data")) ;
  

  // *****************************************************************************************
  // *** Construct a unique statistical interpretatation of the model to be used by RooStats *
  // *****************************************************************************************

  // Create an empty ModelConfig
  RooStats::ModelConfig mc("ModelConfig",&w);

  // Define the pdf, the parameter of interest and the observables
  mc.SetPdf(*w.pdf("model"));
  mc.SetParametersOfInterest(*w.var("mu"));
  mc.SetObservables(*w.var("Nobs"));

  // Define the current value mu (1) as an hypothesis 
  mc.SetSnapshot(*w.var("mu"));

  mc.Print();

  // import model in the workspace 
  w.import(mc);

  w.writeToFile("model.root") ;
}  

 
