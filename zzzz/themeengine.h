#ifndef THEMEENGINE_H
#define THEMEENGINE_H

#include <grantlee_core.h>

class ThemeEngine
{
    public:
        static ThemeEngine* self();
        ~ThemeEngine();
        QString render( const QVariantList& postList ) const;
    private:
        explicit ThemeEngine();
        static ThemeEngine* m_self;

        Grantlee::Engine* m_engine;
};

#endif // THEMEENGINE_H
