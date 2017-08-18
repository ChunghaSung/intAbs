#ifdef CBMC
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

#include <stdint.h>

//TODO: endianness test does not work
/*
#if _IEEE_WORD_ORDER == _BIG_ENDIAN

typedef union
{
  double value;
  struct
  {
    uint32_t msw;
    uint32_t lsw;
  } parts;
  struct
  {
    uint64_t w;
  } xparts;
} ieee_double_shape_type;

#endif
*/

#if _IEEE_WORD_ORDER == _LITTLE_ENDIAN

typedef union
{
  double value;
  struct
  {
    uint32_t lsw;
    uint32_t msw;
  } parts;
  struct
  {
    uint64_t w;
  } xparts;
} ieee_double_shape_type;

#endif

#define EXTRACT_WORDS(ix0,ix1,d)    \
  do {            \
    ieee_double_shape_type ew_u;    \
    ew_u.value = (d);       \
    (ix0) = ew_u.parts.msw;     \
    (ix1) = ew_u.parts.lsw;     \
  } while (0)

#define INSERT_WORDS(d,ix0,ix1)     \
  do {            \
    ieee_double_shape_type iw_u;    \
    iw_u.parts.msw = (ix0);     \
    iw_u.parts.lsw = (ix1);     \
    (d) = iw_u.value;       \
  } while (0)

typedef union
{
  float value;
  uint32_t word;
} ieee_float_shape_type;

#define GET_FLOAT_WORD(i,d)     \
  do {            \
    ieee_float_shape_type gf_u;     \
    gf_u.value = (d);       \
    (i) = gf_u.word;        \
  } while (0)

#define SET_FLOAT_WORD(d,i)     \
  do {            \
    ieee_float_shape_type sf_u;     \
    sf_u.word = (i);        \
    (d) = sf_u.value;       \
  } while (0)

static const double huge_double = 1.0e300;
static const float huge_float = 1.0e30;

double ceil(double x)
{
  int32_t i0,i1,j0;
  uint32_t i,j;
  EXTRACT_WORDS(i0,i1,x);
  j0 = ((i0>>20)&0x7ff)-0x3ff;
  if(j0<20) {
    if(j0<0) { /* raise inexact if x != 0 */
      if(huge_double+x>0.0) {/* return 0*sign(x) if |x|<1 */
  if(i0<0) {i0=0x80000000;i1=0;}
  else if((i0|i1)!=0) { i0=0x3ff00000;i1=0;}
      }
    } else {
      i = (0x000fffff)>>j0;
      if(((i0&i)|i1)==0) return x; /* x is integral */
      if(huge_double+x>0.0) { /* raise inexact flag */
  if(i0>0) i0 += (0x00100000)>>j0;
  i0 &= (~i); i1=0;
      }
    }
  } else if (j0>51) {
    if(j0==0x400) return x+x; /* inf or NaN */
    else return x;  /* x is integral */
  } else {
    i = ((uint32_t)(0xffffffff))>>(j0-20);
    if((i1&i)==0) return x; /* x is integral */
    if(huge_double+x>0.0) { /* raise inexact flag */
      if(i0>0) {
  if(j0==20) i0+=1;
  else {
    j = i1 + (1<<(52-j0));
    if(j<i1) i0+=1; /* got a carry */
    i1 = j;
  }
      }
      i1 &= (~i);
    }
  }
  INSERT_WORDS(x,i0,i1);
  return x;
}

