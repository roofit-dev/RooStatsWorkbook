Bayesian
========

Methodology
-----------

The Bayesian approach has been used for a number of ATLAS results, primarily for upper-limits on a signal cross-section. The Bayesian approach needs two key pieces of information: the likelihood :math:`L(\mu,\alpha)` and the prior :math:`\pi(\mu,\alpha)`, where :math:`\mu` denotes the parameter(s) of interest and :math:`\alpha` denotes the nuisance parameters.  

As mentioned in Section~\ref{S:modeling}, if one has a full statistical model :math:`f(data|\mu,\alpha)`, then one evaluates the likelihood via :math:`L(\mu,\alpha) \equiv f(\textrm{observed~data} | \mu,\alpha)`. It is possible that one defines a likelihood function :math:`L(\mu,\alpha)` without providing the ability to generate pseudo data, but this is approach rules out the ability to compare to or provide complementary analysis with frequentist methods.


Prior
-----

The prior :math:`\pi(\mu,\alpha)` must also be specified for Bayesian methods.  There are several approaches to specifying the prior ranging from a fully subjective ''prior belief'' to priors derived from formal rules.  Often the prior is factorized as :math:`\pi(\mu,\alpha) = \pi(\mu)\pi(\alpha)`.  While priors may be informed by previous measurements, there is always some trace of an original prior, as described below. Recall that a change of variables influences the prior. In particular, the change of variables from :math:`\mu \to \nu(\mu)` introduces a Jacobian factor for the prior  :math:`\pi(\mu) \to \pi(\nu) = \pi(\mu)/ | d\nu/d\mu |`. This implies that a statement like ''uniform prior'' is meaningless unless you say that is uniform with respect to some particular variable. This is relevant for situations in which the signal rate is a function of some theoretical parameters -- like the energy scale :math:`\Lambda` for a contact interaction. The choice of ''flat'' prior in :math:`s`, :math:`\Lambda`, :math:`\Lambda^2`, etc. are not equivalent.

The prior for the nuisance parameters is often dealt with in way that is closely connected to frequentist approach. In particular, there is often some auxiliary measurement described by the statistical model :math:`f(a | \alpha)`, where :math:`a` is the data associated to that auxiliary measurement used to measure/constrain :math:`\alpha`. Often the statistical model for these auxiliary measurements are adequately approximated by a Gaussian or Poisson. The corresponding prior used for the main measurement can be seen as the posterior from the auxiliary measurement.  In particular, it is given by Bayes' theorem with :math:`\pi(\alpha) \propto f(a|\alpha) \eta(\alpha)`, where :math:`\eta(\alpha)` is some original prior (sometimes called the ''ur-prior'' in reference to the German ''ur-'') from before both the main measurement and the auxiliary measurement. Most commonly, the :math:`\eta(\alpha)` is taken to be uniform in :math:`\alpha` -- which is mainly irrelevant when the parameter has small relative uncertainty after the auxiliary measurement -- in which case :math:`\pi(\alpha) \propto f(a|\alpha)`.  Table~\ref{tab:constraints} provides a few common and consistent relationships between the auxiliary measurement, the ur-prior , and the resulting  posterior from the auxiliary measurement (which is used as prior for the main measurement).

In cases like theoretical uncertainties, there may be no auxiliary measurement for the parameter :math:`\alpha`.
In the Bayesian approach, one can directly start with an assumed :math:`\pi(\alpha)`, while the constraint term used in the frequentist approach is typically introduced artificially following Table~\ref{tab:constraints} backwards (\i.e. :math:`\pi(\alpha) \to f(a|\alpha)`, ). 

The prior for the parameter of interest is more delicate as it directly impacts the inference on :math:`\mu`. As mentioned above,  for situations in which the signal rate is a function of some theoretical parameters like the energy scale :math:`\Lambda` for a contact interaction, then the choice of ''flat'' prior in :math:`s`, :math:`\Lambda`, or :math:`\Lambda^2` will lead to different answers.  The discussion of which prior to use is beyond the scope of this document, but the most common choice is to use a uniform prior for the signal yield.  While not a justification, it is an important to know that 1-sided Bayesian upper-limits using this prior agree with the 1-sided CLs upper-limits in almost all problems that have been investigated, even complex Higgs combinations~\cite{ATLAS:2011gia}.  

The other priors that have been advocated are Jeffreys's prior~\cite{Jeffreys} and the Reference prior~\cite{Demortier:2010sn,Casadei:2011hx}, which are both the result of a formal procedure based on the full statistical model :math:`f(data|\mu,\alpha)` (another reason it is good to describe the full statistical model, not just the likelihood).  These priors have nice invariance properties to reparametrization (e.g. :math:`s(\Lambda) \to \Lambda`) and strive to be `non-informative' in a precise sense. Unfortunately, both priors are difficult to calculate except in rather simple problems. 
For example, the reference prior has been calculated for the common number counting problem 

.. math::

  f(n,m | \mu, \alpha) = \Pois_{SR}(n|\mu+\alpha) \, \Pois_{CR}(n|\tau\alpha) \;,

where :math:`\tau` is a fixed extrapolation factor. Note, the reference prior for the number counting experiment is implemented in BAT.

Sampling and Marginalization Tools
----------------------------------

The key object of Bayesian analysis is the posterior distribution, which is proportional to the product of the likelihood :math:`L(\mu,\alpha)` and the prior :math:`\pi(\mu,\alpha)`.  When interested in inference on the parameter of interest :math:`\mu` the Bayesian approach is to marginalize (integrate) over the nuisance parameters :math:`\alpha`. These integrals can be difficult and are typically performed with numerical algorithms that produce a large sample of :math:`\{\mu,\alpha\}` that is proportional to the posterior. The primary algorithms for this sampling  are the Markov Chain Monte Carlo (Metropolis-Hastings or Gibbs sampling) and Nested Sampling. The two primary tools that implement these algorithms are the BAT toolkit~\cite{BAT} and RooStats, and there is interoperability between these two tools~\cite{roobat}.  BAT provides MCMC sampling for likelihoods implemented in both BAT's modeling language or RooFit modeling language. RooStats can use either it's internal \texttt{MCMCCalculator}, the interface to BAT's MCMC algorithm, or the nested sampling algorithm implemented in MultiNest~\cite{Feroz:2008xx}.

Improvements Needed/Planned
^^^^^^^^^^^^^^^^^^^^^^^^^^^

The interface between BAT/RooFit interfaces should be maintained and well tested for complicated examples.  The RooStats tool for numerically evaluating the Jeffreys prior for an arbitrary RooFit model \cite{RooJeffreys} should be further developed and documented (as this can be used by both RooStats and BAT). Diagnostics of various sorts are available, but can always be improved.
