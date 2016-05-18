#include "MediumSet.h"



CMediumSet::CMediumSet()
{
}


CMediumSet::~CMediumSet()
{
	ANS_colloids.clear();
	ANS_constituents.clear();
	ANS_hyd.clear();
}

CMediumSet::CMediumSet(string filename)
{
	
	CLIDconfig lid(filename);
	
	int n_experiments = lid.lookupkeyword("n_experiments");
	vector<string> names = lid.get_experiment_names();
	if (lid.lookupkeyword("n_experiments") != -1)
		Medium.resize(atoi(lid.value[lid.lookupkeyword("n_experiments")].c_str()));
	else if (names.size()>0)
		Medium.resize(names.size());
	else
		Medium.resize(1);
	
	for (int i = 0; i < Medium.size(); i++)
	{
		if (names.size() == 0)
			Medium[i].name = numbertostring(i);
		else
			Medium[i].name = names[i];
	}

	set_default();
	f_get_environmental_params(lid);
	f_get_params(lid);
	f_get_observed(lid);
	f_get_particle_types(lid);
	f_get_constituents(lid);
	f_get_reactions(lid);
	f_get_buildup(lid);
	f_get_external_flux(lid);
	f_get_evaporation_model(lid);

	

	vector<CLIDconfig> L = lid.extract_subsets();
	
	for (int i = 0; i < Medium.size(); i++)
		Medium[i].create(L[i], this);
	
}

CMediumSet::CMediumSet(const CMediumSet &M)
{
	failed = M.failed;
	Medium = M.Medium;
	Solid_phase = M.Solid_phase;
	parameters = M.parameters;
	SP = M.SP;
	std = M.std;
	FI = M.FI;
	PE_info_filename = M.PE_info_filename;
	formulas = M.formulas;
	RXN = M.RXN;
	for (int i = 0; i < Medium.size(); i++)
		Medium[i].parent = this;
	set_features = M.set_features;
	measured_quan = M.measured_quan;
	measured_data = M.measured_data;
	buildup = M.buildup;
	externalflux = M.externalflux;
	evaporation_model = M.evaporation_model;
	ANS_obs = M.ANS_obs;
	for (int i = 0; i < Medium.size(); i++)
	{
		ANS_hyd.push_back(&Medium[i].ANS);
		ANS_colloids.push_back(&Medium[i].ANS_colloids);
		ANS_constituents.push_back(&Medium[i].ANS_constituents);
	}

}

CMediumSet& CMediumSet::operator=(const CMediumSet &M)
{
	failed = M.failed;
	Medium = M.Medium;
	Solid_phase = M.Solid_phase;
	parameters = M.parameters;
	std = M.std;
	SP = M.SP;
	FI = M.FI;
	PE_info_filename = M.PE_info_filename;
	formulas = M.formulas;
	RXN = M.RXN;
	for (int i = 0; i < Medium.size(); i++)
		Medium[i].parent = this;
	set_features = M.set_features;
	measured_quan = M.measured_quan;
	measured_data = M.measured_data;
	buildup = M.buildup;
	externalflux = M.externalflux;
	evaporation_model = M.evaporation_model;
	ANS_hyd.clear();
	ANS_colloids.clear();
	ANS_constituents.clear();
	for (int i = 0; i < Medium.size(); i++)
	{
		ANS_hyd.push_back(&Medium[i].ANS);
		ANS_colloids.push_back(&Medium[i].ANS_colloids);
		ANS_constituents.push_back(&Medium[i].ANS_constituents);
	}
	ANS_obs = M.ANS_obs;
	return *this;
}

