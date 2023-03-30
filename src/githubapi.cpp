#include <QFile>
#include <QFileInfo>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>

#include "githubapi.h"

void GitHubApi::getFile(QString path)
{
    QNetworkRequest request;
    request.setUrl(QUrl("https://api.github.com/repos" + m_path));
    request.setRawHeader("Accept", "application/vnd.github+json");
    request.setRawHeader("Authorization", "Bearer " + m_key.toUtf8());
    request.setRawHeader("X-GitHub-Api-Version", "2022-11-28");

    m_manager->get(request);
}

void GitHubApi::putFile(QString filePath)
{
    QFileInfo info(filePath);
    QFile file(info.filePath());
    if (file.open(QFile::ReadOnly) == false)
        return;

    auto data = file.readAll().toBase64();
    QJsonObject user = {
        {"name", "zkun"},
        {"email", "admin@zkun.eu.org"}
    };

    QJsonObject obj = {
        {"committer", user},
        {"message", "上传文件..."},
        {"content", data.constData()},
    };

    auto json = QJsonDocument(obj).toJson(QJsonDocument::Compact);
    auto name = QDateTime::currentDateTime().toString("yyyy.MM.dd_hh:mm:ss.") + info.completeSuffix();

    QNetworkRequest request;
    request.setUrl(QUrl("https://api.github.com/repos" + m_path + name));
    request.setHeader(QNetworkRequest::ContentLengthHeader, json);
    request.setRawHeader("Accept", "application/vnd.github+json");
    request.setRawHeader("Authorization", "Bearer " + m_key.toUtf8());
    request.setRawHeader("X-GitHub-Api-Version", "2022-11-28");

    QNetworkReply *reply = m_manager->put(request, json);
    reply->setProperty("fileName", info.fileName());

    connect(reply, &QNetworkReply::uploadProgress, this, &GitHubApi::replyProgress);
}

void GitHubApi::replyFinished(QNetworkReply *reply)
{
    auto code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    auto json = QJsonDocument::fromJson(reply->readAll()).object();
    if (code == 201) {
        auto content = json.value("content").toObject();
        auto path = content.value("path").toString();
        auto url = m_url + path;
        Upload::setText(url);

        auto fileName = reply->property("fileName").toString();

        qDebug().noquote() << fileName << "上传成功。\n链接: " << url;
    } else
        qDebug() << code << json;
}
