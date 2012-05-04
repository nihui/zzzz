#include "timelinewidget.h"

#include "typeswrapper.h"
#include "timelinemodel.h"

#include <types.h>

#include <KIcon>
#include <KStandardDirs>
#include <KToolInvocation>

#include <KDebug>

#include <QLineEdit>
#include <QResizeEvent>

static bool postWrapperGreaterThan( const PostWrapper& p1, const PostWrapper& p2 )
{
    return p1.creationDateTime() > p2.creationDateTime();
}

TimelineWidget::TimelineWidget( QWidget* parent )
: QListView(parent)
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setAlternatingRowColors(true);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setSelectionMode(QAbstractItemView::NoSelection);


    TimelineDelegate* delegate = new TimelineDelegate;
    setItemDelegate(delegate);

    connect(delegate, SIGNAL(anchorClicked(const QString&, const QModelIndex&)),
            this, SLOT(slotAnchorClicked(const QString&, const QModelIndex&)));
}

TimelineWidget::~TimelineWidget()
{
}

void TimelineWidget::resizeEvent( QResizeEvent * event )
{
    scheduleDelayedItemsLayout();
}

// void TimelineWidget::updateHTML()
// {
// // kWarning();
//     qSort( m_posts.begin(), m_posts.end(), postWrapperGreaterThan );
// 
//     QImage none = KIcon("image-missing").pixmap( 48 ).toImage();
//     foreach (const QString& url, avatarUrls) {
//         m_textbrowser->document()->addResource( QTextDocument::ImageResource, url, none );
//     }
//     foreach (const QString& url, imageUrls) {
//         m_textbrowser->document()->addResource( QTextDocument::ImageResource, url, none );
//     }
// 
//     m_html = ThemeEngine::self()->render( m_posts );
// }

void TimelineWidget::slotAnchorClicked(const QString& anchor, const QModelIndex& index)
{
    if ( anchor.isEmpty() )
        return;

    kWarning() << anchor;
    PostWrapper post = index.data(Qt::UserRole).value<PostWrapper>();
    if ( anchor == "zzzz:user" ) {
        emit userClicked( post );
    }
    else if ( anchor == "zzzz:reply" ) {
        emit replyClicked( post );
    }
    else if ( anchor == "zzzz:retweet:" ) {
        emit retweetClicked( post );
    }
    else if ( anchor.startsWith( "zzzz:username:" ) ) {
        QString username = anchor.section( ':', 2, 2 );
        emit usernameClicked( post, username );
    }
    else if ( anchor.startsWith( "zzzz:topic:" ) ) {
        QString topic = anchor.section( ':', 2, 2 );
        emit topicClicked( post, topic );
    }
    else {
        KToolInvocation::invokeBrowser( anchor );
    }
}