void CMediumSet::set_formulas()
{
	formulas.formulasH.resize(10);


	formulas.formulasH[Soil] = "_frs[(f[5]-(((1/f[53])*(((_max(_min(f[9]:1):0.00001)^(f[54]/(1-f[54])))-1)^(1/f[54])))*((1-f[9])/(0.01+1-f[9])))+(_pos(f[10]-f[51])/f[57]))]";
	formulas.formulasH[Darcy] = "f[6]+f[5]+((f[10]-f[51])/f[57])";
	formulas.formulasH[Storage] = "f[5]+(_pos(f[4])/(f[2]*f[51]))+(_hsd(f[9]-0.98)*(f[9]-1)*f[51]/f[57])-(f[65]/(f[9]^f[66]))";
	formulas.formulasH[NativeSoil] = "f[5]+(f[4]/(f[2]*f[51]))";
	formulas.formulasH[Pond] = "f[5]+(f[4]/f[2])";
	formulas.formulasH[Catchment] = "f[5]+(f[4]/f[2])";
	formulas.formulasH[Stream] = "f[5]+(f[4]/f[2])";
	formulas.formulasH[Manhole] = "f[5]+(f[4]/f[2])";


	formulas.formulas.resize(10);
	formulas.formulasQ.resize(10);
	formulas.formulasA.resize(10);
	formulas.const_area.resize(10);

	for (int i = 0; i < formulas.formulasQ.size(); i++)	 formulas.formulasQ[i].resize(10);
	for (int i = 0; i < formulas.formulasA.size(); i++)
	{
		formulas.formulasA[i].resize(10);
		for (int j = 0; j < formulas.formulasA[i].size(); j++) formulas.formulasA[i][j] = "(s[2]+e[2])/2";
	}

	for (int i = 0; i < formulas.const_area.size(); i++)	 {
		formulas.const_area[i].resize(10); for (int j = 0; j < formulas.const_area[i].size(); j++) formulas.const_area[i][j] = true;

	}


	formulas.vaporTransport.resize(10);
	for (int i = 0; i < formulas.vaporTransport.size(); i++)	 formulas.vaporTransport[i].resize(10);

	formulas.settling.resize(10);
	for (int i = 0; i <formulas.settling.size(); i++) formulas.settling[i].resize(10);

	formulas.vaporTransport[Soil][Soil] = 1;
	formulas.vaporTransport[Soil][Pond] = 1;
	formulas.vaporTransport[Soil][Storage] = 1;
	formulas.vaporTransport[Soil][Darcy] = 1;
	formulas.vaporTransport[Soil][Stream] = 1;
	formulas.vaporTransport[Soil][Catchment] = 1;

	formulas.vaporTransport[Pond][Soil] = 1;
	formulas.vaporTransport[Storage][Soil] = 1;
	formulas.vaporTransport[Darcy][Soil] = 1;
	formulas.vaporTransport[Stream][Soil] = 1;
	formulas.vaporTransport[Catchment][Soil] = 1;

	formulas.settling[Storage][Storage] = 1;
	formulas.settling[Pond][Soil] = 1;
	formulas.settling[Soil][Pond] = 1;
	formulas.settling[Stream][Darcy] = 1;
	formulas.settling[Darcy][Stream] = 1;
	formulas.settling[Pond][Darcy] = 1;
	formulas.settling[Darcy][Pond] = 1;

	formulas.air_phase.resize(10);

	formulas.air_phase[Soil] = 1;

	formulas.formulasQ2 = formulas.formulasQ;

	formulas.formulasA[Catchment][Catchment] = "0.5*f[55]*((s[1]-s[5])+(e[1]-e[5]))";
	formulas.formulasA[Catchment][Pond] = "f[55]*(s[1]-s[5])";
	formulas.formulasA[Pond][Catchment] = "f[55]*(e[1]-e[5])";
	formulas.formulasA[Catchment][Stream] = "f[55]*(s[1]-s[5])";
	formulas.formulasA[Stream][Catchment] = "f[55]*(e[1]-e[5])";
	formulas.formulasA[Stream][Stream] = "0.5*f[55] * (_pos(s[1] - _max(s[5]:f[60])) + _pos(e[1] - _max(e[5]:f[60])))";
	formulas.formulasA[Stream][Pond] = "f[55]*_pos(s[1]-_max(s[5]:f[60]))";
	formulas.formulasA[Pond][Pond] = "0.5*f[55]*(_pos(s[1]-_max(s[5]:f[60]))+_pos(e[1]-_max(e[5]:f[60])))";
	formulas.formulasA[Storage][Storage] = "0.5*f[55]*((s[4]/(s[2]*s[51]))+(e[4]/(e[2]*e[51])))";

	formulas.const_area[Catchment][Catchment] = false;
	formulas.const_area[Catchment][Pond] = false;
	formulas.const_area[Pond][Catchment] = false;
	formulas.const_area[Catchment][Stream] = false;
	formulas.const_area[Stream][Catchment] = false;
	formulas.const_area[Stream][Stream] = false;
	formulas.const_area[Stream][Pond] = false;
	formulas.const_area[Pond][Pond] = false;
	formulas.const_area[Storage][Storage] = false;



	formulas.formulasQ[Soil][Storage] = "(_frs[((f[50])*(_max(_min(f[9]:1):0)^f[56])*((1-((1-(_max(_min(f[9]:1):0)^(1/f[55])))^f[55]))^2))]*(_pos(s[1]-_max(e[1]:s[5]))-_pos(e[1]-s[1]))/f[6]*f[2])";
	formulas.formulasQ[Storage][Soil] = "(_frs[(f[50]*(_max(_min(f[9]:1):0)^f[56])*((1-((1-(_max(_min(f[9]:1):0)^(1/f[55])))^f[55]))^2))]*(s[1]-e[1])/f[6]*f[2])";
	formulas.formulasQ[Storage][Storage] = "f[50]*(s[1]-e[1])/f[6]*f[2]";
	formulas.formulasQ2[Storage][Storage] = "f[50]*(s[1]-e[1])/f[6]*f[2]";
	formulas.formulasQ[Darcy][Darcy] = "f[50]*(s[1]-e[1])/f[6]*f[2]";
	formulas.formulasQ[Pond][Storage] = "f[50]*(s[1]-e[1])/f[6]*f[2]";
	formulas.formulasQ[Storage][Pond] = "f[50]*(s[1]-e[1])/f[6]*f[2]";
	formulas.formulasQ[Darcy][Pond] = "f[50]*(s[1]-e[1])/f[6]*f[2]";
	formulas.formulasQ2[Pond][Darcy] = "f[50]*(s[1]-e[1])/f[6]*f[2]";
	formulas.formulasQ2[Darcy][Stream] = "f[50]*(s[1]-e[1])/f[6]*f[2]";
	formulas.formulasQ[Stream][Darcy] = "f[50]*(s[1]-e[1])/f[6]*f[2]";
	formulas.formulasQ2[Pond][Storage] = "f[50]*(s[1]-e[1])/f[6]*f[2]";
	formulas.formulasQ2[Storage][Pond] = "f[50]*(s[1]-e[1])/f[6]*f[2]";
	formulas.formulasQ[Catchment][Storage] = "f[50]*(s[1]-e[1])/f[6]*f[2]";
	formulas.formulasQ[Storage][Catchment] = "f[50]*(s[1]-e[1])/f[6]*f[2]";
	formulas.formulasQ2[Catchment][Storage] = "f[50]*(s[1]-e[1])/f[6]*f[2]";
	formulas.formulasQ2[Storage][Catchment] = "f[50]*(s[1]-e[1])/f[6]*f[2]";


	formulas.formulasQ2[Storage][Soil] = "(_frs[((-f[50])*(_max(_min(f[9]:1):0)^f[56])*((1-((1-(_max(_min(f[9]:1):0)^(1/f[55])))^f[55]))^2))]*(_pos(e[1]-_max(s[1]:e[5]))-_pos(s[1]-e[1]))/f[6]*f[2])";
	formulas.formulasQ2[Soil][Storage] = "(_frs[(f[50]*(_max(_min(f[9]:1):0)^f[56])*((1-((1-(_max(_min(f[9]:1):0)^(1/f[55])))^f[55]))^2))]*(s[1]-e[1])/f[6]*f[2])";

	formulas.formulasQ[Soil][Soil] = "(_frs[(f[50]*(_max(_min(f[9]:1):0)^f[56])*((1-((1-(_max(_min(f[9]:1):0)^(1/f[55])))^f[55]))^2))]*(s[1]-e[1])/f[6]*f[2])";
	formulas.formulasQ2[Soil][Soil] = "(_frs[(f[50]*(_max(_min(f[9]:1):0)^f[56])*((1-((1-(_max(_min(f[9]:1):0)^(1/f[55])))^f[55]))^2))]*(s[1]-e[1])/f[6]*f[2])";
	formulas.formulasQ[Pond][Pond] = "f[55]/f[56]*((_sq2(_pos((s[1]-e[1])/f[6]):0.001)*_mon((s[1]-_max(s[5]:f[60])):0.01))-(_sq2(_pos((e[1]-s[1])/f[6]):0.001)*_mon((e[1]-_max(e[5]:f[60])):0.01)))*(((_pos(s[1]-_max(s[5]:f[60]))+_pos(e[1]-_max(e[5]:f[60])))/2)^1.66667)";
	formulas.formulasQ2[Pond][Pond] = "f[55]/f[56]*((_sq2(_pos((s[1]-e[1])/f[6]):0.001)*_mon((s[1]-_max(s[5]:f[60])):0.01))-(_sq2(_pos((e[1]-s[1])/f[6]):0.001)*_mon((e[1]-_max(e[5]:f[60])):0.01)))*(((_pos(s[1]-_max(s[5]:f[60]))+_pos(e[1]-_max(e[5]:f[60])))/2)^1.66667)";
	formulas.formulasQ[Stream][Stream] = "f[55]/f[56]*((_sq2(_pos((s[1]-e[1])/f[6]):0.001)*_mon((s[1]-_max(s[5]:f[60])):0.01))-(_sq2(_pos((e[1]-s[1])/f[6]):0.001)*_mon((e[1]-_max(e[5]:f[60])):0.01)))*(((_pos(s[1]-_max(s[5]:f[60]))+_pos(e[1]-_max(e[5]:f[60])))/2)^1.66667)";
	formulas.formulasQ2[Stream][Stream] = "f[55]/f[56]*((_sq2(_pos((s[1]-e[1])/f[6]):0.001)*_mon((s[1]-_max(s[5]:f[60])):0.01))-(_sq2(_pos((e[1]-s[1])/f[6]):0.001)*_mon((e[1]-_max(e[5]:f[60])):0.01)))*(((_pos(s[1]-_max(s[5]:f[60]))+_pos(e[1]-_max(e[5]:f[60])))/2)^1.66667)";
	formulas.formulasQ2[Stream][Pond] = "f[55]/f[56]*((_sq2(_pos((s[1]-e[1])/f[6]):0.001)*_mon((s[1]-_max(s[5]:f[60])):0.01))-(_sq2(_pos((e[1]-s[1])/f[6]):0.001)*_mon((e[1]-_max(e[5]:f[60])):0.01)))*(((_pos(s[1]-_max(s[5]:f[60]))+_pos(e[1]-_max(e[5]:f[60])))/2)^1.66667)";
	formulas.formulasQ2[Pond][Stream] = "f[55]/f[56]*((_sq2(_pos((s[1]-e[1])/f[6]):0.001)*_mon((s[1]-_max(s[5]:f[60])):0.01))-(_sq2(_pos((e[1]-s[1])/f[6]):0.001)*_mon((e[1]-_max(e[5]:f[60])):0.01)))*(((_pos(s[1]-_max(s[5]:f[60]))+_pos(e[1]-_max(e[5]:f[60])))/2)^1.66667)";
	formulas.formulasQ[Stream][Pond] = "f[55]/f[56]*((_sq2(_pos((s[1]-e[1])/f[6]):0.001)*_mon((s[1]-_max(s[5]:f[60])):0.01))-(_sq2(_pos((e[1]-s[1])/f[6]):0.001)*_mon((e[1]-_max(e[5]:f[60])):0.01)))*(((_pos(s[1]-_max(s[5]:f[60]))+_pos(e[1]-_max(e[5]:f[60])))/2)^1.66667)";
	formulas.formulasQ[Pond][Stream] = "f[55]/f[56]*((_sq2(_pos((s[1]-e[1])/f[6]):0.001)*_mon((s[1]-_max(s[5]:f[60])):0.01))-(_sq2(_pos((e[1]-s[1])/f[6]):0.001)*_mon((e[1]-_max(e[5]:f[60])):0.01)))*(((_pos(s[1]-_max(s[5]:f[60]))+_pos(e[1]-_max(e[5]:f[60])))/2)^1.66667)";


	formulas.formulasQ[Pond][Soil] = "0.5*(_frs[(f[50]*(_max(_min(f[9]:1):0)^f[56])*((1-((1-(_max(_min(f[9]:1):0)^(1/f[55])))^f[55]))^2))]+(f[50]*_hsd(s[9])))*(s[1]-e[1])/f[6]*f[2]*(_mon((s[4]/s[2]):0.001))";
	formulas.formulasQ[Soil][Pond] = "0";
	formulas.formulasQ2[Pond][Soil] = "0";
	formulas.formulasQ2[Soil][Pond] = "0.5*(_frs[(-f[50]*(_max(_min(f[9]:1):0)^f[56])*((1-((1-(_max(_min(f[9]:1):0)^(1/f[55])))^f[55]))^2))]-(f[50]*_hsd(s[9])))*(e[1]-s[1])/f[6]*f[2]*(_mon((e[4]/e[2]):0.001))";

	formulas.formulasQ[Catchment][Catchment] = "f[55]/f[56]*((_sq2(_pos((s[1]-e[1])/f[6]):0.001)*_mon((s[1]-s[5]):0.01))-(_sq2(_pos((e[1]-s[1])/f[6]):0.001)*_mon((e[1]-e[5]):0.01)))*(((_hsd(s[1]-e[1])*_pos(s[1]-s[5]-s[12]))+(_hsd(e[1]-s[1])*_pos(e[1]-e[5]-s[12])))^(1+f[58]))";
	formulas.formulasQ[Catchment][Pond] = "f[55]/f[56]*_sqs((s[1]-e[1])/f[6])*_mon(_abs(s[1]-e[1])/f[6]:0.001)*((_pos(s[1]-s[5]-s[12])^(1+f[58]))";
	formulas.formulasQ[Catchment][Stream] = "f[55]/f[56]*_sqs((s[1]-e[1])/f[6])*_mon(_abs(s[1]-e[1])/f[6]:0.001)*((_pos(s[1]-s[5]-s[12])^(1+f[58]))";
	formulas.formulasQ[Catchment][Storage] = "f[55]/f[56]*_sqs((s[1]-e[1])/f[6])*_mon(_abs(s[1]-e[1])/f[6]:0.001)*((_pos(s[1]-s[5]-s[12])^(1+f[58]))";
	formulas.formulasQ[Catchment][Soil] = "(_frs[(f[50]*(_max(_min(f[9]:1):0)^f[56])*((1-((1-(_max(_min(f[9]:1):0)^(1/f[55])))^f[55]))^2))]*(s[1]-e[1])/f[6]*f[2]*(_mon((e[4]/e[2]):0.001)))";

	formulas.formulasQ[Darcy][Soil] = "(_frs[(f[50]*(_max(_min(f[9]:1):0)^e[56])*((1-((1-(_max(_min(f[9]:1):0)^(1/e[55])))^e[55]))^2))]*(s[1]-e[1])/f[6]*f[2])";
	formulas.formulasQ[Soil][Darcy] = "(_frs[(f[50]*(_max(_min(f[9]:1):0)^s[56])*((1-((1-(_max(_min(f[9]:1):0)^(1/s[55])))^s[55]))^2))]*(s[1]-e[1])/f[6]*f[2])";
	formulas.formulasQ2[Darcy][Soil] = "(_frs[(f[50]*(_max(_min(f[9]:1):0)^e[56])*((1-((1-(_max(_min(f[9]:1):0)^(1/e[55])))^e[55]))^2))]*(s[1]-e[1])/f[6]*f[2])";
	formulas.formulasQ2[Soil][Darcy] = "(_frs[(f[50]*(_max(_min(f[9]:1):0)^s[56])*((1-((1-(_max(_min(f[9]:1):0)^(1/s[55])))^s[55]))^2))]*(s[1]-e[1])/f[6]*f[2])";

	formulas.formulasQ[Pond][Catchment] = "0";
	formulas.formulasQ[Stream][Catchment] = "0";
	formulas.formulasQ[Storage][Catchment] = "0";
	formulas.formulasQ[Soil][Catchment] = "0";

	formulas.formulasQ2[Catchment][Catchment] = "f[55]/f[56]*((_sq2(_pos((s[1]-e[1])/f[6]):0.001)*_mon((s[1]-s[5]):0.01))-(_sq2(_pos((e[1]-s[1])/f[6]):0.001)*_mon((e[1]-e[5]):0.01)))*(((_hsd(s[1]-e[1])*_pos(s[1]-s[5]-s[12]))+(_hsd(e[1]-s[1])*_pos(e[1]-e[5]-s[12])))^(1+f[58]))";

	formulas.formulasQ2[Catchment][Pond] = "0";
	formulas.formulasQ2[Catchment][Stream] = "0";
	formulas.formulasQ2[Catchment][Storage] = "0";
	formulas.formulasQ2[Catchment][Soil] = "0";

	formulas.formulasQ2[Pond][Catchment] = "(-f[55])/f[56]*_sqs((e[1]-s[1])/f[6])*_mon(_abs(e[1]-s[1])/f[6]:0.001)*((_pos(e[1]-e[5]-e[12])^(1+f[58]))";
	formulas.formulasQ2[Stream][Catchment] = "(-f[55])/f[56]*_sqs((e[1]-s[1])/f[6])*_mon(_abs(s[1]-e[1])/f[6]:0.001)*((_pos(e[1]-e[5]-e[12])^(1+f[58]))";
	formulas.formulasQ2[Storage][Catchment] = "(-f[55])/f[56]*_sqs((e[1]-s[1])/f[6])*_mon(_abs(s[1]-e[1])/f[6]:0.001)*((_pos(e[1]-e[5]-e[12])^(1+f[58]))";
	formulas.formulasQ2[Soil][Catchment] = "_frs[((-f[50])*(_max(_min(f[9]:1):0)^f[56])*((1-((1-(_max(_min(f[9]:1):0)^(1/f[55])))^f[55]))^2))]*(e[1]-s[1])/f[6]*f[2]*(_mon((s[4]/s[2]):0.001))";

	formulas.formulas[Normal] = "f[55]/f[56]*((_sq2(_pos((s[5]-e[5])/f[6]):0.001)*_mon((s[1]-s[5]):0.01))-(_sq2(_pos((e[5]-s[5])/f[6]):0.001)*_mon((e[1]-e[5]):0.01)))*(((_pos(s[1]-s[5])+_pos(e[1]-e[5]))/2)^(1+f[58])";
	formulas.formulas[QDarcy] = "f[50]*(s[1]-e[1])/f[6]*f[2]";
	formulas.formulas[Vapor] = "(((e[13]+s[13])/2*(s[10]-e[10])/f[6]*f[2]*(_pos(s[51]-s[10])+_pos(e[51]-e[10]))/2))";
	formulas.formulas[Pipe1] = "(f[52]^2.63)*23760*f[67]*((_pos(s[1]-f[61])/f[6])^0.54)*_hsd(s[1]-f[60])";
	formulas.formulas[Pipe2] = "(f[52]^2.63)*23760*f[67]*_ply(_max(((e[1]-f[61])/f[52]):((s[1]-f[60])/f[52])))*_mo1(((((_pos(s[1]-_max(e[1]:f[61]))/f[6])^0.54))-(((_pos(e[1]-_max(s[1]:f[60]))/f[6])^0.54))):0.001)"; //Q in m3/d
	formulas.formulas[Rating_curve] = "(_hsd(s[1]-e[1])*f[62]*(_pos(s[1]-f[64])^f[63]))-(_hsd(e[1]-s[1])*f[62]*(_pos(e[1]-f[64])^f[63]))";
	set_features.formulas = true;
}

