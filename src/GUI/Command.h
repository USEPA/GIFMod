#pragma once
#include "qstring.h"
#include "qmap.h"
#include "XString.h"
#include "StringOP.h"

class Node; 
class Edge; 
class Entity; 


class CCommand //this class stores commands 
{
public:
	CCommand(); //default constructor 
	CCommand(QString s);
	CCommand(const CCommand &s);
    CCommand& operator=(const CCommand &s);
	QString toQString();
	CCommand(string s);
	~CCommand();
	QString get_name() const;
	bool add_command(Node * n);
	bool add_command(Edge * e);
	bool add_command(Entity * en);
	QString command;
	QStringList values; 
	QMap<QString,XString> parameters; 
	QString Validate_text; 
	
};

QList<CCommand> sort_by_name(const QList<CCommand> &commands);


