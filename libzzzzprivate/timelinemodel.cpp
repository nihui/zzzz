#include "timelinemodel.h"
#include "accountmanager.h"
#include "themeengine.h"
#include "mediafetcher.h"

#include <QTextDocument>
#include <QUrl>
#include <QPixmapCache>
#include <QTextBlock>
#include <QTextEdit>

#include <KDebug>

PostDocument::PostDocument()
{
    connect(MediaFetcher::self(), SIGNAL(gotPixmap(const QUrl&, const QPixmap&)),
            this, SIGNAL(contentsChanged()));
}

PostDocument::~PostDocument()
{
}

QVariant PostDocument::loadResource(int type, const QUrl& name)
{
    if (type == QTextDocument::ImageResource) {
        QPixmap pixmap;
        if (MediaFetcher::self()->findPixmap(name, &pixmap)) {
            return pixmap;
        }
    }
    return QTextDocument::loadResource(type, name);
}

TimelineModel::TimelineModel()
{
    connect(AccountManager::self(), SIGNAL(accountRemoved(const QString&, const Account*)),
            this, SLOT(slotAccountRemoved(const QString&, const Account*)));
}

TimelineModel::~TimelineModel()
{
}

int TimelineModel::columnCount(const QModelIndex& parent) const
{
    return 1;
}

QVariant TimelineModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::UserRole) {
        const PostWrapper& post = m_posts.at(index.row());
        return QVariant::fromValue(post);
    }

    if (role == Qt::UserRole + 1) {
        QTextDocument* doc = m_docs.at(index.row());
        return QVariant::fromValue(static_cast<QObject*>(doc));
    }

    return QVariant();
}

Qt::ItemFlags TimelineModel::flags(const QModelIndex& index) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant TimelineModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}

QModelIndex TimelineModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    if (!parent.isValid())
        return createIndex(row, column);

    return QModelIndex();
}

QModelIndex TimelineModel::parent(const QModelIndex& /*index*/) const
{
    return QModelIndex();
}

int TimelineModel::rowCount(const QModelIndex& parent) const
{
    if (!parent.isValid())
        return m_posts.count();

    return 0;
}

void TimelineModel::appendPost(const PostWrapper& post)
{
    if (m_posts.contains(post))
        return;

    beginInsertRows(QModelIndex(), m_posts.count(), m_posts.count());
    m_posts.append(post);
    PostDocument* doc = new PostDocument;
    connect(doc, SIGNAL(contentsChanged()), this, SLOT(slotContentsChanged()));
    doc->setUndoRedoEnabled(false);
    doc->setHtml(ThemeEngine::self()->render(post));
    m_docs.append(doc);
    endInsertRows();
}

void TimelineModel::prependPost(const PostWrapper& post)
{
    if (m_posts.contains(post))
        return;

    beginInsertRows(QModelIndex(), 0, 0);
    m_posts.prepend(post);
    PostDocument* doc = new PostDocument;
    connect(doc, SIGNAL(contentsChanged()), this, SLOT(slotContentsChanged()));
    doc->setUndoRedoEnabled(false);
    doc->setHtml(ThemeEngine::self()->render(post));
    m_docs.prepend(doc);
    endInsertRows();
}

void TimelineModel::clearPosts()
{
    beginResetModel();
    m_posts.clear();
    qDeleteAll(m_docs);
    m_docs.clear();
    endResetModel();
}

void TimelineModel::slotContentsChanged()
{
    PostDocument* doc = static_cast<PostDocument*>(sender());
    int i = m_docs.indexOf(doc);
    emit dataChanged(index(i, 0), index(i, 0));
}

void TimelineModel::slotAccountRemoved(const QString& alias, const Account* oldAccount)
{
    for (int i = 0; i < m_posts.count();) {
        const PostWrapper& post = m_posts.at(i);
        if (post.myAccount() == oldAccount) {
            beginRemoveRows(QModelIndex(), i, i);
            m_posts.removeAt(i);
            delete m_docs.takeAt(i);
            endRemoveRows();
        } else {
            ++i;
        }
    }
}

