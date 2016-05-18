#pragma once

#include <vector>
#include <string>

class CRxnNetwork;

using namespace std;

enum physical_params
{
	ks = 0, theta_s = 1, theta_r = 2, vg_alpha = 3, vg_n = 4, vg_m = 5, lambda = 6, storativity = 7, storage_epsilon = 15, storage_n = 16, light = 18, temperature = 19, wind=20, humidity=21
};

enum functions
{
	exp_ = 1, hsd_ = 2, min_ = 3, max_ = 4, lne_ = 5, lnt_ = 6, sgm_ = 7, pos_ = 8, sq1_ = 9, sqr_ = 10, frs_ = 11, fas_ = 12, ply_ = 13, mon_ = 14, sq2_ = 15, abs_ = 16, sqs_ = 17, mo1_=18
};


class CStringOP
{
public:
	CStringOP(void);
	CStringOP(string S);
	CStringOP(string S, CRxnNetwork *R);
	~CStringOP(void);
	CStringOP& CStringOP::operator = (const CStringOP &P);
	CStringOP(const CStringOP &P);
	vector<CStringOP> terms;
	int nterms;
	int nopts;
	vector<int> operators;
	vector<string> CStringOP::breakparents(string S);
	vector<int> CStringOP::breakops(string S);
	bool physical_ch;
	bool constant;
	bool parameter;
	bool s_concentration;
	bool concentration;
	bool function;
	bool s_block, t_block, a_block;
	double value;
	int number;
	int phase;
	int particle_type;
	string expression;
};

int opertr(char a);
bool isnumber(char S);
vector<string> getline(ifstream&); 
vector<vector<string>> getline_op(ifstream&,char del1);
vector<vector<string>> getline_op(ifstream&,vector<char> del1);
vector<string> split(const string &s, char del); 
vector<string> split(const string &s, const vector<char> &del);
vector<string> split_curly_semicolon(string s);
vector<vector<string>> getline_op_eqplus(ifstream&);
string trim(string s);
vector<int> ATOI(vector<string> ii);
vector<double> ATOF(vector<string> ii);
int getoperator(string S);
string tolower(string S);
vector<string> tolower(vector<string> S);
void writeline(ofstream&, vector<string>, string del);
void writeline(ofstream&, vector<vector<string>>, string del1, string del2);
void writestring(ofstream&, string s);
void writestring(string filename, string s);
void writenumber(ofstream&, double s);
void writeendl(ofstream&);
double Heavyside(double x);
double Pos(double x);
double pipe_poly(double x);
double mon(double x, double y);
vector<int> look_up(string s, char del); 
string numbertostring(double x);
string numbertostring(int x);
string numbertostring(vector<int> x);
string numbertostring(vector<double> x);
bool isintegernumber(string S);
int quan(string S);
//template<class _Ty> _Ty min(_Ty _X, _Ty _Y);
//template<class _Ty> _Ty max(_Ty _X, _Ty _Y);