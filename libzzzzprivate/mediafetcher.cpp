#include "mediafetcher.h"

#include <QPixmapCache>
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

void MediaFetcher::requestImage(const QUrl& url)
{
    QPixmap pixmap;
    if (QPixmapCache::find(url.toString(), &pixmap)) {
        // already in cache
        emit gotImage(url);
        return;
    }

    if (m_ontheway.contains(url)) {
        // already on the way
        return;
    }

    kWarning() << url;
    // fetch from remote
    m_ontheway.insert(url);

    KIO::Job* job = KIO::storedGet(url, KIO::NoReload, KIO::HideProgressInfo);
    m_jobUrl[ job ] = url;
    connect(job, SIGNAL(result(KJob*)), this, SLOT(slotImageFetched(KJob*)));
    job->start();
}

void MediaFetcher::slotImageFetched(KJob* job)
{
    QUrl url = m_jobUrl.take(job);

    if (job->error()) {
        kWarning() << "Job Error: " << job->errorString();
        emit errorImage(url);
        return;
    }

    KIO::StoredTransferJob* j = static_cast<KIO::StoredTransferJob*>(job);

    QPixmap pixmap;
    pixmap.loadFromData(j->data());
    QPixmapCache::insert(url.toString(), pixmap);

    m_ontheway.remove(url);

    emit gotImage(url);
}
