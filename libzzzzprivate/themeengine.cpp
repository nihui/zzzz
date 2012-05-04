#include "themeengine.h"

#include "account.h"
#include <grantlee_core.h>

#include <KStandardDirs>

ThemeEngine* ThemeEngine::m_self = 0;

ThemeEngine* ThemeEngine::self()
{
    if (!m_self)
        m_self = new ThemeEngine;
    return m_self;
}

ThemeEngine::ThemeEngine()
{
    Grantlee::registerMetaType<PostWrapper>();

    m_engine = new Grantlee::Engine;
    Grantlee::FileSystemTemplateLoader::Ptr loader(new Grantlee::FileSystemTemplateLoader());

    KStandardDirs ksd;
    QStringList templateDirs = ksd.findDirs("data", "zzzz/themes");
// kWarning() << templateDirs;
    loader->setTemplateDirs(templateDirs);
    m_engine->addTemplateLoader(loader);
}

ThemeEngine::~ThemeEngine()
{
    delete m_engine;
}

QString ThemeEngine::render(const PostWrapper& post) const
{
    post.setUserLink("zzzz:user");
    post.setReplyLink("zzzz:reply");
    post.setRetweetLink("zzzz:retweet");

    Account* account = post.myAccount();
    if (account) {
        QString zzzztext = post.text();
        // @username
        QRegExp usernameRegex = account->microblog()->usernameMatch();
        zzzztext.replace(usernameRegex, "<a href=\"zzzz:username:\\1\">@\\1</a>");
        // #topic
        QRegExp topicRegex = account->microblog()->topicMatch();
        zzzztext.replace(topicRegex, "<a href=\"zzzz:topic:\\1\">#\\1#</a>");

        post.setZzzztext(zzzztext);
    }

    QVariantHash mapping;
    mapping.insert("post", QVariant::fromValue(post));

    Grantlee::Context c(mapping);
    Grantlee::Template t = m_engine->loadByName("simple.html");

    return t->render(&c);
}
