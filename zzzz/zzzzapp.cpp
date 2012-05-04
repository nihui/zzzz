
#include "zzzzapp.h"
#include "mainwindow.h"

ZzzzApplication::ZzzzApplication()
    : KUniqueApplication()
{
    m_mainwindow = new MainWindow;
    m_mainwindow->show();
}

ZzzzApplication::~ZzzzApplication()
{
}

int ZzzzApplication::newInstance()
{
    return KUniqueApplication::newInstance();
}

void ZzzzApplication::loadCommandLineOptionsForNewInstance()
{
}
