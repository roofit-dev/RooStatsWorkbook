// 
// Build a morphing model based on the binned likelihood model version of the ex12 example
// 
//   construct a histogram template SH_nom(mgg) with a prediction for the nominal binned signal shape
//   construct a histogram template SH_low(mgg) with a prediction for the low systematic variation of the binned signal shape
//   construct a histogram template SH_hig(mgg) with a prediction for the high systematic variation of the binned signal shape
//
//   construct a histogram template BH(mgg) with a prediction for the binned background shape
//
//   construct a probability model model(mgg) = Morphing(SH_low(mgg),SH_nom(mgg),SH_high(mgg) + BH(mgg)
//

void ex13_build_binned_morphing()
{
  // **********************************************************************
  // *** Construct simulation workspace to generate template histograms ***
  // **********************************************************************

  RooWorkspace wsim("wsim") ;

  // Exponential distribution for the background 
  wsim.factory("Exponential::bkg(mgg[40,400],alpha[-0.01,-10,0])") ; 

  // Gaussian distribution for the signal
  wsim.factory("Gaussian::sig(mgg,mean[125,80,400],width[3,1,10])") ;

  // Generate three templates: one at mean=125 (nominal), one at mean=123 (low var) and one at mean=127 (high var)
  wsim.var("mean")->setVal(125)   ; RooDataHist* hist_sig_nom = wsim.pdf("sig")->generateBinned(*wsim.var("mgg"),500) ;
  wsim.var("mean")->setVal(124)   ; RooDataHist* hist_sig_low = wsim.pdf("sig")->generateBinned(*wsim.var("mgg"),500) ;
  wsim.var("mean")->setVal(126)   ; RooDataHist* hist_sig_hig = wsim.pdf("sig")->generateBinned(*wsim.var("mgg"),500) ;

  RooDataHist* hist_bkg = wsim.pdf("bkg")->generateBinned(*wsim.var("mgg"),100000) ;

  // Mock data distribution with mu=1.5
  wsim.var("mean")->setVal(125) ;
  wsim.factory("expr::S('mu*Snom',mu[1.5],Snom[50])") ;
  wsim.factory("SUM::model(S*sig,Bnom[10000]*bkg)") ;

  RooDataHist* hist_data = wsim.pdf("model")->generateBinned(*wsim.var("mgg")) ;
  
  // **************************************
  // *** Set up binned likelihood model ***
  // **************************************

  RooWorkspace w("w") ;
  
  // First Import template and mock data histograms
  w.import(*hist_sig_nom,RooFit::Rename("template_sig_nom")) ;
  w.import(*hist_sig_low,RooFit::Rename("template_sig_low")) ;
  w.import(*hist_sig_hig,RooFit::Rename("template_sig_hig")) ;
  w.import(*hist_bkg,RooFit::Rename("template_bkg")) ;
  w.import(*hist_data,RooFit::Rename("observed_data")) ;


  // Now build signal and background models 
  w.factory("HistFunc::sig_nom(mgg,template_sig_nom)") ;
  w.factory("HistFunc::sig_low(mgg,template_sig_low)") ;
  w.factory("HistFunc::sig_hig(mgg,template_sig_hig)") ;

  // Construct (vertical) morphing interpolation model from the three signal templates
  w.factory("PiecewiseInterpolation::sig(sig_nom,sig_low,sig_hig,alpha[-1,1])") ;

  // Configure morphing model to truncate negative rate extrapolations at zero (too keep the model physical)
  ((PiecewiseInterpolation*)w.function("sig"))->setPositiveDefinite(kTRUE) ;

  // Change the interpolation model to polynomial
  ((PiecewiseInterpolation*)w.function("sig"))->setAllInterpCodes(4) ;
  
  // From here on the same again as ex12...
  w.factory("HistFunc::bkg(mgg,template_bkg)") ;

  w.factory("binw[0.277]") ; // == 1/(400-30)
  w.factory("L[0.1]") ; // == 1/(400-30)
  w.factory("expr::S('mu*L*binw',mu[1,-1,6],L,binw[0.277])") ;
  w.factory("expr::B('Bscale*L*binw',Bscale[0,6],L,binw)") ;
  w.factory("ASUM::model_phys(S*sig,B*bkg)") ;

  // But now we need to introduce the subsidiary measurement of alpha
  w.factory("Gaussian:subs_alpha(alpha_nom[0],alpha,1)") ;
  w.factory("PROD::model(model_phys,subs_alpha)") ;
  
  // Fit the binned probability model to the binned data
  w.pdf("model")->fitTo(*hist_data) ;

  RooPlot* frame = w.var("mgg")->frame() ;
  hist_data->plotOn(frame) ;
  w.pdf("model")->plotOn(frame) ;
  w.pdf("model")->plotOn(frame,RooFit::Components("bkg"),RooFit::LineStyle(kDashed)) ;

  // Visualize effect of morphing parameter change
  w.var("alpha")->setVal(-1) ;
  w.pdf("model")->plotOn(frame,RooFit::LineColor(kRed)) ;
  w.var("alpha")->setVal(1) ;
  w.pdf("model")->plotOn(frame,RooFit::LineColor(kGreen)) ;
  frame->Draw() ;

  // Now save the workspace with the data a modelconfig so that you can use RooStats to extract limits

  // Create an empty ModelConfig
  RooStats::ModelConfig mc("ModelConfig",&w);
  mc.SetPdf(*w.pdf("model"));
  mc.SetParametersOfInterest(*w.var("mu"));
  mc.SetNuisanceParameters(RooArgSet(*w.var("Bscale"),*w.var("alpha")));
  mc.SetObservables(*w.var("mgg"));
  w.var("mu")->setVal(1) ;
  mc.SetSnapshot(*w.var("mu"));
  w.import(mc);

  w.writeToFile("model.root") ; 

  
} 
