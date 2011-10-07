#include "mainwindow.h"
#include "composerwidget.h"
#include "navbuttonswidget.h"
#include "timelinewidget.h"
#include "typeswrapper.h"

#include <account.h>
#include <accountmanager.h>

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

#include <KDebug>

#include <QButtonGroup>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QStackedLayout>
#include <QVBoxLayout>

#include <QTimer>

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

    createTimelineWidget( Zzzz::MicroBlog::Home, "user-home" );
    createTimelineWidget( Zzzz::MicroBlog::Public, "applications-internet" );
    createTimelineWidget( Zzzz::MicroBlog::User, "user-identity" );
    createTimelineWidget( Zzzz::MicroBlog::Mentions, "face-angel" );

    connect( m_buttonsWidget, SIGNAL(currentIndexChanged(int)),
             m_stackedLayout, SLOT(setCurrentIndex(int)) );

    setupActions();

    setupGUI();

    /// lazy initialization
//     QTimer::singleShot( 0, this, SLOT(loadMicroBlogPlugins()) );
}

MainWindow::~MainWindow()
{
    delete m_settingsDialog;
}

void MainWindow::slotConfigure()
{
    if ( !m_settingsDialog )
        m_settingsDialog = new KSettings::Dialog;
    m_settingsDialog->show();
}

void MainWindow::updateTimelines()
{
    updateTimeline( Zzzz::MicroBlog::Home );
    updateTimeline( Zzzz::MicroBlog::Public );
//     updateTimeline( Zzzz::MicroBlog::User );
    updateTimeline( Zzzz::MicroBlog::Mentions );
//     QList<Zzzz::TimelineType> timelines;
//     timelines << Zzzz::Home << Zzzz::Public;
// 
//     QList<Zzzz::TimelineType>::ConstIterator timelineit = timelines.constBegin();
//     QList<Zzzz::TimelineType>::ConstIterator timelineend = timelines.constEnd();
//     while ( timelineit != timelineend ) {
//         Zzzz::TimelineType timeline = *timelineit;
//         updateTimeline( timeline );
//     }

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

void MainWindow::updateTimeline( Zzzz::MicroBlog::Timeline t )
{
    TimelineWidget* tw = m_timelineWidget.value( t );
    tw->clearPosts();

    QList<Account*> accounts = AccountManager::self()->accounts();
    QList<Account*>::ConstIterator it = accounts.constBegin();
    QList<Account*>::ConstIterator end = accounts.constEnd();
    while ( it != end ) {
        Account* account = *it;
        ++it;
        if ( !account->isAuthorized() )
            continue;

        Zzzz::MicroBlog* microblog = account->microblog();

        QString apiUrl;
        Zzzz::MicroBlog::ParamMap params;
        /// update timeline using microblog for account
        microblog->updateTimeline( t, apiUrl, params );

        KUrl url( apiUrl );
        QByteArray hs = account->createParametersString( apiUrl, Zzzz::MicroBlog::GET, params );
        url.setQuery( hs );

        KIO::StoredTransferJob* job = KIO::storedGet( url, KIO::Reload, KIO::HideProgressInfo );
        job->addMetaData( "content-type", "Content-Type: application/x-www-form-urlencoded" );
        connect( job, SIGNAL(result(KJob*)), this, SLOT(slotUpdateTimeline(KJob*)) );
        m_jobAccount[ job ] = account;
        m_jobTimeline[ job ] = t;
        job->start();
    }

}

void MainWindow::slotUpdateTimeline( KJob* job )
{
    if ( job->error() ) {
        qWarning() << "Job Error: " << job->errorString();
        return;
    }

    Account* account = m_jobAccount.take( job );
    Zzzz::MicroBlog::Timeline timeline = m_jobTimeline.take( job );
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
//         m_jobPost[ job ] = post;
        connect( job, SIGNAL(result(KJob*)), this, SLOT(slotCreatePost(KJob*)) );
        job->start();
    }
    else {
        /// general quicktwit for all accounts
        QList<Account*> accounts = AccountManager::self()->accounts();
        QList<Account*>::ConstIterator it = accounts.constBegin();
        QList<Account*>::ConstIterator end = accounts.constEnd();
        while ( it != end ) {
            Account* account = *it;
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
    //         m_jobPost[ job ] = post;
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
//     Zzzz::Post originalPost = m_jobPost.take( job );
    KIO::StoredTransferJob* j = static_cast<KIO::StoredTransferJob*>(job);
    qWarning() << QString::fromUtf8( j->data() );
    Zzzz::MicroBlog* microblog = account->microblog();
    Zzzz::Post p;
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
    TimelineWidget* tw = m_timelineWidget.value( Zzzz::MicroBlog::Home );
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
//         m_jobPost[ job ] = post;
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

void MainWindow::createTimelineWidget( Zzzz::MicroBlog::Timeline t, const QString& iconName )
{
    TimelineWidget* tw = new TimelineWidget;
    m_timelineWidget[ t ] = tw;
    connect( tw, SIGNAL(replyClicked(const PostWrapper*)),
             m_composerWidget, SLOT(composeReply(const PostWrapper*)) );
    connect( tw, SIGNAL(retweetClicked(const PostWrapper*)),
             this, SLOT(retweetPost(const PostWrapper*)) );
    m_stackedLayout->addWidget( tw );
    m_buttonsWidget->addButton( iconName );
}
