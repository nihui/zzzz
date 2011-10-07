#ifndef PLUGINCONFIG_H
#define PLUGINCONFIG_H

#include <KCModule>

class KPluginSelector;

class ZzzzPluginConfig : public KCModule
{
    Q_OBJECT
    public:
        explicit ZzzzPluginConfig( QWidget* parent, const QVariantList &args );
        virtual ~ZzzzPluginConfig();
    public Q_SLOTS:
        virtual void load();
        virtual void save();
        virtual void defaults();
    private:
        KPluginSelector* m_pluginSelector;
};

#endif // PLUGINCONFIG_H
