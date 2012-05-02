#include "themeengine.h"

#include "account.h"
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
    Grantlee::registerMetaType<PostWrapper>();

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

QString ThemeEngine::render( const QList<PostWrapper>& posts ) const
{
    QVariantList postList;

    QList<PostWrapper>::ConstIterator it = posts.constBegin();
    QList<PostWrapper>::ConstIterator end = posts.constEnd();
    int i = 0;
    while ( it != end ) {
        const PostWrapper& post = *it;

        post.setUserLink( QString( "zzzz:user:%1" ).arg( i ) );
        post.setReplyLink( QString( "zzzz:reply:%1" ).arg( i ) );
        post.setRetweetLink( QString( "zzzz:retweet:%1" ).arg( i ) );

        QString zzzztext = post.text();
        // @username
        QRegExp usernameRegex = post.myAccount()->microblog()->usernameMatch();
        zzzztext.replace( usernameRegex, QString( "<a href=\"zzzz:username:%1:\\1\">@\\1</a>" ).arg( i ) );
        // #topic
        QRegExp topicRegex = post.myAccount()->microblog()->topicMatch();
        zzzztext.replace( topicRegex, QString( "<a href=\"zzzz:topic:%1:\\1\">#\\1#</a>" ).arg( i ) );

        post.setZzzztext( zzzztext );

        ++i;

        ++it;

        postList.append( QVariant::fromValue( post ) );
//         kWarning() << p.user.profileImageUrl;
    }

    QVariantHash mapping;
    mapping.insert( "posts", postList );

    Grantlee::Context c( mapping );
    Grantlee::Template t = m_engine->loadByName( "simple.html" );

    return t->render( &c );
}
