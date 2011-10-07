#include "timelinewidget.h"

#include "typeswrapper.h"

#include <types.h>

#include <khtmlview.h>
#include <khtml_part.h>
#include <khtml_settings.h>
#include <kwebview.h>

#include <KStandardDirs>
#include <KToolInvocation>

#include <KDebug>

#include <QLineEdit>
#include <QResizeEvent>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QWebSettings>

#include <grantlee_core.h>

class ZzzzWebView : public KWebView
{
    public:
        explicit ZzzzWebView( QWidget* parent = 0 ) : KWebView(parent) {}
        virtual QSize sizeHint() const {
            return QSize( 200, 200 );
        }
};

TimelineWidget::TimelineWidget( QWidget* parent )
: QWidget(parent)
{
    setSizePolicy( QSizePolicy::Minimum, QSizePolicy::MinimumExpanding );

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setMargin( 0 );
    setLayout( mainLayout );

    m_kwebview = new ZzzzWebView;
    m_kwebview->settings()->setAttribute( QWebSettings::JavascriptEnabled, false );
    m_kwebview->settings()->setAttribute( QWebSettings::JavaEnabled, false );
    m_kwebview->settings()->setAttribute( QWebSettings::PluginsEnabled, false );
    m_kwebview->setHtml( m_html );
    m_kwebview->page()->setLinkDelegationPolicy( QWebPage::DelegateAllLinks );
    connect( m_kwebview, SIGNAL(linkClicked(const QUrl&)), this, SLOT(slotLinkClicked(const QUrl&)) );

    m_khtmlpart = new KHTMLPart;
    m_khtmlpart->setJavaEnabled( false );
    m_khtmlpart->setJScriptEnabled( false );
    m_khtmlpart->setPluginsEnabled( false );
    m_khtmlpart->view()->setMarginHeight( 0 );
    m_khtmlpart->view()->setMarginWidth( 0 );
    m_khtmlpart->begin();
    m_khtmlpart->write( m_html );
    m_khtmlpart->end();
    connect( m_khtmlpart->browserExtension(), SIGNAL(openUrlRequestDelayed(const KUrl&,const KParts::OpenUrlArguments&,const KParts::BrowserArguments&)),
             this, SLOT(slotOpenUrlRequestDelayed(const KUrl&)) );

    mainLayout->addWidget( m_kwebview );
//     mainLayout->addWidget( m_khtmlpart->view() );

    m_engine = new Grantlee::Engine( this );
    Grantlee::FileSystemTemplateLoader::Ptr loader( new Grantlee::FileSystemTemplateLoader() );

    KStandardDirs ksd;
    QStringList templateDirs = ksd.findDirs( "data", "zzzz/themes" );
kWarning() << templateDirs;
    loader->setTemplateDirs( templateDirs );
    m_engine->addTemplateLoader( loader );
//     m_engine->setPluginPaths( QStringList() << GRANTLEE_PLUGIN_PATH );
}

TimelineWidget::~TimelineWidget()
{
    clearPosts();
}

void TimelineWidget::appendPost( const PostWrapper* post )
{
    m_posts.append( post );
}

void TimelineWidget::prependPost( const PostWrapper* post )
{
    m_posts.prepend( post );
}

void TimelineWidget::clearPosts()
{
    qDeleteAll( m_posts );
    m_posts.clear();
}

void TimelineWidget::updateHTML()
{
// kWarning();
    QVariantHash mapping;
    QVariantList postList;

    QList<const PostWrapper*>::ConstIterator it = m_posts.constBegin();
    QList<const PostWrapper*>::ConstIterator end = m_posts.constEnd();
    int i = 0;
    while ( it != end ) {
        const PostWrapper* post = *it;

        post->m_replyLink = QString( "zzzz:reply:%1" ).arg( i );
        post->m_retweetLink = QString( "zzzz:retweet:%1" ).arg( i );
        ++i;

        ++it;

        const QObject* postWrapper = post;
        postList.append( QVariant::fromValue( const_cast<QObject*>(postWrapper) ) );
//         kWarning() << p.user.profileImageUrl;
    }

    mapping.insert( "posts", postList );

    Grantlee::Context c( mapping );
    Grantlee::Template t = m_engine->loadByName( "simple.html" );

    m_html = t->render( &c );

    m_kwebview->setHtml( m_html );

    m_khtmlpart->begin();
    m_khtmlpart->write( m_html );
    m_khtmlpart->end();
}

void TimelineWidget::slotLinkClicked( const QUrl& url )
{
    handleUrlString( url.toString() );
}

void TimelineWidget::slotOpenUrlRequestDelayed( const KUrl& url )
{
    handleUrlString( url.url() );
}

void TimelineWidget::handleUrlString( const QString& url )
{
    if ( url.isEmpty() )
        return;

    kWarning() << url;
    if ( url.startsWith( "zzzz:reply" ) ) {
        int i = url.section( ':', 2, -1 ).toInt();
        emit replyClicked( m_posts.at( i ) );
    }
    else if ( url.startsWith( "zzzz:retweet" ) ) {
        int i = url.section( ':', 2, -1 ).toInt();
        emit retweetClicked( m_posts.at( i ) );
    }
    else {
        KToolInvocation::invokeBrowser( url );
    }
}
