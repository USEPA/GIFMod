#include "stdafx.h"
#include "Expression.h"
#include "utility_funcs.h"
#include "Wizard/Wizard_Script_Reader.h"
#include "qlist.h"


CExpression::CExpression(void)
{
	funcs << "_min" << "_max" << "_exp" << "_log" << "_abs" << "_sqr";
	opts << "+" << "-" << "*" << ";" << "/" << "^";
}

CExpression::CExpression(QString S)
{
	text = S; 
	funcs << "_min" << "_max" << "_exp" << "_log" << "_abs" << "_sqr";
	opts << "+" << "-" << "*" << ";" << "/" << "^";
    if (S.contains("|"))
    {   unit = extract_between(S,"|","|");
        S = S.left(S.indexOf("|"));
    }
    QStringList out;
	bool inside_quote = false;
	int paranthesis_level = 0;
	int last_operator_location = -1; 
	if (!parantheses_balance(S))
	{
		_errors.append("Parantheses do not match in" + S);
		return;
	}
	if (funcs.contains(S.left(4)))
	{
		function = S.left(4).right(3);
	}
	if (S.left(1) == "(")
	{
		if (corresponding_parenthesis(S,0) == -1 )
		{
			_errors.append("Parantheses do not match in" + S);
		}
		else if (corresponding_parenthesis(S,0) == S.size()-1)
		{
			S.remove(0, 1);
			S.remove(S.size() - 1, 1);
			//if (opts.contains(S.left(1)))
			//	terms.append(CExpression("0"));
			if (funcs.contains(S.left(4)))
			{
				function = S.left(4).right(3);
			}

		}
	}
	if (isnumber(S.toStdString()))
	{
		param_constant_expression = "constant"; 
		constant = S.toDouble(); 
	}
	else
	{
		if (S.contains("(") || S.contains(")") || S.contains("+") || S.contains("-") || S.contains("*") || S.contains("/") || S.contains("^") || S.contains(";"))
		{
			param_constant_expression = "expression";
			for (int i = 0; i < S.size(); i++)
			{
				if (S.mid(i, 1) == "(")
					paranthesis_level++;

				if (S.mid(i, 1) == ")")
					paranthesis_level--;

				if (paranthesis_level == 0)
					if ((S.mid(i, 1) == "+") || (S.mid(i, 1) == "-") || (S.mid(i, 1) == "*") || (S.mid(i, 1) == "/") || (S.mid(i, 1) == "^"))
					{
						operators.append(S.mid(i, 1));
						CExpression sub_exp = CExpression(S.mid(last_operator_location+1, i -1- last_operator_location).trimmed());
						if (sub_exp.text != "")
						{
							if (operators.size() > 1)
								sub_exp.sign = operators[operators.size() - 2];
							else
								sub_exp.sign = "+";
							terms.append(sub_exp);
						}
						else
						{
							sub_exp = CExpression("0");
							sub_exp.sign = "+";
							terms.append(sub_exp);
						}
						_errors.append(sub_exp._errors);
						last_operator_location = i;
					}
			}
				
			CExpression sub_exp = CExpression(S.mid(last_operator_location+1, S.size() - last_operator_location).trimmed());
			if (operators.size() > 0)
				sub_exp.sign = operators[operators.size() - 1];
			else
				sub_exp.sign = "+";
			terms.append(sub_exp);
			_errors.append(sub_exp._errors);
				
		}
		else
		{
			param_constant_expression = "parameter";
			parameter = S; 
		}
	}
	
	
	
}

CExpression::CExpression(const CExpression & S)
{
	operators = S.operators;
	constant = S.constant;
	terms = S.terms; 
	sign = S.sign;
	funcs = S.funcs;
	opts = S.opts; 
	function = S.function;
	parameter = S.parameter;
	param_constant_expression = S.param_constant_expression;
	unit = S.unit;
	text = S.text;
}

CExpression & CExpression::operator=(const CExpression &S)
{
	operators = S.operators;
	constant = S.constant;
	terms = S.terms;
	sign = S.sign;
	funcs = S.funcs;
	opts = S.opts;
	function = S.function;
	parameter = S.parameter;
	param_constant_expression = S.param_constant_expression;
	unit = S.unit;
	text = S.text;

	return *this; 
}


CExpression::~CExpression(void)
{
}

QStringList CExpression::extract_operators(QString s)
{
	QStringList out;
	bool inside_quote = false;
	int paranthesis_level = 0;
	for (int i = 0; i < s.size(); i++)
	{
		if (s.mid(i, 1) == "(")
			paranthesis_level++;

		if (s.mid(i, 1) == ")")
			paranthesis_level--;

		if (paranthesis_level == 0)
			if ((s.mid(i, 1) == "+") || (s.mid(i, 1) == "-") || (s.mid(i, 1) == "*") || (s.mid(i, 1) == "/") || (s.mid(i, 1) == "^")) out.append(s.mid(i, 1));


	}
		
	return out;
}

QStringList CExpression::extract_terms(QString s)
{
	bool inside_quote = false;
	for (int i = 0; i < s.size(); i++)
	{
		if (s.mid(i, 1) == "'")
		{
			inside_quote = !inside_quote;
			s.remove(i, 1);
		}
		if (inside_quote)
			if (s.mid(i, 1) == " ") s.replace(i, 1, "|");

	}
	QStringList out = s.split(" ");
	for (int i = 0; i < out.size(); i++) out[i].replace("|", " ");
	return out;
}


