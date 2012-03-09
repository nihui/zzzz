#ifndef TCMICROBLOG_H
#define TCMICROBLOG_H

#include <microblog.h>

namespace Zzzz {

class ZZZZ_EXPORT TwitterCompatibleAPIMicroBlog : public MicroBlog
{
    public:
        explicit TwitterCompatibleAPIMicroBlog( QObject* parent, const QVariantList& args );
        virtual ~TwitterCompatibleAPIMicroBlog();
        virtual QString apiRoot() const = 0;
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
        virtual void createFavorite( const Zzzz::Post& post, QString& apiUrl, ParamMap& params );
        virtual void removeFavorite( const Zzzz::Post& post, QString& apiUrl, ParamMap& params );
        virtual void createFriendship( const Zzzz::User& user, QString& apiUrl, ParamMap& params );
        virtual void removeFriendship( const Zzzz::User& user, QString& apiUrl, ParamMap& params );
    protected:
        virtual void readPostFromJsonMap( const QVariantMap& varmap, Zzzz::Post& post );
        virtual void readUserFromJsonMap( const QVariantMap& varmap, Zzzz::User& user );
};

}

#endif // TCMICROBLOG_H
