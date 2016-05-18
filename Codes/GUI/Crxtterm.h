#pragma once

#include <vector>

using namespace std;

class Crxtterm
{
public:
	Crxtterm(void);
	char type;
	int rxtno;
	vector<int> reactantno;
	~Crxtterm(void);
	vector<double> params;

};

