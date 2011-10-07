#! /bin/bash 
#Script to extract translateble messages from the code.
#The following part is used to do the same as Scripty in KDE svn

XGETTEXT="xgettext --copyright-holder=This_file_is_part_of_KDE --from-code=UTF-8 -C --kde -ci18n -ki18n:1 -ki18nc:1c,2 -ki18np:1,2 -ki18ncp:1c,2,3 -ktr2i18n:1 -kI18N_NOOP:1 -kI18N_NOOP2:1c,2 -kaliasLocale -kki18n:1 -kki18nc:1c,2 -kki18np:1,2 -kki18ncp:1c,2,3 --msgid-bugs-address=http://bugs.kde.org"

echo 'i18nc("NAME OF TRANSLATORS","Your names");' >> rc.cpp
echo 'i18nc("EMAIL OF TRANSLATORS","Your emails");' >> rc.cpp 

$XGETTEXT src/*.cpp libzzzz/*.cpp libzzzzprivate/*.cpp microblogs/*/*.cpp src/config/*/*.cpp rc.cpp -o zzzz.pot
rm -f rc.cpp
