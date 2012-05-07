#include "timelinedelegate.h"

#include "timelinemodel.h"

#include <QAbstractTextDocumentLayout>
#include <QApplication>
#include <QPainter>

#include <KDebug>

#include "composerwidget.h"

TimelineDelegate::TimelineDelegate(QObject* parent)
    : QAbstractItemDelegate(parent)
{
    m_editingRow = -1;
    m_editingModel = 0;
}

TimelineDelegate::~TimelineDelegate()
{
}

QWidget* TimelineDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    ComposerWidget* w = new ComposerWidget(parent);
    connect(w, SIGNAL(postComposed(const PostWrapper&)),
            this, SIGNAL(postComposed(const PostWrapper&)));
    connect(w, SIGNAL(postComposed(const PostWrapper&)), this, SLOT(closeComposer()));
    connect(w, SIGNAL(postDiscarded()), this, SLOT(closeComposer()));
    return w;
}

bool TimelineDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index)
{
    QStyleOptionViewItemV4 opt(option);

    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* e = static_cast<QMouseEvent*>(event);
        PostDocument* doc = static_cast<PostDocument*>(index.data(Qt::UserRole + 1).value<QObject*>());
        QString anchor = doc->documentLayout()->anchorAt(e->pos() - opt.rect.topLeft());
        if (e->button() == Qt::LeftButton) {
            if (m_editingRow != -1 && m_editingRow != index.row()) {
                emit sizeHintChanged(model->index(m_editingRow, 0));
                m_editingRow = -1;
                m_editingModel = 0;
            }
            if (!anchor.isEmpty()) {
                emit anchorClicked(anchor, index);
                if (anchor == "zzzz:reply") {
                    m_editingRow = index.row();
                    m_editingModel = model;
                    emit sizeHintChanged(index);
                }
            }
        }
    }

    return QAbstractItemDelegate::editorEvent(event, model, option, index);
}

void TimelineDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItemV4 opt(option);
//     QStyle* style = opt.widget ? opt.widget->style() : QApplication::style();
//     style->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter, opt.widget);

    QAbstractTextDocumentLayout::PaintContext ctx;

//     // highlight text if item is selected
//     if (opt.state & QStyle::State_Selected)
//         ctx.palette.setColor(QPalette::Text, opt.palette.color(QPalette::Active, QPalette::HighlightedText));

//     PostWrapper post = index.data(Qt::UserRole).value<PostWrapper>();

    PostDocument* doc = static_cast<PostDocument*>(index.data(Qt::UserRole + 1).value<QObject*>());
//     doc.setHtml(ThemeEngine::self()->render(post));
    doc->setTextWidth(opt.rect.width());

    painter->save();
    painter->translate(opt.rect.topLeft());
    doc->documentLayout()->draw(painter, ctx);
    painter->restore();
}

QSize TimelineDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItemV4 opt(option);

//     PostWrapper post = index.data(Qt::UserRole).value<PostWrapper>();

    PostDocument* doc = static_cast<PostDocument*>(index.data(Qt::UserRole + 1).value<QObject*>());
//     doc.setHtml(ThemeEngine::self()->render(post));
    doc->setTextWidth(opt.rect.width());

    if (index.row() == m_editingRow) {
        return QSize(doc->idealWidth(), doc->size().height() + 100);
    }
    return QSize(doc->idealWidth(), doc->size().height());
}

void TimelineDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    kWarning();
    ComposerWidget* w = static_cast<ComposerWidget*>(editor);
    PostWrapper post = index.data(Qt::UserRole).value<PostWrapper>();
    w->composeReply(post);
}

void TimelineDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    kWarning();
}

void TimelineDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    editor->setGeometry(option.rect.x(), option.rect.y() + option.rect.height() - 100,
                        option.rect.width(), 100);
}

void TimelineDelegate::closeComposer()
{
    ComposerWidget* w = static_cast<ComposerWidget*>(sender());
    emit closeEditor(w, QAbstractItemDelegate::NoHint);
    emit sizeHintChanged(m_editingModel->index(m_editingRow, 0));
    m_editingRow = -1;
    m_editingModel = 0;
}
