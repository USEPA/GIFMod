#pragma once

#include <qstring.h>
#include <vector>
#include "qlist.h"
#include "qstringlist.h"

class Wizard_Script_Reader; 


using namespace std; 
class CExpression
{
public:
	CExpression(void);
	CExpression(QString S);
	CExpression(const CExpression &S);
    	CExpression& operator=(const CExpression&);
	QList<QString> operators; 
	QList<CExpression> terms; 
	QString sign;
	QString function; 
	QString parameter; 
	double constant; 
	QString param_constant_expression; 
	~CExpression(void);
	QStringList extract_operators(QString s);
	QStringList _errors;
	QStringList extract_terms(QString s);
	double calc(Wizard_Script_Reader * W);
	double func(QString & f, double val);
	double func(QString & f, double val1, double val2);
	double oprt(QString & f, double val1, double val2);
	double oprt(QString & f, int i1, int i2, Wizard_Script_Reader * W);
	QStringList funcs;
	QString unit; 
	QString text; 
	QStringList opts; 

private:
	vector<double> term_vals;
	vector<bool> terms_calculated;
	QList<QList<int>> sources;
};

int corresponding_parenthesis(QString S, int i);
bool parantheses_balance(QString S);

