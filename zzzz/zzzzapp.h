#ifndef ZZZZAPP_H
#define ZZZZAPP_H

#include <KUniqueApplication>

class MainWindow;
class ZzzzApplication : public KUniqueApplication
{
    Q_OBJECT
    public:
        explicit ZzzzApplication();
        virtual ~ZzzzApplication();
        virtual int newInstance();
    public Q_SLOTS:
        virtual void loadCommandLineOptionsForNewInstance();
    private:
        MainWindow* m_mainwindow;
};

#endif // ZZZZAPP_H
