#include "RxnNetwork.h"


CRxnNetwork::CRxnNetwork(void)
{
	
}


CRxnNetwork::CRxnNetwork(int nRxns, int nparams, int nconstts)
{
	n_constts = nconstts;
	n_params = nparams;
	n_Rxns = nRxns;
	cons.resize(n_constts);
	params.resize(n_params);
	param_corrected.resize(n_params);
	Rxts.resize(n_Rxns);
}

CRxnNetwork::CRxnNetwork(const CRxnNetwork &CRxnNetwork)
{
	ASM_ID = CRxnNetwork.ASM_ID;
	cons = CRxnNetwork.cons;
	n_constts = CRxnNetwork.n_constts;
	n_params = CRxnNetwork.n_params;
	n_Rxns = CRxnNetwork.n_Rxns;
	Rxts = CRxnNetwork.Rxts;
	params = CRxnNetwork.params;
	param_corrected = CRxnNetwork.param_corrected;
	volume = CRxnNetwork.volume; 
	inflows = CRxnNetwork.inflows; //added
	inflowconsts = CRxnNetwork.inflowconsts; //added
	controlconsts = CRxnNetwork.inflowconsts;
	pathname = CRxnNetwork.pathname; //added
	temperature = CRxnNetwork.temperature;
	ntseris = CRxnNetwork.ntseris;
	rate_change_norm = CRxnNetwork.rate_change_norm;
	inflows_avg = CRxnNetwork.inflows_avg;
	temperature_avg = CRxnNetwork.temperature_avg;
	parameters = CRxnNetwork.parameters;
}

CRxnNetwork CRxnNetwork::operator=(const CRxnNetwork &CR)
{
	ASM_ID = CR.ASM_ID;
	cons = CR.cons;
	n_constts = CR.n_constts;
	n_params = CR.n_params;
	n_Rxns = CR.n_Rxns;
	Rxts = CR.Rxts;
	params = CR.params;
	param_corrected = CR.param_corrected;
	volume = CR.volume; 
	inflows = CR.inflows; //added
	inflowconsts = CR.inflowconsts; //added
	controlconsts = CR.inflowconsts;
	pathname = CR.pathname; //added
	ntseris = CR.ntseris;
	temperature = CR.temperature;
	rate_change_norm = CR.rate_change_norm;
	inflows_avg = CR.inflows_avg;
	temperature_avg = CR.temperature_avg;
	parameters = CR.parameters;
	return *this;
}


CRxnNetwork::~CRxnNetwork(void)
{
	cons.clear();
	params.clear();
	param_corrected.clear();
	Rxts.clear();
}

void CRxnNetwork::setnumconcs(int n)
{
	cons.resize(n);
	n_constts = n;
}

void CRxnNetwork::setnumparams(int n)
{
	n_params = n;
}

void CRxnNetwork::getreactions(ifstream& file)
{
	vector<string> s;
	int i=0;
	n_Rxns = 0;
	while (file.eof()==false)
	{
		s = getline(file);
				
		if (s[0].substr(0,2)!="//")
		{
			Rxts[i].set_n_prdcts(atoi(s[0].c_str())); 
			Rxts[i].rate = CStringOP(s[1]);
			for (int j=0; j<Rxts[i].nproducts; j++)
			{
				Rxts[i].products[j] = atoi(s[2*j+2].c_str()); 
				Rxts[i].prodrates[j] = CStringOP(s[2*j+3]); 
			}
			i++;
			n_Rxns++;
		}
	}

}


CVector CRxnNetwork::getrate(vector<CConstituent> &c_star, int ts)
{	
	CVector rates(n_Rxns);  // rate of each reaction

	for (int i=0; i<n_Rxns; i++)
	{  
		if (temperature.size()==0)
			rates[i] = calcterm(Rxts[i].rate, c_star);
		else
			rates[i] = calcterm(Rxts[i].rate,ts,temperature_avg[ts], c_star);		
	}

	return rates;	
}