void CMediumSet::solve()
{
	failed = false;
	if (FI.write_details)
	{
		FILE *FILEBTC = fopen((FI.outputpathname + "Solution_details.txt").c_str(), "w");
		fclose(FILEBTC);
	}
	failed = false;
	ANS_hyd.clear();
	ANS_colloids.clear();
	ANS_constituents.clear();
	for (int i = 0; i < Medium.size(); i++)
	{
		Medium[i].solve();
		failed = failed || Medium[i].failed;
		ANS_hyd.push_back(&Medium[i].ANS);
		ANS_colloids.push_back(&Medium[i].ANS_colloids);
		ANS_constituents.push_back(&Medium[i].ANS_constituents);
	}

	ANS_obs = CBTCSet(measured_quan.size());
	for (int i = 0; i < measured_quan.size(); i++)
	{
		ANS_obs.BTC[i] = Medium[lookup_medium(measured_quan[i].experiment)].ANS_obs.BTC[i];
		ANS_obs.setname(i, measured_quan[i].name);
	}
}

void CMediumSet::set_default()
{

	SP.nr_iteration_treshold_max = 8;
	SP.nr_iteration_treshold_min = 4;
	SP.dt_change_rate = 0.75;
	SP.dt_change_failure = 0.1;
	SP.nr_failure_criteria = 100;
	SP.max_J_interval = 100;
	FI.write_interval = 1;
	FI.write_details = false;
	SP.wiggle_tolerance = 0.02;
	FI.uniformoutput = true;
	SP.mass_balance_check = false;
	SP.forward = false;
	SP.colloid_transport = false;
	SP.constituent_transport = false;
	SP.epoch_limit = 500000;
	SP.avg_dt_limit = 1e-5;
	SP.restore_interval = 20;

	set_formulas();
}

