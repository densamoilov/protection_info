#ifndef _CRYPTOLIB_H_
#define _CRYPTOLIB_H_

typedef long long llong;

llong generic_euclid(llong a, llong b, llong &gcd, llong &x, llong &y);
llong pow_module(llong, llong, llong);
#endif