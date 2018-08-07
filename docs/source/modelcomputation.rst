Understanding computational aspects of the likelihood
=====================================================

Understanding computational performance
---------------------------------------

As probability models grow in complexity, the calculation time of likelihood functions can become a limiting factor. Models expressed in RooFit allow the RooFit core code complete introspection in the model structure and can automatically apply a series of performance optimizations to make calculations more effient and thus allow for faster calculations. Most of these techniques are applied automatically, few of them must be optionally activated. This section outlines the essence of optimization techniques that are applied and serves to further the understanding of model computation times in RooFit.

*When are optimizations applied?*

RooFit generally distinguishes two modes of operation - inside bulk operations (likelihood calculation, or toy event generation) and outside bulk operations. Most optimizations are applied inside the context of a bulk operation, where performance is more critical, and where more optimization opportunities exist as the use case of probability models is more clearly spelled out. Bulk operations in RooFit always operate on a clone of the original pdf, allowing to make non-resverrable use-case specific optimisation modifications to the model. The cloned model is discarded at the end of the bulk operation

**Analytical integrals where possible**

When RooFit is used to specify probability density functions (e.g. a Gaussian in a real-valued observable x) rather than as a probabillity model
(e.g. a Poisson in a discrete-valued observable n), integrals must be explicitly calculated to make such probability density functions
unit-normalized. As RooFit pdfs have no intrinsic convention of which of their variables are observables versus parameters, 
a calculation of a pdf as normalized probability density function must always be accompanied with a 'normalization set’, which defines
the subset of pdf variables that must be interpret as observables, and hence over which the expression must be normalized:

.. code-block:: c

    w.factory("Gaussian::g(x[-10,10],m[-10,10],s[3,0.1,10])") ; 
    RooArgSet normset(*w.var("x")) ;
    Double_t g_normalized_wrt_x = w.pdf("g")->getVal(normSet) ; // calculate g as pdf for observable x

When called in this form, a separate RooFit object is created of class ``RooRealIntegral`` that represents the integral ``int_g(m,g) = Int g(x,m,g) dx`` over the defined range of variable ``x``. RooFit function objects can advertise and implement (partial) analytical integrals in any (sub)set of their variables, for the cases where these expressions are analytically known. Whenever an integral over a pdf is needed in a likelihood evaluation (or other RooFit operation), the RooFit integral representation object RooRealIntegral will negotiate with the pdf if any suitable (partial) analytical integrals exists that match the requeste integration, and use those. Any remaining partial integrals that can not be integrated analytically, will be integrated numerically. When numerical integrals are used, information messages are emitted on the command line, and some user configuration/intervention may be needed to optimize performance (see section XXX on performance tuning for further details)

Integral representation objects that are used for pdf normalization purposes, as shown in the example above, are cached and owned by the pdf that requires them - hence the negotiated optimal calculation strategy is not redetermined at every call for a normalized pdf value, but kept for the lifetime of the pdf object, or until the pdf is structurally modified by the user in which case it is discarded and will be recreated on demand later if needed. 

**Change tracking and lazy evaluation (likelihood calculation & normal use)**

The output value of each RooFit function object, as calculated by the classes implementation of method ``RooAbsReal::evaluate()`` is cached automatically in data member ``RooAbsReal::_value``, when ``RooAbsReal::getVal()`` is called. A subsequent call to ``getVal()`` will simply return the previously calculated and cached value, unless it is detetermined that one or more of the inputs to the calculation has changed. RooFit tracks such changes through client-server links it maintains between all components of a pdf (whenever a dependent component of a RooFit function object is held in a data member of class ``RooRealProxy``, ``RooListProxy`` or ``RooSetProxy``, such links are automatically initiated). Each link (and each proxy) indicates if the object it is member of depends on the value or the shape of the server object.  For real-valued objects, the shape of servers objects relates to the boundaries of the allowed values for that object.

For example, for a Gaussian pdf constructed as follows

.. code-block:: c


   w.factory("Gaussian::g(x[-10,10],m[-10,10],s[3,0.1,10])") ; 

