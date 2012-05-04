#ifndef THEMEENGINE_H
#define THEMEENGINE_H

#include "zzzzprivate_export.h"

#include <typeswrapper.h>
#include <grantlee_core.h>

class ZZZZPRIVATE_EXPORT ThemeEngine
{
public:
    static ThemeEngine* self();
    ~ThemeEngine();
    QString render(const PostWrapper& post) const;
private:
    explicit ThemeEngine();
    static ThemeEngine* m_self;

    Grantlee::Engine* m_engine;
};

#endif // THEMEENGINE_H
