#include "mainwindow.h"
#include "composerwidget.h"
#include "navbuttonswidget.h"
#include "timelinewidget.h"
#include "typeswrapper.h"

#include <account.h>
#include <accountmanager.h>
#include <pluginmanager.h>

#include <microblog.h>
#include <types.h>

#include <KPushButton>
#include <KIO/Job>
#include <KIO/StoredTransferJob>

#include <KApplication>
#include <KAction>
#include <KActionCollection>
#include <KSettings/Dialog>
#include <KStandardAction>
#include <KStatusNotifierItem>

#include <KDebug>

#include <QButtonGroup>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QStackedLayout>
#include <QVBoxLayout>

#include <QTimer>

static QString timeline2String( Zzzz::MicroBlog::Timeline t )
{
    switch ( t ) {
        case Zzzz::MicroBlog::Home:
            return QString( "__HOME__" );
        case Zzzz::MicroBlog::Public:
            return QString( "__PUBLIC__" );
        case Zzzz::MicroBlog::User:
            return QString( "__USER__" );
        case Zzzz::MicroBlog::Mentions:
            return QString( "__MENTIONS__" );
        default:
            return QString();
    }
}

static Zzzz::MicroBlog::Timeline string2Timeline( const QString& s )
{
    if ( s == "__HOME__" )
        return Zzzz::MicroBlog::Home;
    if ( s == "__PUBLIC__" )
        return Zzzz::MicroBlog::Public;
    if ( s == "__USER__" )
        return Zzzz::MicroBlog::User;
    if ( s == "__MENTIONS__" )
        return Zzzz::MicroBlog::Mentions;
    return Zzzz::MicroBlog::UserMin;
}

MainWindow::MainWindow()
: KXmlGuiWindow()
{
    m_settingsDialog = 0;

    QWidget* mainWidget = new QWidget;
    setCentralWidget( mainWidget );
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setMargin( 0 );
    mainLayout->setSpacing( 0 );
    mainWidget->setLayout( mainLayout );

    m_buttonsWidget = new NavButtonsWidget;
    mainLayout->addWidget( m_buttonsWidget );
    m_stackedLayout = new QStackedLayout;
    m_stackedLayout->setMargin( 0 );
    m_stackedLayout->setSpacing( 0 );
    mainLayout->addLayout( m_stackedLayout );

    m_composerWidget = new ComposerWidget;
    mainLayout->addWidget( m_composerWidget );
    connect( m_composerWidget, SIGNAL(postComposed(const PostWrapper*)),
             this, SLOT(createPost(const PostWrapper*)) );

    createTimelineWidget( "__HOME__", "user-home" );
    createTimelineWidget( "__PUBLIC__", "applications-internet" );
    createTimelineWidget( "__MENTIONS__", "face-angel" );

    connect( m_buttonsWidget, SIGNAL(timelineClicked(const QString&)),
             this, SLOT(setCurrentTimeline(const QString&)) );

    m_buttonsWidget->clickButton( "__HOME__" );

    setupActions();

    setupGUI();

    KStatusNotifierItem* tray = new KStatusNotifierItem( this );
    tray->setIconByName( "face-angel" );
    tray->setTitle( i18n( "Zzzz" ) );
    tray->setToolTipIconByName( "face-angel" );
    tray->setToolTipTitle( i18n( "Zzzz" ) );
    tray->setToolTipSubTitle( i18n( "KDE MicroBlog Client" ) );
    tray->setCategory( KStatusNotifierItem::ApplicationStatus );
    tray->setStatus( KStatusNotifierItem::Active );

    /// lazy initialization
    PluginManager::self()->loadMicroBlogPlugin();
//     QTimer::singleShot( 0, this, SLOT(loadMicroBlogPlugins()) );
}

MainWindow::~MainWindow()
{
}

void MainWindow::setCurrentTimeline( const QString& timelineName )
{
    m_stackedLayout->setCurrentWidget( m_timelineWidget[ timelineName ] );
}

void MainWindow::slotConfigure()
{
    if ( !m_settingsDialog ) {
        m_settingsDialog = new KSettings::Dialog( this );
        m_settingsDialog->resize( 640, 480 );
    }
    m_settingsDialog->show();
}

