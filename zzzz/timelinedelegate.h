#ifndef TIMELINEDELEGATE_H
#define TIMELINEDELEGATE_H

#include <QAbstractItemDelegate>

class TimelineDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    explicit TimelineDelegate(QObject* parent = 0);
    virtual ~TimelineDelegate();
    virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    virtual bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index);
    virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    virtual void setEditorData(QWidget* editor, const QModelIndex& index) const;
    virtual void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
    virtual QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
    virtual void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const;
Q_SIGNALS:
    void anchorClicked(const QString& anchor, const QModelIndex& index);
private:
    mutable int m_editingRow;
};

#endif // TIMELINEDELEGATE_H
