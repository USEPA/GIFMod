#pragma once
#include "Vector.h"
#include <vector>

class CRestoreInfo
{
public:
	CVector X_res;
	CVector corr_fac_res;
	vector<int> fix_stats_res;
	CVector CG_res;
	CVector G_res;
	double t_res;
	double dt_res;
	bool fixed_connect;
	CRestoreInfo();
	CRestoreInfo(const CRestoreInfo &F);
	CRestoreInfo& CRestoreInfo::operator=(const CRestoreInfo &F);
	~CRestoreInfo();
	int iii;
};

