#include "pluginconfig.h"

#include <KPluginFactory>
#include <KPluginInfo>
#include <KPluginLoader>
#include <KPluginSelector>
#include <KServiceTypeTrader>

#include <QVBoxLayout>

K_PLUGIN_FACTORY( ZzzzPluginConfigFactory, registerPlugin<ZzzzPluginConfig>(); )
K_EXPORT_PLUGIN( ZzzzPluginConfigFactory("kcm_zzzz_pluginconfig") )

ZzzzPluginConfig::ZzzzPluginConfig( QWidget* parent, const QVariantList& args )
: KCModule(ZzzzPluginConfigFactory::componentData(),parent,args)
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setMargin( 0 );
    m_pluginSelector = new KPluginSelector;
    mainLayout->addWidget( m_pluginSelector );
    setLayout( mainLayout );

    connect( m_pluginSelector, SIGNAL(changed(bool)), this, SLOT(changed()) );
//     connect( m_pluginSelector, SIGNAL(configCommitted(const QByteArray&) ),
//              this, SLOT(reparseConfiguration(const QByteArray&)) );

    KService::List offers = KServiceTypeTrader::self()->query( "Zzzz/MicroBlog" );
    m_pluginSelector->addPlugins( KPluginInfo::fromServices( offers ),
                                  KPluginSelector::ReadConfigFile, i18n( "MicroBlog" ),
                                  "MicroBlog", KGlobal::config() );

    m_pluginSelector->load();
}

ZzzzPluginConfig::~ZzzzPluginConfig()
{
}

void ZzzzPluginConfig::load()
{
    m_pluginSelector->load();
}

void ZzzzPluginConfig::save()
{
    m_pluginSelector->save();
}

void ZzzzPluginConfig::defaults()
{
    m_pluginSelector->defaults();
}

