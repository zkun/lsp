#ifndef GITHUBAPI_H
#define GITHUBAPI_H

#include "upload.h"

class GitHubApi : public Upload
{
    Q_OBJECT
    Q_PROPERTY(QString url MEMBER m_url NOTIFY urlChanged)
    Q_PROPERTY(QString key MEMBER m_key NOTIFY keyChanged)
    Q_PROPERTY(QString path MEMBER m_path NOTIFY pathChanged)
public:
    using Upload::Upload;

    void getFile(QString path = "/");
    void putFile(QString filePath) override;
    void replyFinished(QNetworkReply *reply) override;
signals:
    void urlChanged();
    void keyChanged();
    void pathChanged();

private:
    QString m_url, m_key, m_path;
};

#endif // GITHUBAPI_H
