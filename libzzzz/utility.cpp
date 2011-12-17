#include "utility.h"

#include <QRegExp>

namespace Zzzz {

void Utility::urlize( QString& text )
{
    // NOTE: regex from http://www.regexguru.com/2008/11/detecting-urls-in-a-block-of-text/
    static QRegExp urlRegex( "((https?|ftp)://[-A-Z0-9+&@#/%?=~_|!:,.;]*[A-Z0-9+&@#/%=~_|])", Qt::CaseInsensitive );
    text.replace( urlRegex, "<a href=\"\\1\">\\1</a>" );
}

}
