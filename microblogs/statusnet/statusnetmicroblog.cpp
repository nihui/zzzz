#include "statusnetmicroblog.h"

ZZZZ_EXPORT_PLUGIN( StatusNetMicroBlog )

StatusNetMicroBlog::StatusNetMicroBlog( QObject* parent, const QVariantList& args )
: Zzzz::TwitterCompatibleAPIMicroBlog(parent,args)
{
}

StatusNetMicroBlog::~StatusNetMicroBlog()
{
}

QString StatusNetMicroBlog::apiRoot() const
{
    return QString( "https://identi.ca/api" );
}

QByteArray StatusNetMicroBlog::oauthConsumerKey() const
{
    return QByteArray( "747d09d8e7b9417f5835f04510cb86ed" );
}

QByteArray StatusNetMicroBlog::oauthConsumerSecret() const
{
    return QByteArray( "57605f8507a041525a2d5c0abef15b20" );
}
