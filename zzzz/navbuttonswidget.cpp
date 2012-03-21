#include "navbuttonswidget.h"

#include "mediafetcher.h"

#include <types.h>

#include <KAction>
#include <KDebug>
#include <KIcon>
#include <KLocale>
#include <KMenu>

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

    m_currentButton = 0;

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
        if ( event->type() == QEvent::MouseButtonDblClick ) {
            if ( button != m_currentButton ) {
                // should not get here
                return true;
            }
            if ( button->objectName().startsWith( "__" ) ) {
                // never close internal timelines
                return true;
            }
            emit timelineClosed( button->objectName() );
            int layoutIndex = m_buttonLayout->indexOf( button );
            m_buttonLayout->removeWidget( button );
            delete button;
            // switch to the next timeline if exist, the previous one otherwise
            int toClickIndex = qMin( layoutIndex, m_buttonLayout->count() - 1 );
            QWidget* bw = m_buttonLayout->itemAt( toClickIndex )->widget();
            QPushButton* buttonToClick = static_cast<QPushButton*>(bw);
            buttonToClick->setChecked( true );
            m_currentButton = buttonToClick;
            emit timelineClicked( m_currentButton->objectName() );
            return true;
        }
        if ( event->type() == QEvent::MouseButtonPress ) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            m_currentButton->setChecked( false );
            button->setChecked( true );
            m_currentButton = button;
            emit timelineClicked( m_currentButton->objectName() );
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

void NavButtonsWidget::addButton( const QString& timelineName, const QString& iconName, bool checked )
{
    QPushButton* button = new QPushButton;
    button->setObjectName( timelineName );
    button->setIconSize( QSize( 32, 32 ) );
    button->setCheckable( true );
    button->setChecked( checked );
    button->setFlat( true );
    button->setFocusPolicy( Qt::NoFocus );

    // make button transparent
    QPalette pal = button->palette();
    pal.setColor( QPalette::Button, Qt::transparent );
    button->setPalette( pal );

    button->installEventFilter( this );

    m_buttonLayout->addWidget( button );

    if ( checked ) {
        m_currentButton = button;
    }

    if ( iconName.startsWith( "http://" ) ) {
        /// load icon from remote url
        m_urlButton[ iconName ] = button;
        MediaFetcher::self()->requestAvatar( iconName );
    }
    else {
        button->setIcon( KIcon( iconName ) );
    }
}

void NavButtonsWidget::wheelEvent( QWheelEvent* event )
{
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;

    int currentIndex = m_buttonLayout->indexOf( m_currentButton );
    int buttonCount = m_buttonLayout->count();
    int toClickIndex = ( currentIndex - numSteps ) % buttonCount;
    if ( toClickIndex < 0 )
        toClickIndex += buttonCount;

    QWidget* bw = m_buttonLayout->itemAt( toClickIndex )->widget();
    QPushButton* buttonToClick = static_cast<QPushButton*>(bw);

    m_currentButton->setChecked( false );
    buttonToClick->setChecked( true );

    m_currentButton = buttonToClick;
    emit timelineClicked( m_currentButton->objectName() );

    event->accept();
}

void NavButtonsWidget::slotGotAvatar( const QString& url, const QImage& image )
{
    if ( !m_urlButton.contains( url ) )
        return;

    QPushButton* button = m_urlButton.take( url );
    button->setIcon( KIcon( QPixmap::fromImage( image ) ) );
}

void NavButtonsWidget::slotErrorAvatar( const QString& url )
{
    if ( !m_urlButton.contains( url ) )
        return;

    QPushButton* button = m_urlButton.take( url );
    button->setIcon( KIcon( "image-missing" ) );
}
