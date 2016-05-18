#include "Well.h"
#include "Distribution.h"
#include "StringOP.h"

CWell::CWell(void)
{
}


CWell::~CWell(void)
{
}

CWell::CWell(const CWell &W)
{
	fraction_old = W.fraction_old;
	young_age_distribution = W.young_age_distribution;
	params = W.params;
	name = W.name;
	distribution = W.distribution;
	hist_bin_num = W.hist_bin_num;
	hist_binsize = W.hist_binsize;
	vz_delay = W.vz_delay;
	age_old = W.age_old;
	fm = W.fm;
}

CWell& CWell::operator=(const CWell &m)
{
	fraction_old = m.fraction_old;
	young_age_distribution = m.young_age_distribution;
	name = m.name;
	params = m.params;
	distribution = m.distribution;
	hist_bin_num = m.hist_bin_num;
	hist_binsize = m.hist_binsize;
	vz_delay = m.vz_delay;
	age_old = m.age_old;
	fm = m.fm;
	return *this;
}

void CWell::createdistribution(vector<double> pdfparams, double tlast, int numinc, double mult)
{
	CDistribution D;
// ask ARASH	D.name = distribution;
	CBTC A;
	string dist = tolower(distribution);

	if ((dist=="dirac") || (dist=="piston"))
		young_age_distribution = creat_age_dist_dirac(pdfparams, tlast, numinc, mult);
	if ((dist=="gamma"))
		young_age_distribution = creat_age_dist_Gamma(pdfparams, tlast, numinc, mult);
	if ((dist == "igaussian" || dist == "inverse gaussian"))
		young_age_distribution = creat_age_dist_InvG(pdfparams, tlast, numinc, mult);
	if ((dist == "lognormal" || dist == "log normal"))
		young_age_distribution = creat_age_dist_lognormal(pdfparams, tlast, numinc, mult);
	if ((dist=="hist"))
		young_age_distribution = creat_age_dist_Hist(pdfparams, tlast, numinc, mult);
	if ((dist=="dispersion"))
		young_age_distribution = creat_age_dispersion(pdfparams, tlast, numinc, mult);
	if ((dist=="exp") || (dist=="exponential"))
		young_age_distribution = creat_age_dist_exp(pdfparams, tlast, numinc, mult);
	if (dist == "shifted_exp" || dist == "shifted exponential")
		young_age_distribution = creat_age_dist_Shexp(pdfparams, tlast, numinc, mult);
	if (dist=="levy") 
		young_age_distribution = creat_age_dist_Levy(pdfparams, tlast, numinc, mult);
	if (dist == "shifted_levy" || dist == "shifted levy")
		young_age_distribution = creat_age_dist_ShLevy(pdfparams, tlast, numinc, mult);
	if (dist == "generalized_igaussian" || dist == "generalized inverse gaussian")
		young_age_distribution = creat_age_dist_GIG(pdfparams, tlast, numinc, mult);
	
}




CBTC CWell::creat_age_dist_dirac (vector<double> params, double tlast, int numinc, double mult)
{
	CBTC Age_Dist(numinc+1); 
	double dt0;
	if (mult!=0)
		dt0 = tlast/(1+pow(1+mult,numinc))*mult;
	else
		dt0 = tlast/numinc;

	Age_Dist.t[0] = 0;
	for (int i=1; i<numinc+1; i++)
		Age_Dist.t[i] = Age_Dist.t[i-1]+dt0*pow(1+mult,i);

	for (int i=1; i<numinc+1; i++)
	{	Age_Dist.C[i] = 0; 
		if ((0.5*(Age_Dist.t[i-1]+Age_Dist.t[i])<params[0]) && (0.5*(Age_Dist.t[i]+Age_Dist.t[i+1])>params[0])) 
			Age_Dist.C[i] += 2.0/(Age_Dist.t[i+1]-Age_Dist.t[i-1]);
	} //check this!
	
		return Age_Dist;
}
//*************************************************************************************************************************

// 2-exp
CBTC CWell::creat_age_dist_exp(vector<double> params, double tlast, int numinc, double mult)
{
	CBTC Age_Dist(numinc+1); 
	double dt0;
	if (mult!=0)
		dt0 = tlast/(1+pow(1+mult,numinc))*mult;
	else
		dt0 = tlast/numinc;

	Age_Dist.t[0] = 0;
	for (int i=1; i<numinc+1; i++)
		Age_Dist.t[i] = Age_Dist.t[i-1]+dt0*pow(1+mult,i);
	
	for (int i=0; i<numinc+1; i++)
		Age_Dist.C[i] = (1/params[0])*exp(-Age_Dist.t[i]/params[0]);

	return Age_Dist;

}
//*************************************************************************************************************************

