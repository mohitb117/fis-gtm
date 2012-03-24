/****************************************************************
 *								*
 *	Copyright 2001, 2011 Fidelity Information Services, Inc	*
 *								*
 *	This source code contains the intellectual property	*
 *	of its copyright holder(s), and is made available	*
 *	under a license.  If you do not know the terms of	*
 *	the license, please stop and do not read further.	*
 *								*
 ****************************************************************/

#ifndef __MVALCONV_H__
#define  __MVALCONV_H__

/* Several of the following functions suffer from a lack of precision when used
 * on a 64bit system.  ints are 4 byte values, but the functions i2mval and i2usmval
 * i2usmval are written as if they were taking an 8 byte value as a parameter.
 */
void		i2smval(mval *v, uint4 i);
void		i2usmval(mval *v, unsigned int i);
void		i2mval(mval *v, int i);
#ifdef GTM64
void		ul2mval(mval *v, unsigned long i);
void		l2mval(mval *v, long i);
#endif
void		double2mval(mval *dst, double src);
double		mval2double(mval *v);
int4		mval2i(mval *v);
uint4		mval2ui(mval *v);
boolean_t	isint (mval *v, int4 *intval);

#endif
