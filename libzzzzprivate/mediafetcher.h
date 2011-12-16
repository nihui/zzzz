#ifndef MEDIAFETCHER_H
#define MEDIAFETCHER_H

#include "zzzzprivate_export.h"

#include <QCache>
#include <QHash>
#include <QImage>
#include <QObject>
#include <QSet>

class KJob;

class ZZZZPRIVATE_EXPORT MediaFetcher : public QObject
{
    Q_OBJECT
    public:
        static MediaFetcher* self();
        ~MediaFetcher();
        void requestAvatar( const QString& url );
        void requestImage( const QString& url );
    Q_SIGNALS:
        void gotAvatar( const QString& url, const QImage& image );
        void gotImage( const QString& url, const QImage& image );
        void errorAvatar( const QString& url );
        void errorImage( const QString& url );
    private Q_SLOTS:
        void slotAvatarFetched( KJob* job );
        void slotImageFetched( KJob* job );
    private:
        explicit MediaFetcher();
        static MediaFetcher* m_self;

        QSet<QString> m_ontheway;
        QHash<KJob*, QString> m_jobUrl;

        QHash<QString, QImage> m_avatarCache;
        QCache<QString, QImage> m_imageCache;
};

#endif // MEDIAFETCHER_H