double CRxnNetwork::calcterm(CStringOP &term)
{
	double out = 0;
	if ((term.nterms == 1) && (term.nopts == 0))
	{
		if (term.parameter == true)
			out = param_corrected[term.number];
		else if (term.concentration == true)
			out = H(cons[term.number].conc);
		else if (term.constant == true)
			out = term.value;
		else 
			out = calcterm(term.terms[0]);
	}

	if ((term.nterms == 1) && (term.nopts == 1))
	{
		if (term.operators[0] == 0)
			out = calcterm(term.terms[0]);

		if (term.operators[0] == 1)
			out = -calcterm(term.terms[0]);
	}

	if ((term.nterms == 2) && (term.nopts == 1))
	{
		if (term.operators[0] == 0)
			out = calcterm(term.terms[0])+calcterm(term.terms[1]);
		if (term.operators[0] == 1)
			out = calcterm(term.terms[0])-calcterm(term.terms[1]);
		if (term.operators[0] == 2)
			out = calcterm(term.terms[0])*calcterm(term.terms[1]);
		if (term.operators[0] == 3)
			out = calcterm(term.terms[0])/calcterm(term.terms[1]);
		if (term.operators[0] == 4)
			out=pow(calcterm(term.terms[0]),calcterm(term.terms[1]));
	}

	if ((term.nterms == 2) && (term.nopts == 2))
	{
		double sum = 0;
		if (term.operators[0] == 0)
			sum = calcterm(term.terms[0]);
		else if (term.operators[0] == 1)
			sum = -calcterm(term.terms[0]);
		
		if (term.operators[1] == 0)
			out = sum+calcterm(term.terms[1]);
		if (term.operators[1] == 1)
			out = sum-calcterm(term.terms[1]);
		if (term.operators[1] == 2)
			out = sum*calcterm(term.terms[1]);
		if (term.operators[1] == 3)
			out = sum/calcterm(term.terms[1]);
		if (term.operators[1] == 4)
			out = (sum/fabs(sum))*pow(fabs(sum),calcterm(term.terms[1]));

	}

	if ((term.nterms>2) && (term.nopts == term.nterms-1))
	{	
		out = calcterm(term.terms[0]);
		for (int j=1; j<term.nterms; j++)
		{	
			if (term.operators[j-1] == 0)
				out+=calcterm(term.terms[j]);
			if (term.operators[j-1] == 1)
				out-=calcterm(term.terms[j]);
			if (term.operators[j-1] == 2)
				out*=calcterm(term.terms[j]);
			if (term.operators[j-1] == 3)
				out/=calcterm(term.terms[j]);
			if (term.operators[j-1] == 4)
				out = pow(out,calcterm(term.terms[j]));
		}
		
	}


	if ((term.nterms>2) && (term.nopts == term.nterms))
	{	
		out = 0;
		if (term.operators[0] == 0)	
			out = calcterm(term.terms[0]);
		else if (term.operators[0] == 1)
			out = -calcterm(term.terms[0]);
	
		
		for (int j=1; j<term.nterms; j++)
		{	
			if (term.operators[j] == 0)
				out+=calcterm(term.terms[j]);
			if (term.operators[j] == 1)
				out-=calcterm(term.terms[j]);
			if (term.operators[j] == 2)
				out*=calcterm(term.terms[j]);
			if (term.operators[j] == 3)
				out/=calcterm(term.terms[j]);
			if (term.operators[j] == 4)
				out=pow(out,calcterm(term.terms[j]));

		}
		
	}
	if (term.function==true)
	{	if (term.number == 1)
			return exp(out);
		if (term.number == 2)
			return H(out);
		if (term.number == 3)
			return min(calcterm(term.terms[0]), calcterm(term.terms[1]));
		if (term.number == 4)
			return max(calcterm(term.terms[0]), calcterm(term.terms[1]));
		if (term.number == 5)
			return log(out);
		if (term.number == 6)
			return log10(out);
		if (term.number == 7)
			return 1.0/(1.0+exp(-out));
		if (term.number == 8)
			return 0.5*(fabs(out)+out);
		if (term.number == 9)
			return 0.5/calcterm(term.terms[1])*(calcterm(term.terms[0])*calcterm(term.terms[1])+sqrt(pow(calcterm(term.terms[0])*calcterm(term.terms[1]),2)+1));
		if (term.number == 10)
			return sqrt(out);
	}
	else
		return out;
}

