#include "account.h"

#include "pluginmanager.h"

#include "microblog.h"

#include <KConfigGroup>
#include <KDebug>
#include <KGlobal>
#include <KInputDialog>
#include <KIO/AccessManager>
#include <KLocale>
#include <KMessageBox>
#include <KToolInvocation>

#include <QtOAuth/QtOAuth>

#include <QUrl>

Account::Account( Zzzz::MicroBlog* microblog, QObject* parent )
: QObject(parent), m_microblog(microblog)
{
    /// TODO KDE 4.5 Change to use new class
    m_qoauth = new QOAuth::Interface( new KIO::AccessManager( this ), this );
    m_qoauth->setConsumerKey( microblog->oauthConsumerKey() );
    m_qoauth->setConsumerSecret( microblog->oauthConsumerSecret() );
    m_qoauth->setRequestTimeout( 10000 );
    m_qoauth->setIgnoreSslErrors( true );

    m_isEnabled = true;
    m_isAuthorized = false;
}

Account::~Account()
{
    delete m_qoauth;
}

void Account::readConfig()
{
    if ( m_alias.isEmpty() )
        return;

    KConfigGroup* m_configGroup = new KConfigGroup( KGlobal::config(), QString::fromLatin1( "Account_%1" ).arg( m_alias ) );
    m_isEnabled = m_configGroup->readEntry( "Enabled", false );
    m_isAuthorized = m_configGroup->readEntry( "Authorized", false );
    m_oauthToken = m_configGroup->readEntry( "OAuthToken", QByteArray() );
    m_oauthTokenSecret = m_configGroup->readEntry( "OAuthTokenSecret", QByteArray() );
    delete m_configGroup;
}

void Account::writeConfig()
{
    if ( m_alias.isEmpty() )
        return;

    KConfigGroup* m_configGroup = new KConfigGroup( KGlobal::config(), QString::fromLatin1( "Account_%1" ).arg( m_alias ) );
    m_configGroup->writeEntry( "Alias", m_alias );
    m_configGroup->writeEntry( "MicroBlog", PluginManager::self()->microBlogPluginName( m_microblog ) );
    m_configGroup->writeEntry( "Enabled", m_isEnabled );
    m_configGroup->writeEntry( "Authorized", m_isAuthorized );
    m_configGroup->writeEntry( "OAuthToken", m_oauthToken );
    m_configGroup->writeEntry( "OAuthTokenSecret", m_oauthTokenSecret );
    m_configGroup->sync();
    delete m_configGroup;
}

Zzzz::MicroBlog* Account::microblog() const
{
    return m_microblog;
}

bool Account::isAuthorized() const
{
    return m_isAuthorized;
}

void Account::authorize( bool force )
{
    if ( isAuthorized() && !force )
        return;

    m_isAuthorized = false;
    /// request token
    QString requestTokenApiUrl;
    QOAuth::HttpMethod method = QOAuth::GET;
    Zzzz::MicroBlog::HttpMethod methodType = Zzzz::MicroBlog::GET;
    Zzzz::MicroBlog::ParamMap params;
    m_microblog->oauthRequestToken( requestTokenApiUrl, methodType, params );
    if ( methodType == Zzzz::MicroBlog::GET )
        method = QOAuth::GET;
    else if ( methodType == Zzzz::MicroBlog::POST )
        method = QOAuth::POST;
    QOAuth::ParamMap requestTokenReply = m_qoauth->requestToken( requestTokenApiUrl, method, QOAuth::HMAC_SHA1, params );
    if ( m_qoauth->error() != QOAuth::NoError ) {
        qWarning() << "ERROR request token: " << m_qoauth->error();
        return;
    }
    m_oauthToken = requestTokenReply.value( QOAuth::tokenParameterName() );
    m_oauthTokenSecret = requestTokenReply.value( QOAuth::tokenSecretParameterName() );

    /// authorize
    QString authorizeApiUrl;
    Zzzz::MicroBlog::ParamMap params2;
    m_microblog->oauthAuthorize( authorizeApiUrl, params2 );
    params2.insert( "oauth_token", m_oauthToken );
    KUrl url( authorizeApiUrl );
    QByteArray hs = m_qoauth->inlineParameters( params2, QOAuth::ParseForInlineQuery );
    url.setQuery( hs );
    KToolInvocation::invokeBrowser( url.url() );
}

void Account::access( const QString& pincode )
{
    /// access token
    QString accessTokenApiUrl;
    QOAuth::HttpMethod method = QOAuth::GET;
    Zzzz::MicroBlog::HttpMethod methodType = Zzzz::MicroBlog::GET;
    Zzzz::MicroBlog::ParamMap params;
    m_microblog->oauthAccessToken( accessTokenApiUrl, methodType, params );
    if ( methodType == Zzzz::MicroBlog::GET )
        method = QOAuth::GET;
    else if ( methodType == Zzzz::MicroBlog::POST )
        method = QOAuth::POST;
    params.insert( "oauth_verifier", pincode.toAscii() );
    QOAuth::ParamMap accessTokenReply = m_qoauth->accessToken( accessTokenApiUrl, method, m_oauthToken,
                                                               m_oauthTokenSecret, QOAuth::HMAC_SHA1, params );
    if ( m_qoauth->error() != QOAuth::NoError ) {
        qWarning() << "ERROR access token: " << m_qoauth->error();
        return;
    }
    m_oauthToken = accessTokenReply.value( QOAuth::tokenParameterName() );
    m_oauthTokenSecret = accessTokenReply.value( QOAuth::tokenSecretParameterName() );

    m_isAuthorized = true;
    qWarning() << "Zzzz is authorized successfully.";
}

QByteArray Account::createParametersString( const QString& requstUrl,
                                            Zzzz::MicroBlog::HttpMethod httpMethod,
                                            const Zzzz::MicroBlog::ParamMap& params )
{
    if ( httpMethod == Zzzz::MicroBlog::GET ) {
        return m_qoauth->createParametersString( requstUrl, QOAuth::GET, m_oauthToken, m_oauthTokenSecret,
                                                 QOAuth::HMAC_SHA1, params, QOAuth::ParseForInlineQuery );
    }
    else /* httpMethod == Zzzz::MicroBlog::POST */ {
        return m_qoauth->createParametersString( requstUrl, QOAuth::POST, m_oauthToken, m_oauthTokenSecret,
                                                 QOAuth::HMAC_SHA1, params, QOAuth::ParseForRequestContent );
    }
}

void Account::setAlias( const QString& alias )
{
    m_alias = alias;
}

QString Account::alias() const
{
    return m_alias;
}

void Account::setEnabled( bool isEnabled )
{
    m_isEnabled = isEnabled;
}

bool Account::isEnabled() const
{
    return m_isEnabled;
}
