#ifndef ACCOUNTCONFIG_H
#define ACCOUNTCONFIG_H

#include <KCModule>

class QListWidget;
class KAction;
class Account;

class ZzzzAccountConfig : public KCModule
{
    Q_OBJECT
public:
    explicit ZzzzAccountConfig(QWidget* parent, const QVariantList& args);
    virtual ~ZzzzAccountConfig();
public Q_SLOTS:
    virtual void load();
    virtual void save();
    virtual void defaults();
private Q_SLOTS:
    void slotAddAccount();
//         void slotModifyAccount();
    void slotRemoveAccount();
    void addAccountItem(const QString& alias, const Account* newAccount);
    void removeAccountItem(const QString& alias, const Account* oldAccount);
private:
    QListWidget* m_accountListView;
    KAction* m_addAccountAction;
//         KAction* m_modifyAccountAction;
    KAction* m_removeAccountAction;
};

#endif // ACCOUNTCONFIG_H
