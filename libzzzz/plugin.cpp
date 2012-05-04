#include "plugin.h"

namespace Zzzz
{

class PluginPrivate
{
public:
    explicit PluginPrivate(Plugin* q) : q(q) {}
    Plugin* const q;
};

Plugin::Plugin(QObject* parent, const QVariantList& args)
    : QObject(parent), KXMLGUIClient(), d(new PluginPrivate(this))
{
    Q_UNUSED(args)
//     KSettings::Dispatcher::registerComponent( instance, this, "settingsChanged" );
}

Plugin::~Plugin()
{
    delete d;
}

}
