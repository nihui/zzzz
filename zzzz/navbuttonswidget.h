#ifndef NAVBUTTONSWIDGET_H
#define NAVBUTTONSWIDGET_H

#include <QWidget>
#include <QHash>

class QButtonGroup;

class KPushButton;
class KJob;

class NavButtonsWidget : public QWidget
{
    Q_OBJECT
    public:
        explicit NavButtonsWidget( QWidget* parent = 0 );
        virtual ~NavButtonsWidget();
        void addButton( const QString& timelineName, const QString& iconName );
    Q_SIGNALS:
        void currentIndexChanged( int index );
    protected:
        virtual void contextMenuEvent( QContextMenuEvent* event );
        virtual void wheelEvent( QWheelEvent* event );
    private Q_SLOTS:
        void slotLoadIcon( KJob* job );
        void slotArrangeItem();
    private:
        int m_buttonCount;
        QButtonGroup* m_buttonGroup;

        QHash<QString, KPushButton*> m_navButton;

        QHash<KJob*, QString> m_jobIcon;
};

#endif // NAVBUTTONSWIDGET_H
