
#include "zzzzapp.h"

#include <KAboutData>
#include <KCmdLineArgs>
#include <KLocale>

#include <cstdio>

int main(int argc, char* argv[])
{
    KAboutData about("zzzz", 0, ki18n("Zzzz"), "0.1", ki18n("KDE Microblog Client"),
                     KAboutData::License_GPL_V3, ki18n("(C) 2011 Ni Hui"),
                     KLocalizedString(), 0);
    about.addAuthor(ki18n("Ni Hui"), ki18n("Author"),
                    "shuizhuyuanluo@126.com", "http://shuizhuyuanluo.blog.163.com");
    about.setProgramIconName("face-angel");

    KCmdLineArgs::init(argc, argv, &about);

    KUniqueApplication::addCmdLineOptions();

    if (!KUniqueApplication::start()) {
        fprintf(stderr, "zzzz is already running!\n");
        return 0;
    }

    ZzzzApplication app;
    return app.exec();
}
