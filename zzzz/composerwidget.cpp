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

ComposerWidget* ComposerWidget::m_self = 0;

ComposerWidget* ComposerWidget::self()
{
    if (!m_self)
        m_self = new ComposerWidget;
    return m_self;
}

ComposerWidget::ComposerWidget(QWidget* parent)
    : KTextEdit(parent)
{
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);

    m_limit = 150;
    m_replyAccount = 0;

    connect(this, SIGNAL(textChanged()), this, SLOT(slotTextChanged()));
}

ComposerWidget::~ComposerWidget()
{
}

void ComposerWidget::setCharLimit(int limit)
{
    m_limit = limit;
    slotTextChanged();
}

void ComposerWidget::reset()
{
    clear();
    m_replyAccount = 0;
    m_replyToStatusId.clear();
}

void ComposerWidget::composeReply(const PostWrapper& post)
{
    m_replyAccount = post.myAccount();
    m_replyToStatusId = post.id();
    setPlainText("@" + post.userScreenName() + " " + toPlainText());
    setFocus();
}

void ComposerWidget::focusOutEvent(QFocusEvent* event)
{
    reset();
    hide();
}

void ComposerWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Return) {
        QString text = toPlainText();
        if (!text.isEmpty()) {
            kWarning() << "enter pressed" << text;
            Zzzz::Post p;
            p.replyToStatusId = m_replyToStatusId;
            p.text = text;
            PostWrapper post(p);
            post.setMyAccount(m_replyAccount);
            emit postComposed(post);
            reset();
            hide();
        }
        return;
    }
    if (event->key() == Qt::Key_Escape) {
        reset();
        hide();
        return;
    }
    KTextEdit::keyPressEvent(event);
}

void ComposerWidget::resizeEvent(QResizeEvent* event)
{
    slotTextChanged();
    KTextEdit::resizeEvent(event);
}

void ComposerWidget::slotTextChanged()
{
    int num = m_limit - toPlainText().size();
    QPixmap pixmap(width(), height());
    pixmap.fill(Qt::white);
    QPainter p(&pixmap);
    QFont font = p.font();
    font.setPointSize(20);
    p.setFont(font);
    QPen pen = p.pen();
    if (num < 0)
        pen.setColor(QColor(255, 100, 100));
    else
        pen.setColor(QColor(100, 200, 100));
    p.setPen(pen);
    p.drawText(0, 0, width() - 10, height(), Qt::AlignBottom | Qt::AlignRight, QString::number(m_limit - toPlainText().size()));
    QPalette pal = palette();
    pal.setBrush(QPalette::Base, QBrush(pixmap));
    setPalette(pal);
}