void CMediumSet::f_get_environmental_params(CLIDconfig &lid_config)
{
	SP.pos_def_limit = true;
	SP.negative_concentration_allowed = false;
	for (int i = 0; i<lid_config.keyword.size(); i++)
	{
		if (tolower(lid_config.keyword[i]) == "path") FI.pathname = lid_config.value[i].c_str();
		if (tolower(lid_config.keyword[i]) == "outputpath") FI.outputpathname = lid_config.value[i].c_str();
		if (tolower(lid_config.keyword[i]) == "forward") SP.forward = atoi(lid_config.value[i].c_str());
		if (tolower(lid_config.keyword[i]) == "solution_method") SP.solution_method = atoi(lid_config.value[i].c_str());
		if (tolower(lid_config.keyword[i]) == "wiggle_tolerance") SP.wiggle_tolerance = atof(lid_config.value[i].c_str());
		if (tolower(lid_config.keyword[i]) == "max_j_update_interval") SP.max_J_interval = atoi(lid_config.value[i].c_str());
		if (tolower(lid_config.keyword[i]) == "dt") SP.dt = atof(lid_config.value[i].c_str());
		if (tolower(lid_config.keyword[i]) == "w") SP.w = atof(lid_config.value[i].c_str());
		if (tolower(lid_config.keyword[i]) == "tol") SP.tol = atof(lid_config.value[i].c_str());
		
		if (tolower(lid_config.keyword[i]) == "uniformoutput") FI.uniformoutput = atoi(lid_config.value[i].c_str());

		if (tolower(lid_config.keyword[i]) == "nr_iteration_treshold_max") SP.nr_iteration_treshold_max = atof(lid_config.value[i].c_str());
		if (tolower(lid_config.keyword[i]) == "nr_iteration_treshold_min") SP.nr_iteration_treshold_min = atof(lid_config.value[i].c_str());
		if (tolower(lid_config.keyword[i]) == "dt_change_rate") SP.dt_change_rate = atof(lid_config.value[i].c_str());
		if (tolower(lid_config.keyword[i]) == "dt_change_failure") SP.dt_change_failure = atof(lid_config.value[i].c_str());
		if (tolower(lid_config.keyword[i]) == "nr_failure_criteria") SP.nr_failure_criteria = atoi(lid_config.value[i].c_str());
		if (tolower(lid_config.keyword[i]) == "write_details") FI.write_details = atoi(lid_config.value[i].c_str());
		if (tolower(lid_config.keyword[i]) == "mass_balance_check") SP.mass_balance_check = atoi(lid_config.value[i].c_str());
		if (tolower(lid_config.keyword[i]) == "colloid_transport") SP.colloid_transport = atoi(lid_config.value[i].c_str());
		if (tolower(lid_config.keyword[i]) == "water_quality") SP.constituent_transport = atoi(lid_config.value[i].c_str());
		if (tolower(lid_config.keyword[i]) == "epoch_limit") SP.epoch_limit = atoi(lid_config.value[i].c_str());
		if (tolower(lid_config.keyword[i]) == "avg_dt_limit") SP.avg_dt_limit = atof(lid_config.value[i].c_str());
		if (tolower(lid_config.keyword[i]) == "restore_interval") SP.restore_interval = atoi(lid_config.value[i].c_str());
		if (tolower(lid_config.keyword[i]) == "log_file") FI.log_file_name = FI.outputpathname + lid_config.value[i];
		if (tolower(lid_config.keyword[i]) == "pe_info_filename") PE_info_filename = lid_config.value[i];
		if (tolower(lid_config.keyword[i]) == "writeinterval") FI.write_interval = atoi(lid_config.value[i].c_str());
		
		if (tolower(lid_config.keyword[i]) == "pos_def_limit") SP.pos_def_limit = atoi(lid_config.value[i].c_str());
		if (tolower(lid_config.keyword[i]) == "negative_concentration_allowed") SP.negative_concentration_allowed = atoi(lid_config.value[i].c_str());
		if (tolower(lid_config.keyword[i]) == "steady_state_hydro") SP.steady_state_hydro = atoi(lid_config.value[i].c_str());

		if (tolower(lid_config.keyword[i]) == "detout_obs_filename")
		{
			vector<string> names = split_curly_semicolon(lid_config.value[i]);
			for (int ii = 0; ii<names.size(); ii++)
				FI.detoutfilename_obs = names[ii];
		}
	}

	if (SP.constituent_transport) SP.colloid_transport = true;
	set_features.environmental_vars = true;
}


