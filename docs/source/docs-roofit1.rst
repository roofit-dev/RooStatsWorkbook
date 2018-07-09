RooFit as a model building language
===================================

The main feature of the design of the RooFit/RooStats suite of statistical analysis tools is that the tools
to build statistical models and the tools to perform statistical inference are separate, but interoperable.
This interoperability is universally possible because all statistical inference is based on the likelihood function.

The goal is that all tools in the RooStats suite can analyse any model built in RooFit. The interoperability
is facilitated in practice by the concept of the workspace (RooFit class RooWorkspace) that allows to
persist statistical models of arbitrary complexity into ROOT files and thus allow to separate the model building
phase and the statistical inference phase of a data analysis project both in space and time

RooFit language basics and the RooFit workspace
===============================================

RooFit basics
-------------

This section will cover the essentials of the RooFit modelling language, which wil aid in the understanding
of the role of higher level modelling tools and model manipulation.

The key concept of RooFit probability models is that all components that form the mathematical expression
of a probability model or probability density function are expressed in separate C++ objects. Base classes exist
to represent e.g. variables, functions, normalised probability density functions, integrals of function, datasets,
and implementations of many concrete functions and probability density functions are provided. 

The examples below show how a Gaussian and Poisson probability model are built by constructing first
the component objects (the parameters and observables), and the the probability (density) functions.


+--------------------------------------------------------------+-----------------------------------------------------------------------+
|.. code-block:: c					       |.. code-block:: python					               |
|   		 					       |   							      	       | 
|   // Construct a Gaussian probability density model	       |   # Construct a Gaussian probability density model	      	       |
|   RooRealVar x("x","x",0,-10,10) ;			       |   x = ROOT.RooRealVar("x","x",0,-10,10)      		      	       |
|   RooRealVar mean("mean","Mean of Gaussian",0,-10,10) ;      |   mean = ROOT.RooRealVar("mean","Mean of Gaussian",0,-10,10) 	       |
|   RooRealVar width("width","With of Gaussian",3,0.1,10) ;    |   width = ROOT.RooRealVar("width","With of Gaussian",3,0.1,10)        |
|   RooGaussian g("g","Gaussian",x, mean,width) ;	       |   g = ROOT.RooGaussian("g","Gaussian",ROOT.RooArgSet(x), mean,width)  |
|   				    			       |       							  	       |
|   // Construct a Poisson probability model		       |   # Construct a Poisson probability model			       |
|   RooRealVar n("n","Observed event cont",0,0,100) ;	       |   n = ROOT.RooRealVar("n","Observed event cont",0,0,100)	       |
|   RooRealVar mu("mu","Expected event count",10,0,100) ;      |   mu = ROOT.RooRealVar("mu","Expected event count",10,0,100)          |
|   RooPoisson p("p","Poisson",n, mu) ;			       |   p = ROOT.RooPoisson("p","Poisson",n, mu)  			       |
+--------------------------------------------------------------+-----------------------------------------------------------------------+       

RooFit implements all basic functionality of statistical models: toy data generation, plotting, and fitting (interfacing ROOTs minimisers for the actual -logL minimisation process). 


