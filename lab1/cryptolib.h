#ifndef _CRYPTOLIB_H_
#define _CRYPTOLIB_H_

typedef long long llong;

llong generalized_euclid(llong, llong, llong&, llong&, llong&);
llong pow_module(llong, llong, llong);
llong diffie_hellman(llong, llong, llong&, llong&, llong&);
llong baby_step_giant_step(llong, llong, llong, llong&);


#endif