#ifndef ZZZZ_PLUGIN_H
#define ZZZZ_PLUGIN_H

#include "zzzz_export.h"

#include <KXMLGUIClient>

#include <QObject>
#include <QVariantList>

namespace Zzzz {

class PluginPrivate;
class ZZZZ_EXPORT Plugin : public QObject, public KXMLGUIClient
{
    Q_OBJECT
    public:
        explicit Plugin( QObject* parent, const QVariantList& args );
        virtual ~Plugin();
    private:
        PluginPrivate* const d;
};

}

#endif // ZZZZ_PLUGIN_H