+---------------------------------------------------------------------------------------+-------------------------------------------------------------+
|.. code-block:: c								        |.. code-block:: python					      |
|   		 	    		 					        |   							      |
|   // generate unbnined dataset of 10k events		       				|   # generate unbnined dataset of 10k events		      |
|   RooDataSet* toyData = g.generate(x,10000) ; 	       				|   toyData = g.generate(ROOT.RooArgSet(x),10000)	      |
|   			  		      		       				|   	     						      |
|   // Perform unbinned ML fit to toy data		       				|   # Perform unbinned ML fit to toy data		      |
|   g.fitTo(*toyData) ;        	      			       				|   g.fitTo(toyData)   	     	    			      |
|   		      					       				|   							      |
|   // Plot toy data and pdf in observable x		       				|   # Plot toy data and pdf in observable x		      |
|   RooPlot* frame = x.frame() ; 	   		       				|   frame = x.frame()   	      		 	      |
|   toyData->plotOn(frame) ;   				       				|   toyData.plotOn(frame)				      |
|   g.plotOn(frame) ;	   				       			     	|   g.plotOn(frame)					      |
|   frame->Draw() ; 					                                |   frame.Draw()					      |
|   		  									|   							      |
|.. image:: http://swanserver.web.cern.ch/swanserver/images/badge_swan_white_150.png	|.. image:: http://swanserver.web.cern.ch/	    	      |
|   :target: https://cern.ch/swanserver/cgi-bin/go/?projurl=				|           swanserver/images/badge_swan_white_150.png        |
|            https://raw.githubusercontent.com/roofit-dev/				|    :target: https://cern.ch/swanserver/cgi-bin/  	      |
|            RooStatsWorkbook/master/misc_notebooks/RooFitBasics.ipynb			|    	       go/?projurl=https://raw.githubusercontent.com  |
|   :alt: ROOT C++									|             /roofit-dev/RooStatsWorkbook/master/	      |
|   	       										|             misc_notebooks/RooFitBasics.ipynb	              |
|											|    :alt: pyROOT				    	      |
+---------------------------------------------------------------------------------------+-------------------------------------------------------------+

|docs-gauss|

.. |docs-gauss| raw:: html

    <div class="popup" onclick="newFunction()"><figure><img src="https://raw.githubusercontent.com/roofit-dev/RooStatsWorkbook/master/docs/source/docs-fig01.png" alt="Gaussian model distribution :math:`\mu` = 0 and :math:`\sigma` = 3."> <figcaption>Fig1. - gaussian distribution &mu; = 0 and &sigma; = 3. </figcaption></figure><span class="popuptext" id="GaussModelPopup"><p><a target="_blank" href="https://cern.ch/swanserver/cgi-bin/go/?projurl=https://raw.githubusercontent.com/roofit-dev/RooStatsWorkbook/master/misc_notebooks/RooFitBasics.ipynb"><img src="http://swanserver.web.cern.ch/swanserver/images/badge_swan_white_150.png"> C++</a></p><p><a target="_blank" href="https://cern.ch/swanserver/cgi-bin/go/?projurl=https://raw.githubusercontent.com/roofit-dev/RooStatsWorkbook/master/misc_notebooks/RooFitBaiscs_PY.ipynb"><img src="http://swanserver.web.cern.ch/swanserver/images/badge_swan_white_150.png"> pyROOT</a></p></span></div>

As the likelihood function plays an important role in many statistical techniques, it can also be explicitly constructed
as a RooFit function object for more detailed control


