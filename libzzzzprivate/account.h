#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "zzzzprivate_export.h"

#include <microblog.h>
#include <types.h>

#include <QtOAuth/QtOAuth>

#include <QObject>

namespace Zzzz { class MicroBlog; }

class ZZZZPRIVATE_EXPORT Account : public QObject
{
    Q_OBJECT
    public:
        explicit Account( Zzzz::MicroBlog* microblog, QObject* parent = 0 );
        virtual ~Account();
        void readConfig();
        void writeConfig();
        Zzzz::MicroBlog* microblog() const;
        bool isAuthorized() const;
        void authorize( bool force = false );
        void access( const QString& pincode );
        QByteArray createParametersString( const QString& requstUrl,
                                           Zzzz::MicroBlog::HttpMethod type,
                                           const Zzzz::MicroBlog::ParamMap& params );

        void setAlias( const QString& alias );
        QString alias() const;
        void setEnabled( bool isEnabled );
        bool isEnabled() const;
    private:
        Zzzz::MicroBlog* m_microblog;
        QByteArray m_oauthToken;
        QByteArray m_oauthTokenSecret;
        QOAuth::Interface* m_qoauth;
        bool m_isAuthorized;
        QString m_alias;
        bool m_isEnabled;
};

#endif // ACCOUNT_H
