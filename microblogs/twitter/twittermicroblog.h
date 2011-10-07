#ifndef TWITTERMICROBLOG_H
#define TWITTERMICROBLOG_H

#include <tcmicroblog.h>

class TwitterMicroBlog : public Zzzz::TwitterCompatibleAPIMicroBlog
{
    public:
        explicit TwitterMicroBlog( QObject* parent, const QVariantList& args );
        virtual ~TwitterMicroBlog();
        virtual QString apiRoot() const;
        virtual QByteArray oauthConsumerKey() const;
        virtual QByteArray oauthConsumerSecret() const;
};

#endif // TWITTERMICROBLOG_H
