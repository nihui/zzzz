#ifndef ADDACCOUNTWIZARDPAGE1_H
#define ADDACCOUNTWIZARDPAGE1_H

#include <QWidget>

class QTreeWidget;

class AddAccountWizardPage1 : public QWidget
{
    public:
        explicit AddAccountWizardPage1( QWidget* parent = 0 );
        virtual ~AddAccountWizardPage1();
        QString selectedService() const;
    private:
        QTreeWidget* m_treeWidget;
};

#endif // ADDACCOUNTWIZARDPAGE1_H
