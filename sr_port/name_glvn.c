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

#include "mdef.h"
#include "compiler.h"
#include "opcode.h"
#include "toktyp.h"
#include "subscript.h"
#include "fnname.h"
#include "advancewindow.h"

error_def(ERR_COMMA);
error_def(ERR_EXTGBLDEL);
error_def(ERR_GBLNAME);
error_def(ERR_GVNAKEDEXTNM);
error_def(ERR_MAXNRSUBSCRIPTS);

int name_glvn(boolean_t gblvn, oprtype *a)
{
	boolean_t	vbar;
	char		x;
	int		fnname_type;
	/* Note:  MAX_LVSUBSCRIPTS and MAX_GVSUBSCRIPTS are currently equal.  Should that change,
			this should also change */
	oprtype subscripts[MAX_LVSUBSCRIPTS + 1], *sb1, *sb2;
	triple 	*ref, *t1, *t2;
	DCL_THREADGBL_ACCESS;

	SETUP_THREADGBL_ACCESS;
	sb1 = sb2 = subscripts;
	sb1++;						/* save room for type indicator */
	if (gblvn)
	{	fnname_type = FNGBL;
		if ((TK_LBRACKET == TREF(window_token)) || (TK_VBAR == TREF(window_token)))
		{
			vbar = (TK_VBAR == TREF(window_token));
			if (vbar)
				fnname_type |= FNVBAR;
			advancewindow();
			if (EXPR_FAIL == (vbar ? expr(sb1++, MUMPS_EXPR) : expratom(sb1++)))
				return FALSE;
			if (TK_COMMA != TREF(window_token))
				fnname_type |= FNEXTGBL1;
			else
			{
				fnname_type |= FNEXTGBL2;
				advancewindow();
				if (EXPR_FAIL == (vbar ? expr(sb1++, MUMPS_EXPR) : expratom(sb1++)))
					return FALSE;
			}
			if ((!vbar && (TK_RBRACKET != TREF(window_token))) || (vbar && (TK_VBAR != TREF(window_token))))
			{
				stx_error(ERR_EXTGBLDEL);
				return FALSE;
			}
			advancewindow();
		}
	} else
		fnname_type = FNLCL;
	if (TK_IDENT != TREF(window_token))
	{
		assert(fnname_type & FNGBL);
		if (fnname_type != FNGBL)
		{
			stx_error(ERR_GVNAKEDEXTNM);
			return FALSE;
		}
		if (TK_LPAREN != TREF(window_token))
		{
			stx_error(ERR_GBLNAME);
			return FALSE;
		}
		fnname_type = FNNAKGBL;
	} else
	{
		*sb1++ = put_str((TREF(window_ident)).addr, (TREF(window_ident)).len);
		advancewindow();
	}
	if (TK_LPAREN == TREF(window_token))
	{	for (;;)
		{	if (sb1 - sb2 > MAX_GVSUBSCRIPTS)
			{
				stx_error(ERR_MAXNRSUBSCRIPTS);
				return FALSE;
			}
			advancewindow();
			if (EXPR_FAIL == expr(sb1, MUMPS_EXPR))
				return FALSE;
			sb1++;
			if (TK_RPAREN == (x = TREF(window_token)))	/* NOTE assignment  */
			{
				advancewindow();
				break;
			}
			if (TK_COMMA != x)
			{
				stx_error(ERR_COMMA);
				return FALSE;
			}
		}
	}
	subscripts[0] = put_ilit(fnname_type);
	ref = t1 = newtriple(OC_PARAMETER);
	ref->operand[0] = put_ilit((mint)(sb1 - sb2 + 2)); /* # of subscripts + dst + depth argument (determine at f_name) */
	for ( ; sb2 < sb1 ; sb2++)
	{
		t2 = newtriple(OC_PARAMETER);
		t1->operand[1] = put_tref(t2);
		t1 = t2;
		t1->operand[0] = *sb2;
	}
	*a = put_tref(ref);
	return TRUE;
}