void CMediumSet::f_get_params(CLIDconfig &lid_config)
{
	for (int i = 0; i<lid_config.keyword.size(); i++)
	{
		if (tolower(lid_config.keyword[i]) == "parameter")
		{
			range P;
			P.low = 0;
			P.high = 0;
			P.fixed = false;
			P.log = false;
			P.applytoall = true;
			P.tempcorr = 1;
			P.name = lid_config.value[i];

			for (int j = 0; j<lid_config.param_names[i].size(); j++)
			{
				if (tolower(lid_config.param_names[i][j]) == "low") P.low = atof(lid_config.param_vals[i][j].c_str());  //low range
				if (tolower(lid_config.param_names[i][j]) == "high") P.high = atof(lid_config.param_vals[i][j].c_str());
				if (tolower(lid_config.param_names[i][j]) == "temp_corr") P.tempcorr = atof(lid_config.param_vals[i][j].c_str());
				if (tolower(lid_config.param_names[i][j]) == "fixed") P.fixed = atoi(lid_config.param_vals[i][j].c_str());
				if (tolower(lid_config.param_names[i][j]) == "applytoall") P.applytoall = atoi(lid_config.param_vals[i][j].c_str());
				if (tolower(lid_config.param_names[i][j]) == "log") P.log = atoi(lid_config.param_vals[i][j].c_str());
			}
			parameters.push_back(P);
			if (P.log == 0)
				P.value = 0.5*(P.low + P.high);
			else
				P.value = P.low / fabs(P.low)*sqrt(P.low*P.high);

		}
	}
	set_features.parameters = true;
}

