#ifndef TYPESWRAPPER_H
#define TYPESWRAPPER_H

#include <types.h>

class Account;

class UserWrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id)
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString screenName READ screenName)
    Q_PROPERTY(QString description READ description)
    Q_PROPERTY(QString location READ location)
    Q_PROPERTY(QString profileImageUrl READ profileImageUrl)
    Q_PROPERTY(QString url READ url)
    Q_PROPERTY(int followersCount READ followersCount)
    public:
        explicit UserWrapper( const Zzzz::User& user );
        QString id() const;
        QString name() const;
        QString screenName() const;
        QString description() const;
        QString location() const;
        QString profileImageUrl() const;
        QString url() const;
        int followersCount() const;
    public:
        Zzzz::User m_user;
};

class PostWrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id)
    Q_PROPERTY(QString text READ text)
    Q_PROPERTY(QObject* user READ user)
    Q_PROPERTY(QString source READ source)
    Q_PROPERTY(QString creationDateTime READ creationDateTime)
    Q_PROPERTY(QString replyToStatusId READ replyToStatusId)
    Q_PROPERTY(QString replyToUserId READ replyToUserId)
    Q_PROPERTY(QString replyToUserName READ replyToUserName)
    Q_PROPERTY(bool favorited READ favorited)
    Q_PROPERTY(QString thumbnailPic READ thumbnailPic)
    Q_PROPERTY(QString originalPic READ originalPic)

    Q_PROPERTY(QString replyLink READ replyLink)
    Q_PROPERTY(QString retweetLink READ retweetLink)
    public:
        explicit PostWrapper( const Zzzz::Post& post );
        virtual ~PostWrapper();
        QString id() const;
        QString text() const;
        QObject* user() const;
        QString source() const;
        QString creationDateTime() const;
        QString replyToStatusId() const;
        QString replyToUserId() const;
        QString replyToUserName() const;
        bool favorited() const;
        QString thumbnailPic() const;
        QString originalPic() const;

        QString replyLink() const;
        QString retweetLink() const;
    public:
        Zzzz::Post m_post;
        UserWrapper* m_userWrapper;

        mutable QString m_replyLink;
        mutable QString m_retweetLink;


        Account* myAccount;
};

#endif // TYPESWRAPPER_H
