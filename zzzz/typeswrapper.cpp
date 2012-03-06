#include "typeswrapper.h"

#include <QDateTime>

PostWrapper::PostWrapper( const Zzzz::Post& post ) : m_post(post),myAccount(0) {}

QString PostWrapper::id() const { return m_post.id; }
QString PostWrapper::text() const { return m_post.text; }

QString PostWrapper::userId() const { return m_post.user.id; }
QString PostWrapper::userName() const { return m_post.user.name; }
QString PostWrapper::userScreenName() const { return m_post.user.screenName; }
QString PostWrapper::userDescription() const { return m_post.user.description; }
QString PostWrapper::userLocation() const { return m_post.user.location; }
QString PostWrapper::userProfileImageUrl() const { return m_post.user.profileImageUrl; }
QString PostWrapper::userUrl() const { return m_post.user.url; }
int PostWrapper::userFollowersCount() const { return m_post.user.followersCount; }

QString PostWrapper::source() const { return m_post.source; }
QString PostWrapper::creationDateTime() const { return QDateTime::fromTime_t( m_post.creationDateTime ).toString(); }
QString PostWrapper::replyToStatusId() const { return m_post.replyToStatusId; }
QString PostWrapper::replyToUserId() const { return m_post.replyToUserId; }
QString PostWrapper::replyToUserName() const { return m_post.replyToUserName; }
bool PostWrapper::favorited() const { return m_post.favorited; }
QString PostWrapper::thumbnailPic() const { return m_post.thumbnailPic; }
QString PostWrapper::originalPic() const { return m_post.originalPic; }

QString PostWrapper::userLink() const { return m_userLink; }
QString PostWrapper::replyLink() const { return m_replyLink; }
QString PostWrapper::retweetLink() const { return m_retweetLink; }
