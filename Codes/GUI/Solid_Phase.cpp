#include "Solid_Phase.h"


CSolid_Phase::CSolid_Phase(int n)
{
	n_phases = 3;
	f.resize(n_phases);
	K.resize(n_phases);
	for (int i=0; i<n_phases; i++) K[i].resize(n_phases);
	mobility_factor.resize(n_phases);
	capacity.resize(n_phases);
	c_params.resize(30);
	dispersion=CStringOP("f[3011]*_abs(f[8])");
	mobility_factor.resize(3);
	mobility_factor[0]=1;
	K[0][1]=CStringOP("f[3003]*f[3004]*f[3005]*f[3006]*_abs(f[8])*(f[3007]-g[1]-g[2])/f[3007]") ;  
	K[0][2]=CStringOP("(1-f[3003])*f[3004]*f[3005]*f[3006]*_abs(f[8])*(f[3007]-g[1]-g[2])/f[3007]") ;  //abs was added
	K[2][0]=CStringOP("f[3004]*f[3008]*_pos(_abs(f[8])-f[3009])") ;  
	capacity[0]=CStringOP("_max((f[4]+(f[3002]*f[15])):0.0000000001)");
	capacity[1]=CStringOP("f[3010]*f[3]");
	capacity[2]=CStringOP("f[3010]*f[3]");
	vs = 0;
	diffusion = 0;
	
}

CSolid_Phase::CSolid_Phase(string m)
{
	/*
	"single_phase": Only mobile equilibrium with air
	"dual_phase_lm": Mobile and reversible attached equilibrium with air
	"triple_phase_lm": Mobile, reversible and irreversible attachment equilibrium with air
	"triple_phase_eq_air_lm":Mobile, reversible and irreversible attachment and equilibrium with air
	*/
	model = m;
	dispersion=CStringOP("f[3011]*_abs(f[8])");
	vs = 0;
	c_params.resize(30);
	c_params[7] = 1e12; // jamming limit

	if (model=="single_phase")
	{
		n_phases = 1;
		phase_names.resize(n_phases);
		f.resize(n_phases);
		K.resize(n_phases);
		for (int i=0; i<n_phases; i++) K[i].resize(n_phases);
		mobility_factor.resize(n_phases);
		capacity.resize(n_phases);
		capacity_c.resize(n_phases);

		mobility_factor.resize(n_phases);
		mobility_factor[0]=1;
		capacity[0] = CStringOP("_max((f[4]+(f[3002]*f[15])):0.0000000001)");
		capacity_c[0]=CStringOP("_max((g[0]):0.0000000000000000001)");
		phase_names[0] = "mobile";

	}

	if ((model == "dual_phase_lm") || (model == "double_phase_lm"))
	{
		n_phases = 2;
		phase_names.resize(n_phases);
		f.resize(n_phases);
		K.resize(n_phases);
		for (int i=0; i<n_phases; i++) K[i].resize(n_phases);
		mobility_factor.resize(n_phases);
		capacity.resize(n_phases);
		capacity_c.resize(n_phases);
		c_params[3] = 1;

		mobility_factor.resize(n_phases);
		mobility_factor[0]=1;
		K[0][1]=CStringOP("f[3003]*f[3004]*f[3005]*f[3006]*_abs(f[8])*(f[3007]-g[1])/f[3007]") ;  
		K[1][0]=CStringOP("f[3004]*f[3008]*_pos(_abs(f[8])-f[3009])") ;  
		capacity[0] = CStringOP("_max((f[4]+(f[3002]*f[15])):0.0000000001)");
		capacity[1] = CStringOP("f[3010]*f[3]"); 
		capacity_c[0]=CStringOP("_max((g[0]):0.0000000000000000001)");
		capacity_c[1]=CStringOP("_max((g[1]):0.0000000000000000001)");
		phase_names[0] = "mobile";
		phase_names[1] = "attached";
	}

	if (model=="triple_phase_lm")
	{
		n_phases = 3;
		phase_names.resize(n_phases);
		f.resize(n_phases);
		K.resize(n_phases);
		for (int i=0; i<n_phases; i++) K[i].resize(n_phases);
		mobility_factor.resize(n_phases);
		capacity.resize(n_phases);

		mobility_factor.resize(n_phases);
		capacity_c.resize(n_phases);
		mobility_factor[0]=1;
		K[0][1]=CStringOP("f[3003]*f[3004]*f[3005]*f[3006]*_abs(f[8])*(f[3007]-g[1]-g[2])/f[3007]") ;  
		K[0][2]=CStringOP("(1-f[3003])*f[3004]*f[3005]*f[3006]*_abs(f[8])*(f[3007]-g[1]-g[2])/f[3007]") ;  //abs was added
		K[2][0]=CStringOP("f[3004]*f[3008]*_pos(_abs(f[8])-f[3009])") ;  
		capacity[0] = CStringOP("_max((f[4]+(f[3002]*f[15])):0.0000000001)");
		capacity[1]=CStringOP("f[3010]*f[3]");
		capacity[2]=CStringOP("f[3010]*f[3]");
		capacity_c[0]=CStringOP("_max((g[0]):0.0000000000000000001)");
		capacity_c[1]=CStringOP("_max((g[1]):0.0000000000000000001)");
		capacity_c[2]=CStringOP("_max((g[2]):0.0000000000000000001)");
		phase_names[0] = "mobile";
		phase_names[1] = "ir_attached";
		phase_names[2] = "r_attached";
	}

	if (model=="triple_phase_eq_air_lm")
	{
		n_phases = 3;
		f.resize(n_phases);
		K.resize(n_phases);
		phase_names.resize(n_phases);
		phase_names.resize(n_phases);
		for (int i=0; i<n_phases; i++) K[i].resize(n_phases);
		mobility_factor.resize(n_phases);
		capacity.resize(n_phases);
		capacity_c.resize(n_phases);

		mobility_factor.resize(n_phases);
		mobility_factor[0]=1;
		K[0][1]=CStringOP("f[3003]*f[3004]*f[3005]*f[3006]*_abs(f[8])*(f[3007]-g[1]-g[2])/f[3007]") ;  
		K[0][2]=CStringOP("(1-f[3003])*f[3004]*f[3005]*f[3006]*_abs(f[8])*(f[3007]-g[1]-g[2])/f[3007]") ;  //abs was added
		K[2][0]=CStringOP("f[3004]*f[3008]*_pos(_abs(f[8])-f[3009])") ;  
		capacity[0] = CStringOP("_max((f[4]+(f[3002]*f[15])):0.0000000001)");
		capacity[1]=CStringOP("f[3010]*f[3]");
		capacity[2]=CStringOP("f[3010]*f[3]");
		capacity_c[0]=CStringOP("_max((g[0]):0.0000000000000000001)");
		capacity_c[1]=CStringOP("_max((g[1]):0.0000000000000000001)");
		capacity_c[2]=CStringOP("_max((g[2]):0.0000000000000000001)");
		phase_names[0] = "mobile";
		phase_names[1] = "ir_attached";
		phase_names[2] = "r_attached";
	}
	
}


