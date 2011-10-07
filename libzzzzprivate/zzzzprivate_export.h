#ifndef ZZZZPRIVATE_EXPORT_H
#define ZZZZPRIVATE_EXPORT_H

#include <kdemacros.h>

#ifdef MAKE_ZZZZPRIVATE_LIB
// we are building this library
#define ZZZZPRIVATE_EXPORT KDE_EXPORT
#else
// we are using this library
#define ZZZZPRIVATE_EXPORT KDE_IMPORT
#endif

#endif // ZZZZPRIVATE_EXPORT_H
