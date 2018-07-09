RooFit
======

RooFit - a model building language
----------------------------------

The main feature of the design of the RooFit/RooStats suite of statistical analysis tools is that the tools
to build statistical models and the tools to perform statistical inference are separate, but interoperable.
This interoperability is universally possible because all statistical inference is based on the likelihood function.

The goal is that all tools in the RooStats suite can analyse any model built in RooFit. The interoperability
is facilitated in practice by the concept of the workspace (RooFit class RooWorkspace) that allows to
persist statistical models of arbitrary complexity into ROOT files and thus allow to separate the model building
phase and the statistical inference phase of a data analysis project both in space and time

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


The workspace factory
---------------------

To simplify the logistical process of building RooFit models, the workspace is also interfaced to a factory method that
allows to fill a workspace with objects with a shorthand notation. A short description of the factory language is given
here with the goal to be able to understand code examples given later in this section. A full description of the
language is, along with a comprehensive description of other features of the workspace is given here <INSERT_LINK>
 
The workspace factory is invoked by calling the factory() method of the workspace and passing a string
with a construction specification, which typically look like this:

+---------------------------------------------------------------------+---------------------------------------------------------------------+
|.. code-block:: c						      |.. code-block:: python				                    |
|								      |									    |
|    RooWorkspace w("w") ;					      |    RooWorkspace w("w")				                    | 
|    w.factory("Gaussian::g(x[-10,10],m[0,-10,10],s[3,0.1,10])") ;    |    w.factory("Gaussian::g(x[-10,10],m[0,-10,10],s[3,0.1,10])")      |
|    w.factory("Poisson::p(n[0,100],mu[3,0,100])") ;		      |    w.factory("Poisson::p(n[0,100],mu[3,0,100])") ;		    |
+---------------------------------------------------------------------+---------------------------------------------------------------------+
    
The syntax of the factory, as shown, is simple because it is limited - the two essential operations
are 1) creation of variable objects and 2) the creation of (probability) functions. 

For the first task, the following set of expressions exist to create variables:

* ``name[xmin,xmax]`` creates a variable with given name and range. The initial value is the middle of the range
* ``name[value, xmin,xmax]`` creates a variable with given name and range and initial value
* ``name[value]`` creates a named parameter with constant value
* ``value`` creates an unnamed (and immutable) constant-value object
* ``name[label,label,…]`` creates a discrete-valued variable (similar to a C++ enum) with a finite set of named states

For the second task, a single syntax exists to instantiate all RooFit pdf and function classes

* ClassName::objectname(…) creates an instance of ClassName with the given object name (and identical title).
The three important rules are

1)  The class must be known to ROOT (i.e. if it is defined in a shared library, it must have been loaded already). The prefix "Roo" of any class name can optionally be omitted for brevity 

2) The meaning of the arguments supplied in the parentheses map to the constructor arguments of that class that follow after the name and title. In case multiple constructors exist in a class, they are tried in the order in which they are specified in the header file of the class.  

3) Any argument that is specified must map to the name of an object that already exists in the workspace, or be created on the fly.

These factory language rules can be used to create objects one at a time, as is shown here


+-----------------------------------------------------------------------+-----------------------------------------------------------------------+
|.. code-block:: c							| .. code-block:: python			                        |
|   		 							|    		  							|
|   // Create empty workspace						|   # Create empty workspace						|
|   RooWorkspace w("w") ;						|   w = ROOT.RooWorkspace("w")						|
|   		 							|     									|
|   // Create 3 variable objects -					|   # Create 3 variable objects - 					|
|   //      to be used as observable and parameters of 			|   #      to be used as observable and parameters of 			|
|   //      a Gaussian probability density model    			|   #      a Gaussian probability density model	   			|
|   w.factory("x[-10,10]") ;	   	   				|   w.factory("x[-10,10]") 	  	  				|
|   w.factory("mx[0,-10,10]") ;						|   w.factory("mx[0,-10,10]") 						|
|   w.factory("sx[3,0.1,10])") ;					|   w.factory("sx[3,0.1,10])")						|
|   			       						|   									|
|   // Create a Gaussian model using the previously defined variables	|   # Create a Gaussian model using the previously defined variables	|
|   w.factory("Gaussian::sigx(x,mx,sx)") ;	    	    		|   w.factory("Gaussian::sigx(x,mx,sx)")	   	   		|
|   					 				|   									|
|   // Print workspace contents						|   # Print workspace contents						|
|   w.Print("t") ;     							|   w.Print("t")      							|
+-----------------------------------------------------------------------+-----------------------------------------------------------------------+
|.. code-block:: c													                        |
|   		      																|
|   RooWorkspace(w) w contents															|
|   		      																|
|   variables																	|
|   ---------																	|
|   (mx,sx,x)																	|
|   																		|
|   p.d.f.s																	|
|   -------																	|
|   RooGaussian::sig[ x=x mean=mx sigma=sx ] = 1												|
+-----------------------------------------------------------------------------------------------------------------------------------------------+