void CMediumSet::f_get_observed(CLIDconfig &lid_config)
{
	for (int i = 0; i<lid_config.keyword.size(); i++)
	{
		if (tolower(lid_config.keyword[i]) == "observed")
		{
			measured_chrc M;
			M.error_structure = 0;
			M.name = lid_config.value[i];
			for (int j = 0; j<lid_config.param_names[i].size(); j++)
			{
				if (tolower(lid_config.param_names[i][j]) == "id") M.id = split(lid_config.param_vals[i][j], '+');  //location id
				if (tolower(lid_config.param_names[i][j]) == "loc_type") if (tolower(lid_config.param_vals[i][j]) == "b") M.loc_type = 0; else if (tolower(lid_config.param_vals[i][j]) == "c") M.loc_type = 1;
				if (tolower(lid_config.param_names[i][j]) == "quan") M.quan = lid_config.param_vals[i][j].c_str();
				if (tolower(lid_config.param_names[i][j]) == "std_no") M.std_no = atoi(lid_config.param_vals[i][j].c_str());
				if (tolower(lid_config.param_names[i][j]) == "error_structure") M.error_structure = atoi(lid_config.param_vals[i][j].c_str());
				if (tolower(lid_config.param_names[i][j]) == "experiment") M.experiment = lid_config.param_vals[i][j].c_str();

			}
			measured_quan.push_back(M);
		}
		if (tolower(lid_config.keyword[i]) == "observed_data")
		{
			measured_data = CBTCSet(FI.pathname + lid_config.value[i], 1);
			if (measured_data.names.size() == 0)
				writetolog("observed data lacks name field");
		}
	}

	vector<int> stds;
	for (int i = 0; i<measured_quan.size(); i++)
	{
		if (lookup(stds, measured_quan[i].std_no) == -1)
		{
			stds.push_back(measured_quan[i].std_no);
			measured_quan[i].std_to_param = int(parameters.size());
			range P;

			P.fixed = false;
			P.log = true;
			P.applytoall = true;
			P.tempcorr = 1;
			P.name = "std_" + numbertostring(i);
			P.low = exp(-4); P.high = exp(4);
			P.value = sqrt(P.high*P.low);
			parameters.push_back(P);


		}
	}
	std.resize(stds.size());
	set_features.observed = true;
}

void CMediumSet::writetolog(string S)
{
	fstream file(FI.outputpathname + FI.log_file_name);
	file << S << endl;
	file.close();

}

void CMediumSet::f_get_particle_types(CLIDconfig &lid_config)
{
	for (int i = 0; i<lid_config.keyword.size(); i++)
	{
		if ((tolower(lid_config.keyword[i]) == "particulate_phase") || (tolower(lid_config.keyword[i]) == "solid_phase") || (tolower(lid_config.keyword[i]) == "particle"))
		{

			CSolid_Phase S(lid_config.param_vals[i][lookup(lid_config.param_names[i], "model")]);
			S.name = lid_config.value[i];


			for (int j = 0; j<lid_config.param_names[i].size(); j++)
			{
				S.set_val(lid_config.param_names[i][j], atof(lid_config.param_vals[i][j].c_str()));
			}

			Solid_phase.push_back(S);

		}
	}
	set_features.solids = true;
}

void CMediumSet::f_get_constituents(CLIDconfig &lid_config)
{
	for (int i = 0; i<lid_config.keyword.size(); i++)
	{
		if (tolower(lid_config.keyword[i]) == "constituent")
		{
			CConstituent S;
			S.name = lid_config.value[i];

			for (int j = 0; j<lid_config.param_names[i].size(); j++)
			{
				vector<char> del2; del2.push_back('['); del2.push_back(']'); del2.push_back(':');
				if (tolower(split(lid_config.param_names[i][j], del2)[0]) == "kd")
				{
					S.capacity.push_back(atof(lid_config.param_vals[i][j].c_str()));
					S.capacity_ptr.push_back(split(lid_config.param_names[i][j], del2)[1]);
				}
				if (tolower(split(lid_config.param_names[i][j], del2)[0]) == "rate")
				{
					S.rate_exchange.push_back(atof(lid_config.param_vals[i][j].c_str()));
					S.rate_exchange_ptr.push_back(split(lid_config.param_names[i][j], del2)[1]);
				}
				if (tolower(lid_config.param_names[i][j]) == "diffusion")
				{
					S.diffusion = atof(lid_config.param_vals[i][j].c_str());
				}
				if (tolower(lid_config.param_names[i][j]) == "exchange_rate_factor")
				{
					S.exchange_rate_scale_factor = CStringOP(lid_config.param_vals[i][j]);
				}
				if (tolower(lid_config.param_names[i][j]) == "exchange_rate_param")
				{
					S.exchange_params.push_back(atof(lid_config.param_vals[i][j].c_str()));
				}
				if (tolower(lid_config.param_names[i][j]) == "vs")
				{
					S.vs = atof(lid_config.param_vals[i][j].c_str());
				}

			}
			RXN.cons.push_back(S);

			for (int ii = 0; ii<lid_config.est_param[i].size(); ii++)
			{
				if (lookup_parameters(lid_config.est_param[i][ii]) != -1)
				{
					parameters[lookup_parameters(lid_config.est_param[i][ii])].location.push_back(RXN.cons.size() - 1);
					parameters[lookup_parameters(lid_config.est_param[i][ii])].quan.push_back(lid_config.param_names[i][ii]);
					parameters[lookup_parameters(lid_config.est_param[i][ii])].location_type.push_back(4);
					parameters[lookup_parameters(lid_config.est_param[i][ii])].experiment_id.push_back("");
				}
			}
		}
	}

	set_features.constituents = true; 
}

