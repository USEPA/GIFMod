#include "stdafx.h"
#include "Connection.h"
//QUI
#include "enums.h"


CConnection::CConnection(void)
{
	fixed = false;
	A=0;
	d=0;
	v=0;
	A_star=0;
	Q_star=0;
	v_star=0;
	Q=0;
	flow_factor=1;
	Q_v=0;
	Q_v_star=0;
	flow_params.resize(30);
	flow_params[z1] = -1000000;
	flow_params[z2] = -1000000;
	flow_params[flow_exponent] = 0.6667;
	dispersivity = 0;
	dispersion_expression = string("f[14]*f[7]/(f[2]+0.00000001)");
	dispersion_strng = "f[14]*f[7]/f[2]";
	settling = -1;
	const_area = true;
	presc_flow = false;
	control = false;
}


CConnection::~CConnection(void)
{
	for (int i=0; i<Solid_phase.size(); i++) Solid_phase[i] = NULL;
}

CConnection::CConnection(const CConnection &CC)
{
	Block1ID = CC.Block1ID;
	Block2ID = CC.Block2ID;
	Block1 = CC.Block1;
	Block2 = CC.Block2;
	A = CC.A;
	d = CC.d;
	v = CC.v;
	v_star = CC.v_star;
	A_star = CC.A_star;
	c_dispersion = CC.c_dispersion;
	c_dispersion_star = CC.c_dispersion_star;
	Q_star = CC.Q_star;
	Q_v = CC.Q_v;
	flow_params = CC.flow_params;
	Q = CC.Q;
	Q_v_star = CC.Q_v_star;
	dispersion = CC.dispersion;
	dispersion_star = CC.dispersion_star;
	Solid_phase_id = CC.Solid_phase_id;
	Solid_phase = CC.Solid_phase;
	flow_expression = CC.flow_expression;
	flow_expression_strng = CC.flow_expression_strng;
	fixed = CC.fixed;
	fixed_val = CC.fixed_val;
	funcs = CC.funcs;
	flow_factor=CC.flow_factor;
	flow_expression_v = CC.flow_expression_v; 
	flow_expression_strng_v = CC.flow_expression_strng_v;
	dispersivity = CC.dispersivity;
	dispersion_expression = CC.dispersion_expression;
	dispersion_strng = CC.dispersion_strng;
	area_expression = CC.area_expression;
	area_expression_strng = CC.area_expression_strng;
	settling = CC.settling;
	const_area = CC.const_area;
	ID = CC.ID;
	presc_flow = CC.presc_flow;
	pre_flow_filename = CC.pre_flow_filename;
	presc_flowrate = CC.presc_flowrate;
	control = CC.control;
	controller_id = CC.controller_id;
}

CConnection& CConnection::operator=(const CConnection &CC)
{
	Block1ID = CC.Block1ID;
	Block2ID = CC.Block2ID;
	Block1 = CC.Block1;
	Block2 = CC.Block2;
	A = CC.A;
	d = CC.d;
	v = CC.v;
	v_star = CC.v_star;
	A_star = CC.A_star;
	c_dispersion = CC.c_dispersion;
	Q_star = CC.Q_star;
	Q_v = CC.Q_v;
	Q_v_star = CC.Q_v_star;
	flow_params = CC.flow_params;
	Q = CC.Q;
	Solid_phase_id = CC.Solid_phase_id;
	Solid_phase = CC.Solid_phase;
	dispersion_star = CC.dispersion_star;
	dispersion = CC.dispersion;
	c_dispersion_star = CC.c_dispersion_star;
	flow_expression = CC.flow_expression;
	flow_expression_strng = CC.flow_expression_strng;
	fixed = CC.fixed;
	fixed_val = CC.fixed_val;
	funcs = CC.funcs;
	flow_factor=CC.flow_factor;
	flow_expression_v = CC.flow_expression_v; 
	flow_expression_strng_v = CC.flow_expression_strng_v;
	dispersivity = CC.dispersivity;
	dispersion_expression = CC.dispersion_expression;
	dispersion_strng = CC.dispersion_strng;
	area_expression = CC.area_expression;
	area_expression_strng = CC.area_expression_strng;
	settling = CC.settling;
	const_area = CC.const_area;
	ID = CC.ID;
	presc_flow = CC.presc_flow;
	pre_flow_filename = CC.pre_flow_filename;
	presc_flowrate = CC.presc_flowrate;
	control = CC.control;
	controller_id = CC.controller_id;
	return *this;

}

