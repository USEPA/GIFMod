#include "RestoreInfo.h"


CRestoreInfo::CRestoreInfo()
{
	iii = 0;
}


CRestoreInfo::~CRestoreInfo()
{
}

CRestoreInfo::CRestoreInfo(const CRestoreInfo &F)
{
	X_res = F.X_res;
	corr_fac_res = F.corr_fac_res;
	fix_stats_res = F.fix_stats_res;
	CG_res = F.CG_res;
	G_res = F.G_res;
	t_res = F.t_res;
	dt_res = F.dt_res;
	iii = F.iii;
	
}

CRestoreInfo& CRestoreInfo::operator=(const CRestoreInfo &F)
{
	X_res = F.X_res;
	corr_fac_res = F.corr_fac_res;
	fix_stats_res = F.fix_stats_res;
	CG_res = F.CG_res;
	G_res = F.G_res;
	t_res = F.t_res;
	dt_res = F.dt_res;
	iii = F.iii;
	return *this;
}