#pragma once
//#include <string>
#include "StringOP.h"

using namespace std;

class variant
{
public:
	variant() {
		value = "";
	};
	variant(bool b) {
		if (b)
			value = "true";
		else
			value = "false";
	};
	variant(int i) {
		value = to_string(i);
	};
	variant(float f) {
		value = to_string(f);
	};
	variant(double d) {
		value = to_string(d);
	};
	variant(string s) {
		value = s;
	};

	~variant()
	{};

	string value;
	string toString() {
		return value;
	}
	int toInt() {
		return atoi(value.c_str());
	}
	float toFloat() {
		return atof(value.c_str());
	}
	double toDouble() {
		return atof(value.c_str());
	}
	bool toBool() {
		string s = tolower(trim(value));
		if (s == "0" || s == "no" || s == "false" || s == "off")
			return false;
		else
			return true;
	}


};

