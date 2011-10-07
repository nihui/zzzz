#ifndef ZZZZ_EXPORT_H
#define ZZZZ_EXPORT_H

#include <kdemacros.h>
#include <KPluginFactory>
#include <KPluginLoader>

#ifdef MAKE_ZZZZ_LIB
// we are building this library
#define ZZZZ_EXPORT KDE_EXPORT
#else
// we are using this library
#define ZZZZ_EXPORT KDE_IMPORT
#endif

#define ZZZZ_EXPORT_PLUGIN( classname ) \
    K_PLUGIN_FACTORY( ZzzzFactory, registerPlugin< classname >(); ) \
    K_EXPORT_PLUGIN( ZzzzFactory( "classname" ) )

#endif // ZZZZ_EXPORT_H
