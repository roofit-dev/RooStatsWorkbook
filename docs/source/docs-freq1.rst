
Frequentist
============

Methodology
-----------

The recommended frequentist methodology is based on the profile likelihood ratio as a test statistic with the corresponding modifications made to be appropriate for discovery, 1-sided upper-limits, and measurements~\cite{Cowan:2010js}. The procedure for the mode of search and 1-sided upper-limits based on CLs~\cite{CLs} is further documented in Ref.~\cite{ATLAS:2011tau} and Ref.~\cite{Recommendations}.  The extension of these recommendations for measurement problems (68\% and 95\% confidence intervals in a single parameter or multiple parameters with or without physical boundaries) is described in Ref.~\cite{ExtendedRecommendations}.

Upper-Limits
-------------

In the case of upper limits we start with the statistical model :math:`f(data | \mu, \alpha)`, where :math:`\mu` is the parameter of interests (like a cross-section, signal yield, or signal strength) and :math:`\alpha` are the nuisance parameters.  In most cases, we have the physical boundary :math:`\mu\ge 0`, which motivates the use of the CLs procedure for 1-sided upper-limits in order to avoid the "sensitivity problem" (the possibility of excluding arbitrarily small values of  :math:`\mu` where we have no sensitivity due to downward fluctuations). The profile likelihood ratio :math:`\tilde{q}_\mu` (as defined in Ref.~\cite{Cowan:2010js}) is used as a test statistic. The p-values for the :math:`\mu` (s+b) and :math:`\mu=0` (b-only) hypotheses can be evaluated either with ensembles of pseudo-experiments (toy Monte Carlo) or by using the asymptotic distributions.  Both techniques are non-trivial to code. The  toy Monte Carlo approach which requires dealing with randomizing the global observables associated to nuisance parameters in a frequentist way (as opposed to the mixed frequentist-Bayesian hybrid procedure that is implemented in several  tools) and the ''profile-construction'' for making the multi-dimensional Neyman-Construction computationally tractable~\cite{Chuang:2000tba,Cranmer:2005hi,ATLAS:2011tau,Demortier}.

The RooStats tool ``HypoTestInverter`` performs the "hypothesis test inversion" of the Neyman-Construction, and can be configured to use  either ``FrequentistCalculator`` (toy Monte Caro) or ``AsymptoticCalculator`` (asymptotics) to calculate p-values.  The ``FrequentistCalculator`` can use several test statistics, but for upper-limits one uses the ``ProfileLikelihoodRatioTestStat`` to calculate the 1-sided profile-likelihood ratio test statistic :math:`q_\mu`.
The RooStats tool ``FrequentistCalculator`` implements the fully-frequentist p-value calculation based on toys Monte Carlo with the recommended treatment of global observables and the profile-construction. 

.. toctree::
   :maxdepth: 1

   docs-plr
   docs-cls
   docs-cls_toys

Improvements Needed/Planned
^^^^^^^^^^^^^^^^^^^^^^^^^^^

An improved procedure for the :math:`\pm 1,2\sigma` bands for limits based on the asymptotic formulae have been investigated~\cite{AsymptoticBands}. The key issue there is that :math:`\sigma^2`, the variance of :math:`\mu`, depends on :math:`\mu`, which is a second-order effect, while the original formulation assumed constant :math:`\sigma^2`. The improved procedure was described and used for several Higgs results, but the corresponding code has not yet been ported into the ROOT codebase. This is a work package for the Statistics Forum.  

Various improvements and feature requests  are also work packages for the statistics forum. This includes improved robustness, improved scanning algorithms, and requests for streamlined, user-friendly interfaces to these underlying tools.

Background-only p-values for Searches
-------------------------------------

In the case of searches, we are interested in calculating a background-only p-value. Typically we start with the statistical model :math:`f(data | \mu, \alpha)`, where :math:`\mu` is proportional to the signal cross-section (e.g. a signal yield or signal strength) and :math:`\alpha` are the nuisance parameters. The appropriate test statistic :math:`\tilde{q}_0`  (as defined in Ref.~\cite{Cowan:2010js}), with increasing values indicating more events than expected in the background-only hypothesis, is implemented with ``ProfileLikelihoodRatioTestStat``.  The background-only p-value, denoted :math:`p_0`, can be calculated using toy Monte Carlo with RooStats ``FrequentistCalculator`` or by using the asymptotic formulae with the RooStats ``AsymptoticCalculator``. 
The subtleties associated to the treatment of global observables and nuisance parameters described above in the upper-limit section also apply here.

Measurements and Confidence Intervals / Parameter Contours
----------------------------------------------------------

The extended recommendations in Ref.~\cite{ExtendedRecommendations}. are aimed at measurement problems (68\% and 95\% confidence intervals in a single parameter or multiple parameters with or without physical boundaries). The document is a natural extension on the search/upper-limit recommendations. The primary difference is to change the test statistic to :math:`t_\mu`  (as defined in Ref.~\cite{Cowan:2010js}), whcihis appropriate for measurements instead of 1-sided tests. This test statistic is also implemented with the RooStats ``ProfileLikelihoodRatioTestStat``.  As above, the p-values can be calculated either with asymptotic or toy Monte Carlo; however, there are improvements needed and planned in this case.
 
 
Improvements Needed/Planned
^^^^^^^^^^^^^^^^^^^^^^^^^^^


The ``HypoTestInverter`` currently only supports 1-d problems. The `NeymanConstruction` and `FeldmanCousins` classes support N-D problems (with boundaries), but is not as configurable as the ``HypoTestInverter`` class. The planning in RooStats is to unify these tools.  Note, the RooStats ``FrequentistCalculator`` can calculate p-values using toy Monte Carlo and the recommended treatment of global observables and nuisance parameters even in multi-dimensional cases with complex boundaries -- the missing part is not the p-value calculation, but the scan over the parameter space and the actual hypothesis test inversion. Efficient scanning becomes increasingly important for multidimensional problems.

The ``HypoTestInverter`` can also be configured to use the ``AsymptoticCalculator`` to calculate p-values more quickly. The ``AsymptoticCalculator`` only has the 1-d case with a lower-boundary implemented. The 1-d case with upper- and lower-boundaries has been worked out~\cite{Cowan:2012se} and should be implemented as well.  

For multi-dimensional problems, p-value based on toy MC can become quite time consuming. In many cases the asymptotic approach is sufficiently accurate and much faster. The presence of boundaries modifies the asymptotic distributions; however, in general this depends on the shape of the boundary which means there will be no general formulae. It is possible that one can find a formulae for the asymptotic distribution for simple boundaries (e.g. or :math:`\mu_1>0`, :math:`\mu_2>0`, or :math:`\mu_1>0 \&\& \mu_2>0` ). Neglecting these modifications to the boundary leads to over-coverage and some protection to the sensitivity problem near the boundary similar to CLs, thus the current recommendations are to use the uncorrected :math:`\chi^2_n` distribution and make this clear in the text of the paper. Thus, the tools needed for the asymptotic procedure for this non-calibrated procedure are already in place with RooStats ``ProfileLikelihoodRatioTestStat`` and the standard :math:`\chi^2_n` cutoffs for 68\% and 95\% confidence intervals (and have been used in recent Higgs property papers).

Diagnostics are important for all statistical methods, particularly for complicated problems. 
There are a number tools that have been developed that are in use by the physics groups, 
but these need to migrated into the common code bases (either ROOT or the appropriate ATLAS distribution).

