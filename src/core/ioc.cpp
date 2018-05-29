#include "ioc.h"

IResolvable::IResolvable(const char * name){
    if (name!= nullptr)
        registerInstance(QString(name));
}

bool IResolvable::registerInstance(const QString& name){
    unRegisterInstance();
    return IoC::registerNamedInstance(name,this);
}

bool IResolvable::unRegisterInstance(){
    return IoC::unregisterInstance(this);
}

IResolvable::~IResolvable(){
    unRegisterInstance();
    qDebug("dependency unregistred");
}

IoC::~IoC(){
      myNamedResolvableInstance.clear();
}
