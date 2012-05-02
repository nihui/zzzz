#include "timelinewidget.h"

#include "themeengine.h"
#include "typeswrapper.h"

#include "accountmanager.h"
#include "mediafetcher.h"
#include "account.h"

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

static bool postWrapperGreaterThan( const PostWrapper& p1, const PostWrapper& p2 )
{
    return p1.creationDateTime() > p2.creationDateTime();
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

void TimelineWidget::appendPost( const PostWrapper& post )
{
    if ( postExists( post ) )
        return;

    m_posts.append( post );
}

void TimelineWidget::prependPost( const PostWrapper& post )
{
    if ( postExists( post ) )
        return;

    m_posts.prepend( post );
}

void TimelineWidget::clearPosts()
{
    m_posts.clear();
}

void TimelineWidget::updateHTML()
{
// kWarning();

    QSet<QString> avatarUrls;
    QSet<QString> imageUrls;

    qSort( m_posts.begin(), m_posts.end(), postWrapperGreaterThan );

    QList<PostWrapper>::ConstIterator it = m_posts.constBegin();
    QList<PostWrapper>::ConstIterator end = m_posts.constEnd();
    int i = 0;
    while ( it != end ) {
        const PostWrapper& post = *it;
        avatarUrls.insert( post.userProfileImageUrl() );
        imageUrls.insert( post.thumbnailPic() );
        ++it;
    }

    QImage none = KIcon("image-missing").pixmap( 48 ).toImage();
    foreach (const QString& url, avatarUrls) {
        m_textbrowser->document()->addResource( QTextDocument::ImageResource, url, none );
    }
    foreach (const QString& url, imageUrls) {
        m_textbrowser->document()->addResource( QTextDocument::ImageResource, url, none );
    }

    m_html = ThemeEngine::self()->render( m_posts );

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
    QList<PostWrapper>::Iterator it = m_posts.begin();
    QList<PostWrapper>::Iterator end = m_posts.end();
    while ( it != end ) {
        const PostWrapper& post = *it;
        if ( post.myAccount() == oldAccount ) {
            it = m_posts.erase( it );
            end = m_posts.end();
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

bool TimelineWidget::postExists( const PostWrapper& post )
{
    QList<PostWrapper>::ConstIterator it = m_posts.constBegin();
    QList<PostWrapper>::ConstIterator end = m_posts.constEnd();
    while ( it != end ) {
        const PostWrapper& p = *it;
        ++it;

        if ( p.myAccount() == post.myAccount() && p.id() == post.id() ) {
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