void MainWindow::updateTimelines()
{
    updateTimeline( "__HOME__" );
    updateTimeline( "__PUBLIC__" );
    updateTimeline( "__MENTIONS__" );

        /// update custom timelines
        /*
        QStringList timelines = microblog->timelines();
        QStringList::ConstIterator timelineit = timelines.constBegin();
        QStringList::ConstIterator timelineend = timelines.constEnd();
        while ( timelineit != timelineend ) {
            QString timeline = *timelineit;
            ++timelineit;
            /// update timeline using microblog for account
            microblog->updateTimeline( timeline, apiUrl, params );

            QByteArray hs = account->createParametersString( apiUrl, QOAuth::GET, params, QOAuth::ParseForHeaderArguments );
            KIO::StoredTransferJob* job = KIO::storedGet( apiUrl, KIO::Reload, KIO::HideProgressInfo );
            job->addMetaData( "customHTTPHeader", "Authorization: " + hs );
            connect( job, SIGNAL(result(KJob*)), this, SLOT(slotUpdateTimeline(KJob*)) );
            m_jobAccount[ job ] = account;
            m_jobTimeline[ job ] = timeline;
            job->start();
        }
        */
}

void MainWindow::updateTimeline( const QString& timelineName )
{
    const QHash<QString, Account*>& accounts = AccountManager::self()->accounts();
    QHash<QString, Account*>::ConstIterator it = accounts.constBegin();
    QHash<QString, Account*>::ConstIterator end = accounts.constEnd();
    while ( it != end ) {
        Account* account = it.value();
        ++it;
        if ( !account->isAuthorized() )
            continue;

        Zzzz::MicroBlog* microblog = account->microblog();

        QString apiUrl;
        Zzzz::MicroBlog::ParamMap params;
        /// update timeline using microblog for account
        microblog->updateTimeline( string2Timeline( timelineName ), apiUrl, params );

        KUrl url( apiUrl );
        QByteArray hs = account->createParametersString( apiUrl, Zzzz::MicroBlog::GET, params );
        url.setQuery( hs );

        KIO::StoredTransferJob* job = KIO::storedGet( url, KIO::Reload, KIO::HideProgressInfo );
        job->addMetaData( "content-type", "Content-Type: application/x-www-form-urlencoded" );
        connect( job, SIGNAL(result(KJob*)), this, SLOT(slotUpdateTimeline(KJob*)) );
        m_jobAccount[ job ] = account;
        m_jobTimeline[ job ] = timelineName + " zzzz";
        job->start();
    }
}

void MainWindow::updateUserTimeline( const PostWrapper* post )
{
    Account* account = post->myAccount;
    if ( !account || !account->isAuthorized() ) {
        return;
    }

    Zzzz::MicroBlog* microblog = account->microblog();

    QString apiUrl;
    Zzzz::MicroBlog::ParamMap params;
    /// update user timeline using microblog for account
    microblog->updateUserTimeline( apiUrl, params, post->m_post.user );

    KUrl url( apiUrl );
    QByteArray hs = account->createParametersString( apiUrl, Zzzz::MicroBlog::GET, params );
    url.setQuery( hs );

    KIO::StoredTransferJob* job = KIO::storedGet( url, KIO::Reload, KIO::HideProgressInfo );
    job->addMetaData( "content-type", "Content-Type: application/x-www-form-urlencoded" );
    connect( job, SIGNAL(result(KJob*)), this, SLOT(slotUpdateTimeline(KJob*)) );
    m_jobAccount[ job ] = account;
    m_jobTimeline[ job ] = post->m_post.user.screenName + ' ' + post->m_post.user.profileImageUrl;
    job->start();
}

