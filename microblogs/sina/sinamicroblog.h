#ifndef SINAMICROBLOG_H
#define SINAMICROBLOG_H

#include <microblog.h>

class SinaMicroBlog : public Zzzz::MicroBlog
{
    public:
        explicit SinaMicroBlog( QObject* parent, const QVariantList& args );
        virtual ~SinaMicroBlog();
        virtual QByteArray oauthConsumerKey() const;
        virtual QByteArray oauthConsumerSecret() const;
        virtual void oauthRequestToken( QString& apiUrl, HttpMethod& method, ParamMap& params );
        virtual void oauthAuthorize( QString& apiUrl, ParamMap& params );
        virtual void oauthAccessToken( QString& apiUrl, HttpMethod& method, ParamMap& params );
        virtual void updateTimeline( Timeline t, QString& apiUrl, ParamMap& params );
        virtual void updateUserTimeline( QString& apiUrl, ParamMap& params, const QString& screenName );
        virtual QStringList timelines() const;
        virtual void updateTimeline( const QString& timeline, QString& apiUrl, ParamMap& params );
        virtual int charLimit() const;
        virtual void createPost( const Zzzz::Post& post, QString& apiUrl, ParamMap& params );
        virtual void removePost( const Zzzz::Post& post, QString& apiUrl, ParamMap& params );
        virtual void retweetPost( const Zzzz::Post& post, QString& apiUrl, ParamMap& params );
        virtual void readPostFromJsonMap( const QVariantMap& varmap, Zzzz::Post& post );
        virtual void readUserFromJsonMap( const QVariantMap& varmap, Zzzz::User& user );
};

#endif // SINAMICROBLOG_H
