#include "tcmicroblog.h"

#include "types.h"
#include "utility.h"

#include <QDebug>
#include <QUrl>

namespace Zzzz {

TwitterCompatibleAPIMicroBlog::TwitterCompatibleAPIMicroBlog( QObject* parent, const QVariantList& args )
: MicroBlog(parent,args)
{
}

TwitterCompatibleAPIMicroBlog::~TwitterCompatibleAPIMicroBlog()
{
}

void TwitterCompatibleAPIMicroBlog::oauthRequestToken( QString& apiUrl, HttpMethod& method, ParamMap& params )
{
    apiUrl = apiRoot() + "/oauth/request_token";
    method = POST;
    params.insert( "oauth_callback", "oob" );
}

void TwitterCompatibleAPIMicroBlog::oauthAuthorize( QString& apiUrl, ParamMap& params )
{
    apiUrl = apiRoot() + "/oauth/authorize";
}

void TwitterCompatibleAPIMicroBlog::oauthAccessToken( QString& apiUrl, HttpMethod& method, ParamMap& params )
{
    apiUrl = apiRoot() + "/oauth/access_token";
    method = GET;
}

void TwitterCompatibleAPIMicroBlog::updateTimeline( Timeline t, QString& apiUrl, ParamMap& params )
{
    switch ( t ) {
        case Home:
            apiUrl = apiRoot() + "/statuses/home_timeline.json";
            params.insert( "count", "20" );
            break;
        case Public:
            apiUrl = apiRoot() + "/statuses/public_timeline.json";
            params.insert( "count", "20" );
            break;
        case Mentions:
            apiUrl = apiRoot() + "/statuses/mentions.json";
            params.insert( "count", "20" );
            break;
        default:
            break;
    }
}

void TwitterCompatibleAPIMicroBlog::updateUserTimeline( QString& apiUrl, ParamMap& params, const Zzzz::User& user )
{
    apiUrl = apiRoot() + "/statuses/user_timeline.json";
    params.insert( "screen_name", user.screenName.toUtf8().toPercentEncoding() );
    params.insert( "count", "20" );
}

QStringList TwitterCompatibleAPIMicroBlog::timelines() const
{
    return QStringList();// << "friends";
}

void TwitterCompatibleAPIMicroBlog::updateTimeline( const QString& timeline, QString& apiUrl, ParamMap& params )
{
    if ( timeline == "friends" ) {
        apiUrl = apiRoot() + "/statuses/friends_timeline.json";
        params.insert( "count", "20" );
    }
}

int TwitterCompatibleAPIMicroBlog::charLimit() const
{
    return 140;
}

void TwitterCompatibleAPIMicroBlog::createPost( const Zzzz::Post& post, QString& apiUrl, ParamMap& params )
{
    apiUrl = apiRoot() + "/statuses/update.json";
    params.insert( "status", QUrl::toPercentEncoding( post.text ) );
    params.insert( "in_reply_to_status_id", post.replyToStatusId.toUtf8() );
}

void TwitterCompatibleAPIMicroBlog::removePost( const Zzzz::Post& post, QString& apiUrl, ParamMap& params )
{
    apiUrl = apiRoot() + QString( "/statuses/destroy/%1.json" ).arg( post.id );
}

void TwitterCompatibleAPIMicroBlog::retweetPost( const Zzzz::Post& post, QString& apiUrl, ParamMap& params )
{
    apiUrl = apiRoot() + QString( "/statuses/retweet/%1.json" ).arg( post.id );
}

void TwitterCompatibleAPIMicroBlog::readPostFromJsonMap( const QVariantMap& varmap, Zzzz::Post& post )
{
    post.id = varmap["id"].toString();
    post.source = varmap["source"].toString();
    QVariantMap usermap = varmap["user"].toMap();
    readUserFromJsonMap( usermap, post.user );
    post.text = varmap["text"].toString();
    post.creationDateTime = Utility::string2datetime( varmap["created_at"].toString() );
    post.replyToStatusId = varmap["in_reply_to_status_id"].toString();
    post.replyToUserId = varmap["in_reply_to_user_id"].toString();
    post.replyToUserName = varmap["in_reply_to_screen_name"].toString();
    post.favorited = varmap["favorited"].toBool();
}

void TwitterCompatibleAPIMicroBlog::readUserFromJsonMap( const QVariantMap& varmap, Zzzz::User& user )
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

}
