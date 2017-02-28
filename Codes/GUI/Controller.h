#pragma once
#include "Sensor.h"
#include "utility_funcs.h"
//test

class CController
{
public:
	CController();
	~CController();
	CController(const CController &M);
	CController& CController::operator=(const CController &BB);
	string name; 
	CBTC output;
	string type;
	CSensor *Sensor;
	string sensor_id;
	double calc_value(double t, int experiment_id);
	void append(double t, double C); 
	vector<double> params; // for PID controller params[0] = k_p, params[1] = k_i, params[2] = k_d; params[3] = set point
	double P(double t, int experiment_id);
	double I(double t, int experiment_id);
	double D(double t, int experiment_id);
	void CController::set_val(string S, double val);
	double value;
	double interval;
	double min_val = 0;
	double max_val = 1e12;
	range application_spec;
	string zn_controller_type;
	double integral = 0; 
	void set_zn();
};

