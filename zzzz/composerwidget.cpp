#include "composerwidget.h"

#include <QFocusEvent>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPalette>
#include <QBrush>
#include <QPixmap>

#include <KDebug>

#include <types.h>

#include "typeswrapper.h"

ComposerWidget::ComposerWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUi(this);

    setAutoFillBackground(true);

    m_limit = 150;
    m_replyAccount = 0;

    composerEdit->installEventFilter(this);

    connect(composerEdit, SIGNAL(textChanged()), this, SLOT(updateIndicator()));
}

ComposerWidget::~ComposerWidget()
{
}

void ComposerWidget::setCharLimit(int limit)
{
    m_limit = limit;
    updateIndicator();
}

void ComposerWidget::reset()
{
    composerEdit->clear();
    m_replyAccount = 0;
    m_replyToStatusId.clear();
}

void ComposerWidget::composeReply(const PostWrapper& post)
{
    m_replyAccount = post.myAccount();
    m_replyToStatusId = post.id();
    composerEdit->setPlainText("@" + post.userScreenName() + " " + composerEdit->toPlainText());
    composerEdit->setFocus();
}

bool ComposerWidget::eventFilter(QObject* obj, QEvent* event)
{
    if (obj != composerEdit) {
        return QObject::eventFilter(obj, event);
    }

    if (event->type() == QEvent::Resize) {
        updateIndicator();
    }
    else if (event->type() == QEvent::KeyPress) {
        QKeyEvent* e = static_cast<QKeyEvent*>(event);
        if (e->key() == Qt::Key_Return) {
            QString text = composerEdit->toPlainText();
            if (!text.isEmpty()) {
                kWarning() << "enter pressed" << text;
                Zzzz::Post p;
                p.replyToStatusId = m_replyToStatusId;
                p.text = text;
                PostWrapper post(p);
                post.setMyAccount(m_replyAccount);
                emit postComposed(post);
    //             reset();
    //             hide();
            }
        }
        if (e->key() == Qt::Key_Escape) {
    //         reset();
    //         hide();
            emit postDiscarded();
        }
    }

    return QObject::eventFilter(obj, event);
}

void ComposerWidget::updateIndicator()
{
    int num = m_limit - composerEdit->toPlainText().size();
    QPixmap pixmap(composerEdit->width(), composerEdit->height());
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
    p.drawText(0, 0, composerEdit->width() - 10, composerEdit->height(), Qt::AlignBottom | Qt::AlignRight, QString::number(num));
    QPalette pal = composerEdit->palette();
    pal.setBrush(QPalette::Base, QBrush(pixmap));
    composerEdit->setPalette(pal);
}
