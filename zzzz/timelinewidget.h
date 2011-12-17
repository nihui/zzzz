#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <QWidget>

class QUrl;
class QTextBrowser;
class QTimer;
class Account;
class PostWrapper;

class TimelineWidget : public QWidget
{
    Q_OBJECT
    public:
        explicit TimelineWidget( QWidget* parent = 0 );
        virtual ~TimelineWidget();
        void appendPost( const PostWrapper* post );
        void prependPost( const PostWrapper* post );
        void clearPosts();
    public Q_SLOTS:
        void updateHTML();
    Q_SIGNALS:
        void userClicked( const PostWrapper* post );
        void replyClicked( const PostWrapper* post );
        void retweetClicked( const PostWrapper* post );
    private Q_SLOTS:
        void refresh();
        void slotAccountRemoved( const QString& alias, const Account* oldAccount );
        void slotGotMedia( const QString& url, const QImage& image );
        void slotErrorMedia( const QString& url );
        void slotAnchorClicked( const QUrl& url );
    private:
        void delayedRefresh();
        void handleUrlString( const QString& url );
    private:
        QList<const PostWrapper*> m_posts;

        QString m_html;
        QTextBrowser* m_textbrowser;
        QTimer* m_timer;
};

#endif // TIMELINEWIDGET_H