double CRxnNetwork::calcterm(CStringOP &term, vector<CConstituent> &C_prime)
{
	double out = 0;
	if ((term.nterms == 1) && (term.nopts == 0))
	{
		if (term.parameter == true)
			out = param_corrected[term.number];
		else if (term.concentration == true)
			out = H(C_prime[term.number].conc);
		else if (term.constant == true)
			out = term.value;
		else 
			out = calcterm(term.terms[0],C_prime);
	}

	if ((term.nterms == 1) && (term.nopts == 1))
	{
		if (term.operators[0] == 0)
			out = calcterm(term.terms[0],C_prime);

		if (term.operators[0] == 1)
			out = -calcterm(term.terms[0],C_prime);
	}

	if ((term.nterms == 2) && (term.nopts == 1))
	{
		if (term.operators[0] == 0)
			out = calcterm(term.terms[0],C_prime)+calcterm(term.terms[1],C_prime);
		if (term.operators[0] == 1)
			out = calcterm(term.terms[0],C_prime)-calcterm(term.terms[1],C_prime);
		if (term.operators[0] == 2)
			out = calcterm(term.terms[0],C_prime)*calcterm(term.terms[1],C_prime);
		if (term.operators[0] == 3)
			out = calcterm(term.terms[0],C_prime)/calcterm(term.terms[1],C_prime);
		if (term.operators[0] == 4)
			out = pow(calcterm(term.terms[0],C_prime),calcterm(term.terms[1],C_prime));
	}

	if ((term.nterms == 2) && (term.nopts == 2))
	{
		double sum = 0;
		if (term.operators[0] == 0)
			sum = calcterm(term.terms[0],C_prime);
		else if (term.operators[0] == 1)
			sum = -calcterm(term.terms[0],C_prime);
		
		if (term.operators[1] == 0)
			out = sum+calcterm(term.terms[1],C_prime);
		if (term.operators[1] == 1)
			out = sum-calcterm(term.terms[1],C_prime);
		if (term.operators[1] == 2)
			out = sum*calcterm(term.terms[1],C_prime);
		if (term.operators[1] == 3)
			out = sum/calcterm(term.terms[1],C_prime);
		if (term.operators[1] == 4)
			out = pow(sum,calcterm(term.terms[1],C_prime));
	}

	if ((term.nterms>2) && (term.nopts == term.nterms-1))
	{	
		out = calcterm(term.terms[0]);
		for (int j=1; j<term.nterms; j++)
		{	
			if (term.operators[j-1] == 0)
				out+=calcterm(term.terms[j],C_prime);
			if (term.operators[j-1] == 1)
				out-=calcterm(term.terms[j],C_prime);
			if (term.operators[j-1] == 2)
				out*=calcterm(term.terms[j],C_prime);
			if (term.operators[j-1] == 3)
				out/=calcterm(term.terms[j],C_prime);
			if (term.operators[j-1] == 4)
				out=pow(out,calcterm(term.terms[j],C_prime));
		}
		
	}


	if ((term.nterms>2) && (term.nopts == term.nterms))
	{	
		out = 0;
		if (term.operators[0] == 0)	
			out = calcterm(term.terms[0],C_prime);
		else if (term.operators[0] == 1)
			out = -calcterm(term.terms[0],C_prime);
	
		
		for (int j=1; j<term.nterms; j++)
		{	
			if (term.operators[j] == 0)
				out+=calcterm(term.terms[j],C_prime);
			if (term.operators[j] == 1)
				out-=calcterm(term.terms[j],C_prime);
			if (term.operators[j] == 2)
				out*=calcterm(term.terms[j],C_prime);
			if (term.operators[j] == 3)
				out/=calcterm(term.terms[j],C_prime);
			if (term.operators[j] == 4)
				out=pow(out,calcterm(term.terms[j],C_prime));
		}
		
	}
	if (term.function==true)
	{	if (term.number == 1)
			return exp(out);
		if (term.number == 2)
			return H(out);
		if (term.number == 3)
			return min(calcterm(term.terms[0]), calcterm(term.terms[1],C_prime));
		if (term.number == 4)
			return max(calcterm(term.terms[0]), calcterm(term.terms[1],C_prime));
		if (term.number == 5)
			return log(out);
		if (term.number == 6)
			return log10(out);
		if (term.number == 7)
			return 1.0/(1.0+exp(-out));
		if (term.number == 8)
			return 0.5*(fabs(out)+out);
		if (term.number == 9)
			return 0.5/calcterm(term.terms[1],C_prime)*(calcterm(term.terms[0],C_prime)*calcterm(term.terms[1],C_prime)+sqrt(pow(calcterm(term.terms[0],C_prime)*calcterm(term.terms[1],C_prime),2)+1));
		if (term.number == 10)
			return sqrt(out);
	
	}
	else
		return out;
}


