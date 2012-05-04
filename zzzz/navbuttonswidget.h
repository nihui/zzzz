#ifndef NAVBUTTONSWIDGET_H
#define NAVBUTTONSWIDGET_H

#include <QWidget>
#include <QHash>
#include <QPoint>
#include <QUrl>

class QHBoxLayout;
class QPushButton;

class NavButtonsWidget : public QWidget
{
    Q_OBJECT
    public:
        explicit NavButtonsWidget( QWidget* parent = 0 );
        virtual ~NavButtonsWidget();
        virtual bool eventFilter( QObject* watched, QEvent* event );
        void addButton( const QString& timelineName, const QString& iconName, bool checked );
    Q_SIGNALS:
        void timelineClicked( const QString& timelineName );
        void timelineClosed( const QString& timelineName );
    protected:
        virtual void wheelEvent( QWheelEvent* event );
    private Q_SLOTS:
        void slotGotPixmap( const QUrl& url, const QPixmap& pixmap );
        void slotErrorPixmap( const QUrl& url );
    private:
        QHBoxLayout* m_buttonLayout;
        QPushButton* m_currentButton;

        QPoint m_buttonStart;
        QPoint m_dragStart;

        QHash<QUrl, QPushButton*> m_urlButton;
};

#endif // NAVBUTTONSWIDGET_H