the object ``RooGaussian::g`` will consider the objects ``RooRealVar::x``, ``RooRealVar::m`` and ``RooRealVar::m`` as it servers, and the latter 3 objects will consider ``RooGaussian::g`` as its client. When ``RooGaussian::g``  is constructed, it will indicate that it’s own value depends on the value of these three variables (but not on the shape). [ The configuration of this dependency information is encoded in the constructors of the proxy data members that hold x, m and g ]

The effect of that is that whenever one or more of the three variables changes its value, it will propagate a 'value-changed' message to all its clients (in this case ``RooGaussian::g``), and this will set a ‘dirty’ flag on the value cache of that object. The raising of this dirty flag will not automatically trigger a recalculation of the value of g, but merely indicate that this must happen the next time ``g.getVal()`` is called, hence the name ‘lazy evaluation’.

Caching and lazy evaluation is of particular importance for normalization integrals: when the value of pdf g is requested as a normalized probability density function w.r.t to observable x, e.g.

.. code-block:: c

    RooArgSet normset(*w.var("x")) ;
    Double_t g_normalized_wrt_x = w.pdf("g")->getVal(normSet) ;

a separate object is created that represents the integral Int g(x) dx, which depends on the values of parameters m and g, and the shape (range) of observable x, but not on the value of observable x. 

*When is caching and lazy evaluation applied?*

Caching and lazy evaluation is active by default on all RooFit objects, whether directly created on the command line or in a workspace. Inside a likelihood function a slightly different strategy is applied - as a likelihood entails a series of pdf evaluation where the value of the observable changes (in principle) every time there is no point in tracking dependencies of direct or indirect pdf dependencies on dataset observables, as the result is such a check is already predetermined - each one will need to be recalculated for every consecutive event. Hence for all pdf and function components inside a likelihood that depend directly or indirectly on dataset observables, change tracking is disabled, to save the time that is spent in this unnecessary tracking. The notable exception to this are the normalization integral objects, which do not depend on the value of the dataset observables, but just on their range - these will remain in cache/lazy-eval mode inside likelihood objects, and thus only be recalculated on the less frequent occastion that one of the dependent model paremeters changes.

**Constant term detection (likelihood calculation)**

Certain parts of a probability model may depend only on parameter objects that are declared constant (``RooRealVar::setConstant(kTRUE)``). While the constant flag does not prevent manual modification of such a variable through a call to ``RooRealVar::setValue()``, parameters that are flagged as constant will not be modified by the minimizer algorithm during a likelihood minimization are thus effectively constant during a minimization session. RooFit automatically detects all expressions in a likelihood that are effectively constant at the start of each minimization session and precalculates and caches their value along with the dataset. For example, for a composite pdf with signal and background 


.. code-block:: c

    RooWorkspace w("w") ;
    w.factory("SUM::model(fsig[0,1]*sig::Gausian(x[-10,10],m[-10,10],s[3]),bkg::Polynomial(x,{a0[0],a1[1]}))") ;

the background pdf ``bkg::Polynomial`` has only constant parameters (``a0`` and ``a1``) hence its value is precalculated for every value of ``x`` in the dataset of a likelihood, and effectively added as a column to the internal dataset. A message to this effect is shown on the command line when the likelihood is initialized

When each event is loaded from the dataset in the likelihood calculation loop, the precalculated value of ``bkg::Polynomial`` is directly loaded in the value cache of that pdf, and it’s internal recalculation is skipped. Higher-level objects that depend on these cached elements, ``SUM::model`` in the example above, will then simply use the cached expression.

For pdfs expressions with multiple layers of composition operations, it is possible that entire trees of expression become constant. Consider for example

.. code-block::c

    RooWorkspace w("w") ;
    // Background consisting of peaking background and general background
    w.factory("SUM::sumbkg(fpeak[0.1]*peakbkg::Gausian(x[-10,10],m_bkg[10],s_bkg[3]),polybkg::Polynomial(x,{a0[0],a1[1]}))") ;
    // Add signal to composite backgroundl
    w.factory("SUM::model(fsig[0,1]*sig::Gausian(x,m_sig[-10,10],s_sig[3]),sumbkg)") ;

