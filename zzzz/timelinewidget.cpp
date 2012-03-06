#include "timelinewidget.h"

#include "themeengine.h"
#include "typeswrapper.h"

#include "accountmanager.h"
#include "mediafetcher.h"

#include <types.h>

#include <KIcon>
#include <KStandardDirs>
#include <KToolInvocation>

#include <KDebug>

#include <QLineEdit>
#include <QResizeEvent>
#include <QRegExp>
#include <QSet>
#include <QTextBrowser>
#include <QTimer>
#include <QVBoxLayout>

static bool postWrapperGreaterThan( const PostWrapper*& p1, const PostWrapper*& p2 )
{
    return p1->m_post.creationDateTime > p2->m_post.creationDateTime;
}

TimelineWidget::TimelineWidget( QWidget* parent )
: QWidget(parent)
{
    setSizePolicy( QSizePolicy::Minimum, QSizePolicy::MinimumExpanding );

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setMargin( 0 );
    setLayout( mainLayout );

    m_timer = new QTimer( this );
    m_timer->setSingleShot( true );
    connect( m_timer, SIGNAL(timeout()), this, SLOT(refresh()) );

    m_textbrowser = new QTextBrowser;
    m_textbrowser->setOpenExternalLinks( false );
    m_textbrowser->setOpenLinks( false );
    m_textbrowser->setUndoRedoEnabled( false );
//     m_textbrowser->setDocumentMargin( 0 );
    m_textbrowser->setHtml( m_html );
    connect( m_textbrowser, SIGNAL(anchorClicked(const QUrl&)),
             this, SLOT(slotAnchorClicked(const QUrl&)) );

    mainLayout->addWidget( m_textbrowser );

    connect( AccountManager::self(), SIGNAL(accountRemoved(const QString&, const Account*)),
             this, SLOT(slotAccountRemoved(const QString&, const Account*)) );

    connect( MediaFetcher::self(), SIGNAL(gotAvatar(const QString&, const QImage&)),
             this, SLOT(slotGotMedia(const QString&, const QImage&)) );
    connect( MediaFetcher::self(), SIGNAL(gotImage(const QString&, const QImage&)),
             this, SLOT(slotGotMedia(const QString&, const QImage&)) );
}

TimelineWidget::~TimelineWidget()
{
    clearPosts();
}

void TimelineWidget::appendPost( const PostWrapper* post )
{
    if ( postExists( post ) ) {
        delete post;
        return;
    }

    m_posts.append( post );
}

void TimelineWidget::prependPost( const PostWrapper* post )
{
    if ( postExists( post ) ) {
        delete post;
        return;
    }

    m_posts.prepend( post );
}

void TimelineWidget::clearPosts()
{
    qDeleteAll( m_posts );
    m_posts.clear();
}

void TimelineWidget::updateHTML()
{
// kWarning();

    QSet<QString> avatarUrls;
    QSet<QString> imageUrls;

    QVariantList postList;

    qSort( m_posts.begin(), m_posts.end(), postWrapperGreaterThan );

    QList<const PostWrapper*>::ConstIterator it = m_posts.constBegin();
    QList<const PostWrapper*>::ConstIterator end = m_posts.constEnd();
    int i = 0;
    while ( it != end ) {
        const PostWrapper* post = *it;

        avatarUrls.insert( post->m_post.user.profileImageUrl );
        imageUrls.insert( post->m_post.thumbnailPic );

        post->m_userLink = QString( "zzzz:user:%1" ).arg( i );
        post->m_replyLink = QString( "zzzz:reply:%1" ).arg( i );
        post->m_retweetLink = QString( "zzzz:retweet:%1" ).arg( i );

        post->m_zzzztext = post->m_post.text;
        // @username
        static QRegExp usernameRegex( "@(([^\\s\\W]+))", Qt::CaseInsensitive );
        post->m_zzzztext.replace( usernameRegex, QString( "<a href=\"zzzz:username:%1:\\1\">@\\1</a>" ).arg( i ) );
        // #topic
        static QRegExp topicRegex( "#(([^\\s\\W]+))#", Qt::CaseInsensitive );
        post->m_zzzztext.replace( topicRegex, QString( "<a href=\"zzzz:topic:%1:\\1\">#\\1#</a>" ).arg( i ) );

        ++i;

        ++it;

        const QObject* postWrapper = post;
        postList.append( QVariant::fromValue( const_cast<QObject*>(postWrapper) ) );
//         kWarning() << p.user.profileImageUrl;
    }

    QImage none = KIcon("image-missing").pixmap( 48 ).toImage();
    foreach (const QString& url, avatarUrls) {
        m_textbrowser->document()->addResource( QTextDocument::ImageResource, url, none );
    }
    foreach (const QString& url, imageUrls) {
        m_textbrowser->document()->addResource( QTextDocument::ImageResource, url, none );
    }

    m_html = ThemeEngine::self()->render( postList );

    m_textbrowser->setHtml( m_html );

    foreach (const QString& url, avatarUrls) {
        MediaFetcher::self()->requestAvatar( url );
    }
    foreach (const QString& url, imageUrls) {
        MediaFetcher::self()->requestImage( url );
    }
}