double CRxnNetwork::calcterm(CStringOP &term, int ts, double temperature)
{
	double out = 0;
	if ((term.nterms == 1) && (term.nopts == 0))
	{
		if (term.parameter == true)
			out = param_corrected[term.number];
		else if (term.concentration == true)
			out = H(cons[term.number].conc);
		else if (term.constant == true)
			out = term.value;
		else 
			out = calcterm(term.terms[0], ts, temperature);
	}

	if ((term.nterms == 1) && (term.nopts == 1))
	{
		if (term.operators[0] == 0)
			out = calcterm(term.terms[0], ts, temperature);

		if (term.operators[0] == 1)
			out = -calcterm(term.terms[0], ts, temperature);
	}

	if ((term.nterms == 2) && (term.nopts == 1))
	{
		if (term.operators[0] == 0)
			out = calcterm(term.terms[0], ts, temperature)+calcterm(term.terms[1], ts, temperature);
		if (term.operators[0] == 1)
			out = calcterm(term.terms[0], ts, temperature)-calcterm(term.terms[1], ts, temperature);
		if (term.operators[0] == 2)
			out = calcterm(term.terms[0], ts, temperature)*calcterm(term.terms[1], ts, temperature);
		if (term.operators[0] == 3)
			out = calcterm(term.terms[0], ts, temperature)/calcterm(term.terms[1], ts, temperature);
		if (term.operators[0] == 4)
			out = pow(calcterm(term.terms[0], ts, temperature),calcterm(term.terms[1], ts, temperature));
	}

	if ((term.nterms == 2) && (term.nopts == 2))
	{
		double sum = 0;
		if (term.operators[0] == 0)
			sum = calcterm(term.terms[0], ts, temperature);
		else if (term.operators[0] == 1)
			sum = -calcterm(term.terms[0], ts, temperature);
		
		if (term.operators[1] == 0)
			out = sum+calcterm(term.terms[1], ts, temperature);
		if (term.operators[1] == 1)
			out = sum-calcterm(term.terms[1], ts, temperature);
		if (term.operators[1] == 2)
			out = sum*calcterm(term.terms[1], ts, temperature);
		if (term.operators[1] == 3)
			out = sum/calcterm(term.terms[1], ts, temperature);
		if (term.operators[1] == 4)
			out = pow(sum,calcterm(term.terms[1], ts, temperature));
	}

	if ((term.nterms>2) && (term.nopts == term.nterms-1))
	{	
		out = calcterm(term.terms[0], ts, temperature);
		for (int j=1; j<term.nterms; j++)
		{	
			if (term.operators[j-1] == 0)
				out+=calcterm(term.terms[j], ts, temperature);
			if (term.operators[j-1] == 1)
				out-=calcterm(term.terms[j], ts, temperature);
			if (term.operators[j-1] == 2)
				out*=calcterm(term.terms[j], ts, temperature);
			if (term.operators[j-1] == 3)
				out/=calcterm(term.terms[j], ts, temperature);
			if (term.operators[j-1] == 4)
				out=pow(out,calcterm(term.terms[j], ts, temperature));
		}
		
	}


	if ((term.nterms>2) && (term.nopts == term.nterms))
	{	
		out = 0;
		if (term.operators[0] == 0)	
			out = calcterm(term.terms[0], ts, temperature);
		else if (term.operators[0] == 1)
			out = -calcterm(term.terms[0], ts, temperature);
	
		
		for (int j=1; j<term.nterms; j++)
		{	
			if (term.operators[j] == 0)
				out+=calcterm(term.terms[j], ts, temperature);
			if (term.operators[j] == 1)
				out-=calcterm(term.terms[j], ts, temperature);
			if (term.operators[j] == 2)
				out*=calcterm(term.terms[j], ts, temperature);
			if (term.operators[j] == 3)
				out/=calcterm(term.terms[j], ts, temperature);
			if (term.operators[j] == 4)
				out=pow(out,calcterm(term.terms[j], ts, temperature));
		}
		
	}
	if (term.function==true)
	{	if (term.number == 1)
			return exp(out);
		if (term.number == 2)
			return H(out);
		if (term.number == 3)
			return min(calcterm(term.terms[0]), calcterm(term.terms[1],ts, temperature));
		if (term.number == 4)
			return max(calcterm(term.terms[0]), calcterm(term.terms[1],ts, temperature));
		if (term.number == 5)
			return log(out);
		if (term.number == 6)
			return log10(out);
		if (term.number == 7)
			return 1.0/(1.0+exp(-out));
		if (term.number == 8)
			return 0.5*(fabs(out)+out);
		if (term.number == 9)
			return 0.5/calcterm(term.terms[1],ts, temperature)*(calcterm(term.terms[0],ts, temperature)*calcterm(term.terms[1],ts, temperature)+sqrt(pow(calcterm(term.terms[0],ts, temperature)*calcterm(term.terms[1],ts, temperature),2)+1));
		if (term.number == 10)
			return sqrt(out);
	}
	else
		return out;
}

