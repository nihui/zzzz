#ifndef STATUSNETMICROBLOG_H
#define STATUSNETMICROBLOG_H

#include <tcmicroblog.h>

class StatusNetMicroBlog : public Zzzz::TwitterCompatibleAPIMicroBlog
{
    public:
        explicit StatusNetMicroBlog( QObject* parent, const QVariantList& args );
        virtual ~StatusNetMicroBlog();
        virtual QString apiRoot() const;
        virtual QByteArray oauthConsumerKey() const;
        virtual QByteArray oauthConsumerSecret() const;
};

#endif // STATUSNETMICROBLOG_H