double CConnection::calc(CStringOP &term, int ii) //The function to calculate any expression
{
	double out = 0;
	if ((term.nterms == 1) && (term.nopts == 0))
	{
		if (term.parameter == true)
			out = 0.5*(Block1->rxn_params[term.number-2000]+Block2->rxn_params[term.number-2000]);
		else if (term.concentration == true)
			if (Q>0) out = Block1->CG[term.number][term.phase]; else out = Block2->CG[term.number][term.phase];
		else if (term.s_concentration == true)
			if (term.number == -1) 
				if (Q>0) out = Block1->G[ii][term.phase]; else out = Block2->G[ii][term.phase];
			else
				if (Q>0) out = Block1->G[term.number][term.phase]; else out = Block2->G[term.number][term.phase];
		else if (term.physical_ch == true)
			out = get_val(term.number,ii);
		else if (term.constant == true)
			out = term.value;
		else if (term.s_block == true)
		{
			vector<int> jj; jj.push_back(ii);
			out = Block1->get_val(term.number, jj);
		}
		else if (term.t_block == true)
		{
			vector<int> jj; jj.push_back(ii);
			out = Block2->get_val(term.number, jj);
		}
		else 
			out = calc(term.terms[0],ii);
	}

	if ((term.nterms == 1) && (term.nopts == 1))
	{
		if (term.operators[0] == 0)
			out = calc(term.terms[0],ii);

		if (term.operators[0] == 1)
			out = -calc(term.terms[0],ii);
	}

	if ((term.nterms == 2) && (term.nopts == 1))
	{
		if (term.operators[0] == 0)
			out = calc(term.terms[0],ii)+calc(term.terms[1],ii);
		if (term.operators[0] == 1)
			out = calc(term.terms[0],ii)-calc(term.terms[1],ii);
		if (term.operators[0] == 2)
			out = calc(term.terms[0],ii)*calc(term.terms[1],ii);
		if (term.operators[0] == 3)
		{
			double a = calc(term.terms[0], ii);
			double b = calc(term.terms[1], ii);
			if ((a == 0) && (b == 0))
				out = 0;
			else
				out = a / b;

		}
		if (term.operators[0] == 4)
			out = pow(calc(term.terms[0],ii),calc(term.terms[1],ii));
	}

	if ((term.nterms == 2) && (term.nopts == 2))
	{
		double sum = 0;
		if (term.operators[0] == 0)
			sum = calc(term.terms[0],ii);
		else if (term.operators[0] == 1)
			sum = -calc(term.terms[0],ii);
		
		if (term.operators[1] == 0)
			out = sum+calc(term.terms[1],ii);
		if (term.operators[1] == 1)
			out = sum-calc(term.terms[1],ii);
		if (term.operators[1] == 2)
			out = sum*calc(term.terms[1],ii);
		if (term.operators[1] == 3)
			out = sum/calc(term.terms[1],ii);
		if (term.operators[1] == 4)
			out = pow(fabs(sum),calc(term.terms[1],ii))*fabs(sum)/sum;
		
	}

	if ((term.nterms>2) && (term.nopts == term.nterms-1))
	{	
		out = calc(term.terms[0],ii);
		for (int j=1; j<term.nterms; j++)
		{	
			if (term.operators[j-1] == 0)
				out+=calc(term.terms[j],ii);
			if (term.operators[j-1] == 1)
				out-=calc(term.terms[j],ii);
			if (term.operators[j-1] == 2)
				out*=calc(term.terms[j],ii);
			if (term.operators[j - 1] == 3)
			{
				double a = calc(term.terms[j], ii);
				if ((out == 0) && (a == 0))
					out = 0;
				else
					out /= a;
			}
			if (term.operators[j-1] == 4)
				out=pow(out,calc(term.terms[j],ii));
		}
		
	}


	if ((term.nterms>2) && (term.nopts == term.nterms))
	{	
		out = 0;
		if (term.operators[0] == 0)	
			out = calc(term.terms[0],ii);
		else if (term.operators[0] == 1)
			out = -calc(term.terms[0],ii);
	
		
		for (int j=1; j<term.nterms; j++)
		{	
			if (term.operators[j] == 0)
				out+=calc(term.terms[j],ii);
			if (term.operators[j] == 1)
				out-=calc(term.terms[j],ii);
			if (term.operators[j] == 2)
				out*=calc(term.terms[j],ii);
			if (term.operators[j] == 3)
				out/=calc(term.terms[j],ii);
			if (term.operators[j] == 4)
				out=pow(out,calc(term.terms[j],ii));
		}
		
	}
		if (term.function==true)
	{	if (term.number == exp_)
			return exp(out);
		if (term.number == hsd_)
			return Heavyside(out);
		if (term.number == min_)
			return min(calc(term.terms[0],ii), calc(term.terms[1],ii));
		if (term.number == max_)
			return max(calc(term.terms[0],ii), calc(term.terms[1],ii));
		if (term.number == lne_)
			return log(out);
		if (term.number == lnt_)
			return log10(out);
		if (term.number == sgm_)
			return 1.0/(1.0+exp(-out));
		if (term.number == pos_)
			return 0.5*(fabs(out)+out);
		if (term.number == sq1_)
			return 0.5/calc(term.terms[1],ii)*(calc(term.terms[0],ii)*calc(term.terms[1],ii)+sqrt(pow(calc(term.terms[0],ii)*calc(term.terms[1],ii),2)+1));
		if (term.number == sqr_)
			return sqrt(out);
		if (term.number == frs_)
			return funcs[0].evaluate(get_val(9));
		if (term.number == fas_)
			return funcs[0].evaluate(get_val(4));
		if (term.number == ply_)
			return pipe_poly(out);
		if (term.number==mon_)
			return mon(calc(term.terms[0],ii),calc(term.terms[1],ii));
		if (term.number == sq2_)
		{	double term1 = calc(term.terms[0],ii);
			double term2 = calc(term.terms[1],ii);
			return pow(term1,(0.5*term1+term2)/(term1+term2));
		}
		if (term.number==abs_)
			return fabs(out);
		if (term.number==sqs_)
			if (out!=0)
				return out/fabs(out)*sqrt(fabs(out));
			else
				return 0;
		if (term.number = mo1_)
			return mon(calc(term.terms[0], ii), calc(term.terms[1], ii))*calc(term.terms[0], ii) + mon(-calc(term.terms[0], ii), calc(term.terms[1], ii))*calc(term.terms[0], ii);

	}

	else
		return out;
}

