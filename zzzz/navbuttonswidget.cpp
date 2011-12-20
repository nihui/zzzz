#include "navbuttonswidget.h"

#include "mediafetcher.h"

#include <types.h>

#include <KAction>
#include <KDebug>
#include <KIcon>
#include <KLocale>
#include <KMenu>

#include <QButtonGroup>
#include <QContextMenuEvent>
#include <QFocusEvent>
#include <QHBoxLayout>
#include <QPushButton>
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

    connect( MediaFetcher::self(), SIGNAL(gotAvatar(const QString&, const QImage&)),
             this, SLOT(slotGotAvatar(const QString&, const QImage&)) );
    connect( MediaFetcher::self(), SIGNAL(errorAvatar(const QString&)),
             this, SLOT(slotErrorAvatar(const QString&)) );
}

NavButtonsWidget::~NavButtonsWidget()
{
}

bool NavButtonsWidget::eventFilter( QObject* watched, QEvent* event )
{
    QPushButton* button = qobject_cast<QPushButton*>(watched);
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
    QPushButton* button = new QPushButton;
    button->setIconSize( QSize( 32, 32 ) );
    button->setCheckable( true );
    button->setFlat( true );
    button->setFocusPolicy( Qt::NoFocus );

    // make button transparent
    QPalette pal = button->palette();
    pal.setColor( QPalette::Button, Qt::transparent );
    button->setPalette( pal );

    button->installEventFilter( this );

    m_buttonGroup->addButton( button );

    m_buttonLayout->addWidget( button );

    int id = m_buttonGroup->id( button );
    m_buttonIdTimeline[ id ] = timelineName;

    m_navButton[ timelineName ] = button;

    if ( iconName.startsWith( "http://" ) ) {
        /// load icon from remote url
        m_urlTimeline[ iconName ] = timelineName;
        MediaFetcher::self()->requestAvatar( iconName );
    }
    else {
        button->setIcon( KIcon( iconName ) );
    }
}

void NavButtonsWidget::clickButton( const QString& timelineName )
{
    m_navButton[ timelineName ]->click();
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

    QPushButton* buttonToClick = static_cast<QPushButton*>(layout()->itemAt( toClickIndex )->widget());
    buttonToClick->click();

    event->accept();
}

void NavButtonsWidget::slotButtonClicked( int id )
{
    emit timelineClicked( m_buttonIdTimeline[ id ] );
}

void NavButtonsWidget::slotGotAvatar( const QString& url, const QImage& image )
{
    if ( !m_urlTimeline.contains( url ) )
        return;

    QString timelineName = m_urlTimeline.take( url );
    QPushButton* button = m_navButton[ timelineName ];

    button->setIcon( KIcon( QPixmap::fromImage( image ) ) );
}

void NavButtonsWidget::slotErrorAvatar( const QString& url )
{
    if ( !m_urlTimeline.contains( url ) )
        return;

    QString timelineName = m_urlTimeline.take( url );
    QPushButton* button = m_navButton[ timelineName ];

    button->setIcon( KIcon( "image-missing" ) );
}
