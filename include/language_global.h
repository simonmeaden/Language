#pragma once

#include <QtCore/qglobal.h>

#if defined(LANGUAGE_LIBRARY)
  #define LANGUAGE_SHARED_EXPORT Q_DECL_EXPORT
#else
  #define LANGUAGE_SHARED_EXPORT Q_DECL_IMPORT
#endif