double CConnection::calc_star(CStringOP &term, int ii)
{
	double out = 0;
	if ((term.nterms == 1) && (term.nopts == 0))
	{
		if (term.parameter == true)
			out = 0.5*(Block1->rxn_params[term.number-2000]+Block2->rxn_params[term.number-2000]);
		else if (term.concentration == true)
			if (Q>0) out = Block1->CG_star[term.number][term.phase]; else out = Block2->CG_star[term.number][term.phase];
		else if (term.s_concentration == true)
			if (term.number == -1) 
				if (Q>0) out = Block1->G_star[ii][term.phase]; else out = Block2->G_star[ii][term.phase];
			else
				if (Q>0) out = Block1->G_star[term.number][term.phase]; else out = Block2->G_star[term.number][term.phase];
		else if (term.physical_ch == true)
			out = get_val_star(term.number,ii);
		else if (term.constant == true)
			out = term.value;
		else if (term.s_block == true)
		{
			vector<int> jj;
			jj.push_back(ii);
			out = Block1->get_val_star(term.number, jj);
		}
		else if (term.t_block == true)
		{
			vector<int> jj;
			jj.push_back(ii);
			out = Block2->get_val_star(term.number, jj);
		}
		else if (term.a_block == true)
		{
			vector<int> jj;
			jj.push_back(ii);
			out = 0.5*(Block1->get_val_star(term.number, jj) + Block1->get_val_star(term.number, jj));
		}
		else
		{
			out = calc_star(term.terms[0], ii);
		}
	}

	if ((term.nterms == 1) && (term.nopts == 1))
	{
		if (term.operators[0] == 0)
			out = calc_star(term.terms[0],ii);

		if (term.operators[0] == 1)
			out = -calc_star(term.terms[0],ii);
	}

	if ((term.nterms == 2) && (term.nopts == 1))
	{
		if (term.operators[0] == 0)
			out = calc_star(term.terms[0],ii)+calc_star(term.terms[1],ii);
		if (term.operators[0] == 1)
			out = calc_star(term.terms[0],ii)-calc_star(term.terms[1],ii);
		if (term.operators[0] == 2)
			out = calc_star(term.terms[0],ii)*calc_star(term.terms[1],ii);
		if (term.operators[0] == 3)
		{
			double a = calc_star(term.terms[0], ii);
			double b = calc_star(term.terms[1], ii);
			if ((a == 0) && (b == 0))
				out = 0;
			else
				out = a / b;
		
		}
		if (term.operators[0] == 4)
			out = pow(calc_star(term.terms[0],ii),calc_star(term.terms[1],ii));

	}

	if ((term.nterms == 2) && (term.nopts == 2))
	{
		double sum = 0;
		if (term.operators[0] == 0)
			sum = calc_star(term.terms[0],ii);
		else if (term.operators[0] == 1)
			sum = -calc_star(term.terms[0],ii);
		
		if (term.operators[1] == 0)
			out = sum+calc_star(term.terms[1],ii);
		if (term.operators[1] == 1)
			out = sum-calc_star(term.terms[1],ii);
		if (term.operators[1] == 2)
			out = sum*calc_star(term.terms[1],ii);
		if (term.operators[1] == 3)
			out = sum/calc_star(term.terms[1],ii);
		if (term.operators[1] == 4)
			out = pow(fabs(sum),calc_star(term.terms[1],ii))*fabs(sum)/sum;

	}

	if ((term.nterms>2) && (term.nopts == term.nterms-1))
	{	
		out = calc_star(term.terms[0],ii);
		for (int j = 1; j < term.nterms; j++)
		{
			if (term.operators[j - 1] == 0)
				out += calc_star(term.terms[j], ii);
			if (term.operators[j - 1] == 1)
				out -= calc_star(term.terms[j], ii);
			if (term.operators[j - 1] == 2)
				out *= calc_star(term.terms[j], ii);
			if (term.operators[j - 1] == 3)
				out /= calc_star(term.terms[j], ii);
			if (term.operators[j - 1] == 4)
			{
				double a = calc_star(term.terms[j], ii);
				if ((out == 0) && (a == 0))
					out = 0;
				else
					out /= a;
			}
		}
	}


	if ((term.nterms>2) && (term.nopts == term.nterms))
	{	
		out = 0;
		if (term.operators[0] == 0)	
			out = calc_star(term.terms[0],ii);
		else if (term.operators[0] == 1)
			out = -calc_star(term.terms[0],ii);
	
		
		for (int j=1; j<term.nterms; j++)
		{	
			if (term.operators[j] == 0)
				out+=calc_star(term.terms[j],ii);
			if (term.operators[j] == 1)
				out-=calc_star(term.terms[j],ii);
			if (term.operators[j] == 2)
				out*=calc_star(term.terms[j],ii);
			if (term.operators[j] == 3)
				out/=calc_star(term.terms[j],ii);
			if (term.operators[j] == 4)
				out=pow(out,calc_star(term.terms[j],ii));

		}
		
	}
	if (term.function==true)
	{	if (term.number == exp_)
			return exp(out);
		if (term.number == hsd_)
			return Heavyside(out);
		if (term.number == min_)
			return min(calc_star(term.terms[0],ii), calc_star(term.terms[1],ii));
		if (term.number == max_)
			return max(calc_star(term.terms[0],ii), calc_star(term.terms[1],ii));
		if (term.number == lne_)
			return log(out);
		if (term.number == lnt_)
			return log10(out);
		if (term.number == sgm_)
			return 1.0/(1.0+exp(-out));
		if (term.number == pos_)
			return 0.5*(fabs(out)+out);
		if (term.number == sq1_)
			return 0.5/calc_star(term.terms[1],ii)*(calc_star(term.terms[0],ii)*calc_star(term.terms[1],ii)+sqrt(pow(calc_star(term.terms[0],ii)*calc_star(term.terms[1],ii),2)+1));
		if (term.number == sqr_)
			return sqrt(out);
		if (term.number == frs_)
			return funcs[0].evaluate(get_val_star(9));
		if (term.number == fas_)
			return funcs[0].evaluate(get_val_star(4));
		if (term.number == ply_)
			return pipe_poly(out);
		if (term.number==mon_)
			return mon(calc_star(term.terms[0],ii),calc_star(term.terms[1],ii));
		if (term.number == sq2_)
		{	double term1 = calc_star(term.terms[0],ii);
			int sign_ = sgn(term1);
			double term2 = calc_star(term.terms[1],ii);
			return double(sign_)*pow(fabs(term1),(0.5*term1+term2)/(term1+term2));
		}
		if (term.number==abs_)
			return fabs(out);
		if (term.number == sqs_)
		{
			if (out != 0)
				return out / fabs(out)*sqrt(fabs(out));
			else
				return 0;
		}
		if (term.number = mo1_)
			return mon(calc_star(term.terms[0], ii), calc_star(term.terms[1], ii))*calc_star(term.terms[0], ii) + mon(-calc_star(term.terms[0], ii), calc_star(term.terms[1], ii))*calc_star(term.terms[0], ii);

		
	}

	else
		return out;
}

