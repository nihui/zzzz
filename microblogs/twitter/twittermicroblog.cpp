#include "twittermicroblog.h"

ZZZZ_EXPORT_PLUGIN(TwitterMicroBlog)

TwitterMicroBlog::TwitterMicroBlog(QObject* parent, const QVariantList& args)
    : Zzzz::TwitterCompatibleAPIMicroBlog(parent, args)
{
}

TwitterMicroBlog::~TwitterMicroBlog()
{
}

QString TwitterMicroBlog::apiRoot() const
{
    return QString("https://api.twitter.com/1");
}

QByteArray TwitterMicroBlog::oauthConsumerKey() const
{
    return QByteArray("VyXMf0O7CvciiUQjliYtYg");
}

QByteArray TwitterMicroBlog::oauthConsumerSecret() const
{
    return QByteArray("uD2HvsOBjzt1Vs6SnouFtuxDeHmvOOVwmn3fBVyCw0");
}

void TwitterMicroBlog::createMediaPost(const Zzzz::Post& post, QString& apiUrl, ParamMap& params)
{
    Zzzz::TwitterCompatibleAPIMicroBlog::createMediaPost(post, apiUrl, params);
    apiUrl = "https://upload.twitter.com/1/statuses/update_with_media.json";
}
