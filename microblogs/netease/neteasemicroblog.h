#ifndef NETEASEMICROBLOG_H
#define NETEASEMICROBLOG_H

#include <microblog.h>

class NeteaseMicroBlog : public Zzzz::MicroBlog
{
    public:
        explicit NeteaseMicroBlog( QObject* parent, const QVariantList& args );
        virtual ~NeteaseMicroBlog();
        virtual QByteArray oauthConsumerKey() const;
        virtual QByteArray oauthConsumerSecret() const;
        virtual void oauthRequestToken( QString& apiUrl, HttpMethod& method, ParamMap& params );
        virtual void oauthAuthorize( QString& apiUrl, ParamMap& params );
        virtual void oauthAccessToken( QString& apiUrl, HttpMethod& method, ParamMap& params );
        virtual void updateTimeline( Timeline t, QString& apiUrl, ParamMap& params );
        virtual void updateUserTimeline( QString& apiUrl, ParamMap& params, const Zzzz::User& user );
        virtual QStringList timelines() const;
        virtual void updateTimeline( const QString& timeline, QString& apiUrl, ParamMap& params );
        virtual int charLimit() const;
        virtual void createPost( const Zzzz::Post& post, QString& apiUrl, ParamMap& params );
        virtual void removePost( const Zzzz::Post& post, QString& apiUrl, ParamMap& params );
        virtual void retweetPost( const Zzzz::Post& post, QString& apiUrl, ParamMap& params );
        virtual void readPostFromJsonMap( const QVariantMap& varmap, Zzzz::Post& post );
        virtual void readUserFromJsonMap( const QVariantMap& varmap, Zzzz::User& user );
};

#endif // NETEASEMICROBLOG_H
