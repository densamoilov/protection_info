#ifndef _CRYPTOLIB_H_
#define _CRYPTOLIB_H_

typedef long long llong;

llong generic_euclid(llong, llong, llong&, llong&, llong&);
llong pow_module(llong, llong, llong);

#endif