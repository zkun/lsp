#include <QUrl>
#include <QMenu>
#include <QTimer>
#include <QSettings>
#include <QFileInfo>
#include <QMessageBox>
#include <QtConcurrent>
#include <QSystemTrayIcon>
#include <QSystemSemaphore>
#include <QDesktopServices>
#include <QCoreApplication>

#include <QDebug>

#include "services.h"
#include "githubapi.h"

Services::Services(QObject *parent)
    : QObject{parent}
{
    Services::init();

    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/favicon.svg"));
    trayIcon->setToolTip(qAppName());
    trayIcon->show();

    QMenu *menu = new QMenu;
    connect(this, &Services::destroyed, menu, &QMenu::deleteLater);
    menu->addAction("退出", qApp, &QCoreApplication::quit);
    trayIcon->setContextMenu(menu);

    connect(trayIcon, &QSystemTrayIcon::activated, this, [this](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::Trigger)
            setProperty("visible", true);
    });

    updateSettings();
}

void Services::init()
{
    QSettings::setDefaultFormat(QSettings::IniFormat);
#ifdef Q_OS_WINDOWS
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::applicationDirPath());
#endif

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.process(QCoreApplication::arguments());
}

void Services::uploading(QList<QUrl> list)
{
    for (auto &url : list) {
        QFileInfo info(url.toLocalFile());
        if (info.isFile() == false)
            continue;

        m_api->putFile(info.filePath());
    }
}

bool Services::hasFiles(QList<QUrl> list)
{
    bool isfile = false;

    for (auto &url : list) {
        QFileInfo info(url.toLocalFile());
        if (info.isFile() == false)
            return false;
        else
            isfile = true;
    }

    return isfile;
}

void Services::updateSettings()
{
    QSettings setting;

    setting.beginGroup("GitHub");
    auto url = setting.value("url").toString();
    auto key = setting.value("key").toString();
    auto user = setting.value("user").toString();
    auto repo = setting.value("repo").toString();
    auto path = QStringLiteral("/%1/%2/contents/").arg(user, repo);

    m_api = new GitHubApi(this);
    m_api->setProperty("url", url);
    m_api->setProperty("key", key);
    m_api->setProperty("path", path);
    setting.endGroup();
}
