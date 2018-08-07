*******************************************
Tools for Model Building and Good Practices
*******************************************

This section discusses the specific tools for recommended for building valid and robust statistical models. The RooFit pacakge is core to data modeling in ROOT. 

We begin with a discussion of the RooFit philosophy - we want a single language for all measurements: common tools & joint model building (other tools) followed by a description of the RooFit language basics, and the workspace. A series of tools for building & manipulating models built using RooFit as a foundation are described with some links and general usage. Finally a list of good practices are presented regarding issues such as modeling systematics, uncertainties, and model cleaning among others. 

.. include:: docs-roofit1.rst
.. include:: docs-roofit2.rst
.. include:: docs-roofit3.rst

Good Practices
==============

Good practices in model building
--------------------------------
- control and validation regions [ Exo ]

Modeling systematics - Correlations
-----------------------------------
- Correlation modeling (prior and observed) [ Exo, Guide, some new material needed ]

Modeling systematic uncertainties - shape vs rate
-------------------------------------------------
[ Exo, WV slides ]

Modeling systematics - 2point discussion
----------------------------------------
[ Guide to PLL but needs updating ]

Modeling systematcs - pruning and smoothing
-------------------------------------------
[ Exo ]

Modeling systematics - DOF
--------------------------
how many DOFs does your conceptual systematic have? [ Guide to PLL ]

Modeling uncertainties with increasing luminosity uncertainty
-------------------------------------------------------------
[ FAQ ]

.. include:: modelcomputation.rst