// 
// Build a binned likelihood model version of the ex11 example
// 
//   construct a histogram template SH(mgg) with a prediction for the binned signal shape
//   construct a histogram template BH(mgg) with a prediction for the binned background shape
//
//   construct a probability model model(mgg) = SH(mgg) + BH(mgg)
//
//   This model can be 'seen' in two ways
//
//      1) an extended probability model like ex11 that happen to have binned shapes, i.e.
//
//         model(mgg) = Nsig/Nsig+Nbkg * pdf_SH(mgg) +  Nbkg/Nsig+Nbkg * pdf_BH(mgg) + 
//               P(N) = Nsig + Nbkg
// 
//         where pdf_SH,pdf_BH are probability density functions that follow shape of the
//         unit normalized histograms
//
//      2) A product of Poisson counting experiments for each bin
//
//          model(vec_N) = Product(i=0..n-1) Poisson(N_i | S_i + B_i)
//           
//          where N_i i=0...N-1 == vec_N are the observed event counts in each bin
//          and S_i and B_i are the predicted signal and background rate in each bin
//
//  Both representations are mathematically equivalent, but expression (2) is in practice
//  faster to calculate because it does not require a normalization calculation over
//  pdf_SH and pdf_BH to happen. While for this very simple example it does not make
//  a noticable difference because the normalization does not depend on any model parameters
//  in scenarios where it does it will effectivelt double the calculation time

void ex12_build_binned()
{
  // **********************************************************************
  // *** Construct simulation workspace to generate template histograms ***
  // **********************************************************************

  RooWorkspace wsim("wsim") ;

  // Exponential distribution for the background 
  wsim.factory("Exponential::bkg(mgg[40,400],alpha[-0.01,-10,0])") ; 

  // Gaussian distribution for the signal
  wsim.factory("Gaussian::sig(mgg,mean[125,80,400],width[3,1,10])") ;

  RooDataHist* hist_sig = wsim.pdf("sig")->generateBinned(*wsim.var("mgg"),50) ;
  RooDataHist* hist_bkg = wsim.pdf("bkg")->generateBinned(*wsim.var("mgg"),10000) ;

  // Mock data distribution with mu=1.5
  wsim.factory("expr::S('mu*Snom',mu[1.5],Snom[50])") ;
  wsim.factory("SUM::model(S*sig,Bnom[10000]*bkg)") ;

  // Given that the sum is an extended model, no speficiation of the event count is needed
  RooDataHist* hist_data = wsim.pdf("model")->generateBinned(*wsim.var("mgg")) ;
  
  // **************************************
  // *** Set up binned likelihood model ***
  // **************************************

  RooWorkspace w("w") ;
  
  // First Import template and mock data histograms
  w.import(*hist_sig,RooFit::Rename("template_sig")) ;
  w.import(*hist_bkg,RooFit::Rename("template_bkg")) ;
  w.import(*hist_data,RooFit::Rename("observed_data")) ;


  // Now build signal and background models 
  // Note that we build _functions_ here and not pdfs
  w.factory("HistFunc::sig(mgg,template_sig)") ;
  w.factory("HistFunc::bkg(mgg,template_bkg)") ;

  // Also not that we don't need to declare mgg here, 
  // its definition was imported when we imported the histograms

  // Now construct a 'amplitude sum' probability model, defined asa
  //
  //          c_sig * hist_sig(x) +  c_bkg * hist_bkg(x)
  // pdf(x) = ----------------------------------------
  //          c_sig * SUM(hist_sig(x)) +  c_bkg * SUM(hist_bkg(x))
  //
  //
  // here c_sig and c_bkg are coefficients scaling the predictions
  // of the template histograms. If the template histograms encode
  // the nominal event yield, one expects both coefficients to
  // fit to 1 if the data matches the prediction
  //
  // NOTE: If the bin width is not equal to one then the event count of a histogram
  //       is not identical to the integral over a histogram. In RooFit the _integral_
  //       over the histogram is taken as the yield prediction, whereas one usually,
  //       interprets the histogram event count as the prediction. The simplest
  //       way to correct for this is to multiply c_i with a constant which is 1/binwidth
  //
  // In this case we choose c_sig = mu (as usual)
  // and introduce a Bscale as a nuisance parameter that
  // can freely scale the background

  w.factory("binw[0.277]") ; // == 1/(400-30)
  w.factory("expr::S('mu*binw',mu[1,-1,6],binw[0.277])") ;
  w.factory("expr::B('Bscale*binw',Bscale[0,6],binw)") ;
  w.factory("ASUM::model(S*sig,B*bkg)") ;
  
  // Fit the binned probability model to the binned data
  w.pdf("model")->fitTo(*hist_data) ;

  RooPlot* frame = w.var("mgg")->frame() ;
  hist_data->plotOn(frame) ;
  w.pdf("model")->plotOn(frame) ;
  w.pdf("model")->plotOn(frame,RooFit::Components("bkg"),RooFit::LineStyle(kDashed)) ;
  frame->Draw() ;


  // Now save the workspace with the data a modelconfig so that you can use RooStats to extract limits

  // Create an empty ModelConfig
  RooStats::ModelConfig mc("ModelConfig",&w);

  // Define the pdf, the parameter of interest and the observables
  mc.SetPdf(*w.pdf("model"));
  mc.SetParametersOfInterest(*w.var("mu"));
  //mc.SetNuisanceParameters(RooArgSet(*w.var("mean"),*w.var("width"),*w.var("alpha")));
  mc.SetNuisanceParameters(*w.var("Bscale"));
  mc.SetObservables(*w.var("mgg"));

  // Define the current value mu (1) as an hypothesis 
  w.var("mu")->setVal(1) ;
  mc.SetSnapshot(*w.var("mu"));

  mc.Print();

  // import model in the workspace 
  w.import(mc);

  w.writeToFile("model.root") ; 

  
} 