CSolid_Phase::~CSolid_Phase(void)
{
}


CSolid_Phase::CSolid_Phase(const CSolid_Phase& BB)
{
	capacity = BB.capacity;
	capacity_c = BB.capacity_c;
	diffusion = BB.diffusion;
	f = BB.f;
	K = BB.K;
	mobility_factor = BB.mobility_factor;
	c_params = BB.c_params;
	n_phases = BB.n_phases;
	model = BB.model;
	name = BB.name;
	phase_names = BB.phase_names;
	vs = BB.vs;

}



CSolid_Phase& CSolid_Phase::operator=(const CSolid_Phase &BB)
{
	capacity = BB.capacity;
	capacity_c = BB.capacity_c;
	diffusion = BB.diffusion;
	f = BB.f;
	K = BB.K;
	mobility_factor = BB.mobility_factor;
	c_params = BB.c_params;
	n_phases = BB.n_phases;
	model = BB.model;
	name = BB.name;
	phase_names = BB.phase_names;
	vs = BB.vs;
	return *this;
}



void CSolid_Phase::set_val(int i, double val)
{
	c_params[i-3000] = val;
	
}

void CSolid_Phase::set_val(string SS, double val)
{
	vector<char> del;
	del.push_back('[');
	del.push_back(']');
	vector<string> s = split(SS,del);
	
	if (s.size()==1)  
	{ 
		if (tolower(trim(s[0]))=="kair") c_params[kair] = val;              //ka    
		if (tolower(trim(s[0]))=="irrevcolfraction")  c_params[irrevcolfraction]= val;  //betai
		if (tolower(trim(s[0]))=="specsurfarea")  c_params[specsurfarea]= val;      //f
		if (tolower(trim(s[0]))=="attachmenteff") c_params[attachmenteff] = val;     //alphap
		if (tolower(trim(s[0]))=="collectioneff") c_params[collectioneff] = val ;    //eta
		if (tolower(trim(s[0]))=="gmax") c_params[gmax] = val;              //maximum jamming limit
		if (tolower(trim(s[0]))=="releasecoeff") c_params[releasecoeff]=val;        //alphahm
		if (tolower(trim(s[0]))=="critvel") c_params[critvel]=val;             //vc
		if (tolower(trim(s[0]))=="bulkdensity") c_params[bulkdensity]=val;       //Bd
		if (tolower(trim(s[0]))=="dispersivity") c_params[dispersivity]=val;      //alphaD
		if (tolower(trim(s[0])) == "vs") vs = val;
		if (tolower(trim(s[0])) == "diffusion") diffusion = val;

	}

}

int CSolid_Phase::lookup_phase(string S)
{
	int out = -1;
	if (S == "") return out=0;
	for (int i = 0; i < phase_names.size(); i++)
		if (tolower(S) == tolower(phase_names[i]))
			out = i;

	return out;
}