double CRxnNetwork::calcterm(CStringOP &term, int ts, double temperature, vector<CConstituent> &C_prime)
{
	double out = 0;
	if ((term.nterms == 1) && (term.nopts == 0))
	{
		if (term.parameter == true)
			out = param_corrected[term.number];
		else if (term.concentration == true)
			out = H(C_prime[term.number].conc);
		else if (term.constant == true)
			out = term.value;
		else 
			out = calcterm(term.terms[0], ts, temperature, C_prime);
	}

	if ((term.nterms == 1) && (term.nopts == 1))
	{
		if (term.operators[0] == 0)
			out = calcterm(term.terms[0], ts, temperature, C_prime);

		if (term.operators[0] == 1)
			out = -calcterm(term.terms[0], ts, temperature, C_prime);
	}

	if ((term.nterms == 2) && (term.nopts == 1))
	{
		if (term.operators[0] == 0)
			out = calcterm(term.terms[0], ts, temperature, C_prime)+calcterm(term.terms[1], ts, temperature, C_prime);
		if (term.operators[0] == 1)
			out = calcterm(term.terms[0], ts, temperature, C_prime)-calcterm(term.terms[1], ts, temperature, C_prime);
		if (term.operators[0] == 2)
			out = calcterm(term.terms[0], ts, temperature, C_prime)*calcterm(term.terms[1], ts, temperature, C_prime);
		if (term.operators[0] == 3)
			out = calcterm(term.terms[0], ts, temperature, C_prime)/calcterm(term.terms[1], ts, temperature, C_prime);
		if (term.operators[0] == 4)
			out = pow(calcterm(term.terms[0], ts, temperature, C_prime),calcterm(term.terms[1], ts, temperature, C_prime));
	}

	if ((term.nterms == 2) && (term.nopts == 2))
	{
		double sum = 0;
		if (term.operators[0] == 0)
			sum = calcterm(term.terms[0], ts, temperature, C_prime);
		else if (term.operators[0] == 1)
			sum = -calcterm(term.terms[0], ts, temperature, C_prime);
		
		if (term.operators[1] == 0)
			out = sum+calcterm(term.terms[1], ts, temperature, C_prime);
		if (term.operators[1] == 1)
			out = sum-calcterm(term.terms[1], ts, temperature, C_prime);
		if (term.operators[1] == 2)
			out = sum*calcterm(term.terms[1], ts, temperature, C_prime);
		if (term.operators[1] == 3)
			out = sum/calcterm(term.terms[1], ts, temperature, C_prime);
		if (term.operators[1] == 4)
			out = pow(sum,calcterm(term.terms[1], ts, temperature, C_prime));
	}

	if ((term.nterms>2) && (term.nopts == term.nterms-1))
	{	
		out = calcterm(term.terms[0], ts, temperature, C_prime);
		for (int j=1; j<term.nterms; j++)
		{	
			if (term.operators[j-1] == 0)
				out+=calcterm(term.terms[j], ts, temperature, C_prime);
			if (term.operators[j-1] == 1)
				out-=calcterm(term.terms[j], ts, temperature, C_prime);
			if (term.operators[j-1] == 2)
				out*=calcterm(term.terms[j], ts, temperature, C_prime);
			if (term.operators[j-1] == 3)
				out/=calcterm(term.terms[j], ts, temperature, C_prime);
			if (term.operators[j-1] == 4)
				out=pow(out,calcterm(term.terms[j], ts, temperature, C_prime));
		}
		
	}


	if ((term.nterms>2) && (term.nopts == term.nterms))
	{	
		out = 0;
		if (term.operators[0] == 0)	
			out = calcterm(term.terms[0], ts, temperature, C_prime);
		else if (term.operators[0] == 1)
			out = -calcterm(term.terms[0], ts, temperature, C_prime);
	
		
		for (int j=1; j<term.nterms; j++)
		{	
			if (term.operators[j] == 0)
				out+=calcterm(term.terms[j], ts, temperature, C_prime);
			if (term.operators[j] == 1)
				out-=calcterm(term.terms[j], ts, temperature, C_prime);
			if (term.operators[j] == 2)
				out*=calcterm(term.terms[j], ts, temperature, C_prime);
			if (term.operators[j] == 3)
				out/=calcterm(term.terms[j], ts, temperature, C_prime);
			if (term.operators[j] == 4)
				out=pow(out,calcterm(term.terms[j], ts, temperature, C_prime));
		}
		
	}
	if (term.function==true)
	{	if (term.number == 1)
			return exp(out);
		if (term.number == 2)
			return H(out);
		if (term.number == 3)
			return min(calcterm(term.terms[0]), calcterm(term.terms[1],ts, temperature, C_prime));
		if (term.number == 4)
			return max(calcterm(term.terms[0]), calcterm(term.terms[1],ts, temperature, C_prime));
		if (term.number == 5)
			return log(out);
		if (term.number == 6)
			return log10(out);
		if (term.number == 7)
			return 1.0/(1.0+exp(-out));
		if (term.number == 8)
			return 0.5*(fabs(out)+out);
		if (term.number == 9)
			return 0.5/calcterm(term.terms[1],ts, temperature, C_prime)*(calcterm(term.terms[0],ts, temperature, C_prime)*calcterm(term.terms[1],ts, temperature, C_prime)+sqrt(pow(calcterm(term.terms[0],ts, temperature, C_prime)*calcterm(term.terms[1],ts, temperature, C_prime),2)+1));
		if (term.number == 10)
			return sqrt(out);
	}
	else
		return out;
}