Creating models one object at a time, can still be quite space consuming, hence it possible to nest construction operations. Specifically,
any syntax token that generates an object returns (e.g. x[-10,10]’) the name of the object created (‘x’) , so that it can be used immediately in an enclosing
syntax token.  Thus a nested construction specification can construct a composite object in a single token in a natural looking form. 
The four lines of factory code of the previous example can be contracted as follows

.. code-block:: c

  // Create a Gaussian model and all its variables
  w.factory("Gaussian::sigx(x[-10,10],m[0,-10,10],s[3,0.1,10])") ;

Use of existing objects and newly created object can be freely mixed in any expression. The example below creates a second model
for observable y using the same (existing) parameter objects as the Gaussian pdf sig:

.. code-block:: c

  // Create a Gaussian model using the previously defined parameters m,s
  w.factory("Gaussian::sigy(y[-10,10],m,s)") ;
  
As the power of RooFit building lies in the ability combine existing pdfs, operator pdfs that multiply, add, or convolve 
components pdfs are frequently used. To simply their use in the factory, most RooFit-provided operator classes provide
a custom syntax to the factory that is more intuitive to use than the constructor form of these operator classes. 
For example:

* ``SUM::sum(fraction*model1,model2)`` - Sum two or more pdfs with given relative fraction. 
* ``PROD::product(modelx,modely)`` - Multiply two uncorrelated pdfs F(x) and G(y) of observables (x,y) in the same dataset into a joint distribution FG(x,y) = F(x)*G(y)
* ``SIMUL::simpdf(index[SIG,CTL],SIG=sigModel,CTL=ctlModel)`` - Construct a joint model for to disjoint datasets that are described by distributions sigModel and ctlModel respectively
* ``expr::func(’sin(a*x)+b’,a[-10,10],x,b[-10,10])`` - Construct an interpreted RooFit function ‘func’ on the fly with the given formula expression.     

The syntax for operators is not native to the language, but is part of the operator class definition, which can register custom construction expression with the factory.
It is thus explicitly possible for all RooFit pdfs (both for operator and basic pdfs) to introduce additional syntax to the factory language
A comprehensive list of the operator expression of all classes in the standard ROOT distribution of root is given here <INSERT_LINK>. 

A complete example illustrate how the various language features work together to build a two-dimensional probability model with a signal and background component

.. toctree::
   :maxdepth: 2

   docs-factoryunbinned
   docs-factorybinned

A couple of final important points on RooFit models and workspace

* The factory is an efficient way to quickly express what RooFit objects should be instantiated. Objects built with the factory
are not in any way different from those created ‘manually’ and then imported into the workspace.

* All objects (wether created through the factory or imported) can be modified a posteriori, e.g. modification of advanced
settings from class methods, after they have been created and/or imported. All such changes in settings are persisted
with the workspace. The factory does therefore on purpose not provide an interface for such modifications.

* All RooFit objects in a workspace should have unique names. The factory and import methods automatically enforce this

Building joint models and data structures
=========================================

the general case
----------------

