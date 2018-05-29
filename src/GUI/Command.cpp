
#include "Command.h"
#include "utility_funcs.h"
#include "node.h"
#include "edge.h"
#include "entity.h"


CCommand::CCommand()
{
}

CCommand::CCommand(QString s) //create command from script
{
	
	QStringList list = s.split(":");
	if (list.size() == 0) return;
	 
	command = extract_by_space_quote(list[0].trimmed().toLower())[0];
	if (extract_by_space_quote(list[0].trimmed().toLower()).size() > 1) values = XString::reform(extract_by_space_quote(list[0].trimmed()));
	values.removeAt(0);
	for (int i = 0; i < values.count(); i++) values[i] = values[i].trimmed();
	if (list.size() > 1)
	{
		QStringList param_list = list[1].split(",");
		for (int i = 0; i < param_list.size(); i++)
		{
			if (param_list[i].split("=").size() != 2)
				Validate_text = "Syntax error";
			else
			{
				parameters[XString::reform(param_list[i].split("=")[0].trimmed())] = XString::reform(param_list[i].split("=")[1].trimmed().split("[")[0]);
				parameters[XString::reform(param_list[i].split("=")[0].trimmed())].unit = XString::reform(extract_in_between(param_list[i].split("=")[1].trimmed().toLower(), "[", "]"));
			}
		}
	}
}

CCommand::CCommand(const CCommand & s)
{
	values = s.values;
	parameters = s.parameters;
	command = s.command;
	Validate_text = s.Validate_text;
}

CCommand& CCommand::operator=(const CCommand & s)
{
	command = s.command;
	parameters = s.parameters;
	values = s.values;
	Validate_text = s.Validate_text;

	return *this;
}

QString CCommand::toQString()
{
	QStringList gofirst; 
	gofirst << "Name" << "Type" << "SubType";
	QString S; 
	S.append(command);
	S.append(" ");
	for (int i = 0; i < values.count(); i++)
	{
		S.append("'" + values[i] +"'");
		S.append(" ");
	}
	S.append(":");
	int i = 0;
	for (QString key : parameters.keys())
	{
		if (gofirst.contains(key))
		{
			S.append(XString::reformBack(key));
			S.append("=");
			S.append(XString::reformBack(parameters[key].toQString()));
			if (parameters[key].unit != "")
			{
				S.append("[");
				S.append(XString::reformBack(parameters[key].unit));
				S.append("]");
			}
			if (i < parameters.size() - 1) S.append(",");
			i++;
		}
	}
	
	for (QString key : parameters.keys())
	{
		if (!gofirst.contains(key))
		{
			S.append(XString::reformBack(key));
			S.append("=");
			S.append(XString::reformBack(parameters[key].toQString()));
			if (parameters[key].unit != "")
			{
				S.append("[");
				S.append(XString::reformBack(parameters[key].unit));
				S.append("]");
			}
			if (i < parameters.size() - 1) S.append(",");
			i++;
		}
	}
	return S;
}

CCommand::CCommand(string s)
{
	CCommand(QString(s.c_str()));
}




CCommand::~CCommand()
{
}

QList<CCommand> sort_by_name(const QList<CCommand> &commands)
{
	QList<int> dones; 
	QList<CCommand> outcommands; 
	int TopSelected = 0;
	for (int i = 0; i < commands.count(); i++)
	{
		QString TopName = "zzzz";
		
		for (int j = 0; j < commands.count(); j++)
		{
			if (commands[j].get_name() < TopName && !dones.contains(j))
			{
				TopSelected = j;
				TopName = commands[TopSelected].get_name(); 
			}
			
		}
		dones.append(TopSelected);
		outcommands.append(commands[TopSelected]);
	}
	return outcommands;

}

QString CCommand::get_name() const
{
	if (parameters.count("name") > 0)
		return parameters["name"];
	else if (parameters.count("Name") > 0)
		return parameters["Name"];
	else
		return "";
		
}

bool CCommand::add_command(Node *n)
{
	command = "add";
	values.append(n->objectType.ObjectType);
    for (mProp prop : n->getmList(n->objectType).List)
	{
		if (!n->val(prop.VariableCode).isEmpty() && n->val(prop.VariableCode) != "Default")
			if (n->val(prop.VariableCode).replace("...", "").trimmed() != "")
			{
				XString val = n->getValue(prop.VariableName);
				parameters[prop.VariableName] = val.replace("...", "");
				parameters[prop.VariableName].unit = val.unit;
			}
	}

	return true; 
}

bool CCommand::add_command(Edge * e)
{
	command = "connect";
	values.append(e->sourceNode()->Name());
	values.append(e->destNode()->Name());
    for (mProp prop : e->getmList(e->objectType).List)
	{
		if (!e->val(prop.VariableCode).isEmpty() && e->val(prop.VariableCode) != "Default")
			if (e->val(prop.VariableCode).replace("...", "").trimmed() != "")
			{
				XString val = e->getValue(prop.VariableName);
				parameters[prop.VariableName] = val.replace("...", "");
				parameters[prop.VariableName].unit = val.unit;
			}

	}
	return true; 
}

bool CCommand::add_command(Entity * en)
{
	command = "add";
	values.append(en->objectType.ObjectType);
    for (mProp prop : en->getmList(en->objectType).List)
	{
		if (!en->val(prop.VariableCode).isEmpty() && en->val(prop.VariableCode) != "Default")
			if (en->val(prop.VariableCode).replace("...", "").trimmed() != "")
			{
				XString val = en->getValue(prop.VariableName);
				parameters[prop.VariableName] = val.replace("...", "");
				parameters[prop.VariableName].unit = val.unit;
			}
	}
	return true; 
}
