#ifndef ZZZZ_MICROBLOG_H
#define ZZZZ_MICROBLOG_H

#include "plugin.h"

#include "types.h"

#include <qjson/parser.h>

namespace Zzzz {

class Post;
class User;

class ZZZZ_EXPORT MicroBlog : public Plugin
{
    Q_OBJECT
    public:
        typedef enum {
            Home        = 0,
            Public      = 1,
            User        = 2,
            Mentions    = 3,

            UserMin     = 256
        } Timeline;

        typedef enum {
            GET         = 0,
            POST        = 1
        } HttpMethod;

        typedef QMultiMap<QByteArray, QByteArray> ParamMap;

        explicit MicroBlog( QObject* parent, const QVariantList& args );
        virtual ~MicroBlog();
        /// oauth part
        virtual QByteArray oauthConsumerKey() const = 0;
        virtual QByteArray oauthConsumerSecret() const = 0;
        virtual void oauthRequestToken( QString& apiUrl, HttpMethod& method, ParamMap& params ) = 0;
        virtual void oauthAuthorize( QString& apiUrl, ParamMap& params ) = 0;
        virtual void oauthAccessToken( QString& apiUrl, HttpMethod& method, ParamMap& params ) = 0;
        /// timeline part
        virtual void updateTimeline( Timeline t, QString& apiUrl, ParamMap& params ) = 0;
        virtual QStringList timelines() const = 0;
        virtual void updateTimeline( const QString& timeline, QString& apiUrl, ParamMap& params ) = 0;
        /// char limit
        virtual int charLimit() const = 0;
        /// post part
        virtual void createPost( const Zzzz::Post& post, QString& apiUrl, ParamMap& params ) = 0;
        virtual void removePost( const Zzzz::Post& post, QString& apiUrl, ParamMap& params ) = 0;
        virtual void retweetPost( const Zzzz::Post& post, QString& apiUrl, ParamMap& params ) = 0;
        /// data part
        virtual void readTimelineFromData( const QByteArray& data, QList<Zzzz::Post>& postlist, bool* ok );
        virtual void readPostFromData( const QByteArray& data, Zzzz::Post& post, bool* ok );
    protected:
        virtual void readPostFromJsonMap( const QVariantMap& varmap, Zzzz::Post& post ) = 0;
        virtual void readUserFromJsonMap( const QVariantMap& varmap, Zzzz::User& user ) = 0;
    protected:
        QJson::Parser m_parser;
    private:
};

}

#endif // ZZZZ_MICROBLOG_H