Here the background pdf is a sum of a general background and a peaking background that are first added together, before it is added to the signal. In this scenario, three pdf components in the full expression are constant: ``peakbkg``, ``polybkg`` and ``sumbkg``. In this particular scenario it is not needed to precalculate and cache all three pdfs: ``polybkg`` and ``sumbkg`` do not need to be cached separately as all of their clients (in this case just one - ``SUM::sumbkg``) exclusively depend on constant expressions, hence they are never needed during the pdf evaluation - therefore only ``sumbkg`` is precalculated and cached, and ``peakbkg`` and ``polybkg`` are completely deactivated during the likelihood evaluation.

*When is constant term optimization applied?*

Constant term detection and precalculation only applies to likelihood calculations is automatically applied when ``RooAbsPdf::fitTo()`` is called. 
If you set up the minimization yourself, it must be explicitly activated manually when you configure the minimizer

.. code-block:: c


    RooAbsReal* nll = pdf->createNLL(*data,…) ;
    RooMinimizer m(*nll) ;
    m.optimizeConst(1) ; // This line activates constant term optimization
    m.migrad() ;
    m.hesse() ;
    // etc 

**Cache-and-track optimization (likelihood calculation)**

Cache-and-track optimization is an extension of the concept of constant-term optimization that can further reduce calculation times by exploiting typical likelihood usage patterns of minimization algorithms like ``MINUIT``. 

Apart from it’s setup phase, a likelihood minimization in ``Minuit(2)`` alternates two modes of operation: gradient calculations - where one parameter is changed per likelihood call - and gradient descent - where all parameters are changed for every likelihood call. For likelihoods with many parameters the gradient calculation calls dominate ``MINUITs``  likelihood evaluations as it requires *N* (parameter) calls to calculate the full gradient, whereas the gradient descent phase typically takes *O(3)* calls, independent of the number of parameters. If in the majority of likelihood calls from ``MINUIT`` only one parameter is changed, many component pdfs remain unchanged between likelihood calls, as typically a small subset of all pdf components will depend on the single parameter that changed. 

In cache-and-track configuration, in addition to the truly constant terms, all component pdfs of a SUM and PROD composite models will be cached, as if they were constant, but a change tracker is included for these component that determines if the cache needs to be updated later (i.e. when a parameter of the pdf has changed w.r.t to the values that were used to calculate the cache contents). For the example pdf below, with only floating parameters in both signal (``m``) and background ( ``a0`` , ``a1`` )

.. code-block:: c

    RooWorkspace w("w") ;
    w.factory("SUM::model(fsig[0,1]*sig::Gausian(x[-10,10],m[-10,10],s[3]),bkg::Polynomial(x,{a0[0,1],a1[0,1]}))") ;

both sig and bkg components can be cache-and-tracked, which has the following effect on the likelihood evaluation for the gradient calculation

+----------------------+-------------------+--------------------------+
|    gradient parameter| components cached | components recalculated  |
+======================+===================+==========================+
| | fsig	       | | sig,bkg         | | model		      |
| | m 		       | | bkg             | | sig,model	      |
| | a0		       | | sig             | | bkg,model	      |
| | a1		       | | sig             | | bkg,model     	      |
+----------------------+-------------------+--------------------------+

A clear saving is realized for the likelihood evaluations required for ``MINUITs`` gradient calculation, as only 1 or 2 of the 3 components pdfs need recalculation, instead of always all three of them, as would be required without cache-and-track.

*When is cache-and-track optimization applied?*

Cache-and-track optimization only applies to calculations within a likelihood. As the efficiency of cache-and-track optimization is highly dependent on the structure of the likelihood, it is not activated by default. To activate cache-and-track optimization of the likelihood, change the value of the argument to ``RooMinimizer::optimizeConst()`` from 1 to 2, or alternatively pass argument ``Optimize(2)`` to ``RooAbsPdf::fitTo()``.

**Optimizing calculations with zero weights**

In models that multiply pdf components (e.g. inside class ``PROD``) it can happen that a product term evaluates to zero. If this occurs, the product calculation for that event is terminated immediately, and remaining product terms are no longer evaluated. Similarly, for weighted datasets (binned or unbinned), if the event weight is zero, the probability model for that event is not calculated.

*When is zero-weight optimization applied?*

Always automatically applied

Understanding issues with parallel calculation of likelihoods
-------------------------------------------------------------