+----------------------------------------------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------+
|.. code-block:: c												 |.. code-block:: c                                                                                               |
|   		 												 |   		  												  |
|   // Create Likelihood function L(x|mu,sigma) for all x in toy data 						 |   # Create Likelihood function L(x|mu,sigma) for all x in toy data 						  |
|   RooAbsReal* nll = g.createNLL(*toyData) ;	    	     	 						 |   nll = g.createNLL(toyData)	  		    	     	 						  |
|   		      			    									 |   	   													  |
|   // ML estimation of model parameters mean,width								 |   # ML estimation of model parameters mean,width								  |
|   RooMinimizer m(*nll) ;    		 									 |   m = ROOT.RooMinimizer(nll)		 									  |
|   m.migrad() ; // Minimization										 |   m.migrad()  # Minimization											  |
|   m.hesse() ; // Hessian error analysis									 |   m.hesse()   # Hessian error analysis									  |
|      	      	   	   	 										 |   		   	   	 										  |
|   // Result of minimisation and error analysis is propagated 							 |   # Result of minimisation and error analysis is propagated							  |
|   // to variable objects representing model parameters 							 |   # to variable objects representing model parameters       							  |
|   mean.Print() ;	      	  		    	       	  	   	   		      		 |   mean.Print()	      	  		    	       	  	   	   		      		  |
|   width.Print() ;												 |   width.Print()												  |
|   		  												 |   														  |
|   // Visualize likelihood L(mu, sigma)				 					 |   # Visualize likelihood L(mu, sigma)					                                  |
|   // at sigma = sigma_hat in range 2.9<mu<3.1				 					 |   # at sigma = sigma_hat in range 2.9<mu<3.1                                 				  |
|   RooPlot* frame2 = mean.frame(2.9,3.1) ; 	    	      	       						 |   frame2 = mean.frame(2.9,3.1) 	    	    	      	       						  |
|   nll->plotOn(frame2) ;		  									 |   nll.plotOn(frame2)												  |
|   frame2->Draw() ;												 |   frame2.Draw()												  |
|   		   	     		  									 |   							      							  |
|.. image:: http://swanserver.web.cern.ch/swanserver/images/badge_swan_white_150.png				 |.. image:: http://swanserver.web.cern.ch/	    	      							  |
|   :target: https://cern.ch/swanserver/cgi-bin/go/?projurl=							 |           swanserver/images/badge_swan_white_150.png        							  |
|            https://raw.githubusercontent.com/roofit-dev/							 |    :target: https://cern.ch/swanserver/cgi-bin/  	      							  |
|            RooStatsWorkbook/master/misc_notebooks/RooFitBasics.ipynb						 |    	       go/?projurl=https://raw.githubusercontent.com  							  |
|   :alt: ROOT C++												 |             /roofit-dev/RooStatsWorkbook/master/	      							  |
|   	       													 |             misc_notebooks/RooFitBasics.ipynb	              						  |
|														 |    :alt: pyROOT				    	      							  |
+----------------------------------------------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------+

|docs-mean|

.. |docs-mean| raw:: html

    <div class="popup" onclick="newFunction()"><figure><img src="https://raw.githubusercontent.com/roofit-dev/RooStatsWorkbook/master/docs/source/docs-fig02.png" alt="Distribution of the likelihood in the region :math:`\mu` = 2.9 and :math:`\mu` = 3.1"> <figcaption>Fig1. - Distribution of the likelihood in the region &mu; = 2.9 and &mu; = 3.1 </figcaption></figure><span class="popuptext" id="MeanPopup"><p><a target="_blank" href="https://cern.ch/swanserver/cgi-bin/go/?projurl=https://raw.githubusercontent.com/roofit-dev/RooStatsWorkbook/master/misc_notebooks/RooFitBasics.ipynb"><img src="http://swanserver.web.cern.ch/swanserver/images/badge_swan_white_150.png"> C++</a></p><p><a target="_blank" href="https://cern.ch/swanserver/cgi-bin/go/?projurl=https://raw.githubusercontent.com/roofit-dev/RooStatsWorkbook/master/misc_notebooks/RooFitBaiscs_PY.ipynb"><img src="http://swanserver.web.cern.ch/swanserver/images/badge_swan_white_150.png"> pyROOT</a></p></span></div>


RooFit implements a very wide range of options to tailor models and their use that are not discussed here in the interest of brevity, but
are discussed in detail here <insert_link>. RooFit also automatically performs an optimisation of the computation strategy of each likelihood
before each use, so that users that build models generally do not need to worry about specific performance considerations when
expression the model. Example optimisations include automatic detection of unused dataset variables in a likelihood, automatic detection
of expression that only depend on (presently) constant parameters and caching and lazy evaluation of expensive objects such as
numerical integrals. The following links provide further information on a number of RooFit related topics

* Guide to the RooFit workspace and factory language <LINK>
* Guide to (automatic) computational optimizations in RooFit <LINK>
* Guide to writing your own RooFit PDF classes <LINK>

RooFit workspace essentials
---------------------------

The key to the RooFit/RooStats approach of separating model building and statistical inference is the ability
to persist built models into ROOT files and the ability to revive these with minimal effort in a later ROOT session. 
This ability is powered by the RooFit workspace class, in which built models can be important, which organises the persistence
of these models, regardless of their complexity

