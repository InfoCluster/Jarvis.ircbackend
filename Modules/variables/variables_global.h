#ifndef VARIABLES_GLOBAL_H
#define VARIABLES_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(VARIABLES_LIBRARY)
#  define VARIABLESSHARED_EXPORT Q_DECL_EXPORT
#else
#  define VARIABLESSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // VARIABLES_GLOBAL_H
