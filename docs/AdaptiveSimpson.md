# Numerical Integration Using the Adaptive Simpson Method

## 1. The Problem

We want to numerically compute a definite integral

$$
I = \int_a^b f(x) dx
$$

When there is no simple antiderivative for $f(x)$, or when we simply want a numerical result, we can approximate the area under the curve using values of $f$.

One of the classical techniques for this purpose is the Simpson's Rule.

## 2. Simpson's Rule

Consider initially a single interval $[a,b]$.

We define the midpoint as

$$
m = \frac{a+b}{2}.
$$

Simpson's Rule approximates the function over this interval by a **parabola** passing through the three points

$$
(a,f(a)), \qquad (m,f(m)), \qquad (b,f(b)).
$$

The approximation of the integral is then given by

$$
S(a,b)
=

\frac{b-a}{6}
\left[
f(a)+4f(m)+f(b)
\right].
$$

### Example

Consider

$$
\int_0^1 x^2,dx.
$$

We have

$$
f(0)=0,\qquad f(0.5)=0.25,\qquad f(1)=1.
$$

Therefore,

$$
S(0,1)
=

\frac{1}{6}
\left[
0+4(0.25)+1
\right].
$$

Thus,

$$
S(0,1)=\frac{2}{3}.
$$

In this case, the result is **exact**, because $x^2$ is already a parabola.

## 3. The Fundamental Idea Behind Adaptive Simpson

The adaptive method is based on a very simple idea:

> **Use few evaluation points where the function is smooth, and refine the interval only where the function becomes more difficult to approximate.**

We begin by applying Simpson's Rule over the entire interval:

$$
S(a,b).
$$

Next, we split the interval in half by computing the midpoint

$$
m=\frac{a+b}{2},
$$

and apply Simpson's Rule independently to each subinterval:

$$
S(a,m)
$$

and

$$
S(m,b).
$$

The combined approximation is

$$
S_2=S(a,m)+S(m,b).
$$

We now have two approximations of the same integral:

* The original approximation:

  $$
  S(a,b)
  $$

* The refined approximation:

  $$
  S(a,m)+S(m,b).
  $$

If the two results are very close, it indicates that the approximation is already sufficiently accurate.

If they differ significantly, it means that the function exhibits behavior requiring further refinement of the interval.

## 4. Estimating the Error

The difference

$$
|S_2-S|
$$

provides an estimate of the error.

For Simpson's Rule, we use the well-known error estimate

$$
E \approx \frac{|S_2-S|}{15}.
$$

Therefore, we consider that the desired accuracy has been achieved when

$$
\frac{|S_2-S|}{15} < \varepsilon,
$$

where $\varepsilon$ is the desired error tolerance.

### Example

Suppose that

$$
|S_2-S| = 1.5\times10^{-7}.
$$

Then the estimated error is

$$
E \approx
\frac{1.5\times10^{-7}}{15}
=

10^{-8}.
$$

If our tolerance is

$$
\varepsilon = 10^{-7},
$$

then the interval can already be considered sufficiently accurate.

## 5. What Happens When the Error Is Too Large?

Suppose that

$$
\frac{|S_2-S|}{15} > \varepsilon.
$$

In this case, the algorithm does not simply increase the number of evaluation points over the entire interval.

Instead, it **subdivides the interval once again**.

Initially, we have

$$
[a,b],
$$

which has already been divided into

$$
[a,m]
$$

and

$$
[m,b].
$$

Each of these subintervals is then analyzed independently.

For example,

$$
[a,m]
;\rightarrow;
[a,m_1]\cup[m_1,m]
$$

and

$$
[m,b]
;\rightarrow;
[m,m_2]\cup[m_2,b].
$$

The same error estimation procedure is applied to each new subinterval. If the estimated error on a subinterval is still larger than the prescribed tolerance, that subinterval is divided again.

This process continues recursively until every subinterval satisfies the desired error tolerance.
