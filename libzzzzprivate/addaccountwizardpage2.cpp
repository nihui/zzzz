#include "addaccountwizardpage2.h"

#include <KLineEdit>
#include <KLocale>
#include <KPushButton>

#include <QFormLayout>
#include <QVBoxLayout>

AddAccountWizardPage2::AddAccountWizardPage2(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    setLayout(mainLayout);

    QFormLayout* formLayout = new QFormLayout;
    formLayout->setMargin(0);
    mainLayout->addLayout(formLayout);

    m_aliasEdit = new KLineEdit;
    m_pinEdit = new KLineEdit;
    formLayout->addRow(i18n("Alias:"), m_aliasEdit);
    formLayout->addRow(i18n("PIN:"), m_pinEdit);
}

AddAccountWizardPage2::~AddAccountWizardPage2()
{
}

QString AddAccountWizardPage2::alias() const
{
    return m_aliasEdit->text();
}

QString AddAccountWizardPage2::pincode() const
{
    return m_pinEdit->text();
}
