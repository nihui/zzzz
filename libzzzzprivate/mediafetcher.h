#ifndef MEDIAFETCHER_H
#define MEDIAFETCHER_H

#include "zzzzprivate_export.h"

#include <QImage>
#include <QObject>
#include <QSet>
#include <QUrl>

class KJob;

class ZZZZPRIVATE_EXPORT MediaFetcher : public QObject
{
    Q_OBJECT
    public:
        static MediaFetcher* self();
        ~MediaFetcher();
        void requestImage(const QUrl& url);
    Q_SIGNALS:
        void gotImage(const QUrl& url);
        void errorImage(const QUrl& url);
    private Q_SLOTS:
        void slotImageFetched(KJob* job);
    private:
        explicit MediaFetcher();
        static MediaFetcher* m_self;

        QSet<QUrl> m_ontheway;
        QHash<KJob*, QUrl> m_jobUrl;
};

#endif // MEDIAFETCHER_H
