#################
Statistical Tests
#################

simple hypotheses for counting data 
===================================


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
calculated using Eq ref bayes1, where the denominators cancel in the ratio,

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



simple hypotheses for distributions}
====================================

*"p-values"*

Most particle physics analyses are not simple counting experiments,
but study one or more observable distributions that allow to discriminate
signal and background.

Probability models for distributions
------------------------------------

To deal with distribution in statistic inferences, we must first
construct a probability model for distributions. In some cases, the
distributions for observable quantities can be derived from the
physics theory from first principles, resulting in analytically
formulated distributions.  In most cases in todays experiments, and in
particular at the LHC, predicted distributions for observable
quantities are derived from a chain of physics and detector
simulations. The output of such simulations is histogram of simulated
in events in the observable quantity. An example of such an MC simulation
prodiction for a fictious signal and background process is shown 
in Figures binnedPdf. 

While the histograms with simulated signal and background events effectively
describe a distribution, the statistical model for such a binned distribution
is effectively a series of counting experiments that can be described with
a Poisson distribution for each bin

.. math::

   L(\vec{N}|H_{B}) &=& \prod_{i} {\rm Poisson}(N_i|\tilde{b}_i) \label{eq:La}

   L(\vec{N}|H_{S+B}) &=& \prod_{i} {\rm Poisson}(N_i|\tilde{s}_i+\tilde{b}_i) \label{eq:Lb},

where :math:`\tilde{b}_i` and :math:`\tilde{s}_i` are the predicted event counts
for the background and signal process in bin :math:`i` respectively.

Statistical inferences with probability models for distributions
----------------------------------------------------------------

How does the fact that observation is a distribution change
statistical inference?  In the Bayesian paradigm, the likelihoods of
Eq ref La, ref Lb can simply be plugged into
Eq ref bayes2, and all further statistical inference procedures
are unchanged. The frequentist calculation of :math:`L(\vec{N}|H_{B})` also
remains unchanged, but raises the question if the probability of the
observed data is still relevant when drawing conclusions on the
hypotheses considered: :math:`L(\vec{N}|H_{B})` is the probability to
observe *the precise (binned) distribution of data that was
recorded*. That is usually not what we are interested in. We are
interested in the probability to observe this, or any 'similar'
dataset, e.g. with a few statistical fluctuations w.r.t to the
observed data that correspond to the same signal event count, or larger. 
To introduce a precise, unambiguous notion, of what 'more
signal' (or more generically 'more extreme' in any sense) means in the
context of statistical inference, a *test statistic* is introduced
in frequentist inference.

Ordering results by extremity, test statistics and p-values
-----------------------------------------------------------

A test statistic is, generically speaking, *any* function :math:`T(x)`
of the observable data :math:`x`.  The goal of a test statistic is that it
orders all possible observations :math:`x` by extremity: :math:`T(x)>T(x')` means
that the observation :math:`x` is more extreme than observation :math:`x'`.
For example, for a Poisson counting experiment, the trivial choice :math:`T(x)=x`
defines a useful test statistic that orders all possible observation
by extremity as more observed events means more signal for a counting experiment.
With the notion of ordering possible outcomes by extremity, comes the
concept of :math:`p`-values. A :math:`p`-value is the probability to obtain the
observed data, *or more extreme*, in future repeated experiments.
For example, for the probability to observe 7 counts or more
for a Poisson counting experiment with the background hypothesis of the previous example (:math:`\mu=3`)  is

.. math::

   p(H_{B})= \sum_{N=7}^{\infty} {\rm Poisson}(N|\mu=3) = 0.23

A :math:`p`-value is always specific to the hypothesis under which
it is evaluated.  When no specification is given, it usually refers
the to null-hypothesis, which is for discovery-style analyses the
background-only hypothesis.

When the observed data is a distribution, rather than event count, the
choice of :math:`T(x)=x` will no longer work.  We need a test statisticl to
quantity if one (multi-dimensional) histogram of observed data
:math:`\vec{N}` is more extreme than another one. A useful test statistic
for distribution is the likelihood ratio test statistic

.. math::

   \lambda(\vec{N})=\frac{L(\vec{N}|H_{S+B})}{L(\vec{N}|H_{B})}

One can intuitively see that :math:`\lambda(\vec{N})` orders datasets
according to signal extremity: For a dataset :math:`N_S` that is very signal-like
:math:`L(\vec{N_S}|H_{S+B})` will be large, since the data is probable under this hypothesis,
and :math:`\vec{N_S}|H_{B})` will be small, since the data is improbable under this hypothesis,
hence the ratio will be large. Conversely for a dataset :math:`N_B` that is very background-like
:math:`L(\vec{N_B}|H_{S+B})` will be small, since the data is probable under this hypothesis,
and :math:`L({\vec{N_B}}|H_{B})` will be large, since the data is improbable under this hypothesis,
hence the ratio will be large. 

With a likelihood-ratio test statistic, frequentist :math:`p`-values can be
calculated for observable data distributions or arbitrary complexity as the test
statistic :math:`T(\vec{x})` maps *any* dataset :math:`x` into a single number :math:`T(x)`,
reducing the :math:`p`-value calculation to an integral over the expected test statistic
distribution under a given hypothesis

.. math::

   p = \int_{T(\vec{x})_{\rm obs})}^{\infty} f(T|H_{i}) dT

where :math:`f(T|H_{i})` is the expected distribution of values of the test statistic :math:`T`
under the hypothesis :math:`H_i`. Note that the Poisson example of Eq ref poisT follows from
the general form of Eq ref Tdist with the choice :math:`T(N)=N` and :math:`H_i = {\rm Poisson}(\mu=3)`,
where integration was replaced with a summation because of the integer nature :math:`T(N)=N`.
Figure ref tsdist illustrates the concept of the distribution of the test statistic and its relation
to the definition of the p-value.

A practical complication in the calculation of :math:`p`-values for
distribution is that, unlike the Poisson example with :math:`T(x)=x` where
distribution of :math:`T(x)` is known because it simply the Poisson
distribution of :math:`x` itself, the distribution :math:`f(T|H_i)` is generally
*not* known. A simple, but but computionally expensive solution is
the estimate the distribution :math:`f(T|H_i)` from toy Monte Carlo
simulation: a histogram of the :math:`T(x)` values from ensemble of toy
datasets :math:`x` drawn from the hypothesis :math:`H_i` will approximate the
distribution :math:`f(T|H_i)`. For certain choices of :math:`T(x)` analytical distributions
are known under asymptotic conditions, and will be discussed in Section ref composite

While not discussed further in these lecture notes, for situations
where analytical prescriptions are known for the distribution of
observable quantities :math:`x`, the concept of a probability model can be
extended into the concept of a probability density model :math:`f(x)` where
:math:`\int f(x) dx \equiv 1` and the definite integral :math:`\int_a^b f(x) dx`
represents the probability to observe an event in the observable range :math:`a<x<b`.
All of the statistical inference techniques discussion in this section
can be identically executed using such probability density function instead
of probability models.