//4-lognormal
CBTC CWell::creat_age_dist_lognormal(vector<double> params,double tlast, int numinc, double mult)
{
	CBTC Age_Dist(numinc+1); 
	double dt0;
	if (mult!=1)
		dt0 = tlast/(1+pow(1+mult,numinc))*mult;
	else
		dt0 = tlast/numinc;

	Age_Dist.t[0] = 0;
	for (int i=1; i<numinc+1; i++)
		Age_Dist.t[i] = Age_Dist.t[i-1]+dt0*pow(1+mult,i-1);
	
	Age_Dist.C[0]=0;
	for (int i=1; i<numinc+1; i++)
		Age_Dist.C[i] = 1/(Age_Dist.t[i]*log(params[1])*pow(2*(atan(1.0)*4),0.5))*exp(-pow(log(Age_Dist.t[i])-log(params[0]),2)/(2*pow(params[1],2)));
	return Age_Dist;
}
//*************************************************************************************************************************

//5-Inv_Gaussian
CBTC CWell::creat_age_dist_InvG(vector<double> params, double tlast, int numinc, double mult)
{
	double pi = 4*atan(1.0);
	CBTC Age_Dist(numinc+1); 
	double lambda = pow(params[0],3)/pow(params[1],2);
	double dt0;
	if (mult!=1)
		dt0 = tlast/(1+pow(1+mult,numinc))*mult;
	else
		dt0 = tlast/numinc;

	Age_Dist.t[0] = 1e-12;
	for (int i=1; i<numinc+1; i++)
		Age_Dist.t[i] = Age_Dist.t[i-1]+dt0*pow(1+mult,i);
	
	for (int i=0; i<numinc+1; i++)
		//Soroosh
		Age_Dist.C[i] = pow(lambda/(2*pi*pow(Age_Dist.t[i],3)),0.5)*exp(-lambda*pow(Age_Dist.t[i]-params[0],2)/(2*pow(params[0],2)*Age_Dist.t[i]));	

	return Age_Dist;
}

//6-levy
CBTC CWell::creat_age_dist_Levy(vector<double> params, double tlast, int numinc, double mult)
{
	double pi = 4*atan(1.0);
	CBTC Age_Dist(numinc+1); 
	double C_levy = params[0];
	double dt0;
	if (mult!=1)
		dt0 = tlast/(1+pow(1+mult,numinc))*mult;
	else
		dt0 = tlast/numinc;
	
	Age_Dist.t[0] = 1e-12;
	for (int i=1; i<numinc+1; i++)
		Age_Dist.t[i] = Age_Dist.t[i-1]+dt0*pow(1+mult,i);
	
	for (int i=0; i<numinc+1; i++)		
		Age_Dist.C[i] = pow(C_levy/(2*pi),0.5)*exp(-C_levy/(2*Age_Dist.t[i]))/pow(Age_Dist.t[i],1.5);	                  
	
	return Age_Dist;
}

//7-shifted_levy
CBTC CWell::creat_age_dist_ShLevy(vector<double> params, double tlast, int numinc, double mult)
{
	double pi = 4*atan(1.0);
	CBTC Age_Dist(numinc+1); 
	double C_levy = params[0];
	double t_shift = params[1];
	double dt0;
	if (mult!=1)
		dt0 = tlast/(1+pow(1+mult,numinc))*mult;
	else
		dt0 = tlast/numinc;
	
	Age_Dist.t[0] = 1e-12;
	for (int i=1; i<numinc+1; i++)
		Age_Dist.t[i] = Age_Dist.t[i-1]+dt0*pow(1+mult,i);
	
	for (int i=0; i<numinc+1; i++)
	{
		if (Age_Dist.t[i]>t_shift)
			Age_Dist.C[i] = pow(C_levy/(2*pi),0.5)*exp(-C_levy/(2*(Age_Dist.t[i]-t_shift)))/pow((Age_Dist.t[i]-t_shift),1.5);
		else
			Age_Dist.C[i] = 0;
	}
	
	return Age_Dist;
}

//8-shifted_exp
CBTC CWell::creat_age_dist_Shexp(vector<double> params, double tlast, int numinc, double mult)
{
	double pi = 4*atan(1.0);
	CBTC Age_Dist(numinc+1); 
	double lambda = params[0];
	double t_shift = params[1];
	double dt0;
	if (mult!=1)
		dt0 = tlast/(1+pow(1+mult,numinc))*mult;
	else
		dt0 = tlast/numinc;
	
	Age_Dist.t[0] = 1e-12;
	for (int i=1; i<numinc+1; i++)
		Age_Dist.t[i] = Age_Dist.t[i-1]+dt0*pow(1+mult,i);
	
	for (int i=0; i<numinc+1; i++)
	{
		if (Age_Dist.t[i]>t_shift)
			Age_Dist.C[i] = (1/lambda)*exp(-(Age_Dist.t[i]-t_shift)/lambda);
		else
			Age_Dist.C[i] = 0;
	}
	
	return Age_Dist;
}

