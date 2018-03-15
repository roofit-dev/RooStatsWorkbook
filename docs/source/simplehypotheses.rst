Statistical tests with simple hypotheses for counting data 
###########################################################

*"What do we mean with probabilities?*

The central concept in all statistic inference is the
probability model, which assigns a probability to each possible
outcome of an experiment. A well known and simple example in particle
physics is the Poisson model, describing the outcome of a counting
experiment,

.. math::

  P(N|\mu) = \frac{\mu^N e^{-\mu}}{N!},

defining the probability for an observation of :math:`N` counts
for a random process measured in a fixed time interval, where :math:`\mu`
events are expected on average.  Poisson distributions describe a
multitude of physics processes including radiactive decay and any
particle physics counting experiment that analyses data taken in a
fixed time interval. As the Poisson describes the distribution of
possible outcomes of counting analysis with *any* type of event selection,
independent on the complexity of the selection, it is by far the most
common statistical model in particle physics. Given an expected event
count :math:`\mu` [#]_ the Poisson distribution fully specifies the probability of
each possible outcome of a counting experiment.

A counting experiment example
-----------------------------

For a given hypothetical physics measurement in which, on average, 3
background events and 4 signal events are expected, Figure shows the
Poisson probability distributions for the background-only hypothesis
(:math:`\mu=3`) and the signal-plus-background hypothesis (:math:`\mu=7`). Note
that the probabilities assigned by each Poisson model are strictly
speaking conditional on the assumed hypothesis: suppose we observe 7
counts in our experiment, then the probability for that outcome
depends on the assumed hypothesis (background-only, or
signal-plus-background).

.. math::

  L(N|H_{\rm bkg}) = 0.022 (\mu=3) 

  L(N|H_{\rm sig+bkg}) = 0.149 (\mu=7)


The probability of the observed data under a given hypothesis, :math:`P({\rm data}|H)` as shown above, is called the
*Likelihood* and conventionally denoted with the symbol :math:`\mathcal{L}`. The observation :math:`N=7` is thus more likely under the :math:`S+B` hypothesis than under the :math:`B` hypothesis.
But is this what we want to know? Or would we rather know :math:`P(H_{i}|N=7)`, the probability of each hypothesis given the observation of 7 counts?
It turns out we don't have enough information to calculate :math:`P(H_{i}|N=7)` from :math:`L(N=7|H_{i})`.
The relation between probabilities with inverted conditionalities is given by Bayes theorem

.. math::

  P(H|{\rm data}) = L({\rm data}|H) \cdot \frac{P(H)}{P({\rm data})}.

Thus we need to know the probabilities :math:`P(H)` and :math:`P({\rm data})` to be able to calculate :math:`P(H_{i}|N=7)` from :math:`L(N=7|H_{i})`.
Here, :math:`P({\rm data})` is the probability of the data under *any* hypothesis. If only two hypotheses are considered, as is done here, then :math:`P({\rm data})` can be expressed as

.. math::

  P({\rm data}) = L({\rm data}|H_{S+B})\cdot P(H_{S+B}) + L({\rm data}|H_{B})\cdot P(H_{B})

applying to law of total probability. Inserting Eq.tprob in Eq.bayes1 gives

.. math::

  P(H_i|{\rm data}) = \frac{ L({\rm data}|H_i) \cdot P(H_i)}{L({\rm data}|H_{S+B})\cdot P(H_{S+B}) + L({\rm data}|H_{B})\cdot P(H_{B})},

where :math:`i` can be :math:`B` or :math:`S+B`. To be able to answer the question on what the value of :math:`P(H_{i}|N=7)` is, the question
of what the probabilities :math:`P(H_i)` are then remains: these are the probabilities assigned to either hypothesis *prior* to the experiment. These prior probabilities can be based on earlier measurements, or can generically be considered to be a prior belief in the theory.
Suppose our prior belief in :math:`H_{S+B}` and :math:`H_{B}` is equal, i.e. :math:`P(H_{S+B})=P(H_{B})=0.5`, we can then calculate

.. math::

  P(H_{S+B}|N=7) =  \frac{ L(N=7|H_{S+B}) \cdot P(H_{S+B})}{L(N=7|H_{S+B})\cdot P(H_{S+B}) + L(N=7|H_{B})\cdot P(H_{B})}
                =  \frac{ 0.148 \cdot 0.50}{0.149 \cdot 0.50 + 0.022 \cdot 0.5} = 0.87.

Thus the observation :math:`N=7` strengthens the belief in the :math:`H_{S+B}` hypothesis from 0.50 to 0.87, at the expense
of the belief in the :math:`H_{B}` hypothesis, which is reduced to 0.13.

The interpretation of probabilities
-----------------------------------

In the discussion so far probabilities assigned to experimental
outcomes and to theories have both been used, even though they are
conceptually different. Probabilities of observed data can always be
interpreted as the fraction of outcome in repeated future experiment,
i.e. :math:`P(N=7|H_{S+B})=0.14` is interpreted as "in 14\% of all future
repeated identical experiments we expect the outcome :math:`N=7`. This
frequency-based interpretation of probability is the basis of the
classical, or frequentist school of statistics. In the frequentist
framework no probabilities can be assigned to theories as there is no
concept of repetition for hypotheses. The Bayesian school of
statistics on the other hand defines probabilities as a degree of
belief, that can also be assigned to hypotheses. As the Bayesian definition
of probability no rule-based definition as the frequentist notion does,
the probabilities are inherently subjective, although there is large effort
in the statistical community to define rule-based prior probabilities that
aim to reduce subjective aspects of Bayesian inference.

The different notions of probability are reflected in the type of
statements that are made in statistical inference. In the frequentist
framework constants of natures are fixed (the Higgs boson either
exists or it doesn't), and no probabilities can be assigned to
these. Frequentist statements are thus restricted to probabilities on
data. In the Bayesian framework probabilities are assigned to
constants of nature (the top quark mass has a 68\% probability to be
in the interval :math:`172.2 \pm 0.7` GeV). As the ultimate goal of any
experiment is to make statements on a theory, the choice of the
Bayesian of Frequentist framework is largely on the decision at what
level to communicate the (numeric) experimental results that form the
basis of decision. In the frequentist paradigm probabilities of data
are communicated with an objective definition, that can be used for
further (subjective) decision making in a later stafe. In the Bayesian
paradigm, prior probabilities are inevitable included in the
communicated numeric result, and thus communicate a message that
contains more (subjective) information than the pure result of the
experiment, and give more guidance on the conclusions that should be
drawn from the data.

In this context it is intructive to compare the formulation of evidence
for discovery of a new particle in both frameworks. In the Bayesian framework
evidence for a hypothesis is case as an odds ratio. The ratio of probabilities
prior to the experiment defines the prior odds ratio

.. math::

 O_{\rm prior} = \frac{P(H_{S+B})}{P(H_{B})} = \frac{P(H_{S+B})}{1-P(H_{S+B})} 

The posterior odds ratio is defined as the ratio of posterior probabilities,
calculated using Eq~\ref{eq:bayes1}, where the denominators cancel in the ratio,

.. math::

 O_{\rm posterior} = \frac{L({\rm data}|H_{S+B})P(H_{S+B})}{L({\rm data}|H_{B})P(H_{B})} = \frac{L({\rm data}|H_{S+B})}{L({\rm data}|H_{B})}\cdot O_{\rm prior}.

The posterior odds ratio can be factorized as the prior odds ratio
multiplied with the so-called Bayes factor that contains the
experimental information, as shown above.  For example, for equal prior
odds and an observation :math:`L({\rm data}|H_{B})=10^{-7}` and :math:`L({\rm data}|H_{S+B})=0.5` the posterior odds ratio becomes 2.000.000:1 in
favor of the S+B hypothesis. 

In the frequentist paradigm we restrict ourselves to a statement the
probability of the observed data, :math:`L({\rm data}|H_{B})=10^{-7}` and
:math:`L({\rm data}|H_{S+B})=0.5` and no notion of prior probabilities on
the hypotheses exists, and it is these numbers that constitute final
numeric statement.  Traditionally, the conclusion that hypothesis B is
ruled out is based on the observation of a very small value of :math:`P({\rm data}|H_{B})` and a not-so-small value of :math:`P({\rm data}|H_{S+B})`,
and that therefore the signal in the S+B hypothesis is considered
'discovered'. No formal rules exist to define a discovery threshold,
but probality of less than :math:`2.87 \cdot 10^{-7}`, corresponding to the
probability of a :math:`\ge 5 \sigma` fluctuation of a unit Gaussian, is
traditional considered the threshold for discovery.

In the discussion of discovery threshold one should keep in mind that
the probabilistic statement is often only one of the ingredients in
the declaration of a discovery: For example for the Higgs boson
discovery a :math:`5 \sigma` observation was accepted as sufficient
evidence, given that the underlying theory was well accepted, whereas
much stronger statistical evidence for superluminuous neutrinos was
rejected (in retrospect rightfully so), on the basis that they
underlying theory was highly implausible, and that a mistake in the
experimental analysis was more plausible.

The choice for a Bayesian or Frequentist interpretation of
probabilities has a history of long-running discussion in particle
physics. Nowadays most particle physics results are reported in the
frequentist paradigm, whereas most other science displines use the
Bayesian framework. The bulk of this lecture will focus on the
construction of likelihood models, which form the basis of both
methods. In the discussion of statistical inference methods
frequentist methods are discussed in most detail, with the motivation
that these are most relevent for todays particle physics students,
while highlighting salient differences with Bayesian techniques
when applicable.

.. [#] which of course will depend on details of the event selection criteria