The current RooFit version (v3.6x) has a simple built-in strategy to parallelize likelihood calculations over multiple cores on the same host, To activate this option,
add the option ``NumCPU(n)`` to either ``RooAbsPdf::createNLL()`` or ``RooAbsPdf::fitTo()``. It is important to understand the limitations of the paralellization algorithm:

The invocation of ``NumCPU(N)`` splits the workload of the likelihood (or each top-level likelihood object in case the top-level pdf is a ``SIMUL``) in *N* equal sizes by dividing the number of data events offered to each of the *N* subprocesses in equal subsets. For unbinned datasets, the dataset is partitioned in N equal-size contiguous pieces. For binned datasets (i.e. histograms). the data is partitioned in equal sizes with an interleaving algorithm since histograms can be unevenly distributed. (Equal-size contiguous histogram partitions are prone to a substantial inbalance of partitions with zero event counts).

While the protocol overhead for parallelization is low, there are two major factors that spoil the scaling of wall-time speedup with the number of cores:

* Expensive (numerical) integral calculations required in pdfs. Integral calculations are currently not distributed, but replicated among calculation threads.Hence if the total calculation time of a likelihood is dominated by (numerical) integrals, rather than PDF evaluations, wall-time gains realized by parallelization will be limited

* Strongly heterogenous model structure. For wall-time scaling to be efficient, it is important that the workload can be divided in *N* partitions that each require the very similar, and ideally the exact same, calculation time. If the partitions end up not having very similar calculation times, the wall time will be dominated by the calculation time of the longest partition, which easily frustrates scaling beyond *N* =2,3. Models that are prone to load-imbalancing are those that have many binned dataset with variable and small sizes. For example the likelihood calculations based on histogram with 1 or 2 bins is not easily distributed in a balanced way over 4 CPUs.

In practice, all binned likelihoods built i ATLAS/CMS are strongly heterogeneous and thus poorly scalable. Unbinned ML fits, as typically executed in B-physics, are usually well-scalable.  A new version of the RooFit parallel scheduler is being developed that addresses the load balancing issues with a dynamic scheduling polocy for strongly heterogenous pdfs, and is expected to be available late 2018, early 2019.

Also note that parallel calculated likelihood may return marginally different likelihood values, at the level of numeric precision of the calculation, as the order in which likelihood contributions from the data points are summed is inherently different. For marginally stable or unstable fit models, such small perturbations in the calculation may make the difference between convergence and failure, but it is important to diagnose this properly - the cause of the problem is the marginal stability, not the parallelized calculation.

Understanding the numeric precision and stability of calculations
-----------------------------------------------------------------

In complex models it is not uncommon that limiting numeric precision becomes an issues and will lead to numerical instability of calculations. Numerical problems can have many causes, and can manifest themselves in many different forms. This section will focus only on issues arising from limited numeric precision arising in likelihood calculations, and not on issues arising from building models from poor information that are intrinsically well calculable but contain little useful information (e.g. template morphing models with low template statistics).

**At what level of precision must the likelihood be calculable?**

To understand when numeric precision in likelihood calculations becomes limiting it is important to understand how likelihoods are used by (minimization) tools. As most frequentist statistics tools boil down to minimization problems we will focus on what is needed for a stable minimization of a likelihood. At a fundamental level minimizers require that the likelihood, it’s first and second derivative are continuous.  While most likelihoods meet these requirements at the conceptual level, numerical noise may spoil these features in their implementation.  The most susceptible calculation of minimizers to noise is the numeric calculations of the first derivatives, evaluated as :math:`f(x) - f(x+\delta x)/\delta`,  for comparively small values of :math:`\delta`. Jitter in the likelihood function, introduced by numerical noise, may thus result in wildly varying estimates of such derivatives when scanning over the parameters, if the frequency of the noise is comparable in size to :math:`\delta x`.  In practice, an absolute precision at the level of 10 :math:`^{-6}`, is the barely tolerated maximum level of noise for minimizers like ``MINUIT``, with target precisions at 10 :math:`^{-7}` or 10 :math:`^{-8}` often resulting in substantially faster converge and minimization stability.  

**Sources of numeric noise in the likelihood**

There are two common source of noise in the likelihood: limited precision numeric integration of pdfs, and cumulative rounding/truncation effects of likelihoods.

**Numeric integration noise**

