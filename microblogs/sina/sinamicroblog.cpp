#include "sinamicroblog.h"

#include <types.h>
#include <utility.h>

#include <QDateTime>
#include <QDebug>
#include <QUrl>

ZZZZ_EXPORT_PLUGIN( SinaMicroBlog )

SinaMicroBlog::SinaMicroBlog( QObject* parent, const QVariantList& args )
: Zzzz::MicroBlog(parent,args)
{
}

SinaMicroBlog::~SinaMicroBlog()
{
}

QByteArray SinaMicroBlog::oauthConsumerKey() const
{
    return QByteArray( "3958149448" );
}

QByteArray SinaMicroBlog::oauthConsumerSecret() const
{
    return QByteArray( "75c7d73f76c90edbe668650d97b07eee" );
}

void SinaMicroBlog::oauthRequestToken( QString& apiUrl, HttpMethod& method, ParamMap& params )
{
    apiUrl = "http://api.t.sina.com.cn/oauth/request_token";
    method = GET;
    params.insert( "oauth_callback", "null" );
}

void SinaMicroBlog::oauthAuthorize( QString& apiUrl, ParamMap& params )
{
    apiUrl = "http://api.t.sina.com.cn/oauth/authorize";
}

void SinaMicroBlog::oauthAccessToken( QString& apiUrl, HttpMethod& method, ParamMap& params )
{
    apiUrl = "http://api.t.sina.com.cn/oauth/access_token";
    method = POST;
}

void SinaMicroBlog::updateTimeline( Timeline t, QString& apiUrl, ParamMap& params )
{
    switch ( t ) {
        case Home:
            apiUrl = "http://api.t.sina.com.cn/statuses/home_timeline.json";
            params.insert( "count", "20" );
            break;
        case Public:
            apiUrl = "http://api.t.sina.com.cn/statuses/public_timeline.json";
            params.insert( "count", "20" );
            break;
        case Mentions:
            apiUrl = "http://api.t.sina.com.cn/statuses/mentions.json";
            params.insert( "count", "20" );
            break;
    }
}

void SinaMicroBlog::updateUserTimeline( QString& apiUrl, ParamMap& params, const Zzzz::User& user )
{
    apiUrl = "http://api.t.sina.com.cn/statuses/user_timeline.json";
    params.insert( "screen_name", user.screenName.toUtf8().toPercentEncoding() );
    params.insert( "count", "20" );
}

QStringList SinaMicroBlog::timelines() const
{
    return QStringList();
}

void SinaMicroBlog::updateTimeline( const QString& timeline, QString& apiUrl, ParamMap& params )
{
    if ( timeline == "comments" ) {
        apiUrl = "http://api.t.sina.com.cn/statuses/comments_timeline.json";
        params.insert( "count", "20" );
    }
}

int SinaMicroBlog::charLimit() const
{
    return 150;
}

void SinaMicroBlog::createPost( const Zzzz::Post& post, QString& apiUrl, ParamMap& params )
{
    apiUrl = "http://api.t.sina.com.cn/statuses/update.json";
    params.insert( "status", QUrl::toPercentEncoding( post.text ) );
}

void SinaMicroBlog::removePost( const Zzzz::Post& post, QString& apiUrl, ParamMap& params )
{
    apiUrl = QString( "http://api.t.sina.com.cn/statuses/destroy/%1.json" ).arg( post.id );
}

void SinaMicroBlog::retweetPost( const Zzzz::Post& post, QString& apiUrl, ParamMap& params )
{
    apiUrl = "http://api.t.sina.com.cn/statuses/repost.json";
    params.insert( "id", post.id.toUtf8() );
}

void SinaMicroBlog::createFavorite( const Zzzz::Post& post, QString& apiUrl, ParamMap& params )
{
    apiUrl = "http://api.t.sina.com.cn/favorites/create.json";
    params.insert( "id", post.id.toUtf8() );
}

void SinaMicroBlog::removeFavorite( const Zzzz::Post& post, QString& apiUrl, ParamMap& params )
{
    apiUrl = QString( "http://api.t.sina.com.cn/favorites/destroy/%1.json" ).arg( post.id );
}

void SinaMicroBlog::createFriendship( const Zzzz::User& user, QString& apiUrl, ParamMap& params )
{
    apiUrl = QString( "http://api.t.sina.com.cn/friendships/create/%1.json").arg( user.id );
}

void SinaMicroBlog::removeFriendship( const Zzzz::User& user, QString& apiUrl, ParamMap& params )
{
    apiUrl = QString( "http://api.t.sina.com.cn/friendships/destroy/%1.json").arg( user.id );
}

void SinaMicroBlog::readPostFromJsonMap( const QVariantMap& varmap, Zzzz::Post& post )
{
    post.id = varmap["id"].toString();
    post.source = varmap["source"].toString();
    QVariantMap usermap = varmap["user"].toMap();
    readUserFromJsonMap( usermap, post.user );
    post.text = varmap["text"].toString();
    Zzzz::Utility::urlize( post.text );
    post.creationDateTime = Zzzz::Utility::string2datetime( varmap["created_at"].toString() );
    post.replyToStatusId = varmap["in_reply_to_status_id"].toString();
    post.replyToUserId = varmap["in_reply_to_user_id"].toString();
    post.replyToUserName = varmap["in_reply_to_user_name"].toString();
    post.favorited = varmap["favorited"].toBool();
    post.thumbnailPic = varmap["thumbnail_pic"].toString();
    post.originalPic = varmap["original_pic"].toString();

    QVariantMap retweeted_status = varmap["retweeted_status"].toMap();
    if ( !retweeted_status.isEmpty() ) {
        // retweet
        QVariantMap retweeted_user = retweeted_status["user"].toMap();
        post.text.append( " @" + retweeted_user["screen_name"].toString() );
        QString retweetText = retweeted_status["text"].toString();
        Zzzz::Utility::urlize( retweetText );
        post.text.append( " " + retweetText );
    }
}

void SinaMicroBlog::readUserFromJsonMap( const QVariantMap& varmap, Zzzz::User& user )
{
    user.name = varmap["name"].toString();
    user.location = varmap["location"].toString();
    user.id = varmap["id"].toString();
    user.description = varmap["description"].toString();
    user.screenName = varmap["screen_name"].toString();
    user.followersCount = varmap["followers_count"].toInt();
    user.profileImageUrl = varmap["profile_image_url"].toString();
    user.url = varmap["url"].toString();
}