int CMediumSet::lookup_parameters(string S)
{
	int out = -1;
	for (int i = 0; i < parameters.size(); i++)
		if (tolower(S) == tolower(parameters[i].name))

			return i;

	return out;
}

int CMediumSet::lookup_observation(string S) const
{
	int out = -1;
	for (int i = 0; i < measured_quan.size(); i++)
		if (tolower(S) == tolower(measured_quan[i].name))

			return i;

	return out;
}

void CMediumSet::f_get_reactions(CLIDconfig &lid_config)
{

	for (int i = 0; i < lid_config.keyword.size(); i++)
	{
		if (tolower(lid_config.keyword[i]) == "reaction_parameter")
		{
			rxparam rxparameter;
			rxparameter.tempcorr = 1;
			rxparameter.name = lid_config.value[i];
			for (int j = 0; j < lid_config.param_names[i].size(); j++)
			{
				if (tolower(lid_config.param_names[i][j]) == "value") rxparameter.value = atof(lid_config.param_vals[i][j].c_str());
				if (tolower(lid_config.param_names[i][j]) == "temperature_correction") { rxparameter.tempcorr = atof(lid_config.param_vals[i][j].c_str()); }
			}
			RXN.parameters.push_back(rxparameter);
			for (int ii = 0; ii < lid_config.est_param[i].size(); ii++)
			{
				if (lookup_parameters(lid_config.est_param[i][ii]) != -1)
				{
					parameters[lookup_parameters(lid_config.est_param[i][ii])].location.push_back(RXN.parameters.size() - 1);
					parameters[lookup_parameters(lid_config.est_param[i][ii])].quan.push_back(lid_config.value[i]);
					parameters[lookup_parameters(lid_config.est_param[i][ii])].location_type.push_back(3);
					parameters[lookup_parameters(lid_config.est_param[i][ii])].experiment_id.push_back("");
				}
			}
		}
	}

	for (int i = 0; i<lid_config.keyword.size(); i++)
	{
		if (tolower(lid_config.keyword[i]) == "reaction")
		{
			CReaction Rx;
			Rx.name = lid_config.value[i];
			for (int j = 0; j<lid_config.param_names[i].size(); j++)
			{
				vector<char> del2; del2.push_back('['); del2.push_back(']'); del2.push_back(':');
				if (tolower(split(lid_config.param_names[i][j], del2)[0]) == "rate") Rx.rate = CStringOP(lid_config.param_vals[i][j], &RXN);
				if (tolower(split(lid_config.param_names[i][j], del2)[0]) == "product")
				{
					Rx.products.push_back(RXN.look_up_constituent_no(split(lid_config.param_names[i][j], del2)[1])); Rx.prodrates.push_back(CStringOP(lid_config.param_vals[i][j], &RXN));

					if (split(lid_config.param_names[i][j], del2).size()>2)
						Rx.product_p_type.push_back(atoi(split(lid_config.param_names[i][j], del2)[2].c_str()));
					else
						Rx.product_p_type.push_back(-2);

					if (split(lid_config.param_names[i][j], del2).size()>3)
						Rx.product_phase.push_back(atoi(split(lid_config.param_names[i][j], del2)[3].c_str()));
					else
						Rx.product_phase.push_back(0);

				}
			}
			RXN.Rxts.push_back(Rx);
		}
	}
	set_features.reactions = true;
}

void CMediumSet::f_get_buildup(CLIDconfig &lid_config)
{
	for (int i = 0; i<lid_config.keyword.size(); i++)
	{
		if ((tolower(lid_config.keyword[i]) == "build_up") || (tolower(lid_config.keyword[i]) == "buildup"))
		{

			CBuildup S(lid_config.param_vals[i][lookup(lid_config.param_names[i], "model")]);
			S.name = lid_config.value[i];

			for (int j = 0; j<lid_config.param_names[i].size(); j++)
			{
				if (lid_config.param_names[i][j] == "constituent") S.constituent = lid_config.param_vals[i][j];
				if (lid_config.param_names[i][j] == "solid") S.solid = lid_config.param_vals[i][j];
				S.set_val(lid_config.param_names[i][j], atof(lid_config.param_vals[i][j].c_str()));
			}

			if ((S.constituent == "") && (S.solid != "") && (S.phase == "")) S.phase = "attached";
			if ((S.solid == "") && (S.constituent != "") && (S.phase == "")) S.phase = "sorbed";
			buildup.push_back(S);

			for (int ii = 0; ii<lid_config.est_param[i].size(); ii++)
			{
				if (lookup_parameters(lid_config.est_param[i][ii]) != -1)
				{
					parameters[lookup_parameters(lid_config.est_param[i][ii])].location.push_back(externalflux.size() - 1);
					parameters[lookup_parameters(lid_config.est_param[i][ii])].quan.push_back(lid_config.param_names[i][ii]);
					parameters[lookup_parameters(lid_config.est_param[i][ii])].location_type.push_back(5);
					parameters[lookup_parameters(lid_config.est_param[i][ii])].experiment_id.push_back("");
				}
			}

		}
	}
	set_features.buildup = true;
}

