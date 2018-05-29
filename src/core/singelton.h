#ifndef SINGELTON_H
#define SINGELTON_H

#include "globals.h"
#include <mutex>

/**
 * A template class for rapid wrapping the singleton pattern around a class.
 */
template <class T>
class   GenericSingleton {

protected:
    GenericSingleton(){}
public:
    ~GenericSingleton(){}
private :
    static T * myInstance;
    static std::mutex  myMutex;

public :

    static T* instance() {
        if (myInstance == NULL) {
            std::lock_guard<std::mutex> lock(myMutex);
            if (myInstance == NULL)
                myInstance = new T;
        }
        return (T*) myInstance;
    }

    static void killInstance() {
        if ( myInstance!= NULL ) {
            std::lock_guard<std::mutex> lock(myMutex);
            if ( myInstance!= NULL ) {
                DELETE_AND_NULLIFY(myInstance);
            }
        }
    }
    static void reset(){
        std::lock_guard<std::mutex> lock(myMutex);
        if ( myInstance!= NULL  ) killInstance();
    }
};

template  <class T>
T* GenericSingleton<T>::myInstance = NULL;
template  <class T>
std::mutex  GenericSingleton<T>::myMutex;

#endif // SINGELTON_H
