#ifndef ADDACCOUNTWIZARDPAGE2_H
#define ADDACCOUNTWIZARDPAGE2_H

#include <QWidget>

class KLineEdit;

class AddAccountWizardPage2 : public QWidget
{
    public:
        explicit AddAccountWizardPage2( QWidget* parent = 0 );
        virtual ~AddAccountWizardPage2();
        QString alias() const;
        QString pincode() const;
    private:
        KLineEdit* m_aliasEdit;
        KLineEdit* m_pinEdit;
};

#endif // ADDACCOUNTWIZARDPAGE2_H
