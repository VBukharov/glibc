/* s_nearbyintl.c -- long double version of s_nearbyint.c.
 * Conversion to IEEE quad long double by Jakub Jelinek, jj@ultra.linux.cz.
 */

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

/*
 * nearbyintl(x)
 * Return x rounded to integral value according to the prevailing
 * rounding mode.
 * Method:
 *	Using floating addition.
 * Exception:
 *	Inexact flag raised if x not equal to rintl(x).
 */

#include <fenv.h>
#include <math.h>
#include <math_private.h>

static const long double
TWO112[2]={
  5.19229685853482762853049632922009600E+33L, /* 0x406F000000000000, 0 */
 -5.19229685853482762853049632922009600E+33L  /* 0xC06F000000000000, 0 */
};

long double __nearbyintl(long double x)
{
	fenv_t env;
	int64_t i0,j0,sx;
	u_int64_t i1;
	long double w,t;
	GET_LDOUBLE_WORDS64(i0,i1,x);
	sx = (((u_int64_t)i0)>>63);
	j0 = ((i0>>48)&0x7fff)-0x3fff;
	if(j0<112) {
	    if(j0<0) {
		feholdexcept (&env);
	        w = TWO112[sx]+x;
	        t = w-TWO112[sx];
	        fesetenv (&env);
		GET_LDOUBLE_MSW64(i0,t);
		SET_LDOUBLE_MSW64(t,(i0&0x7fffffffffffffffLL)|(sx<<63));
	        return t;
	    }
	} else {
	    if(j0==0x4000) return x+x;	/* inf or NaN */
	    else return x;		/* x is integral */
	}
	feholdexcept (&env);
	w = TWO112[sx]+x;
	t = w-TWO112[sx];
	fesetenv (&env);
	return t;
}
weak_alias (__nearbyintl, nearbyintl)
