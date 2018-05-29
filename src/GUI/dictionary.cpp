#include "dictionary.h"
#include "enums.h"
#include <iostream>
#include <fstream>

using namespace std; 

Dictionary::Dictionary()
	: QMap<QString, QString>()
{

}

Dictionary::Dictionary(QString filename) : QMap<QString, QString>()
{
	
	ifstream file(filename.toStdString());
	if (!file.good()) return;

	while (file.eof() == false)
	{
		string line;
		getline(file, line);

		int a = line.find_first_of('//');
		if (a != 0)
		{
			//			QStringList s = QSplit(QString::fromStdString(line), ',');
			QString XS = QString::fromStdString(line);
			QStringList s = XS.split(',');
			if (s.size() >= 2)
				insert(s[0],s[1]);
			
		}
	}
	
	return; 
}

Dictionary::~Dictionary()
{

}