void MainWindow::updateUserTimeline( const PostWrapper* post, const QString& username )
{
    Account* account = post->myAccount;
    if ( !account || !account->isAuthorized() ) {
        return;
    }

    Zzzz::MicroBlog* microblog = account->microblog();

    QString apiUrl;
    Zzzz::MicroBlog::ParamMap params;
    /// update user timeline using microblog for account
    Zzzz::User user;
    user.name = username;
    user.screenName = username;
    microblog->updateUserTimeline( apiUrl, params, user );

    KUrl url( apiUrl );
    QByteArray hs = account->createParametersString( apiUrl, Zzzz::MicroBlog::GET, params );
    url.setQuery( hs );

    KIO::StoredTransferJob* job = KIO::storedGet( url, KIO::Reload, KIO::HideProgressInfo );
    job->addMetaData( "content-type", "Content-Type: application/x-www-form-urlencoded" );
    connect( job, SIGNAL(result(KJob*)), this, SLOT(slotUpdateTimeline(KJob*)) );
    m_jobAccount[ job ] = account;
    m_jobTimeline[ job ] = user.screenName + " zzzz_fetch";
    job->start();
}

void MainWindow::slotUpdateTimeline( KJob* job )
{
    if ( job->error() ) {
        qWarning() << "Job Error: " << job->errorString();
        return;
    }

    Account* account = m_jobAccount.take( job );
    QString t = m_jobTimeline.take( job );
    QString timeline = t.split( ' ' ).at( 0 );
    QString iconName = t.split( ' ' ).at( 1 );
    KIO::StoredTransferJob* j = static_cast<KIO::StoredTransferJob*>(job);
//     qWarning() << QString::fromUtf8( j->data() );
    Zzzz::MicroBlog* microblog = account->microblog();
    QList<Zzzz::Post> postlist;
    bool ok;
    microblog->readTimelineFromData( j->data(), postlist, &ok );
    if ( !ok ) {
        qWarning() << "JSON parsing failed.";
        return;
    }

    TimelineWidget* tw = m_timelineWidget.value( timeline );
    if ( !tw ) {
        /// create user timeline
        if ( iconName == "zzzz_fetch" ) {
            // click from username
            iconName = postlist.first().user.profileImageUrl;
        }
        qWarning() << "create timeline widget" << timeline << iconName;
        createTimelineWidget( timeline, iconName );
        tw = m_timelineWidget.value( timeline );
    }

    QList<Zzzz::Post>::ConstIterator it = postlist.constBegin();
    QList<Zzzz::Post>::ConstIterator end = postlist.constEnd();
    while ( it != end ) {
        Zzzz::Post p = *it;
        PostWrapper* post = new PostWrapper( p );
        post->myAccount = account;
        /// update widget
//         qWarning() << "got new post";
        tw->appendPost( post );
        ++it;
    }
    tw->updateHTML();
}

void MainWindow::createPost( const PostWrapper* post )
{
    Account* account = post->myAccount;
    if ( account ) {
        /// use the specified account for posting
        if ( !account->isAuthorized() )
            return;
        Zzzz::MicroBlog* microblog = account->microblog();
        /// create post for microblog service
        QString apiUrl;
        Zzzz::MicroBlog::ParamMap params;
        microblog->createPost( post->m_post, apiUrl, params );

        KUrl url( apiUrl );
        QByteArray rc = account->createParametersString( apiUrl, Zzzz::MicroBlog::POST, params );

        KIO::StoredTransferJob* job = KIO::storedHttpPost( rc, url, KIO::HideProgressInfo );
        job->addMetaData( "content-type", "Content-Type: application/x-www-form-urlencoded" );
        m_jobAccount[ job ] = account;
        m_jobPost[ job ] = post;
        int& refs = m_jobPostRefs[ post ];
        refs++;
        connect( job, SIGNAL(result(KJob*)), this, SLOT(slotCreatePost(KJob*)) );
        job->start();
    }
    else {
        /// general quicktwit for all accounts
        const QHash<QString, Account*>& accounts = AccountManager::self()->accounts();
        QHash<QString, Account*>::ConstIterator it = accounts.constBegin();
        QHash<QString, Account*>::ConstIterator end = accounts.constEnd();
        while ( it != end ) {
            Account* account = it.value();
            ++it;
            if ( !account->isAuthorized() )
                continue;

            Zzzz::MicroBlog* microblog = account->microblog();
            /// create post for microblog service
            QString apiUrl;
            Zzzz::MicroBlog::ParamMap params;
            microblog->createPost( post->m_post, apiUrl, params );

            KUrl url( apiUrl );
            QByteArray rc = account->createParametersString( apiUrl, Zzzz::MicroBlog::POST, params );

            KIO::StoredTransferJob* job = KIO::storedHttpPost( rc, url, KIO::HideProgressInfo );
            job->addMetaData( "content-type", "Content-Type: application/x-www-form-urlencoded" );
            m_jobAccount[ job ] = account;
            m_jobPost[ job ] = post;
            int& refs = m_jobPostRefs[ post ];
            refs++;
            connect( job, SIGNAL(result(KJob*)), this, SLOT(slotCreatePost(KJob*)) );
            job->start();
        }
    }
}