When building joint probability models for multiple datasets, e.g. simultaneous fits or profile likelihood models in general, there are some
important point to consider when building the model. In the RooFit paradigm such joint fits are best constructed by first joining all
independent datasets together into a single composite ‘master’ dataset, and by joining all probability models corresponding to
the individual datasets into a composite ‘master’ probability model. If it is done this way, all main statistical operations ranging from 
basic operations like fitting and toy data generation to advanced limit setting procedures work on composite models with exactly the same 
interface as for simple models. This design choices greatly simplifies the syntax of the high level statistical tools, but introduces some additional syntax to express composite datasets and probability models, which are discussion in this section. 

**Joining probability models for disjoint datasets**

A joint probability model that simultaneously describes two or more disjoint datasets is represented in RooFit by class
RooSimultaneous, which is constructed in the factory with the SIMUL operator: 

.. code-block:: c

  // Construct joint model for signal region and a control region
  // This example assumes a pdf sigModel and ctlModel were previously defined 
  // in the workspace (the internal structure of these models is irrelevant)
  //
  w.factory("SIMUL::master(index[SIG,CTL],SIG=sigModel,CTL=ctlModel)") ;

The code above generates two new objects

* A new discrete observable (class RooCategory)  named ‘index' that has two defined states: SIG and CTL that serves
as additional observable to the newly create pdf master (in addition to the observables defined in sigModel and ctlModel). 
The role of this observable is to indicate which component pdf should be used to interpret the remaining observables in the dataset

* A new joint probability model master that describes both distributions. Suppose sigModel describes the distribution
of observable x and ctlModel describes the distribution of observable y, then the observables of the joint model
are {index, x, y}

Joining disjoint datasets into a master dataset

The joint dataset that is needed to construct a likelihood from ‘master’ model must thus also conform to this structure:
it’s observable must include both x and y, as well as the discrete index variable to label to which component dataset each
event belongs. It is instructive to first consider visually what this data organization entails

.. code:: c

     D(x)    D(y)       D(index,x,y)
     1.5     23           SIG, 1.5, -
     2.3     17    —>     SIG, 2.3, -
     4.7     98           SIG, 4.7, -
                          CTL,  - , 23
                          CTL,  - , 17
                          CTL,  - , 98

The joining of datasets is trivially perform in the RooDataSet constructor

.. code-block:: c

    // The example assume this existence of a workspace that defines variable, x,y,index

    // Construct a dataset D(x)
    RooDataSet d_sig("d_sig","d_sig",*w.var("x")) ;
    w.var("x")->setValue(1.5) ; 
    d_sig.add(*w.var("x")) ; // fill data, etc..

    // Construct a dataset D(y)
    RooDataSet d_ctl("d_ctl","d_ctl",*w.var("y")) ;
    w.var("y")->setValue(23) ;
    d_ctl.add(*w.var("xy")) ; // fill data, etc..

    // Construct a joint dataset D(index,x,y)
    map<string,RooDataSet*> dmap;
    dmap["SIG"]=&d_sig ;
    dmap["CTL"]=&d_ctl ;

    RooDataSet d_joint("d_joint","d_joint",Index(*w.cat("index")),Import(dmap)) ;

Note that while a composite RooDataSet externally presents the interface of a simple dataset with observables (index,x,y)
it is internally organised in subdatasets for efficient storage and data retrieval 

.. code:: c

  D(index,x,y)           D(index,x,y)
  SIG, 1.5, -               index==SIG     index==CTL
  SIG, 2.3, -                 D(x)            D(y)
  SIG, 4.7, -       —>        1.5             23   
  CTL,  - , 23                2.3             17
  CTL,  - , 17                4.7             98
  CTL,  - , 98

This internal data organization is also introduced for toy data sets that are sampled from composite datasets.

Working with joint models and datasets

Once the joint model master(index,x,y) and a joint dataset D(index,x,y) are constructed, all operations work exactly
as for simple models. For example, a joint fit is trivially performed as follows


.. code-block:: c

     w.pdf("master")->fitTo(d_joint) ;

An important part of the design of joint models is that there is no performance loss due to computation overhead in the
joint construction:  when the joint likelihood is constructed from a joint model and dataset, it is internally organised again 
in component likelihoods, where each component model of master is match to the corresponding component dataset of d_joint,
and subsequently separately optimised for computational efficiency.

