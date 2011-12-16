#include "mediafetcher.h"

#include <KDebug>
#include <KIO/Job>
#include <KIO/StoredTransferJob>

MediaFetcher* MediaFetcher::m_self = 0;

MediaFetcher* MediaFetcher::self()
{
    if ( !m_self )
        m_self = new MediaFetcher;
    return m_self;
}

MediaFetcher::MediaFetcher()
{
}

MediaFetcher::~MediaFetcher()
{
}

void MediaFetcher::requestAvatar( const QString& url )
{
    if ( m_avatarCache.contains( url ) ) {
        // hit cache
        emit gotAvatar( url, m_avatarCache.value( url ) );
        return;
    }

    if ( m_ontheway.contains( url ) ) {
        // already on the way
        return;
    }

    // fetch from remote
    m_ontheway.insert( url );

    KIO::Job* job = KIO::storedGet( url, KIO::NoReload, KIO::HideProgressInfo );
    m_jobUrl[ job ] = url;
    connect( job, SIGNAL(result(KJob*)), this, SLOT(slotAvatarFetched(KJob*)) );
    job->start();
}

void MediaFetcher::requestImage( const QString& url )
{
    if ( m_imageCache.contains( url ) ) {
        // hit cache
        QImage* image = m_imageCache.object( url );
        emit gotAvatar( url, *image );
        return;
    }

    if ( m_ontheway.contains( url ) ) {
        // already on the way
        return;
    }

    // fetch from remote
    m_ontheway.insert( url );

    KIO::Job* job = KIO::storedGet( url, KIO::NoReload, KIO::HideProgressInfo );
    m_jobUrl[ job ] = url;
    connect( job, SIGNAL(result(KJob*)), this, SLOT(slotImageFetched(KJob*)) );
    job->start();
}

void MediaFetcher::slotAvatarFetched( KJob* job )
{
    QString url = m_jobUrl.take( job );

    if ( job->error() ) {
        kWarning() << "Job Error: " << job->errorString();
        emit errorAvatar( url );
        return;
    }

    KIO::StoredTransferJob* j = static_cast<KIO::StoredTransferJob*>(job);
    QImage image;
    image.loadFromData( j->data() );

    m_avatarCache.insert( url, image );
    m_ontheway.remove( url );

    emit gotAvatar( url, m_avatarCache.value( url ) );
}

void MediaFetcher::slotImageFetched( KJob* job )
{
    QString url = m_jobUrl.take( job );

    if ( job->error() ) {
        kWarning() << "Job Error: " << job->errorString();
        emit errorImage( url );
        return;
    }

    KIO::StoredTransferJob* j = static_cast<KIO::StoredTransferJob*>(job);
    QImage* image = new QImage;
    image->loadFromData( j->data() );

    m_imageCache.insert( url, image );
    m_ontheway.remove( url );

    emit gotImage( url, *image );
}
