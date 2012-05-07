#include "sohumicroblog.h"

#include <types.h>

#include <QDebug>
#include <QUrl>

ZZZZ_EXPORT_PLUGIN(SohuMicroBlog)

SohuMicroBlog::SohuMicroBlog(QObject* parent, const QVariantList& args)
    : Zzzz::MicroBlog(parent, args)
{
}

SohuMicroBlog::~SohuMicroBlog()
{
}

QByteArray SohuMicroBlog::oauthConsumerKey() const
{
    return QByteArray("");
}

QByteArray SohuMicroBlog::oauthConsumerSecret() const
{
    return QByteArray("");
}

void SohuMicroBlog::oauthRequestToken(QString& apiUrl, HttpMethod& method, ParamMap& params)
{
    apiUrl = "http://api.t.sohu.com/oauth/request_token";
    method = GET;
}

void SohuMicroBlog::oauthAuthorize(QString& apiUrl, ParamMap& params)
{
    apiUrl = "http://api.t.sohu.com.cn/oauth/authorize";
}

void SohuMicroBlog::oauthAccessToken(QString& apiUrl, HttpMethod& method, ParamMap& params)
{
    apiUrl = "http://api.t.sohu.com.cn/oauth/access_token";
    method = GET;
}

void SohuMicroBlog::updateTimeline(Timeline t, QString& apiUrl, ParamMap& params)
{
    switch (t) {
    case Home:
        apiUrl = "http://api.t.sohu.com.cn/statuses/friends_timeline.json";
        params.insert("count", "20");
        break;
    case Public:
        apiUrl = "http://api.t.sohu.com.cn/statuses/public_timeline.json";
        params.insert("count", "20");
        break;
    case Mentions:
        apiUrl = "http://api.t.sohu.com.cn/statuses/mentions.json";
        params.insert("count", "20");
        break;
    }
}

void SohuMicroBlog::updateUserTimeline(const Zzzz::User& user, QString& apiUrl, ParamMap& params)
{
    apiUrl = "http://api.t.sohu.com/statuses/user_timeline.json";
    params.insert("nick_name", user.screenName.toUtf8().toPercentEncoding());
    params.insert("count", "20");
}

QStringList SohuMicroBlog::timelines() const
{
    return QStringList();// << "comments";
}

void SohuMicroBlog::updateTimeline(const QString& timeline, QString& apiUrl, ParamMap& params)
{
    if (timeline == "comments") {
        apiUrl = "http://api.t.sohu.com.cn/statuses/comments_timeline.json";
        params.insert("count", "20");
    }
}

int SohuMicroBlog::charLimit() const
{
    return 140;
}

void SohuMicroBlog::createPost(const Zzzz::Post& post, QString& apiUrl, ParamMap& params)
{
    apiUrl = "http://api.t.sohu.com/statuses/update.json";
    params.insert("status", QUrl::toPercentEncoding(post.text));
}

void SohuMicroBlog::createMediaPost(const Zzzz::Post& post, QString& apiUrl, ParamMap& params)
{
    apiUrl = "http://api.t.sohu.com/statuses/upload.json";
    params.insert("status", post.text.toUtf8());
}

void SohuMicroBlog::removePost(const Zzzz::Post& post, QString& apiUrl, ParamMap& params)
{
    apiUrl = QString("http://api.t.sohu.com/statuses/destroy/%1.json").arg(post.id);
}

void SohuMicroBlog::retweetPost(const Zzzz::Post& post, QString& apiUrl, ParamMap& params)
{
    apiUrl = QString("http://api.t.sohu.com/statuses/transmit/%1.json").arg(post.id);
}

void SohuMicroBlog::createFavorite(const Zzzz::Post& post, QString& apiUrl, ParamMap& params)
{
    apiUrl = QString("http://api.t.sohu.com/favourites/create/%1.json").arg(post.id);
}

void SohuMicroBlog::removeFavorite(const Zzzz::Post& post, QString& apiUrl, ParamMap& params)
{
    apiUrl = QString("http://api.t.sohu.com/favourites/destroy/%1.json").arg(post.id);
}

void SohuMicroBlog::createFriendship(const Zzzz::User& user, QString& apiUrl, ParamMap& params)
{
    apiUrl = QString("http://api.t.sohu.com/friendships/create/%1.json").arg(user.id);
}

void SohuMicroBlog::removeFriendship(const Zzzz::User& user, QString& apiUrl, ParamMap& params)
{
    apiUrl = QString("http://api.t.sohu.com/friendships/destroy/%1.json").arg(user.id);
}

void SohuMicroBlog::readPostFromJsonMap(const QVariantMap& varmap, Zzzz::Post& post)
{
}

void SohuMicroBlog::readUserFromJsonMap(const QVariantMap& varmap, Zzzz::User& user)
{
}