profile likelihood models & global observables
----------------------------------------------

While the interface described in the previous sections works for joint models of any shape and form, a dedicated syntax
exists for the formulation of so-called ‘profile likelihood’ models. The key difference between a generic joint model
and a ‘profile likelihood’ joint model is that in the latter the internal structure of the control region (now named ‘subsidiary measurement’ 
is simplified to a Gaussian, Poisson or LogNormal distribution so that the corresponding observation can be represented by a single number, 
where the convention of the subsidiary measurement is furthermore chosen such, through a suitable transformation of the meaning of the parameters, 
that the observed values is either always zero or one

.. math::
   \mathcal{L}_\mathrm{joint}(p,q) = \mathcal{L}_\mathrm{sig}(x_\mathrm{sig} | \mu, \theta) * \mathcal{L}_\mathrm{ctl}( y_\mathrm{ctl} | \theta )
.. math::
   \Downarrow
.. math::
   \mathcal{L}_\mathrm{profile} ( \mu, \alpha) = \mathcal{L}_\mathrm{sig} (x_\mathrm{sig} | \mu, \alpha) * \mathcal{L}_\mathrm{subs}( 0 | \alpha)

Such likelihood can in principle be described with a joint dataset :math:`D`(index,:math:`x_\mathrm{sig}`,0,0,….0) and a corresponding ``SIMUL`` probability model with equally many components, but the carrying around of hundreds of observed values that are always identical to zero can be cumbersome. 

Hence an alternative formulation style exists in RooFit that is specifically suitable for profile likelihood models - here the observed
value of the subsidiary measurements is simply encoded in observable variable of the pdf itself, rather than in an external dataset.
Hence instead of a dataset/model pair in traditional joint format 

.. math::
     \mathcal{D}_\mathrm{simul}(\mathrm{index},x_\mathrm{sig},y_\mathrm{ctl}==0)   
.. math::
   \mathrm{and}
.. math::
  \mathrm{pdf}_\mathrm{simul}(\mathrm{index},x_\mathrm{sig},y_\mathrm{ctl} | \mu, \alpha)  = \mathrm{pdf}_\mathrm{sig}(x_\mathrm{sig}|\mu,\alpha) * \mathrm{pdf}_\mathrm{subs}(y_\mathrm{ctl}|\alpha)

one writes

.. math::
     \mathcal{D}_\mathrm{simul}(x_\mathrm{sig})
.. math::
    \mathrm{and}
.. math::
   \mathrm{pdf}_\mathrm{simul}(x_\mathrm{sig},y_\mathrm{ctl}==0 | \mu, \alpha)  = \mathrm{pdf}_\mathrm{sig}(x_\mathrm{sig}|\mu,\alpha) * \mathrm{pdf}_\mathrm{subs}(0|\alpha)

where the observable :math:`y_\mathrm{ctl}==0` is dropped from the dataset, as well as the index observable, as it is no longer needed 
to distinguish between data belonging to the main measurement and the subsidiary measurement(s). This greatly simplifies
the formulation of datasets of profile likelihood models. On the probability model side, the formulation then changes as follows

* Instead of using ``SIMUL()`` to multiply subsidiary measurements with the main measurement, the ``PROD()`` operator
is used, which does not require the introduction of an index observable to label events

* The model variables that represent the subsidiary measurements must be explicitly labeled in the object as such
(to distinguish these from parameters). This can be either done intrinsically in the model, or this specification
can be provided externally when an operation (fit, toyMC generation etc) is performed

In the LHC jargon the variables in the model that represent the (trivial) values of the observables of the subsidiary
measurements are named the “Global Observables”.  The easiest-to-use way to mark global observables
in the model is to label them as such

.. code-block:: c

     // repeat for each global observable
     w.var(“obs_alpha”)->setAttribute(“MyGlobalObservable”) ;

     // advertise string label used to mark global observables in the top-level pdf object     
     w.pdf(“master”)->setStringAttribute(“GlobalObservablesTag”,”MyGlobalObservable”) ;

Most modeling building tools currently in use, do not yet take advantage to label these observables as such in the model.
Instead it is common practice, to specify these always external through a ``GlobalObservables()`` command-line specification when fitting or generation. 
To simplify this external specification process for RooStats tools, the ``ModelConfig`` interface object allows the global observables to
be specified there, so that its definition is always consistently used by all RooStats tools.

.. code-block:: c

   // Define the pdf, the parameter of interest and the observables
  mc.SetPdf(*w.pdf(“p"));
  mc.SetParametersOfInterest(*w.var("mu"));
  mc.SetGlobalObservables(…..);
  mc.SetObservables(*w.var(“n"));

Any code that works with profile likelihood that wishes this external specification of the global observables, in addition to the internal specifications
must incorporate some explicit handling of thus. Here is a modified version of the run_fit() macro of the previous section that does so


+----------------------------------------------------------------------------------------+-------------------------------------------------------------------+
|.. code-block:: c									 |.. code-block:: python		                             |
|   		 									 |   		  				    	      	     |
|   void run_fit(const char* workspaceName, const char* datasetName) {			 |    def run_fit(workspaceName, datasetName):	    	      	     |
|   	 	       	     		    	  		     		       	 |    	  			 		    	      	     |
|   	// Reviving a model from a workspace					    	 |	  # Reviving a model from a workspace	    	      	     |
|	TFile* f = TFile::Open("model.root") ;						 |	  f = ROOT.TFile.Open("model.root")	    	      	     |
|	RooWorkspace* w = (RooWorkspace*) f->Get("w") ;				         |	  w = f.Get(workspaceName)		    	      	     |
|	RooStats:::ModelConfig* mc = (RooStats::ModelConfig*) w->genobj("ModelConfig") ; |	  mc = w.genobj("ModelConfig")		    	      	     |
|	RooAbsPdf* pdf = w->pdf(mc->GetPdf()->GetName()) ;    			       	 |	  pdf = w.pdf(mc.GetPdf().GetName())	    	      	     |
|       RooArgSet* globs = mc->GetGlobalObservables() ;	 				 |	  globs = mc.GetGlobalObservables()			     |
| 		       	 				 				 |	      					    	      	     |
|	// Load data from workspace							 |	  # Load data from workspace		    	      	     |
|	RooAbsData* data = w->data(datasetName) ;					 |	  data = w.data(datasetName)		    	      	     |
|		    	   								 |	       	 				    	      	     |
|	if (!pdf || !data) return ;							 |	  if pdf is None or data is None: return None 	      	     |
|	   	    	   	  							 |	     	    	    	    	  	 	      	     |
|	if (globs) {									 |	  if globs:						     |
|		pdf->fitTo(*data, GlobalObservables(*globs)) ;				 |	    pdf.fitTo(data, ROOT.RooFit.GlobalObservables(globs))    |
|	} else {		  			     				 |	  else:		    					     |
|	  pdf->fitTo(*data)								 |	    pdf.fitTo(data)					     |
|   }			  								 |						    	      	     |
|.. image:: http://swanserver.web.cern.ch/swanserver/images/badge_swan_white_150.png	 |.. image:: http://swanserver.web.cern.ch/	    	      	     |
|   :target: https://cern.ch/swanserver/cgi-bin/go/?projurl=				 |           swanserver/images/badge_swan_white_150.png       	     |
|            https://raw.githubusercontent.com/roofit-dev/				 |    :target: https://cern.ch/swanserver/cgi-bin/  	      	     |
|            RooStatsWorkbook/master/misc_notebooks/RooFitBasics.ipynb			 |    	       go/?projurl=https://raw.githubusercontent.com  	     |
|   :alt: ROOT C++									 |             /roofit-dev/RooStatsWorkbook/master/	      	     |
|   	       										 |             misc_notebooks/RooFitBasics.ipynb	      	     |
|											 |    :alt: pyROOT				    	      	     |
+----------------------------------------------------------------------------------------+-------------------------------------------------------------------+

The processing of intrinsic global variable specifications is always automatic, but unfortunately it’s use is not common
practice yet in LHC models.