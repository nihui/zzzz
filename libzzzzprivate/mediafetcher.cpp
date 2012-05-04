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
    // 20MB
    m_cache = new KImageCache("zzzz-media", 20*1024*1024);
    m_cache->setEvictionPolicy(KSharedDataCache::EvictLeastRecentlyUsed);
    m_cache->setPixmapCaching(true);
}

MediaFetcher::~MediaFetcher()
{
    delete m_cache;
}

bool MediaFetcher::findPixmap(const QUrl& url, QPixmap* pixmap)
{
    if (m_cache->findPixmap(url.toString(), pixmap)) {
        // hit cache
        return true;
    }

    // request the missing image
    if (m_ontheway.contains(url)) {
        // already on the way
        return false;
    }

    kWarning() << url;
    // fetch from remote
    m_ontheway.insert(url);

    KIO::Job* job = KIO::storedGet(url, KIO::NoReload, KIO::HideProgressInfo);
    m_jobUrl[ job ] = url;
    connect(job, SIGNAL(result(KJob*)), this, SLOT(slotResult(KJob*)));
    job->start();
    return false;
}

void MediaFetcher::slotResult(KJob* job)
{
    QUrl url = m_jobUrl.take(job);

    if (job->error()) {
        kWarning() << "Job Error: " << job->errorString();
        emit errorPixmap(url);
        return;
    }

    KIO::StoredTransferJob* j = static_cast<KIO::StoredTransferJob*>(job);

    QPixmap pixmap;
    pixmap.loadFromData(j->data());
    m_cache->insertPixmap(url.toString(), pixmap);

    m_ontheway.remove(url);

    emit gotPixmap(url, pixmap);
}
