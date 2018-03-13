// Make an extended probability density function for a distribution in m(gamgam)
//
// An extended probability density model is a product
// of a probability density function f(x) in a continuous observable x
// and a Poisson model modeling the observed event count P(Nobs|Nexp)
//
// For composite pdfs (a sum of 2 of more components) the conceptual expression 
//
//    model(x,N) = Nsig*sig(x) + Nbkg*bkg(x)
//
// can be elegantly rewritten in the producy of a  probability density function and Poisson
//
//  f_sig = Nsig / (Nsig + Nbkg)
//    E   = Nsig + Nbkg
//
//   model(x) = f_sig * sig(x) + (1-f_sig) * bkg(x) 
//   P(N|E)   = Poisson (N|E)
//

void ex11_build_unbinned_extended()
{
 
  RooWorkspace w("w") ;

  // Exponential distribution for the background 
  w.factory("Exponential::bkg(mgg[40,400],alpha[-0.01,-10,0])") ; 

  // Gaussian distribution for the signal
  w.factory("Gaussian::sig(mgg,mean[125,80,400],width[3,1,10])") ;

  // Fix signal shape for now
  w.var("mean")->setConstant(true) ;
  w.var("width")->setConstant(true) ;

  // Model is sum of signal and background
  //  
  //  S = mu * Snom[200]
  //  B = Bnom[10000]
  w.factory("expr::S('mu*Snom',mu[1,-3,6],Snom[50])") ;
  w.factory("SUM::model(S*sig,Bnom[10000]*bkg)") ;

  // Sample a toy unbinned toy dataset from the model  
  // If no event count is given, the predicted count
  // of the model is taken (in this case S+B)
  RooDataSet* data = w.pdf("model")->generate(*w.var("mgg")) ;
    
  // Fit model to toy data - the extended option forces the inclusion of the Poisson temr
  // in the likelihood construction
  RooFitResult* r = w.pdf("model")->fitTo(*data,RooFit::Save(),RooFit::Extended()) ;
  
  // Visualize result
  RooPlot* frame = w.var("mgg")->frame() ;
  data->plotOn(frame) ;

  // When plotting an extended pdf you can choose to follow intrinsic prediction
  // for the event count, rather than normalizing the plot to the observed data
  //
  // To do so request a normalization scale factor 1.0 w.r.t the intrinsic expecation
  w.pdf("model")->plotOn(frame,RooFit::Normalization(1.0,RooAbsReal::RelativeExpected)) ;

  // You can also highlight components of the fit as follows
  w.pdf("model")->plotOn(frame,RooFit::Normalization(1.0,RooAbsReal::RelativeExpected),RooFit::Components("bkg"),RooFit::LineStyle(kDashed)) ;

  frame->Draw() ;



  // Now save the workspace with the data a modelconfig so that you can use RooStats to extract limits

  // Save the generated data as the 'observed data'
  w.import(*data,RooFit::Rename("observed_data")) ;

  // Create an empty ModelConfig
  RooStats::ModelConfig mc("ModelConfig",&w);

  // Define the pdf, the parameter of interest and the observables
  mc.SetPdf(*w.pdf("model"));
  mc.SetParametersOfInterest(*w.var("mu"));
  //mc.SetNuisanceParameters(RooArgSet(*w.var("mean"),*w.var("width"),*w.var("alpha")));
  mc.SetNuisanceParameters(*w.var("alpha"));
  mc.SetObservables(*w.var("mgg"));

  // Define the current value mu (1) as an hypothesis 
  w.var("mu")->setVal(1) ;
  mc.SetSnapshot(*w.var("mu"));

  mc.Print();

  // import model in the workspace 
  w.import(mc);

  w.writeToFile("model.root") ; 

}

