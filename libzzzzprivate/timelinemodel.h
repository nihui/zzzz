#ifndef TIMELINEMODEL_H
#define TIMELINEMODEL_H

#include "zzzzprivate_export.h"

#include <QAbstractItemModel>
#include <QAbstractItemDelegate>
#include "typeswrapper.h"
#include <QTextDocument>

class PostDocument : public QTextDocument
{
public:
    explicit PostDocument();
    virtual ~PostDocument();
protected:
    virtual QVariant loadResource(int type, const QUrl& name);
};

class ZZZZPRIVATE_EXPORT TimelineModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit TimelineModel();
    virtual ~TimelineModel();
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex& index) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;

    void appendPost(const PostWrapper& post);
    void prependPost(const PostWrapper& post);
    void clearPosts();
private Q_SLOTS:
    void slotContentsChanged();
    void slotAccountRemoved(const QString& alias, const Account* oldAccount);
private:
    QList<PostWrapper> m_posts;
    QList<PostDocument*> m_docs;
};

class ZZZZPRIVATE_EXPORT TimelineDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    explicit TimelineDelegate(QObject* parent = 0);
    virtual ~TimelineDelegate();
    virtual bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index);
    virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    virtual QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
Q_SIGNALS:
    void anchorClicked(const QString& anchor, const QModelIndex& index);
};

#endif // TIMELINEMODEL_H