double CConnection::get_val(int i, int ii)
{
	/* variable codes: 
	H: 1
	A: 2
	V: 3
	S: 4
	z0: 5
	d: 6
	Q: 7
	v: 8
	Hydraulic Parameters: 50-99
	G: 101-199
	GS: 1000-1999
	Reaction parameters: 2000-2999
	*/

	if (i==1)
	{	if (Q>0)
			return Block1->H;
		else
			return Block2->H;
	}
	if (i==2) return A;
	if (i==3)
	{	if (Q>0)
		return Block1->V;
	else
		return Block2->V;
	}
	if (i==4)
	{	if (Q>0)
		return Block1->S;
	else
		return Block2->S;
	}
	if (i==5)
	{	if (Q>0)
		return Block1->z0;
	else
		return Block2->z0;
	}
	if (i==6) return d;
	if (i==7) return Q;
	if (i==8) return (Q-Q_v)/A*flow_factor;  

	if (i==9)
	{
		if (Block1->indicator==Soil) 
		{
			if (Block2->indicator==Soil)
			{
				vector<int> jj;
				jj.push_back(ii);
				return 0.5*(Block1->get_val(9,jj)+Block2->get_val(9,jj));
			}
			else 
			{
				vector<int> jj;
				jj.push_back(ii);
				return Block1->get_val(9,jj);
			}
		}
		else
		{
			if (Block2->indicator==Soil) 
			{
				vector<int> jj;
				jj.push_back(ii);
				return Block2->get_val(9,jj);
			}
			else 
			{
				return 0;   //error output should be displayed here
			}
		}
	}
	//if (i==11) return Q*flow_factor;  
	if (i==13) return Q_v;
	if (i==14) return dispersivity;
	
	if ((i>=50) && (i<100)) return flow_params[i-50];
	
	if (i>=100 && i<2000) 
	{	if (Q>0)
		return Block1->get_val(i);
	else
		return Block2->get_val(i);
	}
	if (i>=3000 && i<4000) return Solid_phase[ii]->c_params[i-3000];
	if (i==4000) return c_dispersion[i-4000];
	if ((i>=5000) && (i<5100))
		return dispersion[i-5000];
}

