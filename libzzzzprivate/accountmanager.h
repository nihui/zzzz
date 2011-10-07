#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include "zzzzprivate_export.h"

#include <QObject>
#include <QHash>
class Account;

class ZZZZPRIVATE_EXPORT AccountManager : public QObject
{
    Q_OBJECT
    public:
        static AccountManager* self();
        ~AccountManager();
        Account* account( const QString& alias ) const;
        QList<Account*> accounts() const;
        void loadAccounts();
        void saveAccounts();
        void addAccount( Account* newAccount );
        void removeAccount( const QString& alias );
    Q_SIGNALS:
        void accountChanged();
    private:
        explicit AccountManager();
        static AccountManager* m_self;
        QHash<QString, Account*> m_accounts;
};

#endif // ACCOUNTMANAGER_H
