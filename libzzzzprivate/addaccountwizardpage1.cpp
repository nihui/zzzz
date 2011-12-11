#include "addaccountwizardpage1.h"
#include "pluginmanager.h"

#include <QTreeWidget>
#include <QVBoxLayout>

AddAccountWizardPage1::AddAccountWizardPage1( QWidget* parent )
: QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setMargin( 0 );
    mainLayout->setSpacing( 0 );
    setLayout( mainLayout );
    m_treeWidget = new QTreeWidget;
    m_treeWidget->setItemsExpandable( false );
    m_treeWidget->setRootIsDecorated( false );
    m_treeWidget->setExpandsOnDoubleClick( false );
    m_treeWidget->setUniformRowHeights( true );
    m_treeWidget->setHeaderLabels( QStringList() << i18n( "Name" ) << i18n( "Description" ) );
    m_treeWidget->setIconSize( QSize( 32, 32 ) );
    mainLayout->addWidget( m_treeWidget );

    QList<KPluginInfo> microBlogPluginInfos = PluginManager::self()->microBlogPluginInfos();
    QList<KPluginInfo>::ConstIterator it = microBlogPluginInfos.constBegin();
    QList<KPluginInfo>::ConstIterator end = microBlogPluginInfos.constEnd();
    while ( it != end ) {
        KPluginInfo microBlogPluginInfo = *it;
        ++it;

        if ( !microBlogPluginInfo.isPluginEnabled() ) {
            continue;
        }

        QTreeWidgetItem* microBlogPluginItem = new QTreeWidgetItem;
        microBlogPluginItem->setIcon( 0, QIcon::fromTheme( microBlogPluginInfo.icon() ) );
        microBlogPluginItem->setText( 0, microBlogPluginInfo.name() );
        microBlogPluginItem->setText( 1, microBlogPluginInfo.comment() );
        microBlogPluginItem->setData( 0, Qt::UserRole, microBlogPluginInfo.pluginName() );
        m_treeWidget->addTopLevelItem( microBlogPluginItem );
    }
    m_treeWidget->setCurrentItem( m_treeWidget->topLevelItem( 0 ) );
}

AddAccountWizardPage1::~AddAccountWizardPage1()
{
}

QString AddAccountWizardPage1::selectedService() const
{
    QTreeWidgetItem* currentWidgetItem = m_treeWidget->currentItem();
    return currentWidgetItem->data( 0, Qt::UserRole ).toString();
}