+----------------------------------------------------------+-------------------------------------------------------------+
| .. code-block:: c                                        | .. code-block:: python					 |
|                                                          |                                   	 	                 |
|    // Saving a model to the workspace                    |   # Saving a model to the workspace                	 |
|   RooWorkspace w("w") ;     				   |   w = ROOT.RooWorkspace("w")   			  	 |
|   w.import(g) ;					   |   getattr(w,'import')(g) 				  	 |
|   w.import(p) ;					   |   getattr(w,'import')(p)				  	 |
|   w.import(*toyData,RooFit::Rename("toyData")) ;	   |   getattr(w,'import')(toyData,ROOT.RooFit.Rename("toyData"))|
|   w.Print() ;					 	   |   w.Print()						 |
|   w.writeToFile("model.root") ;			   |   w.writeToFile("model.root") 			  	 |
|   							   |							  	 |
|   // ————————————————					   |   # ————————————————				  	 |
|      							   |   	 						  	 |
|   // Reviving a model from a workspace		   |   # Reviving a model from a workspace		  	 |
|   TFile* f = TFile::Open("model.root") ;		   |   f = ROOT.TFile.Open("model.root")		  	 |
|   RooWorkspace * w = (RooWorkspace*) f->Get("w") ;	   |   w = f.Get("w")					  	 |
|   RooAbsPdf* g = w->pdf("g") ;       		   	   |   g = w.pdf("g")			  		  	 | 
|   RooAbsPdf* p = w->pdf("p") ;			   |   p = w.pdf("p")					  	 |
|   RooDataSet* toyDatata = w->data("toyData") ;	   |   toyDatata = w.data("toyData")			  	 |
|   			    		       		   |							  	 |
+----------------------------------------------------------+-------------------------------------------------------------+

The code example above, while straightforward highlights a couple of important points

* Reviving a model or dataset is always trivial, typically 3-4 lines of code depending on the number of objects that must be retrieved from the workspace, even for large workspaces e.g. full Higgs combinations that can contain over ten thousand component objects.

* Retrieval of objects (functions, datasets) is indexed by their internal object, as giving to them in the first C++ constructor argument (all RooFit object inherit from ROOT class TNamed, which identical behaviour).

* For objects where the model building user did not explicitly specify the objects intrinsic name, like the toyData object in the example above, it is possible to rename the object to a given name after the fact. The most convenient path to do this is to do this upon import in the workspace, as shown in the code example above.

The ModelConfig interface object
--------------------------------

Since model and data names do not need to conform to any specific convention, the automatic operation
of statistical tools in RooStats need some guidance from the user to point the tool the correct model and dataset.
The interface between RooFit probability models and RooStats tools is the **RooStats::ModelConfig** object. 
The ModelConfig object has two clarifying roles

1. Identifying which pdf  in the workspace is to be used for the calculation. The workspace content
does not need to follow any particular naming convention. The ModelConfig object will guide the calculator
the desired model

2. Specifying information on the statistical inference problem that is not intrinsic to the probability model:
 i.e. which parameters are "of interest", which ones are "nuisance", what are the observables to be considered,
and (often) a set of parameter values that fully define a specific hypothesis (e.g. the null or alternate hypothesis)

Here is a code example that construct a ModelConfig for the Poisson probability model created earlier

+-----------------------------------------------------------------------+-----------------------------------------------------------------------+
|.. code-block:: c							|.. code-block:: python				                        |
|   		 							|   		 							|
|  // Create an empty ModelConfig					|  # Create an empty ModelConfig					|
|  RooStats::ModelConfig mc("ModelConfig",&w);				|  mc = ROOT.RooStats.ModelConfig("ModelConfig",w)			|
|  			 						|     									|
|  // Define the pdf, the parameter of interest and the observables	|  # Define the pdf, the parameter of interest and the observables	|
|  mc.SetPdf(*w.pdf("p"));	       		    			|  mc.SetPdf(w.pdf("p")) 	      	       	       			|
|  mc.SetParametersOfInterest(*w.var("mu"));				|  mc.SetParametersOfInterest(w.var("mu"))				|
|  mc.SetObservables(*w.var("n"));					|  mc.SetObservables(w.var("n"))					|
|  									|  									|
|  // Define the value mu=1 as 						|  # Define the value mu=1 as 						|
|  // a fully-specified hypothesis to be tested later			|  # a fully-specified hypothesis to be tested later			|
|  w->var("mu")->setVal(1) ;	      	 				|  w.var("mu").setVal(1)	     	       				|
|  mc.SetSnapshot(*w.var("mu"));					|  mc.SetSnapshot(*w.var("mu"))						|
|  									|  									|
|  // import model in the workspace					|  # import model in the workspace					|
|  w.import(mc);      	  						|  getattr(w,'import')(mc)						|
+-----------------------------------------------------------------------+-----------------------------------------------------------------------+