void MainWindow::slotCreatePost( KJob* job )
{
    if ( job->error() ) {
        qWarning() << "Job Error: " << job->errorString();
        return;
    }

    Account* account = m_jobAccount.take( job );
    const PostWrapper* refPost = m_jobPost.take( job );
    KIO::StoredTransferJob* j = static_cast<KIO::StoredTransferJob*>(job);
    qWarning() << QString::fromUtf8( j->data() );
    Zzzz::MicroBlog* microblog = account->microblog();

    Zzzz::Post p = refPost->m_post;

    if ( m_jobPostRefs.contains( refPost ) ) {
        int& refs = m_jobPostRefs[ refPost ];
        refs--;
        if (refs == 0) {
            m_jobPostRefs.remove( refPost );
            delete refPost;
        }
    }

    bool ok;
    microblog->readPostFromData( j->data(), p, &ok );
    if ( !ok ) {
        qWarning() << "JSON parsing failed.";
        return;
    }

    PostWrapper* post = new PostWrapper( p );
    post->myAccount = account;
    /// update widget
    qWarning() << "created new post";
    TimelineWidget* tw = m_timelineWidget.value( "__HOME__" );
    tw->prependPost( post );
    tw->updateHTML();
}

void MainWindow::retweetPost( const PostWrapper* post )
{
    Account* account = post->myAccount;
    /// use the specified account for posting
    if ( !account->isAuthorized() )
        return;

    Zzzz::MicroBlog* microblog = account->microblog();
    /// retweet post for microblog service
    QString apiUrl;
    Zzzz::MicroBlog::ParamMap params;
    microblog->retweetPost( post->m_post, apiUrl, params );

    QByteArray rc = account->createParametersString( apiUrl, Zzzz::MicroBlog::POST, params );

    KIO::StoredTransferJob* job = KIO::storedHttpPost( rc, apiUrl, KIO::HideProgressInfo );
    job->addMetaData( "content-type", "Content-Type: application/x-www-form-urlencoded" );
    m_jobAccount[ job ] = account;
    m_jobPost[ job ] = post;
    connect( job, SIGNAL(result(KJob*)), this, SLOT(slotCreatePost(KJob*)) );
    job->start();
}

void MainWindow::setupActions()
{
    KStandardAction::quit( kapp, SLOT(quit()), actionCollection() );
    KStandardAction::preferences( this, SLOT(slotConfigure()), actionCollection() );

    KAction* actUpdate = new KAction( KIcon( "view-refresh" ), i18n( "Update Timelines" ), this );
    actionCollection()->addAction( QLatin1String( "update_timeline" ), actUpdate );
    connect( actUpdate, SIGNAL(triggered(bool)), this, SLOT(updateTimelines()) );
}

void MainWindow::createTimelineWidget( const QString& timelineName, const QString& iconName )
{
    TimelineWidget* tw = new TimelineWidget;
    m_timelineWidget[ timelineName ] = tw;
    connect( tw, SIGNAL(userClicked(const PostWrapper*)),
             this, SLOT(updateUserTimeline(const PostWrapper*)) );
    connect( tw, SIGNAL(replyClicked(const PostWrapper*)),
             m_composerWidget, SLOT(composeReply(const PostWrapper*)) );
    connect( tw, SIGNAL(retweetClicked(const PostWrapper*)),
             this, SLOT(retweetPost(const PostWrapper*)) );
    connect( tw, SIGNAL(usernameClicked(const PostWrapper*,const QString&)),
             this, SLOT(updateUserTimeline(const PostWrapper*,const QString&)) );

    m_stackedLayout->addWidget( tw );
    m_buttonsWidget->addButton( timelineName, iconName );
}