void CMediumSet::f_get_external_flux(CLIDconfig &lid_config)
{
	for (int i = 0; i<lid_config.keyword.size(); i++)
	{
		if ((tolower(lid_config.keyword[i]) == "external_flux") || (tolower(lid_config.keyword[i]) == "externalflux"))
		{
			CEnvExchange S(lid_config.param_vals[i][lookup(lid_config.param_names[i], "model")]);
			S.name = lid_config.value[i];

			for (int j = 0; j<lid_config.param_names[i].size(); j++)
			{
				if (lid_config.param_names[i][j] == "constituent") S.constituent = lid_config.param_vals[i][j];
				if (lid_config.param_names[i][j] == "solid") S.solid = lid_config.param_vals[i][j];
				S.set_val(lid_config.param_names[i][j], atof(lid_config.param_vals[i][j].c_str()));
				if (lid_config.param_names[i][j] == "expression") S.expression = lid_config.param_vals[i][j];
			}

			externalflux.push_back(S);

			for (int ii = 0; ii<lid_config.est_param[i].size(); ii++)
			{
				if (lookup_parameters(lid_config.est_param[i][ii]) != -1)
				{
					parameters[lookup_parameters(lid_config.est_param[i][ii])].location.push_back(externalflux.size() - 1);
					parameters[lookup_parameters(lid_config.est_param[i][ii])].quan.push_back(lid_config.param_names[i][ii]);
					parameters[lookup_parameters(lid_config.est_param[i][ii])].location_type.push_back(6);
					parameters[lookup_parameters(lid_config.est_param[i][ii])].experiment_id.push_back("");
				}
			}
		}
	}
	set_features.external_flux = true;
}

void CMediumSet::f_get_evaporation_model(CLIDconfig &lid_config)
{
	for (int i = 0; i<lid_config.keyword.size(); i++)
	{
		if ((tolower(lid_config.keyword[i]) == "evaporation_model"))
		{
			CEvaporation S(lid_config.param_vals[i][lookup(lid_config.param_names[i], "model")]);
			S.name = lid_config.value[i];

			for (int j = 0; j<lid_config.param_names[i].size(); j++)
			{
				if (lid_config.param_names[i][j] == "expression") S.expression = lid_config.param_vals[i][j];
				S.set_val(lid_config.param_names[i][j], atof(lid_config.param_vals[i][j].c_str()));
				if (lid_config.param_names[i][j] == "time_series") S.evaporation_filename = lid_config.param_vals[i][j];
				if (lid_config.param_names[i][j] == "uptake_constituents") S.uptake = atoi(lid_config.param_vals[i][j].c_str());
			}

			evaporation_model.push_back(S);

			for (int ii = 0; ii<lid_config.est_param[i].size(); ii++)
			{
				if (lookup_parameters(lid_config.est_param[i][ii]) != -1)
				{
					parameters[lookup_parameters(lid_config.est_param[i][ii])].location.push_back(evaporation_model.size() - 1);
					parameters[lookup_parameters(lid_config.est_param[i][ii])].quan.push_back(lid_config.param_names[i][ii]);
					parameters[lookup_parameters(lid_config.est_param[i][ii])].location_type.push_back(7);
					parameters[lookup_parameters(lid_config.est_param[i][ii])].experiment_id.push_back("");
				}
			}
		}
	}

	set_features.evaporation = true;
}

void CMediumSet::set_param(int param_no, double _value)
{

	for (int i = 0; i<parameters[param_no].location.size(); i++)
	{
		double value;
		if (parameters[param_no].conversion_factor.size())
			value = _value*parameters[param_no].conversion_factor[i];
		else
			value = _value;

		if ((parameters[param_no].location_type[i] == 2) || (parameters[param_no].location_type[i] == 1) || (parameters[param_no].location_type[i] == 0))
		{
			for (int j = 0; j < Medium.size(); j++)
				if (parameters[param_no].experiment_id[i] == Medium[j].name)
					Medium[j].set_param(param_no, value);
		}
		else if (parameters[param_no].location_type[i] == 3)
			RXN.parameters[RXN.look_up_rxn_parameters(parameters[param_no].quan[i])].value = value;
		else if (parameters[param_no].location_type[i] == 4)
			RXN.cons[parameters[param_no].location[i]].set_val(parameters[param_no].quan[i], value);
		else if (parameters[param_no].location_type[i] == 5)
			buildup[parameters[param_no].location[i]].set_val(parameters[param_no].quan[i], value);
		else if (parameters[param_no].location_type[i] == 6)
			externalflux[parameters[param_no].location[i]].set_val(parameters[param_no].quan[i], value);
		else if (parameters[param_no].location_type[i] == 7)
			evaporation_model[parameters[param_no].location[i]].set_val(parameters[param_no].quan[i], value);
	}

	for (int i = 0; i<measured_quan.size(); i++)
		if (measured_quan[i].std_to_param == param_no)
			std[measured_quan[i].std_no] = _value;
}

int CMediumSet::lookup_medium(string S)
{
	int j = -1;
	for (int i = 0; i < Medium.size(); i++)
		if (Medium[i].name == S) return i;
	return j;
}

double CMediumSet::calc_log_likelihood() //calculate sum log likelihood for time series data ts
{
	double sum = 0;
	solve();
	if (failed == true) return -1e30;

	for (int i = 0; i<measured_quan.size(); i++)
		sum += calc_log_likelihood(i);

	return sum;
}

double CMediumSet::calc_log_likelihood(int i) //calculate sum log likelihood for observed quantity i
{
	double sum = 0;
	int k = measured_data.lookup(measured_quan[i].name);
	if (k != -1)
	{
		if (measured_quan[i].error_structure == 0)
		{
			int k = measured_data.lookup(measured_quan[i].name);
			if (k != -1) sum -= diff(ANS_obs.BTC[i], measured_data.BTC[k]) / (2 * std[measured_quan[i].std_no] * std[measured_quan[i].std_no]);

		}
		if (measured_quan[i].error_structure == 1)
		{
			int k = measured_data.lookup(measured_quan[i].name);
			if (k != -1) sum -= diff(ANS_obs.BTC[i].Log(1e-4), measured_data.BTC[k].Log(1e-4)) / (2 * std[measured_quan[i].std_no] * std[measured_quan[i].std_no]);

		}
		sum -= measured_data.BTC[k].n*log(std[measured_quan[i].std_no]);
	}
	

	return sum;
}


void CMediumSet::finalize_set_param()
{
	for (int i = 0; i < Medium.size(); i++)
		Medium[i].finalize_set_param();
}

int CMediumSet::epoch_count()
{
	int out = 0;
	for (int i = 0; i < Medium.size(); i++) out += Medium[i].epoch_count;

	return out; 
}