When the intent is to use RooStats tools on a model, it is common practice to insert a ModelConfig in the workspace in the
model building phase so that the persistent workspace is fully self-guiding when the RooStats tools are run. More details on
this are given in Section 4 on statistical inference tools. The presence of a ModelConfig object can of course also used to 
simply the use of workspace files outside the RooStats tools. 

For example the code fragment below will perform a maximum likelihood fit on any workspace with a model and 
dataset and valid ModelConfig object

+----------------------------------------------------------------------------------------+------------------------------------------------------------+
|.. code-block:: c									 |.. code-block:: python		                      |
|   		 									 |   		  				    	      |
|   void run_fit(const char* workspaceName, const char* datasetName) {			 |    def run_fit(workspaceName, datasetName):	    	      |
|   	 	       	     		    	  		     		       	 |    	  			 		    	      |
|   	// Reviving a model from a workspace					    	 |	  # Reviving a model from a workspace	    	      |
|	TFile* f = TFile::Open("model.root") ;						 |	  f = ROOT.TFile.Open("model.root")	    	      |
|	RooWorkspace* w = (RooWorkspace*) f->Get("w") ;				         |	  w = f.Get(workspaceName)		    	      |
|	RooStats:::ModelConfig* mc = (RooStats::ModelConfig*) w->genobj("ModelConfig") ; |	  mc = w.genobj("ModelConfig")		    	      |
|	RooAbsPdf* pdf = w->pdf(mc->GetPdf()->GetName()) ;    			       	 |	  pdf = w.pdf(mc.GetPdf().GetName())	    	      |
| 		       	 				 				 |	      					    	      |
|	// Load data from workspace							 |	  # Load data from workspace		    	      |
|	RooAbsData* data = w->data(datasetName) ;					 |	  data = w.data(datasetName)		    	      |
|		    	   								 |	       	 				    	      |
|	if (!pdf || !data) return ;							 |	  if pdf is None or data is None: return None 	      |
|	pdf->fitTo(*data) ;								 |	  pdf.fitTo(data)				      |
|   }			  								 |						    	      |
|.. image:: http://swanserver.web.cern.ch/swanserver/images/badge_swan_white_150.png	 |.. image:: http://swanserver.web.cern.ch/	    	      |
|   :target: https://cern.ch/swanserver/cgi-bin/go/?projurl=				 |           swanserver/images/badge_swan_white_150.png       |
|            https://raw.githubusercontent.com/roofit-dev/				 |    :target: https://cern.ch/swanserver/cgi-bin/  	      |
|            RooStatsWorkbook/master/misc_notebooks/RooFitBasics.ipynb			 |    	       go/?projurl=https://raw.githubusercontent.com  |
|   :alt: ROOT C++									 |             /roofit-dev/RooStatsWorkbook/master/	      |
|   	       										 |             misc_notebooks/RooFitBasics.ipynb	      |
|											 |    :alt: pyROOT				    	      |
+----------------------------------------------------------------------------------------+------------------------------------------------------------+

The example above is not completely universal, because it does not deal with ‘global observables’, this issue is
explained further in later sections.

.. toctree::
   :maxdepth: 1

   docs-factoryunbinned
   docs-factorybinned
   docs-histfactory