//
//  Build a combination of the models of ex03 (Poisson with Gaussian subsidiary measurement)
//                                   and ex11 (Unbinned ML fit to Gaussian + exponential distribution)
//
//  Prior to running this macro you must
//    - run macro ex06, and rename resulting model.root to model_ex06.root
//    - run macro ex11, and rename resulting model.root to model_ex11.root

void ex16_combined()
{
  // Create a new work space to build the combination
  
  RooWorkspace w("w") ;
  
  // Import model from ex06. Format is <filename> : <workspace name> : <object name>
  //
  // The RenameAllNodes() argument instructs the import operation to rename
  // branch nodes (== functions & pdf) to be renamed with the given suffix
  // Note that variable names will _not_ be affected
  w.import("model_ex06.root:w:model",RooFit::RenameAllNodes("ex06"),RooFit::RenameVariable("mu","mu_ex06")) ;

  // Import model from ex13. Format is <filename> : <workspace name> : <object name>
  // The RenameAllNodes() argument instructs the import operation to rename
  // branch nodes (== functions & pdf) to be renamed with the given suffix
  //
  // Note that variable names will _not_ be affected
  w.import("model_ex11.root:w:model",RooFit::RenameAllNodes("ex11"),RooFit::RenameVariable("mu","mu_ex11")) ;

  // Note that given that none of the variable names were modified
  // any parameters that occurred in both models are now shared between the model
  //
  // For the given examples ex06 and ex11 only one parameter with an original common name occurs: mu,
  // but these are explicitly renamed to mu_ex06 and mu_ex11 so in this setup the pdfs are fully independent.


  // Now import the data from both workspaces
  w.import("model_ex06.root:w:observed_data",RooFit::Rename("obs_data_ex06")) ;
  w.import("model_ex11.root:w:observed_data",RooFit::Rename("obs_data_ex11")) ;

  w.Print("t") ;

  // Construct first a joint dataset
  
  // Create a category variable (the roofit equivalent of C++ enum) with two named states that will label
  // the two measurements in our combination setup
  w.factory("index[ex06=1,ex11=2]") ;
  
  // Create a joined view of the two dataset by linking the input datasets to a new joined datasets
  // where the added discrete observable 'index' will provide a distinguihsing label for the events from the two measurements

  RooDataSet* data_ex06 = (RooDataSet*) w.data("obs_data_ex06") ;
  RooDataSet* data_ex11 = (RooDataSet*) w.data("obs_data_ex11") ;
  RooArgSet joint_obs(*w.var("Nobs_SR"),*w.var("Nobs_CR"),*w.var("mgg"),*w.cat("index")) ;

  RooDataSet joint_obs_data("joint_obs_data","joint_obs_data",joint_obs,
			    RooFit::Index(*w.cat("index")),     // Index will label the measurements
			    RooFit::Link("ex06",*data_ex06),    // Import dataset obs_data_ex06 mapping its content to index state ex06
			    RooFit::Link("ex11",*data_ex11)) ;  // Import dataset obs_data_ex06 mapping its content to index state ex06


  w.import(joint_obs_data) ;

  // Create a joint model that can operate on the above joint dataset
  w.factory("SIMUL::joint_model(index,ex06=model_ex06,ex11=model_ex11)") ;

  // Perform joint fit
  w.pdf("joint_model")->fitTo(joint_obs_data) ;
  
  // (Not writing a modelconfig here, since it is not uniquely defined,
  //  depending on if mu is merged or split the POIs are different

  // Write model to file
  w.writeToFile("joint_model.root") ;
}
