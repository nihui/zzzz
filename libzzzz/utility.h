#ifndef UTILITY_H
#define UTILITY_H

#include "zzzz_export.h"

#include <QString>

namespace Zzzz {

namespace Utility
{
    /// add html tag around link and mail address
    ZZZZ_EXPORT void urlize( QString& text );
}

}

#endif // UTILITY_H
