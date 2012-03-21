#ifndef NAVBUTTONSWIDGET_H
#define NAVBUTTONSWIDGET_H

#include <QWidget>
#include <QHash>
#include <QPoint>

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
        void slotGotAvatar( const QString& url, const QImage& image );
        void slotErrorAvatar( const QString& url );
    private:
        QHBoxLayout* m_buttonLayout;
        QPushButton* m_currentButton;

        QPoint m_buttonStart;
        QPoint m_dragStart;

        QHash<QString, QPushButton*> m_urlButton;
};

#endif // NAVBUTTONSWIDGET_H
