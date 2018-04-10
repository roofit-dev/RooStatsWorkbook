Improvements Needed/Planned
---------------------------

The core modeling framework of RooFit is quite mature, though a number of optimizations are still planned. In particular, for statistical models based on nominal  and variational histograms, there is are challenges associated to the interpolation between histograms (including the ability to handle simultaneous variation of multiple nuisance parameters) and the handling of Monte Carlo statistical uncertainties in the histograms themselves (which are typically correlated in a non-trivial way among the variational histograms for a particular Monte Carlo sample).  Work on these topics requires a careful dedicated effort, thus it is much more efficient if this effort is coordinated around a common code-base. The Statistics Forum has chosen to organize this effort with the goal of making a HistFactory v2.

There are a number of discussions about the tools being provided by the combined performance groups for evaluating systematics, these topics are outside of the scope of this document.

The issue of merging and "pruning" nuisance parameters from a model given the rapid growth of nuisance parameters we are currently experiencing. This is the topic for a separate document, and suggestions made are expected to propagate into the workplane the Statistics Forum is organizing for improvements to the modeling tools.
