#ifndef ADDACCOUNTWIZARD_H
#define ADDACCOUNTWIZARD_H

#include "zzzzprivate_export.h"

#include <KAssistantDialog>

class Account;
class AddAccountWizardPage1;
class AddAccountWizardPage2;

class ZZZZPRIVATE_EXPORT AddAccountWizard : public KAssistantDialog
{
    Q_OBJECT
    public:
        explicit AddAccountWizard( QWidget* parent = 0 );
        virtual ~AddAccountWizard();
    public Q_SLOTS:
        virtual void back();
        virtual void next();
        virtual void accept();
        virtual void reject();
    private:
        AddAccountWizardPage1* m_page1;
        AddAccountWizardPage2* m_page2;
        Account* m_newAccount;
};

#endif // ADDACCOUNTWIZARD_H
