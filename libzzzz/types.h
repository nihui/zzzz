#ifndef TYPES_H
#define TYPES_H

#include <QString>
#include <QMetaType>

namespace Zzzz
{

class User
{
public:
    QString id;
    QString name;
    QString screenName;
    QString description;
    QString location;
    QString profileImageUrl;
    QString url;
    int followersCount;
};

class Post
{
public:
    QString id;
    QString text;
    User user;
    QString source;
    uint creationDateTime;
    QString replyToStatusId;
    QString replyToUserId;
    QString replyToUserName;
    bool favorited;
    QString thumbnailPic;
    QString originalPic;
};

}

Q_DECLARE_METATYPE(Zzzz::User)
Q_DECLARE_METATYPE(Zzzz::Post)

#endif // TYPES_H
