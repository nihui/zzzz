#include "typeswrapper.h"

UserWrapper::UserWrapper( const Zzzz::User& user ) : m_user(user) {}
QString UserWrapper::id() const { return m_user.id; }
QString UserWrapper::name() const { return m_user.name; }
QString UserWrapper::screenName() const { return m_user.screenName; }
QString UserWrapper::description() const { return m_user.description; }
QString UserWrapper::location() const { return m_user.location; }
QString UserWrapper::profileImageUrl() const { return m_user.profileImageUrl; }
QString UserWrapper::url() const { return m_user.url; }
int UserWrapper::followersCount() const { return m_user.followersCount; }

PostWrapper::PostWrapper( const Zzzz::Post& post ) : m_post(post),m_userWrapper(new UserWrapper(post.user)),myAccount(0) {}
PostWrapper::~PostWrapper() { delete m_userWrapper; }
QString PostWrapper::id() const { return m_post.id; }
QString PostWrapper::text() const { return m_post.text; }
QObject* PostWrapper::user() const { return m_userWrapper; }
QString PostWrapper::source() const { return m_post.source; }
QString PostWrapper::creationDateTime() const { return m_post.creationDateTime; }
QString PostWrapper::replyToStatusId() const { return m_post.replyToStatusId; }
QString PostWrapper::replyToUserId() const { return m_post.replyToUserId; }
QString PostWrapper::replyToUserName() const { return m_post.replyToUserName; }
bool PostWrapper::favorited() const { return m_post.favorited; }
QString PostWrapper::thumbnailPic() const { return m_post.thumbnailPic; }
QString PostWrapper::originalPic() const { return m_post.originalPic; }

QString PostWrapper::replayLink() const { return m_replayLink; }
QString PostWrapper::retweetLink() const { return m_retweetLink; }
