#ifndef NAVBUTTONSWIDGET_H
#define NAVBUTTONSWIDGET_H

#include <QWidget>

class QButtonGroup;

class NavButtonsWidget : public QWidget
{
    Q_OBJECT
    public:
        explicit NavButtonsWidget( QWidget* parent = 0 );
        virtual ~NavButtonsWidget();
        void addButton( const QString& iconName );
    Q_SIGNALS:
        void currentIndexChanged( int index );
    protected:
        virtual void contextMenuEvent( QContextMenuEvent* event );
        virtual void wheelEvent( QWheelEvent* event );
    private Q_SLOTS:
        void slotArrangeItem();
    private:
        int m_buttonCount;
        QButtonGroup* m_buttonGroup;
};

#endif // NAVBUTTONSWIDGET_H
