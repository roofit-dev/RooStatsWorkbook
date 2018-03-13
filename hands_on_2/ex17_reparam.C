//
//  Reparameterize the joint model of ex16 from signal strenghts to a kappa framework
//
//  Prior to running this macro you must
//    - run macro ex16, with the option of having individual mu parameters for each channel

void ex17_reparam()
{
  // Retrieve workspace from file
  TFile* f = TFile::Open("joint_model.root") ;
  RooWorkspace* w = (RooWorkspace*) f->Get("w") ;
  
  // Input model has two measurements with two POIS
  //
  // mu_ex06 = signal strength in measurement ex06
  // mu_ex11 = signal strength in measurement ex11
  //
  //
  // Let's now go with a scenario where these
  // are both Higgs signal strength measurements
  //
  // ex06 = vector-boson-fusion production of Higgs, decay to bosons
  // ex11 = gluon-fusion production of Higgs, decay to bosons
  //
  // If we want to interpret deviations of the signal strength
  // as deviations due to different coupling strengths of
  // the Higgs to fermions and bosons we can reinterpret
  // the signal strength of these experiments as follows
  //
  // mu_ex06 = k_F^2 * k_F^2 / k_H^2 
  // mu_ex11 = k_V^2 * k_F^2 / k_H^2 
  // 
  // where k_F is the Higgs coupling strength modifier to fermions
  // where k_V is the Higgs coupling strength modifier to bosons
  //
  // and k_H is the effective modification of the total Higgs
  // coupling due to modifications of kV,kH: Gamma_H = Gamma_H_SM * K_H^2
  //
  // Concretely k_H^2 = 0.XX * k_V^2 + 0.YY * K_F^2 
  // the coefficients here originate from Higgs physics
  // 

  // Now we can encode an expression for mu_ex06 and mu_ex11
  // as function of the new parameters kV and kF
  
  // Declare the new variables
  w->factory("{kF[1,-5,5],kV[1,-5,5]}") ;

  // Construct expression for k_H
  w->factory("expr::kH2('0.25*kV*kV+0.75*kF*kF',kV,kF)") ;
  
  // Construct expression for mu_ex06, mu_ex11
  w->factory("expr::mu_ex06_func('kF*kF*kF*kF/kH2',kF,kH2)") ;
  w->factory("expr::mu_ex11_func('kV*kF*kV*kF/kH2',kF,kV,kH2)") ;

  // Now construct a clone of model where variables mu_XX are replaced by functions mu_XX_func
  w->factory("EDIT::model_kappa(joint_model,mu_ex06=mu_ex06_func,mu_ex11=mu_ex11_func)") ;

  w->Print("t") ;

  // Now fit the reparameterized model to the observed data
  RooAbsData* data = w->data("joint_obs_data") ;
  
  w->pdf("model_kappa")->fitTo(*data) ;  
}
