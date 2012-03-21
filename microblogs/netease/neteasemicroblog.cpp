#include "neteasemicroblog.h"

#include <types.h>
#include <utility.h>

#include <QDateTime>
#include <QDebug>
#include <QUrl>

ZZZZ_EXPORT_PLUGIN( NeteaseMicroBlog )

NeteaseMicroBlog::NeteaseMicroBlog( QObject* parent, const QVariantList& args )
: Zzzz::MicroBlog(parent,args)
{
}

NeteaseMicroBlog::~NeteaseMicroBlog()
{
}

QByteArray NeteaseMicroBlog::oauthConsumerKey() const
{
    return QByteArray( "pKUrJWMNcOC8pvju" );
}

QByteArray NeteaseMicroBlog::oauthConsumerSecret() const
{
    return QByteArray( "mmv5sfepYXRVnHiTyFe7Gbe9udNF3y0R" );
}

void NeteaseMicroBlog::oauthRequestToken( QString& apiUrl, HttpMethod& method, ParamMap& params )
{
    apiUrl = "http://api.t.163.com/oauth/request_token";
    method = GET;
}

void NeteaseMicroBlog::oauthAuthorize( QString& apiUrl, ParamMap& params )
{
    apiUrl = "http://api.t.163.com/oauth/authorize";
}

void NeteaseMicroBlog::oauthAccessToken( QString& apiUrl, HttpMethod& method, ParamMap& params )
{
    apiUrl = "http://api.t.163.com/oauth/access_token";
    method = GET;
}

void NeteaseMicroBlog::updateTimeline( Timeline t, QString& apiUrl, ParamMap& params )
{
    switch ( t ) {
        case Home:
            apiUrl = "http://api.t.163.com/statuses/home_timeline.json";
            params.insert( "count", "20" );
            break;
        case Public:
            apiUrl = "http://api.t.163.com/statuses/public_timeline.json";
            params.insert( "count", "20" );
            break;
        case Mentions:
            apiUrl = "http://api.t.163.com/statuses/mentions.json";
            params.insert( "count", "20" );
            break;
    }
}

void NeteaseMicroBlog::updateUserTimeline( const Zzzz::User& user, QString& apiUrl, ParamMap& params )
{
    apiUrl = "http://api.t.163.com/statuses/user_timeline.json";
    params.insert( "name", user.name.toUtf8().toPercentEncoding() );
    params.insert( "count", "20" );
}

QStringList NeteaseMicroBlog::timelines() const
{
    return QStringList();// << "retweets" << "location";
}

void NeteaseMicroBlog::updateTimeline( const QString& timeline, QString& apiUrl, ParamMap& params )
{
    if ( timeline == "mentions" ) {
        apiUrl = "http://api.t.163.com/statuses/mentions.json";
        params.insert( "count", "20" );
    }
    else if ( timeline == "user" ) {
        apiUrl = "http://api.t.163.com/statuses/user_timeline.json";
        params.insert( "count", "20" );
    }
    else if ( timeline == "retweets" ) {
        apiUrl = "http://api.t.163.com/statuses/retweets_of_me.json";
        params.insert( "count", "20" );
    }
    else if ( timeline == "location" ) {
        apiUrl = "http://api.t.163.com/statuses/location_timeline.json";
        params.insert( "count", "20" );
    }
}

int NeteaseMicroBlog::charLimit() const
{
    return 163;
}

void NeteaseMicroBlog::createPost( const Zzzz::Post& post, QString& apiUrl, ParamMap& params )
{
    qWarning() << "NeteaseMicroBlog::createPost";
    apiUrl = "http://api.t.163.com/statuses/update.json";
    params.insert( "status", QUrl::toPercentEncoding( post.text ) );
//     params.insert( "source", "Zzzz" );
}

void NeteaseMicroBlog::removePost( const Zzzz::Post& post, QString& apiUrl, ParamMap& params )
{
    qWarning() << "NeteaseMicroBlog::removePost";
    apiUrl = QString( "http://api.t.163.com/statuses/destroy/%1.json" ).arg( post.id );
}

void NeteaseMicroBlog::retweetPost( const Zzzz::Post& post, QString& apiUrl, ParamMap& params )
{
    qWarning() << "NeteaseMicroBlog::retweetPost";
    apiUrl = QString( "http://api.t.163.com/statuses/retweet/%1.json" ).arg( post.id );
}

void NeteaseMicroBlog::createFavorite( const Zzzz::Post& post, QString& apiUrl, ParamMap& params )
{
    apiUrl = QString( "http://api.t.163.com/favorites/create/%1.json" ).arg( post.id );
}

void NeteaseMicroBlog::removeFavorite( const Zzzz::Post& post, QString& apiUrl, ParamMap& params )
{
    apiUrl = QString( "http://api.t.163.com/favorites/destroy/%1.json" ).arg( post.id );
}

void NeteaseMicroBlog::createFriendship( const Zzzz::User& user, QString& apiUrl, ParamMap& params )
{
    apiUrl = "http://api.t.163.com/friendships/create.json";
    params.insert( "user_id", user.id.toUtf8() );
}

void NeteaseMicroBlog::removeFriendship( const Zzzz::User& user, QString& apiUrl, ParamMap& params )
{
    apiUrl = "http://api.t.163.com/friendships/destroy.json";
    params.insert( "user_id", user.id.toUtf8() );
}

void NeteaseMicroBlog::readPostFromJsonMap( const QVariantMap& varmap, Zzzz::Post& post )
{
    post.id = varmap["id"].toString();
    post.source = varmap["source"].toString();
    QVariantMap usermap = varmap["user"].toMap();
    readUserFromJsonMap( usermap, post.user );
    post.text = varmap["text"].toString();
    // extract http://126.fm/xxxx thumbnail_pic
    static QRegExp fm126Regex( "(http://126\\.fm/[^\\s\\W]+)", Qt::CaseSensitive );
    int pos = fm126Regex.indexIn( post.text );
    if ( pos > -1 ) {
        QString fm126url = fm126Regex.cap( 1 );
        post.thumbnailPic = QString( "http://oimagea2.ydstatic.com/image?h=120&url=%1" ).arg( fm126url );
        post.originalPic = QString( "http://oimagea2.ydstatic.com/image?url=%1" ).arg( fm126url );
        post.text = post.text.remove( fm126url ).trimmed();
    }
    Zzzz::Utility::urlize( post.text );
    post.creationDateTime = Zzzz::Utility::string2datetime( varmap["created_at"].toString() );
    post.replyToStatusId = varmap["in_reply_to_status_id"].toString();
    post.replyToUserId = varmap["in_reply_to_user_id"].toString();
    post.replyToUserName = varmap["in_reply_to_user_name"].toString();
    post.favorited = varmap["favorited"].toBool();
}

void NeteaseMicroBlog::readUserFromJsonMap( const QVariantMap& varmap, Zzzz::User& user )
{
    user.name = varmap["name"].toString();
    user.location = varmap["location"].toString();
    user.id = varmap["id"].toString();
    user.description = varmap["description"].toString();
    user.screenName = varmap["screen_name"].toString();
    user.followersCount = varmap["followers_count"].toInt();
    user.profileImageUrl = QUrl::fromPercentEncoding( varmap["profile_image_url"].toString().toUtf8() );
    user.url = varmap["url"].toString();
}
