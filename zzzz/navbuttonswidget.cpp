#include "navbuttonswidget.h"
#include "navarranger.h"

#include <types.h>

#include <KAction>
#include <KIcon>
#include <KLocale>
#include <KMenu>
#include <KPushButton>

#include <QButtonGroup>
#include <QContextMenuEvent>
#include <QFocusEvent>
#include <QHBoxLayout>
#include <QWheelEvent>

NavButtonsWidget::NavButtonsWidget( QWidget* parent )
: QWidget(parent)
{
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->setMargin( 0 );
    buttonLayout->setSpacing( 0 );
    setLayout( buttonLayout );

    m_buttonCount = 0;
    m_buttonGroup = new QButtonGroup( this );
    m_buttonGroup->setExclusive( true );

    connect( m_buttonGroup, SIGNAL(buttonClicked(int)),
             this, SIGNAL(currentIndexChanged(int)) );

//     /// check home button by default
//     m_buttonGroup->button( 0 )->click();
}

NavButtonsWidget::~NavButtonsWidget()
{
}

void NavButtonsWidget::addButton( const QString& iconName )
{
    KPushButton* button = new KPushButton;
    button->setIcon( KIcon( iconName ) );
    button->setIconSize( QSize( 32, 32 ) );
    button->setCheckable( true );
    button->setFlat( true );
    m_buttonGroup->addButton( button, m_buttonCount++ );
    layout()->addWidget( button );
}

void NavButtonsWidget::contextMenuEvent( QContextMenuEvent* event )
{
    if ( event->reason() != QContextMenuEvent::Mouse )
        return;

    KMenu* menu = new KMenu;
    KAction* action = new KAction( this );
    action->setText( i18n( "Arrange item" ) );
    connect( action, SIGNAL(triggered(bool)), this, SLOT(slotArrangeItem()) );
    menu->addAction( action );
    menu->exec( event->globalPos() );
    delete menu;
}

void NavButtonsWidget::wheelEvent( QWheelEvent* event )
{
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;

    int checkedId = m_buttonGroup->checkedId();
    int toClickId = ( checkedId - numSteps ) % m_buttonCount;
    if ( toClickId < 0 )
        toClickId += m_buttonCount;
    QAbstractButton* buttonToClick = m_buttonGroup->button( toClickId );
    buttonToClick->click();
    buttonToClick->setFocus();

    event->accept();
}

void NavButtonsWidget::slotArrangeItem()
{
    NavArranger* arranger = new NavArranger;
    arranger->exec();
    delete arranger;
}

