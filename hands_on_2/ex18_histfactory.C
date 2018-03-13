using namespace RooStats;
using namespace HistFactory;

void ex18_histfactory()
{  
  std::string inputFileName = "./input/example.root";

  // A histfactory model is built as follows
  //
  // A channel is modeled by a stack of samples (signal, backgrounds)
  // A measurement is composed of one or more channels


  // ******************************
  // *** Create the measurement ***
  // ******************************

  Measurement meas("meas", "meas");
  meas.SetOutputFilePrefix( "./results/example_" );

  // Define the name of the POI
  meas.SetPOI("mu");

  // Declare constants
  meas.AddConstantParam("alpha_syst1");
  meas.AddConstantParam("Lumi");

  // Set luminosity and its uncertainty
  meas.SetLumi(1.0);
  meas.SetLumiRelErr(0.10);

  // Technical configuration...
  meas.SetExportOnly(false);
  meas.SetBinHigh(2);

  // *************************
  // *** Create a channel  ***
  // *************************

  Channel chan("channel1");
  chan.SetData("data",inputFileName) ;
  chan.SetStatErrorConfig(0.05, "Poisson");

  // *** Populate this channel with samples

  // Signal sample - scaled by a normalization factor mu
  //               - and with a 5% systematic scale uncertainty named syst1
  Sample signal( "signal", "signal", inputFileName ) ;
  signal.AddOverallSys("syst1",0.95,1.05) ;
  signal.AddNormFactor("mu",1,0,3 ) ;
  chan.AddSample(signal) ;

  // Background sample 1 - with a 5% systematic scale uncertainty named syst2
  //                     - and with modeling of MC statistica, uncertainties
  Sample background1("background1","background1",inputFileName) ;
  background1.ActivateStatError("background1_statUncert",inputFileName) ;
  background1.AddOverallSys("syst2",0.95,1.05) ;
  chan.AddSample(background1) ;

  // Background sample 2 - with a 5% systematic scale uncertainty named syst3
  //                     - and with modeling of MC statistical uncertainties
  Sample background2( "background2", "background2", inputFileName );
  background2.ActivateStatError();
  background2.AddOverallSys("syst3",0.95,1.05);
  chan.AddSample( background2) ;

  // Add channel to the measurementa
  meas.AddChannel( chan );

  // Collect all the required templated histograms from the input file
  meas.CollectHistograms();

  // Now build a workspace with a pdf and a modelconfig
  RooWorkspace* w =  MakeModelAndMeasurementFast(meas) ;

  // Make some adjustments to histfactory workspace to conform
  // to naming conventions of this set of example macros
  w->SetName("w") ;
  w->data("obsData")->SetName("observed_data") ;

  // Print resulting workspace
  w->Print("t") ;

  // Save workspace to file
  w->writeToFile("model.root") ;

}
