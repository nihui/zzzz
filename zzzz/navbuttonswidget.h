#ifndef NAVBUTTONSWIDGET_H
#define NAVBUTTONSWIDGET_H

#include <QWidget>
#include <QHash>
#include <QPoint>

class QButtonGroup;
class QHBoxLayout;

class KPushButton;
class KJob;

class NavButtonsWidget : public QWidget
{
    Q_OBJECT
    public:
        explicit NavButtonsWidget( QWidget* parent = 0 );
        virtual ~NavButtonsWidget();
        virtual bool eventFilter( QObject* watched, QEvent* event );
        void addButton( const QString& timelineName, const QString& iconName );

    Q_SIGNALS:
        void timelineClicked( const QString& timelineName );
    protected:
        virtual void wheelEvent( QWheelEvent* event );
    private Q_SLOTS:
        void slotButtonClicked( int id );
        void slotLoadIcon( KJob* job );
    private:
        QHBoxLayout* m_buttonLayout;
        QButtonGroup* m_buttonGroup;

        QHash<int, QString> m_buttonIdTimeline;

        QHash<QString, KPushButton*> m_navButton;

        QPoint m_buttonStart;
        QPoint m_dragStart;

        QHash<KJob*, QString> m_jobIcon;
};

#endif // NAVBUTTONSWIDGET_H
