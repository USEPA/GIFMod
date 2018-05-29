#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "qmap.h"
#include "qstring.h"


class Dictionary : public QMap<QString, QString>
{
	

public:
	Dictionary();
	Dictionary(QString filename);
	~Dictionary();
	
private:
	
};

#endif // DICTIONARY_H
