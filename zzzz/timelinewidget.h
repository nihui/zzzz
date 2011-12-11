#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <QWidget>

class QUrl;
class KUrl;
class KHTMLPart;
class ZzzzWebView;
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
        void slotLinkClicked( const QUrl& url );
        void slotOpenUrlRequestDelayed( const KUrl& url );
    private:
        void handleUrlString( const QString& url );
    private:
        QList<const PostWrapper*> m_posts;

        QString m_html;
        KHTMLPart* m_khtmlpart;
        ZzzzWebView* m_kwebview;
};

#endif // TIMELINEWIDGET_H
