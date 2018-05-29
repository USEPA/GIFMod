#ifndef GLOBALS_H
#define GLOBALS_H
#ifndef _Debug
#define _Debug(msg)   qDebug(QString(msg).toStdString().data());
#endif

#ifndef SAFE_FREE
#define SAFE_FREE(x) if(x) { free(x); x = NULL; }
#endif

#ifndef DELETE_AND_NULLIFY
#define DELETE_AND_NULLIFY(X)   if (X) { delete X; X = NULL; }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(X) if(X) { delete[] X; X = NULL; }
#endif

#include "typednames.h"

#endif // GLOBALS_H
