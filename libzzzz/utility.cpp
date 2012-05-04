#include "utility.h"

#include <cstdio>
#include <cstring>

#include <QDateTime>
#include <QRegExp>

namespace Zzzz
{

void Utility::urlize(QString& text)
{
    // NOTE: regex from http://www.regexguru.com/2008/11/detecting-urls-in-a-block-of-text/
    static QRegExp urlRegex("((https?|ftp)://[-A-Z0-9+&@#/%?=~_|!:,.;]*[A-Z0-9+&@#/%=~_|])", Qt::CaseInsensitive);
    text.replace(urlRegex, "<a href=\"\\1\">\\1</a>");
}

uint Utility::string2datetime(const QString& text)
{
    // "Mon Apr 18 17:02:51 +0800 2011"
    char monstr[10];
    int year = 1900, month = 1, day = 1;
    int hour = 0, min = 0, sec = 0;
    int tz; // TODO: regard timezone here --- nihui
    sscanf(qPrintable(text), "%*s %s %d %d:%d:%d %d %d", monstr, &day, &hour, &min, &sec, &tz, &year);
    // determine month string
    if (strcmp(monstr, "Jan") == 0) month = 1;
    else if (strcmp(monstr, "Feb") == 0) month = 2;
    else if (strcmp(monstr, "Mar") == 0) month = 3;
    else if (strcmp(monstr, "Apr") == 0) month = 4;
    else if (strcmp(monstr, "May") == 0) month = 5;
    else if (strcmp(monstr, "Jun") == 0) month = 6;
    else if (strcmp(monstr, "Jul") == 0) month = 7;
    else if (strcmp(monstr, "Aug") == 0) month = 8;
    else if (strcmp(monstr, "Sep") == 0) month = 9;
    else if (strcmp(monstr, "Oct") == 0) month = 10;
    else if (strcmp(monstr, "Nov") == 0) month = 11;
    else if (strcmp(monstr, "Dec") == 0) month = 12;
    QDateTime recognized(QDate(year, month, day), QTime(hour, min, sec));
    return recognized.toTime_t();
}

}

