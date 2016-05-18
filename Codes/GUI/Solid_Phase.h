#pragma once
#include "StringOP.h"

enum colloids_params {
	kair = 2, irrevcolfraction = 3, specsurfarea = 4, attachmenteff = 5, collectioneff = 6, gmax = 7, releasecoeff = 8, critvel = 9, bulkdensity = 10, dispersivity = 11
};



class CSolid_Phase
{
public:
	int n_phases;
	vector<vector<CStringOP>> f; 
	vector<vector<CStringOP>> K;
	vector<CStringOP> capacity;
	vector<CStringOP> capacity_c;
	vector<double> mobility_factor;
	CStringOP dispersion;
	vector<double> c_params;
	CSolid_Phase(int n=3);
	~CSolid_Phase(void);
	CSolid_Phase(const CSolid_Phase& BB);
	CSolid_Phase::CSolid_Phase(string model);
	CSolid_Phase& CSolid_Phase::operator=(const CSolid_Phase &BB);
	void CSolid_Phase::set_val(int i, double val);
	void CSolid_Phase::set_val(string SS, double val);
	int indicator;
	string model;
	string name;
	vector<string> phase_names;
	double vs;
	double diffusion;
	int CSolid_Phase::lookup_phase(string S);

};

