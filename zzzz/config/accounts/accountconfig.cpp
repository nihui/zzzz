#include "accountconfig.h"

#include "account.h"
#include "accountmanager.h"
#include "addaccountwizard.h"
#include "pluginmanager.h"

#include <KAction>
#include <KPluginFactory>
#include <KPluginInfo>

#include <KPushButton>

#include <QListWidget>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QVBoxLayout>

K_PLUGIN_FACTORY( ZzzzAccountConfigFactory, registerPlugin<ZzzzAccountConfig>(); )
K_EXPORT_PLUGIN( ZzzzAccountConfigFactory("kcm_zzzz_accountconfig") )


ZzzzAccountConfig::ZzzzAccountConfig( QWidget* parent, const QVariantList& args )
: KCModule(ZzzzAccountConfigFactory::componentData(),parent,args)
{
    QHBoxLayout* mainLayout = new QHBoxLayout;
    setLayout( mainLayout );

    m_accountListView = new QListWidget;
    m_accountListView->setIconSize( QSize( 32, 32 ) );
    mainLayout->addWidget( m_accountListView );

    QVBoxLayout* buttonLayout = new QVBoxLayout;
    mainLayout->addLayout( buttonLayout );

    /// add account
    m_addAccountAction = new KAction( i18n( "&Add Account..." ), this );
    m_addAccountAction->setIcon( KIcon( "list-add" ) );
    KPushButton* addAccountButton = new KPushButton;
    addAccountButton->setText( m_addAccountAction->text() );
    addAccountButton->setIcon( KIcon( "list-add" ) );
    connect( m_addAccountAction, SIGNAL(triggered(bool)), this, SLOT(slotAddAccount()) );
    connect( addAccountButton, SIGNAL(clicked()), this, SLOT(slotAddAccount()) );
    buttonLayout->addWidget( addAccountButton );

//     /// modify account
//     m_modifyAccountAction = new KAction( i18n( "&Modify Account..." ), this );
//     m_modifyAccountAction->setIcon( KIcon( "configure" ) );
//     KPushButton* modifyAccountButton = new KPushButton;
//     modifyAccountButton->setText( m_modifyAccountAction->text() );
//     modifyAccountButton->setIcon( KIcon( "configure" ) );
//     connect( m_modifyAccountAction, SIGNAL(triggered(bool)), this, SLOT(slotModifyAccount()) );
//     connect( modifyAccountButton, SIGNAL(clicked()), this, SLOT(slotModifyAccount()) );
//     buttonLayout->addWidget( modifyAccountButton );

    /// remove account
    m_removeAccountAction = new KAction( i18n( "&Remove Account" ), this );
    m_removeAccountAction->setIcon( KIcon( "edit-delete" ) );
    KPushButton* removeAccountButton = new KPushButton;
    removeAccountButton->setText( m_removeAccountAction->text() );
    removeAccountButton->setIcon( KIcon( "edit-delete" ) );
    connect( m_removeAccountAction, SIGNAL(triggered(bool)), this, SLOT(slotRemoveAccount()) );
    connect( removeAccountButton, SIGNAL(clicked()), this, SLOT(slotRemoveAccount()) );
    buttonLayout->addWidget( removeAccountButton );

    connect( AccountManager::self(), SIGNAL(accountAdded(const QString&, const Account*)),
             this, SLOT(addAccountItem(const QString&, const Account*)) );
    connect( AccountManager::self(), SIGNAL(accountRemoved(const QString&, const Account*)),
             this, SLOT(removeAccountItem(const QString&, const Account*)) );
}

ZzzzAccountConfig::~ZzzzAccountConfig()
{
    AccountManager::self()->saveAccounts();
}

void ZzzzAccountConfig::load()
{
    m_accountListView->clear();
    const QHash<QString, Account*>& accounts = AccountManager::self()->accounts();
    QHash<QString, Account*>::ConstIterator it = accounts.constBegin();
    QHash<QString, Account*>::ConstIterator end = accounts.constEnd();
    while ( it != end ) {
        addAccountItem( it.key(), it.value() );
        ++it;
    }
}

void ZzzzAccountConfig::save()
{
}

void ZzzzAccountConfig::defaults()
{
}

void ZzzzAccountConfig::slotAddAccount()
{
    AddAccountWizard* addAccountWizard = new AddAccountWizard( this );
    addAccountWizard->show();
}

// void ZzzzAccountConfig::slotModifyAccount()
// {
// }

void ZzzzAccountConfig::slotRemoveAccount()
{
    QListWidgetItem* currentAccountItem = m_accountListView->currentItem();
    if ( !currentAccountItem )
        return;

    QString accountAlias = currentAccountItem->text();
    AccountManager::self()->removeAccount( accountAlias );
}

void ZzzzAccountConfig::addAccountItem( const QString& alias, const Account* newAccount )
{
    /// add account item widget
    bool accountEnabled = newAccount->isEnabled();
    Zzzz::MicroBlog* microblog = newAccount->microblog();
    KPluginInfo pluginInfo = PluginManager::self()->microBlogPluginInfo( microblog );
    QListWidgetItem* accountItem = new QListWidgetItem;
    accountItem->setText( alias );
    accountItem->setIcon( QIcon::fromTheme( pluginInfo.icon() ) );
    m_accountListView->addItem( accountItem );
}

void ZzzzAccountConfig::removeAccountItem( const QString& alias, const Account* oldAccount )
{
    /// remove account item widget
    QList<QListWidgetItem*> items = m_accountListView->findItems( alias, Qt::MatchFixedString | Qt::MatchCaseSensitive );
    foreach ( QListWidgetItem* accountItem, items ) {
        m_accountListView->removeItemWidget( accountItem );
        delete accountItem;
    }
}
