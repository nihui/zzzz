#include "themeconfig.h"

#include "themeengine.h"

#include <KAction>

#include <QListWidget>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <KPluginFactory>
#include <KPluginInfo>
#include <KPluginLoader>
#include <KStandardDirs>

K_PLUGIN_FACTORY(ZzzzThemeConfigFactory, registerPlugin<ZzzzThemeConfig>();)
K_EXPORT_PLUGIN(ZzzzThemeConfigFactory("kcm_zzzz_themeconfig"))


ZzzzThemeConfig::ZzzzThemeConfig(QWidget* parent, const QVariantList& args)
    : KCModule(ZzzzThemeConfigFactory::componentData(), parent, args)
{
    QHBoxLayout* mainLayout = new QHBoxLayout;
    setLayout(mainLayout);

    m_themeListView = new QListWidget;
    mainLayout->addWidget(m_themeListView);

    m_preview = new QTextEdit;
    mainLayout->addWidget(m_preview);
}

ZzzzThemeConfig::~ZzzzThemeConfig()
{
}

void ZzzzThemeConfig::load()
{
    Zzzz::Post p;
    p.id = "0";
    p.text = "This is post text";
    p.user.id = "0";
    p.user.name = "Name";
    p.user.screenName = "ScreenName";
    p.source = "preview";
    p.creationDateTime = 0;

    PostWrapper post(p);

    m_preview->setHtml(ThemeEngine::self()->render(post));
}

void ZzzzThemeConfig::save()
{
}

void ZzzzThemeConfig::defaults()
{
}
