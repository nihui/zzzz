#ifndef TYPESWRAPPER_H
#define TYPESWRAPPER_H

#include "zzzzprivate_export.h"

#include <QSharedData>
#include <grantlee_core.h>
#include <types.h>

class Account;

class ZZZZPRIVATE_EXPORT PostWrapperData : public QSharedData
{
public:
    PostWrapperData();
    PostWrapperData(const PostWrapperData& other);
    ~PostWrapperData();

    Zzzz::Post post;
    mutable QString zzzztext;

    mutable QString userLink;
    mutable QString replyLink;
    mutable QString retweetLink;
    mutable Account* myAccount;
};

class ZZZZPRIVATE_EXPORT PostWrapper
{
public:
    PostWrapper();
    PostWrapper(const Zzzz::Post& post);
    PostWrapper(const PostWrapper& other);
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

    const Zzzz::Post& internalPost() const;

    QString zzzztext() const;
    QString userLink() const;
    QString replyLink() const;
    QString retweetLink() const;

    void setZzzztext(const QString& zzzztext) const;
    void setUserLink(const QString& userLink) const;
    void setReplyLink(const QString& replyLink) const;
    void setRetweetLink(const QString& retweetLink) const;

    void setMyAccount(Account* myAccount) const;
    Account* myAccount() const;

    bool operator==(const PostWrapper& rhs) const;

private:
    QSharedDataPointer<PostWrapperData> d;
};

Q_DECLARE_METATYPE(PostWrapper)
Q_DECLARE_METATYPE(QList<PostWrapper>)


GRANTLEE_BEGIN_LOOKUP(PostWrapper)
if (property == "id") return object.id();
else if (property == "text") return object.text();
else if (property == "zzzztext") return object.zzzztext();
else if (property == "userId") return object.userId();
else if (property == "userName") return object.userName();
else if (property == "userScreenName") return object.userScreenName();
else if (property == "userDescription") return object.userDescription();
else if (property == "userLocation") return object.userLocation();
else if (property == "userProfileImageUrl") return object.userProfileImageUrl();
else if (property == "userUrl") return object.userUrl();
else if (property == "userFollowersCount") return object.userFollowersCount();
else if (property == "source") return object.source();
else if (property == "creationDateTime") return object.creationDateTime();
else if (property == "replyToStatusId") return object.replyToStatusId();
else if (property == "replyToUserId") return object.replyToUserId();
else if (property == "replyToUserName") return object.replyToUserName();
else if (property == "favorited") return object.favorited();
else if (property == "thumbnailPic") return object.thumbnailPic();
else if (property == "originalPic") return object.originalPic();
else if (property == "userLink") return object.userLink();
else if (property == "replyLink") return object.replyLink();
else if (property == "retweetLink") return object.retweetLink();
GRANTLEE_END_LOOKUP

#endif // TYPESWRAPPER_H
