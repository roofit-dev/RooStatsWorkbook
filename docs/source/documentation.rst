#######################
Usage and Documentation
#######################

This section discusses the specific tools for recommended for common statistical tasks. In addition, the document identifies requirements that the tools need to meet regarding documentation, testing, deployment and other current short comings that will become goals/deliverables for the (ATLAS) statistics forum.

Statistical Modeling with RooFit
================================
The first step in any statistical method is to specify the statistical model being used to describe the 
data - schematically :math:`f` (data|parameters). This statistical model can be used to generate pseudo-data (toy
Monte Carlo) for a specific parameter point and with specific observed data it defines the likelihood
function :math:`L` (parameters) :math:`\equiv f` (observed data|parameters). A properly defined statistical model can be used 
in conjunction with any statistical method; however, several tools (`BAT`, `MCLimit`, `Collie`, `BILL`, etc.)
provide both the modeling stage and a specific statistical method. The `RooFit` and `RooStats` projects
are designed to separate these two distinct stages; where RooFit provides the modeling language and
`RooStats` provides the statistical tests.

Statistical models implemented within the RooFit framework can be serialized (written) to `ROOT`
files – often referred to as workspaces – for subsequent use by RooStats tools or other packages such as
BAT. Given the successful use of `RooFit` for the Higgs discovery and wide use within the SUSY group,
we see it as the primary tool for statistical modeling.

In addition to the low-level classes that RooFit provides for statistical modeling, there are higher-level
tools (factories) such as `RooWorkspace::factory`, `HistFactory`, `HistFitter`, and other analysis-specific tools that produce RooFit models as an output. Analysis specific tools that produce `RooFit`
workspaces are comparable with these recommendations, though if the statistical model can be implemented with one a more general-purpose high-level tool that is encouraged. For example, `HistFactory`
is a fairly general purpose tool for histogram-based analyses that is well optimized and widely used.
`The HistFitter tool`, which uses HistFactory for modeling and provides additional functionality,
is similarly encouraged.

Improvements Needed/Planned
---------------------------

The core modeling framework of RooFit is quite mature, though a number of optimizations are still planned.

Frequentist
===========

Methodology
-----------

Upper-Limits
------------

Improvements Needed/Planned
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Background-only p-values for Searches
-------------------------------------

Measurements and Confidence Intervals / Parameter Contours
-----------------------------------------------------------

Improvements Needed/Planned
~~~~~~~~~~~~~~~~~~~~~~~~~~~


Bayesian
========

Methodology
-----------

Prior
-----

Sampling and MArginalization Tools
----------------------------------

Improvements Needed/Planned
~~~~~~~~~~~~~~~~~~~~~~~~~~~


Diagnostic Tools
================