double CConnection::get_val_star(int i, int ii)
{
	/* variable codes: 
	H: 1
	A: 2
	V: 3
	S: 4
	z0: 5
	d: 6
	Q: 7
	v: 8
	Hydraulic Parameters: 50-99
	G: 101-199
	GS: 1000-1999
	Reaction parameters: 2000-2999
	*/

	if (i==1)
	{	if (Q>0)
			return Block1->H_star;
		else
			return Block2->H_star;
	}
	if (i==2) return A_star;
	if (i==3)
	{	if (Q>0)
		return Block1->V;
	else
		return Block2->V;
	}
	if (i==4)
	{	if (Q>0)
		return Block1->S_star;
	else
		return Block2->S_star;
	}
	if (i==5)
	{	if (Q>0)
		return Block1->z0;
	else
		return Block2->z0;
	}
	if (i==6) return d;
	if (i==7) return Q_star;
	if (i==8) return (Q_star-Q_v_star)/A_star*flow_factor;  
	
	if (i==9)
	{
		if (Block1->indicator==0) 
		{
			if (Block2->indicator==0)
			{
				return 0.5*(Block1->get_val_star(9)+Block2->get_val_star(9));
			}
			else 
			{
				return Block1->get_val_star(9);
			}
		}
		else
		{
			if (Block2->indicator==0) 
			{
				return Block2->get_val_star(9);
			}
			else 
			{
				return 0;   //error output should be displayed here
			}
		}
	}
	
	
	if (i==13) return Q_v_star;
	if (i==14) return dispersivity;
	
	if ((i>=50) && (i<100)) return flow_params[i-50];
	if (i>=100 && i<2000) 
	{	if (Q_star>0)
		return Block1->get_val_star(i);
	else
		return Block2->get_val_star(i);
	}
	
	if (i>=3000 && i<4000) return Solid_phase[ii]->c_params[i-3000];
	if (i==4000) return c_dispersion_star[i-4000];
	if ((i>=5000) && (i<5100)) 	return dispersion_star[i-5000];
	
	
}

