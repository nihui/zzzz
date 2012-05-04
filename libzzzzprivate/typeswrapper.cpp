#include "typeswrapper.h"

#include <QDateTime>
#include <QRegExp>

PostWrapperData::PostWrapperData()
: myAccount(0)
{
}

PostWrapperData::PostWrapperData(const PostWrapperData& other)
: QSharedData(other),
post(other.post),
zzzztext(other.zzzztext),
userLink(other.userLink),
replyLink(other.replyLink),
retweetLink(other.retweetLink),
myAccount(other.myAccount)
{
}

PostWrapperData::~PostWrapperData()
{
}

PostWrapper::PostWrapper()
: d(new PostWrapperData)
{
}

PostWrapper::PostWrapper( const Zzzz::Post& post )
: d(new PostWrapperData)
{
    d->post = post;
    d->zzzztext = post.text;
}

PostWrapper::PostWrapper( const PostWrapper& other )
: d(other.d)
{
}

QString PostWrapper::id() const { return d->post.id; }
QString PostWrapper::text() const { return d->post.text; }

QString PostWrapper::userId() const { return d->post.user.id; }
QString PostWrapper::userName() const { return d->post.user.name; }
QString PostWrapper::userScreenName() const { return d->post.user.screenName; }
QString PostWrapper::userDescription() const { return d->post.user.description; }
QString PostWrapper::userLocation() const { return d->post.user.location; }
QString PostWrapper::userProfileImageUrl() const { return d->post.user.profileImageUrl; }
QString PostWrapper::userUrl() const { return d->post.user.url; }
int PostWrapper::userFollowersCount() const { return d->post.user.followersCount; }

QString PostWrapper::source() const { return d->post.source; }
QString PostWrapper::creationDateTime() const { return QDateTime::fromTime_t( d->post.creationDateTime ).toString(); }
QString PostWrapper::replyToStatusId() const { return d->post.replyToStatusId; }
QString PostWrapper::replyToUserId() const { return d->post.replyToUserId; }
QString PostWrapper::replyToUserName() const { return d->post.replyToUserName; }
bool PostWrapper::favorited() const { return d->post.favorited; }
QString PostWrapper::thumbnailPic() const { return d->post.thumbnailPic; }
QString PostWrapper::originalPic() const { return d->post.originalPic; }

const Zzzz::Post& PostWrapper::internalPost() const { return d->post; }

QString PostWrapper::zzzztext() const { return d->zzzztext; }
QString PostWrapper::userLink() const { return d->userLink; }
QString PostWrapper::replyLink() const { return d->replyLink; }
QString PostWrapper::retweetLink() const { return d->retweetLink; }

void PostWrapper::setZzzztext(const QString& zzzztext) const { d->zzzztext = zzzztext; }
void PostWrapper::setUserLink(const QString& userLink) const { d->userLink = userLink; }
void PostWrapper::setReplyLink(const QString& replyLink) const { d->replyLink = replyLink; }
void PostWrapper::setRetweetLink(const QString& retweetLink) const { d->retweetLink = retweetLink; }
void PostWrapper::setMyAccount(Account* myAccount) const { d->myAccount = myAccount; }
Account* PostWrapper::myAccount() const { return d->myAccount; }

bool PostWrapper::operator==(const PostWrapper& rhs) const { return myAccount() == rhs.myAccount() && id() == rhs.id(); }
