#ifndef MAFREEBOX_H
#define MAFREEBOX_H

#include <QObject>
#include <QtNetwork>
#include "filesystem.h"
#include "download.h"
class FileSystem;
class Download;
struct ApiInfo;
struct ApiInfo
{
    QString deviceName;
    QString version;
    QString baseUrl;
    QString deviceType;

};
class FbxAPI : public QNetworkAccessManager
{
    Q_OBJECT
public:

    enum AuthStatus {
        UnknownStatus = 0,
        PendingStatus = 1 ,
        TimeOutStatus = 2 ,
        GrantedStatus = 3,
        DeniedStatus  = 4
    };

    explicit FbxAPI(QObject *parent = 0);
    ~FbxAPI();

    //set
    void setHostName(const QString& host, int port = 80) ;
    void setApplicationToken(const QString& token);
    void setApplicationId(const QString& id);
    bool saveApplicationToken();
    bool loadApplicationToken();
    void setBaseUrl(const QString& base);

    //get
    const QString& hostName() const;
    int port() const;
    const QString& applicationToken() const;
    const QString& applicationId() const;
    const QString& sessionToken() const;
    const QString& baseUrl() const;
    const QString& challenge() const;
    const QString& deviceName() const;
    const QString& deviceType() const;
    const QString& errorString() const;
    const QString& errorCode() const;
    const QStringList& permissions() const;

    QNetworkRequest myCreateRequest(const QString& uri) const;
    bool parseResult(const QJsonDocument& doc);
    void sendError(const QString& message, const QString& code);

    //request
    void requestApiInfo();
    void requestAuthorize(const QString& appId, const QString& appName,
                          const QString& appVersion, const QString& deviceName);
    void requestAuthorizeStatus(int trackId);
    void requestLogin();
    void requestSession();

    //get Module
    FileSystem * fileSystem() {return mFileSystem;}
    Download * download() {return mDownload;}



signals:
    void error(const QString& message, const QString& errorCode);
    void authorizeReceived(const QString& applicationToken, int trackId);
    void authorizeStatusChanged(const FbxAPI::AuthStatus& status);

    void apiInfoChanged();
    void challengeChanged();
    void sessionReceived();
    void loginSuccess();

private slots:
    void requestApiInfoFinished();
    void requestAuthorizeFinished();
    void requestAuthorizeStatusFinished();
    void requestLoginFinished();
    void requestSessionFinished();
    void errorReceived(QNetworkReply::NetworkError errCode);

protected:
    static QString hmacSha1(QByteArray key, QByteArray baseString);

private:
    ApiInfo mApiInfo;
    QString mApplicationToken;
    QString mSessionToken;
    QString mApplicationId;
    QString mChallenge;
    QString mHostName;
    QString mErrorString;
    QString mErrorCode;
    QStringList mPermissions;

    int mPort;
    int mRequestLoginAttempt;

    //Modules
   FileSystem * mFileSystem;
   Download * mDownload;

    
};







#endif // MAFREEBOX_H