double floor(double x)
{
  int32_t i0,i1,j0;
  uint32_t i,j;
  EXTRACT_WORDS(i0,i1,x);
  j0 = ((i0>>20)&0x7ff)-0x3ff;
  if(j0<20) {
    if(j0<0) { /* raise inexact if x != 0 */
      if(huge_double+x>0.0) {/* return 0*sign(x) if |x|<1 */
  if(i0>=0) {i0=i1=0;}
  else if(((i0&0x7fffffff)|i1)!=0)
    { i0=0xbff00000;i1=0;}
      }
    } else {
      i = (0x000fffff)>>j0;
      if(((i0&i)|i1)==0) return x; /* x is integral */
      if(huge_double+x>0.0) { /* raise inexact flag */
  if(i0<0) i0 += (0x00100000)>>j0;
  i0 &= (~i); i1=0;
      }
    }
  } else if (j0>51) {
    if(j0==0x400) return x+x; /* inf or NaN */
    else return x;  /* x is integral */
  } else {
    i = ((uint32_t)(0xffffffff))>>(j0-20);
    if((i1&i)==0) return x; /* x is integral */
    if(huge_double+x>0.0) { /* raise inexact flag */
      if(i0<0) {
  if(j0==20) i0+=1;
  else {
    j = i1+(1<<(52-j0));
    if(j<i1) i0 +=1 ; /* got a carry */
    i1=j;
  }
      }
      i1 &= (~i);
    }
  }
  INSERT_WORDS(x,i0,i1);
  return x;
}

float ceilf(float x)
{
  int32_t i0,j0;
  uint32_t i;

  GET_FLOAT_WORD(i0,x);
  j0 = ((i0>>23)&0xff)-0x7f;
  if(j0<23) {
    if(j0<0) { /* raise inexact if x != 0 */
      if(huge_float+x>(float)0.0) {/* return 0*sign(x) if |x|<1 */
  if(i0<0) {i0=0x80000000;}
  else if(i0!=0) { i0=0x3f800000;}
      }
    } else {
      i = (0x007fffff)>>j0;
      if((i0&i)==0) return x; /* x is integral */
      if(huge_float+x>(float)0.0) { /* raise inexact flag */
  if(i0>0) i0 += (0x00800000)>>j0;
  i0 &= (~i);
      }
    }
  } else {
    if(j0==0x80) return x+x;  /* inf or NaN */
    else return x;  /* x is integral */
  }
  SET_FLOAT_WORD(x,i0);
  return x;
}

float floorf(float x)
{
  int32_t i0,j0;
  uint32_t i;
  GET_FLOAT_WORD(i0,x);
  j0 = ((i0>>23)&0xff)-0x7f;
  if(j0<23) {
    if(j0<0) { /* raise inexact if x != 0 */
      if(huge_float+x>(float)0.0) {/* return 0*sign(x) if |x|<1 */
  if(i0>=0) {i0=0;}
  else if((i0&0x7fffffff)!=0)
    { i0=0xbf800000;}
      }
    } else {
      i = (0x007fffff)>>j0;
      if((i0&i)==0) return x; /* x is integral */
      if(huge_float+x>(float)0.0) { /* raise inexact flag */
  if(i0<0) i0 += (0x00800000)>>j0;
  i0 &= (~i);
      }
    }
  } else {
    if(j0==0x80) return x+x;  /* inf or NaN */
    else return x;  /* x is integral */
  }
  SET_FLOAT_WORD(x,i0);
  return x;
}

//Do it the declarative way. Caveat: spurious solutions due to rounding.
double fabs (double);
double fmod(double a, double n)
{
  double q,r;
  __CPROVER_assume(floor(q)==q); //q must be integral //TODO: cheaper solution
  __CPROVER_assume((a==n*q+r) && fabs(r)<fabs(n));
  __CPROVER_assume((a<0 && r<0) || (a>0 && r>0) || (a==0 && r==0)); //r must have sign of a
  return r;
}

float fabsf (float x);
float fmodf(float a, float n)
{
  float q,r;
  __CPROVER_assume(floorf(q)==q); //q must be integral //TODO: cheaper solution
  __CPROVER_assume((a==n*q+r) && fabsf(r)<fabsf(n));
  __CPROVER_assume((a<0 && r<0) || (a>0 && r>0) || (a==0 && r==0)); //r must have sign of a
  return r;
}
#endif
