#ifndef TYPESWRAPPER_H
#define TYPESWRAPPER_H

#include <types.h>

class Account;

class PostWrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id)
    Q_PROPERTY(QString text READ text)

    Q_PROPERTY(QString userId READ userId)
    Q_PROPERTY(QString userName READ userName)
    Q_PROPERTY(QString userScreenName READ userScreenName)
    Q_PROPERTY(QString userDescription READ userDescription)
    Q_PROPERTY(QString userLocation READ userLocation)
    Q_PROPERTY(QString userProfileImageUrl READ userProfileImageUrl)
    Q_PROPERTY(QString userUrl READ userUrl)
    Q_PROPERTY(int userFollowersCount READ userFollowersCount)

    Q_PROPERTY(QString source READ source)
    Q_PROPERTY(QString creationDateTime READ creationDateTime)
    Q_PROPERTY(QString replyToStatusId READ replyToStatusId)
    Q_PROPERTY(QString replyToUserId READ replyToUserId)
    Q_PROPERTY(QString replyToUserName READ replyToUserName)
    Q_PROPERTY(bool favorited READ favorited)
    Q_PROPERTY(QString thumbnailPic READ thumbnailPic)
    Q_PROPERTY(QString originalPic READ originalPic)

    Q_PROPERTY(QString userLink READ userLink)
    Q_PROPERTY(QString replyLink READ replyLink)
    Q_PROPERTY(QString retweetLink READ retweetLink)
    public:
        explicit PostWrapper( const Zzzz::Post& post );
        QString id() const;
        QString text() const;

        QString userId() const;
        QString userName() const;
        QString userScreenName() const;
        QString userDescription() const;
        QString userLocation() const;
        QString userProfileImageUrl() const;
        QString userUrl() const;
        int userFollowersCount() const;

        QString source() const;
        QString creationDateTime() const;
        QString replyToStatusId() const;
        QString replyToUserId() const;
        QString replyToUserName() const;
        bool favorited() const;
        QString thumbnailPic() const;
        QString originalPic() const;

        QString userLink() const;
        QString replyLink() const;
        QString retweetLink() const;
    public:
        Zzzz::Post m_post;

        mutable QString m_userLink;
        mutable QString m_replyLink;
        mutable QString m_retweetLink;

        Account* myAccount;
};

#endif // TYPESWRAPPER_H
