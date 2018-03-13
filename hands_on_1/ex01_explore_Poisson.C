void ex01_explore_Poisson()
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
  

  // *** Plot model(Nobs) ***

  // Make empty plot frame in Nobs
  RooPlot* frame = w.var("Nobs")->frame() ;

  // Plot distribution of model on the frame

  // Given that model is discrete in Nobs, enhance precision of adaptive sampling
  // of plot projection machinery to correctly map jumps in probability at integer values
  // (the plotting machinery generically expects continuous functions)
  w.pdf("model")->plotOn(frame,RooFit::Precision(1e-4)) ;

  // Draw the plot frame on the canvas
  frame->Draw() ;


  // ********************************************************************
  // *** Construct a dataset containing a single observed event count ***
  // ********************************************************************

  // Construct an empty dataset for observable Nobs
  RooDataSet d("d","d",*w.var("Nobs")) ;

  // Set Nobs to desired observed event count
  w.var("Nobs")->setVal(25) ;

  // Add observation of 25 events to dataset
  d.add(*w.var("Nobs")) ;

  // ***************************************************
  // *** Fit model to observed data (one step method ***
  // ***************************************************

  // Perform ML fit of model to data (one-step method)
  w.pdf("model")->fitTo(d) ;
  
  // *********************************************************
  // *** Construct -log Likelihood L(mu) from model and data *
  // *********************************************************

  // Construct -log(L) function
  RooAbsReal* nll = w.pdf("model")->createNLL(d) ;
  
  // Plot -logL(mu) in the range [-1,10]
  RooPlot* frame2 = w.var("mu")->frame(-1,10) ;
  nll->plotOn(frame2) ;
  frame2->Draw() ;

  // Construct the (profile) likelihood ratio Lambda = -log L(mu) / L(mu-hat)
  RooAbsReal* lambda = nll->createProfile(*w.var("mu")) ;

  // Plot the likelihood ratio on the same (with a red line color to distinguish) 
  lambda->plotOn(frame2,RooFit::LineColor(kRed)) ;
  frame2->Draw() ;
}
