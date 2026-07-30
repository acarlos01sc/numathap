# Numerical Integration Using Gauss-Legendre and Gauss-Kronrod (GK15)

## 1. The Problem

We want to numerically compute a definite integral

$$
I=\int_a^b f(x)\,dx.
$$

When the antiderivative of $f(x)$ is unknown or difficult to compute, numerical quadrature methods approximate the integral using evaluations of the function at carefully selected points.

Unlike Newton-Cotes formulas (such as the Trapezoidal or Simpson's Rule), which evaluate the function at equally spaced points, **Gauss-Legendre quadrature chooses both the evaluation points and their associated weights in an optimal way**, achieving much higher accuracy with the same number of function evaluations.

---

## 2. Gauss-Legendre Quadrature

The Gauss-Legendre quadrature is first defined on the standard interval

$$
[-1,1].
$$

The integral is approximated by

$$
\int_{-1}^{1}f(x)\,dx
\approx
\sum_{i=1}^{n}w_i\,f(x_i),
$$

where

- $x_i$ are the **Gauss nodes**;
- $w_i$ are the corresponding **Gauss weights**.

Both the nodes and the weights are chosen so that the quadrature is **exact for every polynomial of degree at most**

$$
2n-1.
$$

This property makes Gauss quadrature one of the most efficient numerical integration methods available.

For example,

| Number of points | Exact for polynomials up to degree |
|-----------------:|-----------------------------------:|
| 1 | 1 |
| 2 | 3 |
| 3 | 5 |
| 4 | 7 |
| 7 | 13 |

Thus, a 7-point Gauss rule integrates **every polynomial of degree 13 or less exactly**.

---

## 3. Gauss Nodes

The Gauss nodes are not chosen arbitrarily.

For the Gauss-Legendre quadrature, they are the roots of the Legendre polynomial

$$
P_n(x).
$$

For example, when $n=2$,

$$
P_2(x)=\frac12(3x^2-1),
$$

whose roots are

$$
x_1=-\frac1{\sqrt3},
\qquad
x_2=\frac1{\sqrt3}.
$$

The corresponding weights are

$$
w_1=w_2=1.
$$

Therefore,

$$
\int_{-1}^{1}f(x)\,dx
\approx
f\!\left(-\frac1{\sqrt3}\right)
+
f\!\left(\frac1{\sqrt3}\right).
$$

Notice that the evaluation points are located **inside the interval**, not necessarily at its endpoints.

---

## 4. Integration Over an Arbitrary Interval

Most practical integrals are defined over an arbitrary interval

$$
[a,b].
$$

To use Gauss-Legendre quadrature, we first transform the interval into

$$
[-1,1]
$$

using the linear change of variables

$$
x=
\frac{a+b}{2}
+
\frac{b-a}{2}t,
$$

where

$$
t\in[-1,1].
$$

Since

$$
dx=\frac{b-a}{2}dt,
$$

we obtain

$$
\int_a^b f(x)\,dx
=
\frac{b-a}{2}
\int_{-1}^{1}
f\!\left(
\frac{a+b}{2}
+
\frac{b-a}{2}t
\right)
dt.
$$

Applying the Gauss-Legendre rule yields

$$
\boxed{
\int_a^b f(x)\,dx
\approx
\frac{b-a}{2}
\sum_{i=1}^{n}
w_i
f\!\left(
\frac{a+b}{2}
+
\frac{b-a}{2}x_i
\right)
}
$$

This is the formula implemented by most numerical integration libraries.

---

## 5. The Main Limitation of Pure Gauss Quadrature

A Gauss rule produces a highly accurate approximation of the integral, but it does **not** directly provide an estimate of its own error.

For adaptive numerical integration, an estimate of the error is essential, since it determines whether the current interval is sufficiently accurate or must be subdivided.

The Gauss-Kronrod construction was developed precisely to address this limitation.

---

# Numerical Integration Using the Gauss-Kronrod 15-Point Rule (GK15)

## 6. The Fundamental Idea Behind Gauss-Kronrod

Suppose we compute a 7-point Gauss approximation,

$$
G_7.
$$

Rather than computing a second approximation independently, Kronrod proposed adding additional nodes to the existing Gauss rule.

The resulting rule uses

- the original 7 Gauss nodes;
- 8 additional nodes.

This produces a new quadrature with

$$
15
$$

evaluation points,

usually denoted

$$
K_{15}.
$$

The important property is that **all Gauss nodes are also Kronrod nodes**.

Therefore, the function values already computed for the Gauss rule are reused.

Only the additional nodes require new evaluations.

---

## 7. Two Approximations from the Same Function Evaluations

After evaluating the function at the fifteen Kronrod nodes, two approximations become available:

The Gauss approximation

$$
G_7,
$$

and the Kronrod approximation

$$
K_{15}.
$$

Because the Kronrod rule has a higher algebraic degree of precision, it is generally much more accurate.

The difference

$$
|K_{15}-G_7|
$$

provides an estimate of the integration error.

This estimate is obtained **without performing a second independent quadrature**, making the method remarkably efficient.

---

## 8. Estimating the Error

The estimated integration error is taken as

$$
E
\approx
|K_{15}-G_7|.
$$

If

$$
E<\varepsilon,
$$

where $\varepsilon$ is the prescribed tolerance, the interval is considered sufficiently accurate and the algorithm returns

$$
K_{15}
$$

as the approximation of the integral.

Otherwise, the interval must be subdivided.

---

## 9. Adaptive Subdivision

Suppose the initial interval is

$$
[a,b].
$$

We first compute

$$
K_{15}(a,b)
$$

together with the corresponding error estimate.

If the estimated error exceeds the tolerance,

$$
E>\varepsilon,
$$

the interval is divided at its midpoint,

$$
m=\frac{a+b}{2},
$$

producing

$$
[a,m]
$$

and

$$
[m,b].
$$

The GK15 rule is then applied independently to each subinterval,

$$
K_{15}(a,m)
$$

and

$$
K_{15}(m,b).
$$

Each subinterval receives its own error estimate.

Only the intervals whose estimated error remains too large are subdivided again.

This recursive process continues until every subinterval satisfies the prescribed tolerance.

---

## 10. Why Adaptive Gauss-Kronrod Is Efficient

The adaptive strategy is based on a simple observation:

> **Smooth regions require few subdivisions, while difficult regions are refined automatically.**

Consequently,

- smooth portions of the interval are integrated using only a few evaluations;
- rapidly varying regions receive additional subdivisions;
- computational effort is concentrated only where necessary.

This makes the adaptive Gauss-Kronrod algorithm significantly more efficient than uniformly refining the entire interval.

---

## 11. Advantages of GK15

Compared with classical adaptive Simpson integration, the Gauss-Kronrod 15-point rule offers several advantages:

- higher-order quadrature on each interval;
- highly accurate error estimation obtained from the same set of function evaluations;
- efficient reuse of Gauss evaluations;
- excellent performance for smooth functions;
- automatic adaptive refinement based on local error estimates.

For these reasons, the Gauss-Kronrod family of quadrature rules forms the basis of many modern scientific computing libraries, including QUADPACK, GSL, Boost.Math, SciPy, and numerous commercial numerical analysis packages.