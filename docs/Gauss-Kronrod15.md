# Numerical Integration Using Gauss-Legendre and Gauss-Kronrod (GK15)

## 1. The Problem

We want to numerically compute a definite integral

$$
I=\int_a^b f(x)\ dx.
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
\int_{-1}^{1}f(x)\ dx
\approx
\sum_{i=1}^{n}w_i\ f(x_i),
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
f\left(-\frac1{\sqrt3}\right)
+
f\left(\frac1{\sqrt3}\right).
$$

Notice that the evaluation points are located **inside the interval**, not necessarily at its endpoints.

---

## 4. Computing the Gauss Weights

Once the Gauss nodes have been determined as the roots of the Legendre polynomial

$$
P_n(x),
$$

the corresponding weights are uniquely determined.

A remarkable result from the theory of orthogonal polynomials shows that the weights are given by

$$
\boxed{
w_i=
\frac{2}
{\left(1-x_i^2\right)
\left[P_n'(x_i)\right]^2}
}
$$

where

- $x_i$ is the $i$-th root of $P_n(x)$;
- $P_n'(x)$ denotes the derivative of the Legendre polynomial.

Therefore, constructing an $n$-point Gauss-Legendre rule requires only two steps:

1. compute the roots of $P_n(x)$;
2. evaluate the derivative of $P_n(x)$ at each root and apply the formula above.

As an example, for

$$
n=2,
$$

the Legendre polynomial is

$$
P_2(x)=\frac12(3x^2-1),
$$

whose derivative is

$$
P_2'(x)=3x.
$$

Since the roots are

$$
x_1=-\frac1{\sqrt3},
\qquad
x_2=\frac1{\sqrt3},
$$

we have

$$
P_2'(x_i)=\pm\sqrt3.
$$

Substituting into the weight formula,

$$
w_i=\frac{2}{\left(1-\frac{1}{3}\right)(3)}=1
$$

which agrees with the previously stated result,

$$
w_1=w_2=1.
$$

In practice, numerical libraries do not symbolically solve the Legendre polynomial. Instead, the roots are computed numerically (typically by Newton's method), after which the weights follow directly from the formula above.

---

## 5. Integration Over an Arbitrary Interval

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
\int_a^b f(x)\,dx=
\frac{b-a}{2}
\int_{-1}^{1}
f\left(
\frac{a+b}{2}+\frac{b-a}{2}t
\right)dt
$$

Applying the Gauss-Legendre rule yields

$$
\boxed{
\int_a^b f(x)\,dx
\approx
\frac{b-a}{2}
\sum_{i=1}^{n}
w_i
f\left(
\frac{a+b}{2}
+
\frac{b-a}{2}x_i
\right)
}
$$

This is the formula implemented by most numerical integration libraries.

---

## 6. The Main Limitation of Pure Gauss Quadrature

A Gauss rule produces a highly accurate approximation of the integral, but it does **not** directly provide an estimate of its own error.

For adaptive numerical integration, an estimate of the error is essential, since it determines whether the current interval is sufficiently accurate or must be subdivided.

The Gauss-Kronrod construction was developed precisely to address this limitation.

---

# Numerical Integration Using the Gauss-Kronrod 15-Point Rule (GK15)

## 7. The Fundamental Idea Behind Gauss-Kronrod

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

## 8. How the Kronrod Nodes and Weights Are Constructed

The additional Kronrod nodes are **not chosen arbitrarily**, nor are they the roots of another Legendre polynomial.

Instead, they are obtained by solving a much more difficult mathematical problem.

The objective is to construct a new quadrature rule that

- preserves every Gauss node;
- inserts additional nodes between them;
- determines a new set of weights for all nodes;
- achieves the highest possible algebraic degree of precision.

More precisely, if the original Gauss rule uses

$$
n
$$

nodes, Kronrod proved that it is possible to build a rule containing

$$
2n+1
$$

nodes while retaining all the original Gauss nodes.

The new nodes are the roots of a polynomial of degree

$$
2n+1,
$$

called the **Kronrod polynomial**.

Unlike the Legendre polynomial, however, the Kronrod polynomial is **not available in closed form** and must be constructed numerically.

Once the complete set of nodes has been obtained, the corresponding weights are determined by imposing the quadrature condition

$$
\int_{-1}^{1}p(x)\,dx
=
\sum_{i=1}^{2n+1}
w_i\,p(x_i)
$$

for every polynomial

$$
p(x)
$$

up to the maximum attainable degree.

This leads to a system of equations whose solution uniquely determines all the Kronrod weights.

Because this construction is considerably more involved than that of the Gauss rule, numerical libraries almost never compute the Kronrod nodes and weights at runtime.

Instead, the nodes and weights for commonly used rules (GK15, GK21, GK31, GK41, GK51 and GK61) are precomputed with very high precision and stored as constant tables.

---

## 9. Two Approximations from the Same Function Evaluations

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

## 10. Estimating the Error

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

## 11. Adaptive Subdivision

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

## 12. Why Adaptive Gauss-Kronrod Is Efficient

The adaptive strategy is based on a simple observation:

> **Smooth regions require few subdivisions, while difficult regions are refined automatically.**

Consequently,

- smooth portions of the interval are integrated using only a few evaluations;
- rapidly varying regions receive additional subdivisions;
- computational effort is concentrated only where necessary.

This makes the adaptive Gauss-Kronrod algorithm significantly more efficient than uniformly refining the entire interval.

---

## 13. Advantages of GK15

Compared with classical adaptive Simpson integration, the Gauss-Kronrod 15-point rule offers several advantages:

- higher-order quadrature on each interval;
- highly accurate error estimation obtained from the same set of function evaluations;
- efficient reuse of Gauss evaluations;
- excellent performance for smooth functions;
- automatic adaptive refinement based on local error estimates.

For these reasons, the Gauss-Kronrod family of quadrature rules forms the basis of many modern scientific computing libraries, including QUADPACK, GSL, Boost.Math, SciPy, and numerous commercial numerical analysis packages.