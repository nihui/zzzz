#include "accountmanager.h"
#include "account.h"
#include "pluginmanager.h"

#include <KConfigGroup>
#include <KDebug>
#include <KGlobal>
#include <KSharedConfig>

AccountManager* AccountManager::m_self = 0;

AccountManager* AccountManager::self()
{
    if ( !m_self )
        m_self = new AccountManager;
    return m_self;
}

AccountManager::AccountManager()
{
//     loadAccounts();
}

AccountManager::~AccountManager()
{
}

Account* AccountManager::account( const QString& alias ) const
{
    return m_accounts.value( alias );
}

QList<Account*> AccountManager::accounts() const
{
    return m_accounts.values();
}

void AccountManager::loadAccounts()
{
    qWarning() << "AccountManager::loadAccounts";

    m_accounts.clear();

    KSharedConfig::Ptr conf = KGlobal::config();
    const QStringList accountGroups = conf->groupList().filter( QRegExp( "^Account_" ) );
    QStringList::ConstIterator it = accountGroups.constBegin();
    QStringList::ConstIterator end = accountGroups.constEnd();
    while ( it != end ) {
        KConfigGroup configGroup( conf, *it );
        ++it;
        /// load account
        QString microblogName = configGroup.readEntry( "MicroBlog", QString() );
        if ( microblogName.isEmpty() ) {
            qWarning() << "MicroBlog entry empty";
            continue;
        }
        KPluginInfo pluginInfo = PluginManager::self()->microBlogPluginInfo( microblogName );
        if ( !pluginInfo.isValid() || !pluginInfo.isPluginEnabled() ) {
            qWarning() << "No such plugininfo for" << microblogName;
            continue;
        }
        Zzzz::MicroBlog* microblog = PluginManager::self()->microBlog( microblogName );
        if ( !microblog ) {
            qWarning() << "No such plugin for" << microblogName;
            continue;
        }
        /// try to load microblog plugin with alias
        Account* account = new Account( microblog );
        QString alias = configGroup.readEntry( "Alias", QString() );
        account->setAlias( alias );
        account->readConfig();
        m_accounts.insert( alias, account );
    }

    emit accountChanged();
}

void AccountManager::saveAccounts()
{
    QHash<QString, Account*>::ConstIterator it = m_accounts.constBegin();
    QHash<QString, Account*>::ConstIterator end = m_accounts.constEnd();
    while ( it != end ) {
        Account* account = it.value();
        account->writeConfig();
        ++it;
    }
}

void AccountManager::addAccount( Account* newAccount )
{
    qWarning() << "AccountManager::addAccount" << newAccount->alias();
    m_accounts.insert( newAccount->alias(), newAccount );
    emit accountChanged();
}

void AccountManager::removeAccount( const QString& alias )
{
    qWarning() << "AccountManager::removeAccount" << alias;
    KSharedConfig::Ptr conf = KGlobal::config();
    conf->deleteGroup( QString( "Account_%1" ).arg( alias ) );
    conf->sync();
    Account* account = m_accounts.take( alias );
    delete account;
    emit accountChanged();
}