Numeric integration of functions is a notoriously hard computational problem, for which many algorithms exist. For reasons of speed and stability, analytical
integrals are strongly preferred, and RooFit classes provide these when available. Nevertheless for many classes of functions no analytical expressions exist,
and RooFit will substitute a numerical calculations.  When considering numeric integrals, one-dimensional and multi-dimensional integrals represent challenges of a vastly different magnitude. 

*Numeric noise in 1-dimensional integrals.*

Excellent numeric algorithms exist for one-dimensional integration that not only estimate the integral, but also its accuracy well, hence letting the (often iterative) integration algorithm reach the target precision for every integral in a reasonable amount of computing time. For well-behaved functions (i.e. no singularities or discontinuities) RooFits default algorithm, the adaptive Gauss-Kronrod integration, will rarely cause numeric problems with the default target precision of 10 :math:`^{-7}` (considered both absolute and relative). 

However, if the integrands are ill-suited, e.g. consider a histogram-shaped function with multiple discontinuities, integration algorithms can fail in various way: its error estimate may underestimate the true error, leading to larger variations of the calculated integral as function of the integrands parameter than the target tolerance, resulting in intolerable numeric noise in the likelihood. Alternatively, the algorithm may start ‘hunting’, switching forth and back between levels of iteration where it declares convergence at the requested level of precision, again as function of the integrands parameters, leading to spikes or jumps in the calculated integral. In particularly adaptive algorithms, that recursively subdivide the domain of the integral in smaller pieces, are susceptible to all of these failings with every additional subdivision. If the integrand is not pathological - the first defence against numeric instability from integrands is simply to increase the target precision. If that does not result in improvements and/or if the function has 'difficult' features, it may be advisable to try non-adaptive integration algorithms, which may require more function calls to converge, but are generally more stable in their result as function of the model parameters. 

*Numeric noise in multi-dimensional integrals.*

Numeric integration in more than one dimension is notoriously hard, with an unfavorable tradeoff between computing time and numeric precision. The best solution is to avoid numeric multi-dimensional integration, by considering the feasibility of partial analytical integrals (RooFit explicitly supports hybrid analytical/numeric integrals in multiple dimensions). If this is not feasible, some careful tuning of algorithms is usually required, as multi-dimensional integrals rarely run at a satisfactory working point concerning speed and precision out of the box. For integrals of low dimensions (2,3), adaptive cubature integrals can be considered (the multidimensional equivalent of Gauss-Kronrod), this is also the default in RooFit. However a ‘mesh’ in 2/3 dimensions is harder to construct than a 1-dimensional segmentation, and a reliable and fast convergence is only obtained for very smooth functions in all integrated dimensions. If multidimensional integrands are spiky, have ridges (e.g. Dalitz plots), near-singularies etc, or have many more than 2 dimensions, cubature algorithms fail dramatically, often vastly overreporting the achieved precision, and thus causing likelihoods that embed these integrals to fail in minimization.  A last resort for such integrals are Monte Carlo methods, that are generally robust against most difficult features, though not against extremely narrow spikes and true singularities, and work in many dimensions.  The main drawback of Monte Carlo method is that it requires very large number so function samples (up to millions in many dimensions), and that the accuracy  is often not reliably calculated, resulting often in variable and insufficient precisions for likelihood minimization purposes. 

As the problem in likelihood minimization due to multidimensional normalization integrals is often not the precision of the integral per se, but rather the variability of the outcome of repeated calculations at slightly different model parameter values, an explicit regularization strategy can help achieve stability: instead of using the calculation of the numerical integral directly, it is cached in a (low)-dimensional histogram as function of the integrands parameters. In that way, the numeric integrals are then not used directly in likelihood, but rather values taken from a histogram that interpolates integral values between sampled points on a fixed grid in parameter space.  In this mode of operation, the integrals used in the likelihood vary in a smooth and reliable (in the sense of repeatable) way as function of the integrands parameters, resulting in a stable fit, even if the integral does not have the required accuracy. This interpolation strategy can be activated in RooFit with a small intervention at the pdf level, as detailed technically here <X>. The RooFit implementation of cached and interpolated integrals employs a lazy filling strategy, hence no large performance penalty is occurred on the first evaluation of the likelihood.

*Persisting result of expensive (parameterized) numeric integrals*

