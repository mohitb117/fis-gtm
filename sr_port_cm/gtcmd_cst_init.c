/****************************************************************
 *								*
 *	Copyright 2001, 2010 Fidelity Information Services, Inc	*
 *								*
 *	This source code contains the intellectual property	*
 *	of its copyright holder(s), and is made available	*
 *	under a license.  If you do not know the terms of	*
 *	the license, please stop and do not read further.	*
 *								*
 ****************************************************************/

#include "mdef.h"
#include "gtm_string.h"
#include "gdsroot.h"
#include "gdsbt.h"
#include "gtm_facility.h"
#include "fileinfo.h"
#include "gdsblk.h"
#include "gdsfhead.h"
#include "filestruct.h"
#include "cmidef.h"
#include "hashtab_mname.h"
#include "hashtab.h"
#include "cmmdef.h"
#include "gtcmd.h"
#include "targ_alloc.h"
#include "dpgbldir.h"

#define DIR_ROOT 1

GBLREF int4		gv_keysize;
GBLREF gv_key		*gv_currkey;
GBLREF gv_key		*gv_altkey;
GBLREF short 		gtcm_ast_avail;

void gtcmd_cst_init(cm_region_head *ptr)
{
	gv_namehead	*g;
	gv_key          *temp_key;
	gd_region	*reg;
	sgmnt_addrs	*csa;

	error_def(CMERR_CMEXCDASTLM);

	reg = ptr->reg;
	if (VMS_ONLY(gtcm_ast_avail > 0) UNIX_ONLY(TRUE))
		gvcst_init(reg);
	else
		rts_error(VARLSTCNT(1) CMERR_CMEXCDASTLM);
	VMS_ONLY(gtcm_ast_avail--);
	GVKEYSIZE_INCREASE_IF_NEEDED(DBKEYSIZE(reg->max_key_size));
	csa = &FILE_INFO(reg)->s_addrs;
	assert(NULL == csa->dir_tree);
	SET_CSA_DIR_TREE(csa, reg->max_key_size, reg);
	init_hashtab_mname(ptr->reg_hash, 0, HASHTAB_NO_COMPACT, HASHTAB_NO_SPARE_TABLE);
	cm_add_gdr_ptr(reg);
}