void CRxnNetwork::fixed_val(double &c, int cons_num, int ts)
{
	int j = cons_num;	 
	
	if (cons[j].fixed_file == true)
		c = cons[j].fixedts_avg[ts];
				
	if (cons[j].fixed_val == true)
		c = cons[j].value_fixed[ts];		
		
	if (cons[j].fixed_val_ts[ts] == true)
		c = cons[j].value_fixed_ts[ts];		
}

bool CRxnNetwork::fixed_val_check(int j, int ts)
{	
	bool fixed_cons = false;
	if (cons[j].fixed_file == true || cons[j].fixed_val == true || cons[j].fixed_val_ts[ts] == true)
		fixed_cons = true;

	return fixed_cons;
}


CVector CRxnNetwork::influx_rate(vector<CConstituent> &c_star, int ts)
{	
	CVector influx_C(n_constts);  // influx of each constituent 

	for (int j=0; j<n_constts; j++)
	{	
		if ((cons[j].influx) && (cons[j].inlflux_model == 'r'))
			influx_C[j] += cons[j].rate_constant_influx[ts]* (cons[j].equilibrium_conc[ts] - c_star[j].conc);
			
		else if ((cons[j].influx) && (cons[j].inlflux_model == 'a'))
			influx_C[j] += cons[j].rate_constant_influx[ts]/volume; 

		else if ((cons[j].influx) && ((cons[j].inlflux_model == 'z')||(cons[j].inlflux_model == 'x')))
			influx_C[j] += inflows_avg[inflowconsts[j][ts]]/volume;

		else if ((cons[j].influx) && ((cons[j].inlflux_model == 'v')||(cons[j].inlflux_model == 'w')))
			influx_C[j] += cons[j].rate_constant_influx[ts]* (inflows_avg[inflowconsts[j][ts]] - c_star[j].conc);
				
	}	

	return influx_C;	
}


