#include "timelinewidget.h"

#include "typeswrapper.h"
#include "timelinemodel.h"

#include <types.h>

#include "composerwidget.h"
#include <KToolInvocation>

#include <KDebug>

#include <QResizeEvent>

TimelineWidget::TimelineWidget(QWidget* parent)
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

void TimelineWidget::resizeEvent(QResizeEvent* event)
{
    scheduleDelayedItemsLayout();
}

void TimelineWidget::slotAnchorClicked(const QString& anchor, const QModelIndex& index)
{
    if (anchor.isEmpty())
        return;

    kWarning() << anchor;
    PostWrapper post = index.data(Qt::UserRole).value<PostWrapper>();

    QRect rect = visualRect(index);

    if (anchor == "zzzz:user") {
        emit userClicked(post);
    } else if (anchor == "zzzz:reply") {
        ComposerWidget::self()->composeReply(post);
        QPoint pos(rect.x(), rect.y() + rect.height());
        ComposerWidget::self()->move(mapToGlobal(pos));
        ComposerWidget::self()->resize(rect.width(), 150);
        ComposerWidget::self()->show();
        ComposerWidget::self()->raise();
        ComposerWidget::self()->activateWindow();
        emit replyClicked(post);
    } else if (anchor == "zzzz:retweet:") {
        emit retweetClicked(post);
    } else if (anchor.startsWith("zzzz:username:")) {
        QString username = anchor.section(':', 2, 2);
        emit usernameClicked(post, username);
    } else if (anchor.startsWith("zzzz:topic:")) {
        QString topic = anchor.section(':', 2, 2);
        emit topicClicked(post, topic);
    } else {
        KToolInvocation::invokeBrowser(anchor);
    }
}
