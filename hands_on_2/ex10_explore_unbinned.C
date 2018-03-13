
void ex10_explore_unbinned()
{
  // Make an probability density function for a distribution in m(gamgam)
  
  RooWorkspace w("w") ;

  // Exponential distribution for the background 
  w.factory("Exponential::bkg(mgg[40,400],alpha[-0.01,-10,0])") ; 

  // Gaussian distribution for the signal
  w.factory("Gaussian::sig(mgg,mean[125,80,400],width[3,1,10])") ;

  // Model is sum of signal and background
  w.factory("SUM::model(fsig[0.02,0,1]*sig,bkg)") ;

  // Sample a toy unbinned toy dataset from the model  
  RooDataSet* data = w.pdf("model")->generate(*w.var("mgg"),10000) ;
    
  // Fit model to toy data
  RooFitResult* r = w.pdf("model")->fitTo(*data,RooFit::Save()) ;
  
  // Visualize result
  RooPlot* frame = w.var("mgg")->frame() ;
  data->plotOn(frame) ;

  // Note that model is probility density function: int model(mgg) dmgg == 1 
  // that makes no prediction on the observed event count

  // When plotting a probability density function on a frame that
  // already contains a dataset will normalize the pdf projection to the
  // event count of the dataset
  w.pdf("model")->plotOn(frame) ;

  // You can also highlight components of the fit as follows
  w.pdf("model")->plotOn(frame,RooFit::Components("bkg"),RooFit::LineStyle(kDashed)) ;

  frame->Draw() ;
}
