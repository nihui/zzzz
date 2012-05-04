#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include "zzzzprivate_export.h"

#include <KPluginInfo>
#include <QObject>
#include <QList>
#include <QHash>
namespace Zzzz
{
class MicroBlog;
}

class ZZZZPRIVATE_EXPORT PluginManager : public QObject
{
public:
    static PluginManager* self();
    ~PluginManager();
    void loadMicroBlogPlugin();
    QString microBlogPluginName(Zzzz::MicroBlog* microblog) const;
    QList<KPluginInfo> microBlogPluginInfos() const;
    KPluginInfo microBlogPluginInfo(Zzzz::MicroBlog* microblog) const;
    KPluginInfo microBlogPluginInfo(const QString& pluginName) const;
    Zzzz::MicroBlog* microBlog(const QString& pluginName) const;
private:
    explicit PluginManager();
    static PluginManager* m_self;
    QHash<QString, KPluginInfo> m_microblogPluginInfos;
    QHash<QString, Zzzz::MicroBlog*> m_microblogs;
};

#endif // PLUGINMANAGER_H
