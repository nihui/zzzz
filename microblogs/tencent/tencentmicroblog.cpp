#include "tencentmicroblog.h"

#include <types.h>

#include <QtCrypto>

#include <QDebug>
#include <QUrl>

ZZZZ_EXPORT_PLUGIN( TencentMicroBlog )

TencentMicroBlog::TencentMicroBlog( QObject* parent, const QVariantList& args )
: Zzzz::MicroBlog(parent,args)
{
}

TencentMicroBlog::~TencentMicroBlog()
{
}

QByteArray TencentMicroBlog::oauthConsumerKey() const
{
    return QByteArray( "f2d56411e41e468ebcba8a52ddec45a4" );
}

QByteArray TencentMicroBlog::oauthConsumerSecret() const
{
    return QByteArray( "f1e645c913e0b3f68ea376d6f35a6a36" );
}

void TencentMicroBlog::oauthRequestToken( QString& apiUrl, HttpMethod& method, ParamMap& params )
{
    apiUrl = "https://open.t.qq.com/cgi-bin/request_token";
    method = POST;
    params.insert( "oauth_callback", "null" );
}

void TencentMicroBlog::oauthAuthorize( QString& apiUrl, ParamMap& params )
{
    apiUrl = "https://open.t.qq.com/cgi-bin/authorize";
}

void TencentMicroBlog::oauthAccessToken( QString& apiUrl, HttpMethod& method, ParamMap& params )
{
    apiUrl = "https://open.t.qq.com/cgi-bin/access_token";
    method = POST;
}

void TencentMicroBlog::updateTimeline( Timeline t, QString& apiUrl, ParamMap& params )
{
    qWarning() << "TencentMicroBlog::updateTimeline" << t;
    switch ( t ) {
        case Home:
            apiUrl = "http://open.t.qq.com/api/statuses/home_timeline";
            params.insert( "format", "json" );
            params.insert( "pageflag", "0" );
            params.insert( "pagetime", "0" );
            params.insert( "reqnum", "20" );
            break;
        case Public:
            apiUrl = "http://open.t.qq.com/api/statuses/public_timeline";
            params.insert( "format", "json" );
            params.insert( "pos", "0" );
            params.insert( "reqnum", "20" );
            break;
        case Mentions:
            apiUrl = "http://open.t.qq.com/api/statuses/mentions_timeline";
            params.insert( "format", "json" );
            params.insert( "pageflag", "0" );
            params.insert( "pagetime", "0" );
            params.insert( "reqnum", "20" );
            break;
    }
}

QStringList TencentMicroBlog::timelines() const
{
    return QStringList();// << "user" << "ht";
}

void TencentMicroBlog::updateTimeline( const QString& timeline, QString& apiUrl, ParamMap& params )
{
    if ( timeline == "mentions" ) {
        apiUrl = "http://open.t.qq.com/api/statuses/mentions_timeline";
        params.insert( "format", "json" );
        params.insert( "reqnum", "20" );
    }
    else if ( timeline == "ht" ) {
        apiUrl = "http://open.t.qq.com/api/statuses/ht_timeline";
        params.insert( "format", "json" );
        params.insert( "reqnum", "20" );
    }
    else if ( timeline == "broadcast" ) {
        apiUrl = "http://open.t.qq.com/api/statuses/broadcast_timeline";
        params.insert( "format", "json" );
        params.insert( "reqnum", "20" );
    }
}

int TencentMicroBlog::charLimit() const
{
    return 150;
}

void TencentMicroBlog::createPost( const Zzzz::Post& post, QString& apiUrl, ParamMap& params )
{
    qWarning() << "TencentMicroBlog::createPost";
    apiUrl = "http://open.t.qq.com/api/t/add";
    params.insert( "format", "json" );
    params.insert( "content", QUrl::toPercentEncoding( post.text ) );
    params.insert( "clientip", "127.0.0.1" );
}

void TencentMicroBlog::removePost( const Zzzz::Post& post, QString& apiUrl, ParamMap& params )
{
    qWarning() << "TencentMicroBlog::removePost";
    apiUrl = "http://open.t.qq.com/api/t/del";
    params.insert( "format", "json" );
    params.insert( "id", post.id.toUtf8() );
}

void TencentMicroBlog::retweetPost( const Zzzz::Post& post, QString& apiUrl, ParamMap& params )
{
    qWarning() << "TencentMicroBlog::retweetPost";
    apiUrl = "http://open.t.qq.com/api/t/re_add";
    params.insert( "format", "json" );
    params.insert( "content", QUrl::toPercentEncoding( post.text ) );
    params.insert( "clientip", "127.0.0.1" );
    params.insert( "reid", post.id.toUtf8() );
}

void TencentMicroBlog::readTimelineFromData( const QByteArray& data, QList<Zzzz::Post>& postlist, bool* ok )
{
    QVariantMap rootmap = m_parser.parse( data, ok ).toMap();
    if ( !ok )
        return;
    QVariantMap datamap = rootmap["data"].toMap();
    QVariantList list = datamap["info"].toList();
    QVariantList::ConstIterator it = list.constBegin();
    QVariantList::ConstIterator end = list.constEnd();
    while ( it != end ) {
        Zzzz::Post post;
        readPostFromJsonMap( it->toMap(), post );
        postlist << post;
        ++it;
    }
}

void TencentMicroBlog::readPostFromJsonMap( const QVariantMap& varmap, Zzzz::Post& post )
{
//     qWarning() << "##############################################################";
//     qWarning() << varmap;
    post.id = varmap["id"].toString();
    post.source = varmap["from"].toString();
    post.text = varmap["text"].toString();
    post.creationDateTime = varmap["timestamp"].toString();

    post.user.name = varmap["name"].toString();
    post.user.location = varmap["location"].toString();
    post.user.id = varmap["uid"].toString();
    post.user.screenName = varmap["nick"].toString();
    post.user.profileImageUrl = QUrl::fromPercentEncoding( varmap["head"].toString().toUtf8() ) + "/50";

    QVariantList imageList = varmap["image"].toList();
    if ( !imageList.isEmpty() ) {
        QString imageUrl = imageList.at( 0 ).toString();
        if ( imageUrl != "null" && imageUrl.startsWith( "http" ) ) {
            post.thumbnailPic = imageUrl + "/120";
            post.originalPic = imageUrl + "/2000";
        }
    }

    int type = varmap["type"].toInt();
    switch ( type ) {
        case 2: {
            // retweet
            QVariantMap source = varmap["source"].toMap();
            post.text.append( " @" + source["name"].toString() );
            post.text.append( " " + source["text"].toString() );
            imageList = source["image"].toList();
            if ( !imageList.isEmpty() ) {
                QString imageUrl = imageList.at( 0 ).toString();
                if ( imageUrl != "null" && imageUrl.startsWith( "http" ) ) {
                    post.thumbnailPic = imageUrl + "/120";
                    post.originalPic = imageUrl + "/2000";
                }
            }
        }
    }
}

void TencentMicroBlog::readUserFromJsonMap( const QVariantMap& varmap, Zzzz::User& user )
{
}
