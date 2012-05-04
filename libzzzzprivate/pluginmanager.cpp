#include "pluginmanager.h"

#include "microblog.h"

#include "accountmanager.h"

#include <KDebug>
#include <KPluginFactory>
#include <KPluginInfo>
#include <KPluginLoader>
#include <KServiceTypeTrader>

PluginManager* PluginManager::m_self = 0;

PluginManager* PluginManager::self()
{
    if (!m_self)
        m_self = new PluginManager;
    return m_self;
}

PluginManager::PluginManager()
{
//     loadMicroBlogPlugin();
}

PluginManager::~PluginManager()
{
}

void PluginManager::loadMicroBlogPlugin()
{
    KService::List offers = KServiceTypeTrader::self()->query("Zzzz/MicroBlog");
    const KConfigGroup plugins = KConfigGroup(KGlobal::config(), "Plugins");
    KService::List::ConstIterator it = offers.constBegin();
    KService::List::ConstIterator end = offers.constEnd();
    while (it != end) {
        KService::Ptr service = *it;
        ++it;
        KPluginInfo pluginInfo(service);
        pluginInfo.load(plugins);
        if (!pluginInfo.isPluginEnabled()) {
            qWarning() << "plugin " << service->library() << " already disabled.";
            m_microblogPluginInfos.insert(service->library(), pluginInfo);
            delete m_microblogs.take(service->library());
            continue;
        }

        if (m_microblogs.contains(service->library())) {
            qWarning() << "already loaded " << service->library();
            continue;
        }

        KPluginFactory* factory = KPluginLoader(service->library()).factory();
        if (!factory) {
            qWarning() << "can not load plugin " << service->library();
            continue;
        }

        Zzzz::MicroBlog* plugin = factory->create<Zzzz::MicroBlog>(this);
        if (!plugin) {
            qWarning() << "error loading plugin " << service->library();
            continue;
        }

        qWarning() << "Load plugin: " << service->library();
        m_microblogPluginInfos.insert(service->library(), pluginInfo);
        m_microblogs.insert(service->library(), plugin);
    }

    // refresh account for microBlog plugin changes
    AccountManager::self()->loadAccounts();
}

QList<KPluginInfo> PluginManager::microBlogPluginInfos() const
{
    return m_microblogPluginInfos.values();
}

KPluginInfo PluginManager::microBlogPluginInfo(Zzzz::MicroBlog* microblog) const
{
    QString pluginName = m_microblogs.key(microblog);
    return microBlogPluginInfo(pluginName);
}

KPluginInfo PluginManager::microBlogPluginInfo(const QString& pluginName) const
{
    return m_microblogPluginInfos.value(pluginName);
}

Zzzz::MicroBlog* PluginManager::microBlog(const QString& pluginName) const
{
    return m_microblogs.value(pluginName);
}
