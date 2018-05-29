#ifndef IOC_H
#define IOC_H

#include "globals.h"
#include "singelton.h"
#include <QString>
class IResolvable {
public:	
    explicit IResolvable(const char * name=nullptr);
	~IResolvable();
	bool registerInstance(const QString& name);
	bool unRegisterInstance();
};

#include <QMap>
#include <QMutexLocker>

class IResolvable;
class IoC : public GenericSingleton<IoC>
{
	friend class GenericSingleton<IoC>;
	IoC() {}

public:
	~IoC();

	static bool registerNamedInstance(const QString & name, IResolvable * theInstance) {
		QMutexLocker locker(&IoC::instance()->myMutex);
		if (name.trimmed().isEmpty()) return false;
		if (IoC::instance()->myNamedResolvableInstance.contains(name))
			return true;
		IoC::instance()->myNamedResolvableInstance.insert(name, theInstance);
		return true;
	}

	static bool unregisterNamedInstance(const QString & name) {
		if (name.trimmed().isEmpty()) return false;
		QMutexLocker locker(&IoC::instance()->myMutex);		
		if (!IoC::instance()->myNamedResolvableInstance.contains(name)) return false;
		IoC::instance()->myNamedResolvableInstance.remove(name);
		return true;
	}

	static bool unregisterInstance(IResolvable * instance) {
		QMutexLocker locker(&IoC::instance()->myMutex);
		QString name = IoC::instance()->myNamedResolvableInstance.key(instance, "");
		if (name == "") return false;
		IoC::instance()->myNamedResolvableInstance.remove(name);
		return true;
	}

	template <class T>
	static T * resolveByName(const QString & name) {
		QMutexLocker locker(&instance()->myMutex);
		IResolvable * theInstance = instance()->myNamedResolvableInstance.value(name, NULL);
		return static_cast<T*>(theInstance);
	}

private:

	QMap<QString, IResolvable *> myNamedResolvableInstance;
	QMutex myMutex;
};

#endif // IOC_H
