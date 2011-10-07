#include "addaccountwizard.h"
#include "account.h"
#include "accountmanager.h"
#include "addaccountwizardpage1.h"
#include "addaccountwizardpage2.h"
#include "pluginmanager.h"

#include "microblog.h"

#include <KDebug>
#include <KLocale>

AddAccountWizard::AddAccountWizard( QWidget* parent )
: KAssistantDialog(parent)
{
    m_page1 = new AddAccountWizardPage1;
    m_page2 = new AddAccountWizardPage2;
    addPage( m_page1, i18n( "Select service" ) );
    addPage( m_page2, i18n( "Account information" ) );

    m_newAccount = 0;
}

AddAccountWizard::~AddAccountWizard()
{
}

void AddAccountWizard::back()
{
    QWidget* currentWidget = currentPage()->widget();
    if ( currentWidget == m_page2 ) {
        delete m_newAccount;
        m_newAccount = 0;
    }
    KAssistantDialog::back();
}

void AddAccountWizard::next()
{
    QWidget* currentWidget = currentPage()->widget();
    if ( currentWidget == m_page1 ) {
        QString microblogName = m_page1->selectedService();
        qWarning() << microblogName;
        Zzzz::MicroBlog* m_microblog = PluginManager::self()->microBlog( microblogName );
        m_newAccount = new Account( m_microblog );
        m_newAccount->authorize();
    }
    KAssistantDialog::next();
}

void AddAccountWizard::accept()
{
    QWidget* currentWidget = currentPage()->widget();
    if ( currentWidget != m_page2 )
        return;

    QString alias = m_page2->alias();
    QString pincode = m_page2->pincode();
    if ( alias.isEmpty() || pincode.isEmpty() ) {
        qWarning() << "empty alias or pincode!";
        return;
    }
    qWarning() << alias << pincode;
    m_newAccount->setAlias( alias );
    m_newAccount->access( pincode );

    AccountManager::self()->addAccount( m_newAccount );
    KAssistantDialog::accept();
}

void AddAccountWizard::reject()
{
    delete m_newAccount;
    m_newAccount = 0;
    KAssistantDialog::reject();
}