void CConnection::set_val(string SS, double val)
{
	vector<string> s = split(SS,',');
	if (s.size()==1)
	{
		if (tolower(trim(s[0]))=="a") A = val;
		if (tolower(trim(s[0]))=="d") d = val;
		if (tolower(trim(s[0]))=="q") Q = val;
		if (tolower(trim(s[0]))=="vel") v = val;
			
		if (tolower(trim(s[0]))=="a*") A_star = val;
		if (tolower(trim(s[0]))=="q*") Q_star = val;
		if (tolower(trim(s[0]))=="vel*") v_star = val;

		if (tolower(trim(s[0]))=="width") flow_params[width] = val;	
		if (tolower(trim(s[0]))=="nmanning") flow_params[n_manning] = val;	
		if (tolower(trim(s[0]))=="flow_exponent") flow_params[flow_exponent] = val;	


		if (tolower(trim(s[0]))=="ks") flow_params[ks] = val;	
		if (tolower(trim(s[0]))=="theta_s") flow_params[theta_s] = val;
		if (tolower(trim(s[0]))=="theta_r") flow_params[theta_r] = val;
		if (tolower(trim(s[0]))=="vg_alpha") flow_params[vg_alpha] = val;
		if (tolower(trim(s[0]))=="vg_n") flow_params[vg_n] = val;
		if (tolower(trim(s[0]))=="vg_m") flow_params[vg_m] = val;
		if (tolower(trim(s[0]))=="lambda") flow_params[lambda] = val;

		if (tolower(trim(s[0]))=="z1") flow_params[z1] = val;	
		if (tolower(trim(s[0]))=="z2") flow_params[z2] = val;
		if (tolower(trim(s[0])) == "dam")
		{
			flow_params[z1] = val;
			flow_params[z2] = val;
		}
		if (tolower(trim(s[0])) == "rating_curve_coeff") flow_params[rating_curve_coeff] = val;
		if (tolower(trim(s[0])) == "rating_curve_power") flow_params[rating_curve_power] = val;
		if (tolower(trim(s[0])) == "rating_curve_datum") 
			flow_params[rating_curve_datum] = val;
		
		if (tolower(trim(s[0]))=="diameter") 
		{
			flow_params[2] = val;
			A_star=atan(1.0)*4*pow(val/2,2);
			A=atan(1.0)*4*pow(val/2,2);
		}
		if (tolower(trim(s[0]))=="pipe_c") flow_params[pipe_c] = val;

		if (tolower(trim(s[0]))=="q_v*") Q_v_star = val; 
		if (tolower(trim(s[0]))=="q_v") Q_v = val;		 
		if (tolower(trim(s[0]))=="dispersivity") dispersivity = val;		 
	}
	
}