void TimelineWidget::refresh()
{
    m_textbrowser->setHtml( m_html );
}

void TimelineWidget::slotAccountRemoved( const QString& alias, const Account* oldAccount )
{
    QList<const PostWrapper*>::Iterator it = m_posts.begin();
    QList<const PostWrapper*>::Iterator end = m_posts.end();
    while ( it != end ) {
        const PostWrapper* post = *it;
        if ( post->myAccount == oldAccount ) {
            it = m_posts.erase( it );
            end = m_posts.end();
            delete post;
        }
        else {
            ++it;
        }
    }
    updateHTML();
}

void TimelineWidget::slotGotMedia( const QString& url, const QImage& image )
{
    m_textbrowser->document()->addResource( QTextDocument::ImageResource, url, image );
    delayedRefresh();
}

void TimelineWidget::slotErrorMedia( const QString& url )
{
    QImage none = KIcon("image-missing").pixmap( 48 ).toImage();
    m_textbrowser->document()->addResource( QTextDocument::ImageResource, url, none );
    delayedRefresh();
}

void TimelineWidget::slotAnchorClicked( const QUrl& url )
{
    handleUrlString( url.toString() );
}

bool TimelineWidget::postExists( const PostWrapper* post )
{
    QList<const PostWrapper*>::ConstIterator it = m_posts.constBegin();
    QList<const PostWrapper*>::ConstIterator end = m_posts.constEnd();
    while ( it != end ) {
        const PostWrapper* p = *it;
        ++it;

        if ( p->myAccount == post->myAccount
            && p->m_post.id == post->m_post.id ) {
            return true;
        }
    }
    return false;
}

void TimelineWidget::delayedRefresh()
{
    m_timer->start( 200 );
}

void TimelineWidget::handleUrlString( const QString& url )
{
    if ( url.isEmpty() )
        return;

    kWarning() << url;
    if ( url.startsWith( "zzzz:user:" ) ) {
        int i = url.section( ':', 2, 2 ).toInt();
        emit userClicked( m_posts.at( i ) );
    }
    else if ( url.startsWith( "zzzz:reply:" ) ) {
        int i = url.section( ':', 2, 2 ).toInt();
        emit replyClicked( m_posts.at( i ) );
    }
    else if ( url.startsWith( "zzzz:retweet:" ) ) {
        int i = url.section( ':', 2, 2 ).toInt();
        emit retweetClicked( m_posts.at( i ) );
    }
    else if ( url.startsWith( "zzzz:username:" ) ) {
        int i = url.section( ':', 2, 2 ).toInt();
        QString username = url.section( ':', 3, 3 );
        emit usernameClicked( m_posts.at( i ), username );
    }
    else if ( url.startsWith( "zzzz:topic:" ) ) {
        int i = url.section( ':', 2, 2 ).toInt();
        QString topic = url.section( ':', 3, 3 );
        emit topicClicked( m_posts.at( i ), topic );
    }
    else {
        KToolInvocation::invokeBrowser( url );
    }
}
