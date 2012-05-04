#include "microblog.h"

#include <types.h>

namespace Zzzz
{

MicroBlog::MicroBlog(QObject* parent, const QVariantList& args)
    : Plugin(parent, args)
{
}

MicroBlog::~MicroBlog()
{
}

void MicroBlog::getEmotion(QString& apiUrl, ParamMap& params)
{
}

QRegExp MicroBlog::usernameMatch() const
{
    return QRegExp("@([^\\s\\W]+)", Qt::CaseInsensitive);
}

QRegExp MicroBlog::topicMatch() const
{
    return QRegExp("#([^\\s\\W]+)#", Qt::CaseInsensitive);
}

QRegExp MicroBlog::emotionMatch() const
{
    return QRegExp();
}

void MicroBlog::readEmotionFromData(const QByteArray& data, QStringList& list, bool* ok)
{
}

void MicroBlog::readTimelineFromData(const QByteArray& data, QList<Zzzz::Post>& postlist, bool* ok)
{
    QVariantList list = m_parser.parse(data, ok).toList();
    if (!ok)
        return;
    QVariantList::ConstIterator it = list.constBegin();
    QVariantList::ConstIterator end = list.constEnd();
    while (it != end) {
        Zzzz::Post post;
        readPostFromJsonMap(it->toMap(), post);
        postlist << post;
        ++it;
    }
}

void MicroBlog::readPostFromData(const QByteArray& data, Zzzz::Post& post, bool* ok)
{
    QVariantMap varmap = m_parser.parse(data, ok).toMap();
    if (!ok)
        return;
    readPostFromJsonMap(varmap, post);
}

}