//9-Generalized Inverse Guassian
CBTC CWell::creat_age_dist_GIG(vector<double> params, double tlast, int numinc, double mult)
{
	double pi = 4*atan(1.0);
	CBTC Age_Dist(numinc+1); 

	double p = params[0];
	double a = params[1];
	double b = params[2];

	double dt0;
	if (mult!=1)
		dt0 = tlast/(1+pow(1+mult,numinc))*mult;
	else
		dt0 = tlast/numinc;
	
	Age_Dist.t[0] = 1e-12;
	for (int i=1; i<numinc+1; i++)
		Age_Dist.t[i] = Age_Dist.t[i-1]+dt0*pow(1+mult,i);
	
	for (int i=0; i<numinc+1; i++)
		Age_Dist.C[i] = pow(Age_Dist.t[i],p-1)*exp(-(a*Age_Dist.t[i]+b/Age_Dist.t[i])/2);
	
	double area = Age_Dist.integrate();
	return (1/area)*Age_Dist;
}

//Dispersion
CBTC CWell::creat_age_dispersion(vector<double> params, double tlast, int numinc, double mult)
{
	double pi = 4*atan(1.0);
	CBTC Age_Dist(numinc+1); 
	double dt0;
	if (mult!=1)
		dt0 = tlast/(1+pow(1+mult,numinc))*mult;
	else
		dt0 = tlast/numinc;

	Age_Dist.t[0] = 1e-12;
	for (int i=1; i<numinc+1; i++)
		Age_Dist.t[i] = Age_Dist.t[i-1]+dt0*pow(1+mult,i);
	
	for (int i=0; i<numinc+1; i++)
		Age_Dist.C[i] = 1/sqrt(2*pi*params[1]*Age_Dist.t[i])*exp(-pow(Age_Dist.t[i]-params[0],2)/(4*params[1]*Age_Dist.t[i]));	

	return Age_Dist;
}

CBTC CWell::creat_age_dist_Hist(vector<double> pdfparams, double tlast, int numinc, double mult)
{
	CBTC Age_Dist(numinc+1); 
	double dt0;
	if (mult!=0)
		dt0 = tlast/(1+pow(1+mult,numinc))*mult;
	else
		dt0 = tlast/numinc;

	Age_Dist.t[0] = 0;
	for (int i=1; i<numinc+1; i++)
		Age_Dist.t[i] = Age_Dist.t[i-1]+dt0*pow(1+mult,i-1);
	
	Age_Dist.C[0]=0;
	for (int i=1; i<numinc+1; i++)
	{	for (int j=0; j<hist_bin_num-1; j++)
			if ((Age_Dist.t[i]>j*hist_binsize) && (Age_Dist.t[i]<=(j+1)*hist_binsize))
				Age_Dist.C[i] = pdfparams[j]/hist_binsize;
		if ((Age_Dist.t[i]>(hist_bin_num-1)*hist_binsize) && (Age_Dist.t[i]<=hist_bin_num*hist_binsize))
			Age_Dist.C[i] = (1-CVector(pdfparams).sum())/hist_binsize;
	}
	return Age_Dist;

}

CBTC CWell::creat_age_dist_Gamma(vector<double> params, double tlast, int numinc, double mult)
{
	double k_gamma = params[0];
	double theta_gamma = params[1];
	
	CBTC Age_Dist(numinc+1); 
	double dt0;
	if (mult!=1)
		dt0 = tlast/(1+pow(1+mult,numinc))*mult;
	else
		dt0 = tlast/numinc;

	Age_Dist.t[0] = 0;
	for (int i=1; i<numinc+1; i++)
		Age_Dist.t[i] = Age_Dist.t[i-1]+dt0*pow(1+mult,i-1);
	
	Age_Dist.C[0]=0;
//	for (int i=1; i<numinc+1; i++)
// ask ARASH		Age_Dist.C[i] = Gammapdf(Age_Dist.t[i],k_gamma,theta_gamma);

	return Age_Dist;
}

void CWell::set_val(string S, double val)
{
	vector<char> del;
	del.push_back('[');
	del.push_back(']');
	del.push_back(':');
	vector<string> s = split(S, del);
	if (s.size() == 1)
	{
		if (tolower(trim(s[0])) == "f") fraction_old = val;
		if (tolower(trim(s[0])) == "fm") fm = val;
		if (tolower(trim(s[0])) == "vz_delay") vz_delay = val;
		if (tolower(trim(s[0])) == "age_old") age_old = val;
	}
	if (s.size() == 2)
		if (tolower(trim(s[0]) == "param")) params[atoi(s[1].c_str())] = val;

}
int CWell::get_numparams(string S, int nbins)
{
	string dist = tolower(distribution);

	if (dist == "dirac" || dist == "piston")
		return 1;
	if (dist == "gamma")
		return 2;
	if (dist == "igaussian" || dist == "inverse gaussian")
		return 2;
	if (distribution == "igaussianm")
		return 4;
	if (dist == "lognormal" || dist == "log normal")
		return 2;
	if (distribution == "hist")
		return nbins;
	if (distribution == "dispersion")
		return 2;
	if (dist == "exp" || dist == "exponential")
		return 1;
	if (dist == "shifted_exp" || dist =="shifted exponential")
		return 2;
	if (distribution == "levy")
		return 1;
	if (dist == "shifted_levy" || dist == "shifted levy")
		return 2;
	if (dist == "generalized_igaussian" || dist == "generalized inverse gaussian")
		return 3;
	if (dist == "generalized inverse gamma")
		return 4;
}