#include "navbuttonswidget.h"

#include <types.h>

#include <KAction>
#include <KDebug>
#include <KIcon>
#include <KIO/Job>
#include <KIO/StoredTransferJob>
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
    m_buttonLayout = new QHBoxLayout;
    m_buttonLayout->setMargin( 0 );
    m_buttonLayout->setSpacing( 0 );
    setLayout( m_buttonLayout );

    m_buttonGroup = new QButtonGroup( this );
    m_buttonGroup->setExclusive( true );

    connect( m_buttonGroup, SIGNAL(buttonClicked(int)),
             this, SLOT(slotButtonClicked(int)) );

//     /// check home button by default
//     m_buttonGroup->button( 0 )->click();
}

NavButtonsWidget::~NavButtonsWidget()
{
}

bool NavButtonsWidget::eventFilter( QObject* watched, QEvent* event )
{
    KPushButton* button = qobject_cast<KPushButton*>(watched);
    if ( button ) {
        if ( event->type() == QEvent::MouseButtonPress ) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            button->click();
            m_buttonStart = button->pos();
            m_dragStart = mouseEvent->globalPos();
            return true;
        }
        if ( event->type() == QEvent::MouseMove ) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            int deltaX = mouseEvent->globalPos().x() - m_dragStart.x();
            button->move( m_buttonStart.x() + deltaX, m_buttonStart.y() );
            return true;
        }
        if ( event->type() == QEvent::MouseButtonRelease ) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            int deltaX = mouseEvent->globalPos().x() - m_dragStart.x();
            int deltaStep = deltaX / button->width();
            if (deltaStep == 0) {
                // restore original position
                button->move( m_buttonStart );
            }
            else {
                int originalLayoutIndex = m_buttonLayout->indexOf( button );
                m_buttonLayout->removeWidget( button );
                m_buttonLayout->insertWidget( originalLayoutIndex + deltaStep, button );
            }
            return true;
        }
    }
    return QObject::eventFilter( watched, event );
}

void NavButtonsWidget::addButton( const QString& timelineName, const QString& iconName )
{
    KPushButton* button = new KPushButton;
    button->setIconSize( QSize( 32, 32 ) );
    button->setCheckable( true );
    button->setFlat( true );
    button->setFocusPolicy( Qt::NoFocus );

    button->installEventFilter( this );

    m_buttonGroup->addButton( button );

    m_buttonLayout->addWidget( button );

    int id = m_buttonGroup->id( button );
    m_buttonIdTimeline[ id ] = timelineName;

    m_navButton[ timelineName ] = button;

    if ( iconName.startsWith( "http://" ) ) {
        /// load icon from remote url
//         qWarning() << "load icon from remote url" << iconName;
        KIO::StoredTransferJob* job = KIO::storedGet( KUrl( iconName ), KIO::Reload, KIO::HideProgressInfo );
        connect( job, SIGNAL(result(KJob*)), this, SLOT(slotLoadIcon(KJob*)) );
        m_jobIcon[ job ] = timelineName;
        job->start();
    }
    else {
        button->setIcon( KIcon( iconName ) );
    }
}

void NavButtonsWidget::wheelEvent( QWheelEvent* event )
{
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;

    QAbstractButton* currentButton = m_buttonGroup->checkedButton();
    int currentIndex = layout()->indexOf( currentButton );

    int buttonCount = layout()->count();

    int toClickIndex = ( currentIndex - numSteps ) % buttonCount;
    if ( toClickIndex < 0 )
        toClickIndex += buttonCount;

    KPushButton* buttonToClick = static_cast<KPushButton*>(layout()->itemAt( toClickIndex )->widget());
    buttonToClick->click();

    event->accept();
}

void NavButtonsWidget::slotButtonClicked( int id )
{
    emit timelineClicked( m_buttonIdTimeline[ id ] );
}

void NavButtonsWidget::slotLoadIcon( KJob* job )
{
    if ( job->error() ) {
        qWarning() << "Job Error: " << job->errorString();
        return;
    }

    QString timelineName = m_jobIcon.take( job );
    KIO::StoredTransferJob* j = static_cast<KIO::StoredTransferJob*>(job);

    KPushButton* button = m_navButton[ timelineName ];

    QPixmap pix;
    pix.loadFromData( j->data() );
    button->setIcon( KIcon( pix ) );
}