CVector CRxnNetwork::reaction_rate(vector<CConstituent> &c_star, int ts)
{
	CVector rate_C(n_constts);     // rate of each constituent 
	vector<double> rates(n_Rxns);  // rate of each reaction

	for (int i=0; i<n_Rxns; i++)
	{  
		if (temperature.size()==0)
			rates[i] = calcterm(Rxts[i].rate, c_star);
		else
			rates[i] = calcterm(Rxts[i].rate,ts,temperature_avg[ts], c_star);
		
		for (int j=0; j<Rxts[i].nproducts; j++)
			if (temperature.size()==0)
				rate_C[Rxts[i].products[j]] += calcterm(Rxts[i].prodrates[j], c_star)*rates[i]; 
			else
				rate_C[Rxts[i].products[j]] += calcterm(Rxts[i].prodrates[j],ts,temperature_avg[ts],c_star)*rates[i];
	}

	return rate_C;	
}

void CRxnNetwork::onestepbatchsolve(double dt)
{
	vector<CConstituent> c_new = cons;
	for (int i=0; i<n_Rxns; i++)
		for (int j=0; j<Rxts[i].nproducts; j++)
			c_new[Rxts[i].products[j]].conc = cons[Rxts[i].products[j]].conc + dt*calcterm(Rxts[i].prodrates[j])*calcterm(Rxts[i].rate); 

	for (int i=0; i<n_constts; i++) cons[i].conc_0 = cons[i].conc;
	cons = c_new;
	c_new.clear();
}

void CRxnNetwork::setfixedvals(int ts, double t)
{
	for (int i=0; i<n_constts; i++)
	{
		if (cons[i].fixed_file == true)
		{
			if (cons[i].fixedts.size()<ntseris)
				cons[i].conc = cons[i].conc_0 = cons[i].fixedts[0].interpol(t);
			else
				cons[i].conc = cons[i].conc_0 = cons[i].fixedts[ts].interpol(t);
		}
		if (cons[i].fixed_val == true)
		{
			if (cons[i].value_fixed.size()<ntseris) 
				cons[i].conc = cons[i].conc_0 = cons[i].value_fixed[0];
			else
				cons[i].conc = cons[i].conc_0 = cons[i].value_fixed[ts];
		}
		if (cons[i].fixed_val_ts[ts] == true)
			cons[i].conc = cons[i].conc_0 = cons[i].value_fixed_ts[ts];
		
	}
}

void CRxnNetwork::setiniconcs(int ts)
{
	for (int i=0; i<n_constts; i++)
		if (cons[i].set_ini_conc[ts]==true) 
			{cons[i].conc = cons[i].conc_0 = cons[i].ini_conc[ts];} 
}


int CRxnNetwork::look_up_constituent_no(string const_name)
{
	int ii=-1;
	for (int i=0; i<cons.size(); i++)
		if (tolower(cons[i].name) == tolower(const_name)) ii = i;

	return ii;
}

int CRxnNetwork::look_up_rxn_parameters(string S)
{
	int ii = -1;
	for (int i = 0; i<parameters.size(); i++)
		if (parameters[i].name == S) ii = i;

	return ii;
}