As the calculation of accurate multi-dimensional numeric integrals (parameterized or not) can be computationally very expensive, it is convenient is such numeric results can be persisted along with the model in a transparent and easy-to-use way. This is possible with 'expensive object cache' of the RooFit workspace. RooFit objects in the workspace may declare expensive (numeric) internal (partial) results as 'expensive' and worth persisting in the 'expensive object cache'. This is done automatically, if a pdf implements this functionality, and ensures that expensive calculations are kept for the lifetime of the workspace in memory.

However the expensive object cache is also persisted as part of the workspace, thus if a pdf in a workspace with expensive integrals is evaluated before the workspace is persisted, this will trigger a filling of the expensive object cache, and the cached results will be persisted along with the pdf. Any user that subsequently opens the workspace will benefit from the persisted results of the expensive calculations. All expensive objects have their dependencies automatically tracked, hence do not invalidate the generality of the model in the workspace. If a (parameter) configuration is entered that does not correspond to the cached result, the expensive object is then recalculated (and cached).

**Likelihood noise from limited IEEE floating point precision and cumulative rounding/truncation effects**

A second common issue with numeric precision in likelihood arises from a mismatch between the IEEE floating format that computers employ, which encode a fixed relative level of precision, and the concept of the (profile) likelihood, where an absolute level of precision is meaningful. 

IEEE double precision floating points consist of a sign bit, a 52-bit 'fraction', a number with approximately 16 decimal digits of precision, and an 11-bit exponent, that encodes the magnitude of the number. If the number stored is O(1), the exponent is :math:`2^0`, and the precision of the fraction component also maps to a 16-digit absolute precision. If the number stored is >>1, the magnitude is absorbed in the exponent part of the number (valued :math:`2^N`, where *N* is chosen such that the fraction is as close as possible to O(1)). The result is that the fraction part will encode the 16  most significant digits of the number and will keep the relative precision of the stored number constant, but the absolute precision will degrade  with the magnitude of the number. For example, a likelihood stored in an IEEE double of O(1) has an absolute stored precision of 10 :math:`^{-16}`, an double of O(10 :math:`^{8}`) has an absolute precision of 10 :math:`^{-8}`, and a double of O(10 :math:`^{16}`) has an absolute precision of O(1).

In all frequentist-style statistical analysis of the likelihood, the fundamental concept underlying parameter estimation, error analysis, confidence interface is the profile likelihood :math:`\Lambda(x|\mu) = \frac{L(x|\mu)}{L(x|\hat{\mu})}`, or equivalently in log-form: :math:`\log(\Lambda(x|\mu)) = \log L(x|\mu) - \log L(x|\hat{mu})`. This quantity is by construction O(1) in the region of interest, since for :math:`mu` equal to :math:`\hat{\mu}` it is zero by definition. The profile likelihood is always interpreted in an absolute sense in frequentists statistics, e.g. a rise of 0.5 absolute unit w.r.t zero of the profile likelihood constitutes the asyptotic frequentist confidence interval on the  parameter mu. Thus for any numeric calculation of the profile likelihood the absolute numeric precision of :math:`\Lambda` is the relevant metric.  In practice ``MINUIT`` needs an absolute precision of the likelihood of O(10 :math:`^{-6}`) to O(10 :math:`^{-8}`) to function reliably. If ``MINUIT`` were to directly minimize the profile likelihood, IEEE storage precision would never be a limiting factor, since it is O(10 :math:`^{-16}`) for number of O(1). 

However, for obvious practical reasons ``MINUIT`` minimizes the likelihood rather the profile likelihood, since the denominator of the profile likelihood,  :math:`L(x|\hat{\mu})` is by definition not known at the beginning of the minimization, since it is the outcome of the operation.  As the absolute value of the likelihood  has no statistical meaning (unlike the profile likelihood), its practical value is unconstrained, and could be large. If it is very large, i.e. larger than :math:`10^8` , needed information on  the absolute precision is lost: less than 8 digits of absolute precision are retained, Once the likelihood is converted to a profile likelihood, it retains that reduced absolute precision, and any analysis done in that interpretation (where it is irrelevant for the precision whether the denominator of the PLL is actually  subtracted or not in ``MINUIT``), may have insufficient precision for ``MINUIT`` to converge. If the likelihood is seen as a monolithic entity, this loss of precision is unavoidable, however in practice, likelihoods are often composite as the result of a joint fit, and some precision information may be rescued with an intervention in the summation process.