double CExpression::calc(Wizard_Script_Reader *W)
{
	if (this->param_constant_expression == "constant") 
		return constant;
	if (this->param_constant_expression == "parameter")
	{
		XString out; 
		out = W->get_parameter_value(parameter);
		if (W->get_parameter_value(parameter).unit != "" && unit == "")
			unit = W->get_parameter_value(parameter).unit;
		return out.toDouble(); 
	}
	if (this->param_constant_expression == "expression")
	{
		
		for (int i = 0; i < terms.size(); i++)
		{
			sources.append(QList<int>());
			sources[i].append(i);
		}
		term_vals.resize(terms.size());
		for (int i = 0; i < terms.size(); i++) terms_calculated.push_back(false);
		for (int i = operators.size() - 1; i >= 0; i--)
		{
			if (operators[i] == "^")
				oprt(operators[i], i, i + 1, W);
		}
		for (int i = operators.size() - 1; i >= 0; i--)
		{
			if (operators[i] == "*")
				oprt(operators[i], i, i + 1, W);
		}

		for (int i = operators.size() - 1; i >= 0; i--)
		{
			if (operators[i] == "/")
				oprt(operators[i], i, i + 1, W);
		}

		for (int i = operators.size() - 1; i >= 0; i--)
		{
			if (operators[i] == "+")
				oprt(operators[i], i, i + 1, W);
			
		}

		for (int i = operators.size() - 1; i >= 0; i--)
		{
			if (operators[i] == "-")
			{
				oprt(operators[i], i, i + 1, W);
			}
		}
		if (function == "")
			return term_vals[0];
		else if (!operators.contains(";"))
			return func(function, term_vals[0]);
		else 
			return func(function, term_vals[0], term_vals[1]);
	}
	
}

double CExpression::func(QString &f, double val)
{
	
	if (f == "_exp")
			return exp(val);
	if (f == "_log")
		return log(val);
	if (f == "_abs")
		return fabs(val);
	if (f == "_sqr")
		return sqrt(val);
	return val;
}

double CExpression::func(QString &f, double val1, double val2)
{
	if (f == "_min")
		return min(val1, val2);
	if (f == "_max")
		return max(val1, val2);
	return val1; 
}

double CExpression::oprt(QString &f, double val1, double val2)
{
	if (f == "^") return pow(val1, val2);
	if (f == "+") return val1 + val2;
	if (f == "-") return val1 + val2;
	if (f == "/") return val1 * val2;
	if (f == "*") return val1*val2;
}

double CExpression::oprt(QString &f, int i1, int i2, Wizard_Script_Reader *W)
{

	for (int j = 0; j < sources[i1].size(); j++)
	{
		if (sources.size() > i2)
			for (int k=0; k<sources[i2].size(); k++)
				if (!sources[sources[i2][k]].contains(sources[i1][j])) sources[sources[i2][k]].append(sources[i1][j]);
		
	}
	if (sources.size() > i2)
	for (int j = 0; j < sources[i2].size(); j++)
	{
		for (int k = 0; k<sources[i1].size(); k++)
			if (!sources[sources[i1][k]].contains(sources[i2][j])) sources[sources[i1][k]].append(sources[i2][j]);

	}

	double val1; 
	double val2; 
	if (terms_calculated[i1]) val1 = term_vals[i1]; else val1 = terms[i1].calc(W);
	if (terms[i1].sign == "/") val1 = 1/val1;
	if (terms[i1].sign == "-") val1 = -val1;
	if (sources.size() > i2)
		if (terms_calculated[i2]) val2 = term_vals[i2]; else
		{
			val2 = terms[i2].calc(W);
			if (terms[i2].sign == "/") val2 = 1 / val2;
			if (terms[i2].sign == "-") val2 = -val2;
		}
	else
	{
		val1 = 0;
		val2 = val1;
	}

	if (unit == "")
	{
		if (terms[i1].unit != "") unit = terms[i1].unit;
		else if (terms[i2].unit != "") unit = terms[i2].unit;
	}

	for (int j = 0; j<sources[i1].size(); j++)
		term_vals[sources[i1][j]] = oprt(f, val1, val2);

	terms_calculated[i1] = true;
	terms_calculated[i2] = true;

	return term_vals[sources[i1][0]];
}

int corresponding_parenthesis(QString S, int i)
{
	string s = S.toStdString(); 
    if (S.at(i) == QChar('('))
	{
		int paranthesis_level = 1; 
		for (int j = i+1; j < S.size(); j++)
		{
            if (S.at(j) == '(')
				paranthesis_level++;
            if (S.at(j) == ')')
				paranthesis_level--;

			if (paranthesis_level == 0)
				return j;
		}
		return -1;
	}
	

    if (S.at(i) == ')')
	{
		int paranthesis_level = 1;
		for (int j = i-1; j > 0; j--)
		{
            if (S.at(j) == ')')
				paranthesis_level++;
            if (S.at(j) == '(')
				paranthesis_level--;

			if (paranthesis_level == 0)
				return j;
		}
		return -1;
	}
}

bool parantheses_balance(QString S)
{
	if (S.count("(") == S.count(")"))
		return true;
	else
		return false;
}