void CConnection::get_funcs(CStringOP &term)  //Works w/o reference(&)
{
	double out = 0;
	if ((term.nterms == 1) && (term.nopts == 0))
	{
		if (term.terms.size()>0)
			get_funcs(term.terms[0]);
	}

	if ((term.nterms == 2) && (term.nopts == 1))
	{
		get_funcs(term.terms[0]);
		get_funcs(term.terms[1]);
	}

	if ((term.nterms == 2) && (term.nopts == 2))
	{
		get_funcs(term.terms[0]);
		get_funcs(term.terms[1]);
		
	}

	if ((term.nterms>2) && (term.nopts == term.nterms-1))
	{	
		for (int j=0; j<term.nterms; j++)
			get_funcs(term.terms[j]);
	}


	if ((term.nterms>2) && (term.nopts == term.nterms))
	{	
		for (int j=0; j<term.nterms; j++)
			get_funcs(term.terms[j]);
		
	}
	if (term.function==true) 
		if (term.number == 11)
		{	CFunction XX;
			XX.Expression = term;
			XX.Expression.function=false;
			XX.var_id = 0;
			XX._min=-0.3;
			XX._max=1.3;
			funcs.push_back(XX);
		}
		else if (term.number==12)
		{	CFunction XX;
			XX.Expression = term;
			XX.Expression.function=false;
			XX.var_id = 1;
			XX._min = -0.3*max(Block1->V,Block2->V);
			XX._max = 1.3*max(Block1->V,Block2->V);
			funcs.push_back(XX);
		}

}

void CConnection::evaluate_functions(int i) //i=0->small s; i=1->large S
{
	funcs[i].X.clear();
	funcs[i].X.structured = true;
		
	for (double x=funcs[i]._min; x<=funcs[i]._max; x+=(funcs[i]._max-funcs[i]._min)/double(funcs[i].n_steps))
	{	if (funcs[i].var_id==0)
		{	Block1->set_val_star(9,x);
			Block2->set_val_star(9,x);
		}
		if (funcs[i].var_id==1)
		{	Block1->set_val_star(4,x);
			Block2->set_val_star(4,x);
		}
				
		funcs[i].X.append(x,calc_star(funcs[i].Expression));
	}

	funcs[i].X.structured = true;		

}

void CConnection::evaluate_functions() 
{
	for (int i=0; i<funcs.size(); i++) evaluate_functions(i);
}

void CConnection::evaluate_dispersion()
{
	for (int i=0; i<Solid_phase.size(); i++)
		c_dispersion[i] = calc(Solid_phase[i]->dispersion, i) + Solid_phase[i]->diffusion;
	
}

void CConnection::evaluate_dispersion_star()
{
	for (int i=0; i<Solid_phase.size(); i++)
		c_dispersion_star[i] = calc_star(Solid_phase[i]->dispersion, i) + Solid_phase[i]->diffusion;
	
}

void CConnection::evaluate_const_dispersion()
{
	for (int i=0; i<RXN->cons.size(); i++)
		dispersion[i] = calc(dispersion_expression)+RXN->cons[i].diffusion;
	
}

void CConnection::evaluate_const_dispersion_star()
{
	for (int i=0; i<RXN->cons.size(); i++)
		dispersion_star[i] = calc_star(dispersion_expression)+RXN->cons[i].diffusion;
}

double CConnection::get_val(string S)
{
	/* variable codes:
	H: 1
	A: 2
	V: 3
	S: 4
	z0: 5
	d: 6
	Q: 7
	v: 8
	Hydraulic Parameters: 50-99
	G: 101-199
	GS: 1000-1999
	Reaction parameters: 2000-2999
	*/

	if (tolower(S) == "h")
	{
		if (Q>0)
			return Block1->H;
		else
			return Block2->H;
	}
	if (tolower(S) == "a") return A;
	if (tolower(S) == "volume")
	{
		if (Q>0)
			return Block1->V;
		else
			return Block2->V;
	}
	if (tolower(S) == "s")
	{
		if (Q>0)
			return Block1->S;
		else
			return Block2->S;
	}
	if (tolower(S) == "z0")
	{
		if (Q>0)
			return Block1->z0;
		else
			return Block2->z0;
	}
	if (tolower(S) == "d") return d;
	if (tolower(S) == "q") return Q*flow_factor;;
	if (tolower(S) == "q_net") return (Q - Q_v) / A*flow_factor;

	if (tolower(S) == "se")
	{
		if (Block1->indicator == Soil)
		{
			if (Block2->indicator == Soil)
			{
				return 0.5*(Block1->get_val("se") + Block2->get_val("se"));
			}
			else
			{
				return Block1->get_val("se");
			}
		}
		else
		{
			if (Block2->indicator == Soil)
			{
				return Block2->get_val("se");
			}
			else
			{
				return 0;   //error output should be displayed here
			}
		}
	}
	
	if (tolower(S) == "qv") return Q_v;
	if (tolower(S) == "dispersivity") return dispersivity;
		
}
