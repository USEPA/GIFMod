
#pragma once
#include <vector>

using namespace std;

class CBinary  
{
public:
	CBinary();
	CBinary(int n);
	CBinary::CBinary(int n, int preci);
	virtual ~CBinary();
	int nDigits;
	vector<bool> Digit;
	int precision;
	CBinary(const CBinary &B);
	CBinary CBinary::operator = (const CBinary &B);
	CBinary CBinary::operator + (const CBinary &B);
	CBinary CBinary::extract(int spoint, int epoint);
	double CBinary::decode(double minrange);
	bool& CBinary::operator[](int i);
	void CBinary::show();
	bool sign;
	void CBinary::mutate(double mu);
	
};

CBinary code(double x, double minrange, double maxrange, int precision);
void cross(CBinary &B1, CBinary &B2, int p);
void cross(CBinary &B1, CBinary &B2, vector<int> p);
void cross2p(CBinary &B1, CBinary &B2, int p1, int p2);

