#include "composerwidget.h"

#include <KDebug>
#include <KPushButton>

#include <QHBoxLayout>
#include <QFocusEvent>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPalette>
#include <QBrush>
#include <QPixmap>

#include <types.h>

#include "typeswrapper.h"

ComposerWidget::ComposerWidget( QWidget* parent )
: KTextEdit(parent)
{
    setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::Preferred );

    m_post = new PostWrapper( Zzzz::Post() );

    setEditing( false );

    m_limit = 150;

    connect( this, SIGNAL(textChanged()), this, SLOT(slotTextChanged()) );
}

ComposerWidget::~ComposerWidget()
{
}

void ComposerWidget::setCharLimit( int limit )
{
    m_limit = limit;
    slotTextChanged();
}

void ComposerWidget::composeReply( const PostWrapper* post )
{
    m_post->myAccount = post->myAccount;
    m_post->m_post.replyToStatusId = post->m_post.replyToStatusId;
    setFocus();
}

void ComposerWidget::focusInEvent( QFocusEvent* event )
{
    setEditing( true );
    KTextEdit::focusInEvent( event );
}

void ComposerWidget::focusOutEvent( QFocusEvent* event )
{
    if ( event->reason() != Qt::PopupFocusReason
        && event->reason() != Qt::ActiveWindowFocusReason )
        setEditing( false );
    KTextEdit::focusInEvent( event );
}

void ComposerWidget::keyPressEvent( QKeyEvent* event )
{
    if ( event->key() == Qt::Key_Return ) {
        QString text = toPlainText();
        if ( !text.isEmpty() ) {
            kWarning() << "enter pressed" << text;
            m_post->m_post.text = text;
            emit postComposed( m_post );
            clear();
            setEditing( false );
        }
        return;
    }
    KTextEdit::keyPressEvent( event );
}

void ComposerWidget::resizeEvent( QResizeEvent* event )
{
    slotTextChanged();
    KTextEdit::resizeEvent( event );
}

void ComposerWidget::slotTextChanged()
{
    int num = m_limit - toPlainText().size();
    QPixmap pixmap( width(), height() );
    pixmap.fill( Qt::white );
    QPainter p( &pixmap );
    QFont font = p.font();
    font.setPointSize( 20 );
    p.setFont( font );
    QPen pen = p.pen();
    if ( num < 0 )
        pen.setColor( QColor( 255, 100, 100 ) );
    else
        pen.setColor( QColor( 100, 200, 100 ) );
    p.setPen( pen );
    p.drawText( 0, 0, width() - 10, height(), Qt::AlignBottom | Qt::AlignRight, QString::number( m_limit - toPlainText().size() ) );
    QPalette pal = palette();
    pal.setBrush( QPalette::Base, QBrush( pixmap ) );
    setPalette( pal );
}

void ComposerWidget::setEditing( bool isEditing )
{
    if ( isEditing ) {
        setMaximumHeight( 80 );
    }
    else {
        setMaximumHeight( 40 );
    }
}
