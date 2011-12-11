#include "themeengine.h"

#include <grantlee_core.h>

#include <KStandardDirs>

ThemeEngine* ThemeEngine::m_self = 0;

ThemeEngine* ThemeEngine::self()
{
    if ( !m_self )
        m_self = new ThemeEngine;
    return m_self;
}

ThemeEngine::ThemeEngine()
{
    m_engine = new Grantlee::Engine;
    Grantlee::FileSystemTemplateLoader::Ptr loader( new Grantlee::FileSystemTemplateLoader() );

    KStandardDirs ksd;
    QStringList templateDirs = ksd.findDirs( "data", "zzzz/themes" );
// kWarning() << templateDirs;
    loader->setTemplateDirs( templateDirs );
    m_engine->addTemplateLoader( loader );
}

ThemeEngine::~ThemeEngine()
{
    delete m_engine;
}

QString ThemeEngine::render( const QVariantList& postList ) const
{
    QVariantHash mapping;
    mapping.insert( "posts", postList );

    Grantlee::Context c( mapping );
    Grantlee::Template t = m_engine->loadByName( "simple.html" );

    return t->render( &c );
}