Consider the following examples of a composite likelihood that sums the component likelihoods of a signal region that measures parameter of interest :math:`\mu`, and is sensitive to nuisance parameter :math:`\alpha`, and a control region that measures nuisance parameter :math:`\alpha`.

A) If :math:`L_\mathrm{sig}(\mu,\alpha)` is O(1) and :math:`L_\mathrm{ctl}(\alpha)` is O(1), then the :math:`L_\mathrm{total}(\mu,\alpha)` is O(1) and there is no issue concerning precision.
B) If :math:`L_\mathrm{sig}(\mu,\alpha)` is O(1) and :math:`L_\mathrm{ctl}(\alpha)` is O(10 :math:`^{10}`), then the :math:`L_\mathrm{total}(\mu,\alpha)` is O(10 :math:`^{10}`) and important precision is lost in the total.
C) If :math:`L_\mathrm{sig}(\mu,\alpha)` is O(10 :math:`^{10}`) and :math`:L_\mathrm{ctl}(\alpha)` is O(10 :math:`^{10}`), then the :math:`L_\mathrm`total(\mu,\alpha)` is O(10 :math:`^{10}`) and important precision is lost in the total.

Scenarios *A* and *C* require little discussion: in scenario A nothing needs to be done as there is no problem, whereas in scenario C nothing can be done, as the required precision is already lost at the component level. The interesting case is  scenario *B*: here information lost at the level where component likelihoods are combined with fixed relative precision, e.g.

.. math::

   1,000000XXXXXXXXXX + 1.000.000.001,0000000 = 1.000.000.002,000000X 

If instead of combining the component likelihoods, as shown above, they are combined, after an appropropriately chosen offset is first substracted from each of them

.. math::

    L_\mathrm{total} = ( L_\mathrm{sig} - L_\mathrm{sig offset})  + ( L_\mathrm{ctl} - L_\mathrm{ctl offset})

i.e.

.. math::

  ( 1,000000XXXXXXXXXX - 1)  + ( 1.000.000.001,0000000 - 1.000.000.001) = 2,000000XXXXXXXX 

then the precision of the first likelihood is retained in the summed likelihood. Note that some of the precision of the 2nd (large-valued) control region likelihood remains lost, but that is unavoidable since it was never available at the level of likelihood combinations. The net result is that such a per-component likelihood offsetting prior to combining component likelihood salvages the numeric precision of the small-valued component  likelihoods , or conversely,  that adding large-valued likelihood components - that intrinsically have poor absolute precision - do not deterioriate the information contained in other likelihood components in a combination.

Summation with likelihood offsetting is conceptually similar to combining profile likelihoods, with the important distinction that for the issue of numeric precision retention it is not important that the subtracted offset corresponds to the precise minimum of the likelihood, but only has the same order of magnitude. Such order-of-magnitude approximations of the likelihood of each component can be obtained in practice by using the likelihood values at the starting point of the minimization process.    

*When is likelihood offsetting applied?*

For reasons of backward compatibility, likelihood offsetting is not applied by default in RooFit, It can be activated by adding ``Offset(kTRUE)`` to ``RooAbsReal::createNLL()`` or ``RooAbsReal::fitTo()``, and users are recommended to do this for all non-trivial fits.

RooFit deploys further strategies to  limits loss of numeric precision inside component likelihoods. As a component likelihood consist of repeated additions of per-event likelihoods to the running sum, loss of precision may occur due to cumulative rounding effects in the repeated addition. The cumulative error of such repeated regular additions scales with :math:`\sqrt{\mathrm{n}_\mathrm{Event}})` for  adding random numbers, but can be proportional to :math:`\mathrm{n}_\mathrm{Event}` in a worst-case scenario. Instead if regular summation inside likelihoods the Kahan summation procedure is used, which keeps a second double precision number that tracks a running compensation offset,  and results in a maximal loss of precision that is small and independent of :math:`\mathrm{n}_\mathrm{Event}`.

*When is Kahan summation applied?*

Kahan summation is always applied when likelihoods are repeatedly summed.