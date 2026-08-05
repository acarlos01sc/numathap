# Taylor and Maclaurin Series

## 1. The Main Idea

Suppose we know the value of a function at a single point.

For example,

$$
f(0)=1.
$$

With only this information, the best approximation we can make is the constant function

$$
f(x)\approx1.
$$

Now suppose we also know the slope of the function at that point,

$$
f'(0)=2.
$$

We can then construct the tangent line,

$$
f(x)\approx1+2x.
$$

This approximation is much more accurate near

$$
x=0.
$$

If we also know the curvature,

$$
f''(0),
$$

we can improve the approximation further by adding a quadratic term,

$$
f(x)\approx
1+2x+\frac{f''(0)}{2}x^2.
$$

Each successive derivative provides additional information about the local behavior of the function.

The Taylor series is obtained by continuing this process indefinitely.

---

## 2. The General Formula

Suppose a function is infinitely differentiable at a point

$$
a.
$$

Its Taylor series is

$$
\boxed{
f(x)=
\sum_{n=0}^{\infty}
\frac{f^{(n)}(a)}{n!}(x-a)^n.
}
$$

Each term has a geometric interpretation:

* $f(a)$ is the value of the function;
* $f'(a)$ determines its slope;
* $f''(a)$ describes its curvature;
* $f'''(a)$ measures how the curvature changes;
* and so on.

The factorial in the denominator naturally compensates for the repeated differentiation of polynomial terms.

---

## 3. Why Does the Factorial Appear?

Consider the polynomial

$$
x^5.
$$

Differentiating repeatedly,

$$
\begin{aligned}
\frac{d}{dx}x^5 &=5x^4,\
\frac{d^2}{dx^2}x^5 &=20x^3,\
\frac{d^3}{dx^3}x^5 &=60x^2,\
\frac{d^4}{dx^4}x^5 &=120x,\
\frac{d^5}{dx^5}x^5 &=120.
\end{aligned}
$$

Notice that

$$
120=5!.
$$

Every differentiation multiplies the coefficient by the current exponent.

After differentiating a polynomial term exactly

$$
n
$$

times, the accumulated factor is

$$
n!.
$$

Dividing each coefficient by

$$
n!
$$

ensures that the coefficients of the Taylor series recover the original derivatives of the function.

---

## 4. The Maclaurin Series

The Maclaurin series is simply a special case of the Taylor series.

Instead of expanding around an arbitrary point

$$
a,
$$

we choose

$$
a=0.
$$

The formula then becomes

$$
\boxed{
f(x)=
\sum_{n=0}^{\infty}
\frac{f^{(n)}(0)}{n!}x^n.
}
$$

Every Maclaurin series is therefore a Taylor series centered at the origin.

---

## 5. Example: The Exponential Function

Consider

$$
f(x)=e^x.
$$

This function has the remarkable property that every derivative equals the function itself,

$$
f'=f.
$$

Consequently,

$$
f(0)=1,
$$

and every derivative evaluated at the origin is also equal to one,

$$
f^{(n)}(0)=1,
\qquad
n=0,1,2,\ldots
$$

Substituting these values into the Maclaurin formula yields

$$
e^x=
1+x+\frac{x^2}{2!}
+\frac{x^3}{3!}
+\frac{x^4}{4!}
+\cdots
$$

This is one of the most famous power series in mathematics.

---

## 6. Example: The Sine Function

Now consider

$$
f(x)=\sin x.
$$

Its derivatives repeat cyclically,

| Order | Derivative |
| ----: | ---------- |
|     0 | $\sin x$   |
|     1 | $\cos x$   |
|     2 | $-\sin x$  |
|     3 | $-\cos x$  |
|     4 | $\sin x$   |

Evaluating these derivatives at

$$
x=0
$$

gives

| Order | Value |
| ----: | ----: |
|     0 |     0 |
|     1 |     1 |
|     2 |     0 |
|     3 |    -1 |
|     4 |     0 |
|     5 |     1 |

Therefore,

$$
\sin x=
x
-\frac{x^3}{3!}
+\frac{x^5}{5!}
-\frac{x^7}{7!}
+\cdots
$$

Notice that every even-power term disappears because the corresponding derivatives vanish at the origin.

---

## 7. Example: The Cosine Function

For

$$
f(x)=\cos x,
$$

the derivatives follow a similar cyclic pattern.

Evaluating them at the origin produces

$$
\cos x=
1
-\frac{x^2}{2!}
+\frac{x^4}{4!}
-\frac{x^6}{6!}
+\cdots
$$

In this case, every odd-power term vanishes.

---

## 8. Successive Polynomial Approximations

A Taylor series can be viewed as a sequence of increasingly accurate polynomial approximations.

For the sine function,

the first approximation is simply

$$
P_1(x)=x.
$$

Including the cubic term gives

$$
P_3(x)=
x-\frac{x^3}{6}.
$$

Adding the fifth-degree term produces

$$
P_5(x)=
x-\frac{x^3}{6}
+\frac{x^5}{120}.
$$

Each additional term improves the approximation in a neighborhood of the expansion point.

As more terms are included, the polynomial reproduces the local behavior of the original function with increasing accuracy.
