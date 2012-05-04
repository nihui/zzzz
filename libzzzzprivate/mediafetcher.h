#ifndef MEDIAFETCHER_H
#define MEDIAFETCHER_H

#include "zzzzprivate_export.h"

#include <QPixmap>
#include <QObject>
#include <QSet>
#include <QUrl>
#include <KImageCache>

class KJob;

class ZZZZPRIVATE_EXPORT MediaFetcher : public QObject
{
    Q_OBJECT
public:
    static MediaFetcher* self();
    ~MediaFetcher();
    bool findPixmap(const QUrl& url, QPixmap* pixmap);
Q_SIGNALS:
    void gotPixmap(const QUrl& url, const QPixmap& pixmap);
    void errorPixmap(const QUrl& url);
private Q_SLOTS:
    void slotResult(KJob* job);
private:
    explicit MediaFetcher();
    static MediaFetcher* m_self;

    QSet<QUrl> m_ontheway;
    QHash<KJob*, QUrl> m_jobUrl;
    KImageCache* m_cache;
};

#endif // MEDIAFETCHER_H
