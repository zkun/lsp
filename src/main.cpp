#include <QApplication>
#include <QCommandLineParser>
#include <QQmlApplicationEngine>

#include "services.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QCoreApplication::setOrganizationName("zkun");
    QCoreApplication::setApplicationVersion("0.1");

    if (argc == 1) {
        QApplication app(argc, argv);

        qmlRegisterType<Services>("zkun", 1, 0, "Services");

        QQmlApplicationEngine engine;
        const QUrl url(QStringLiteral("qrc:/main.qml"));
        QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
            &app, [url](QObject *obj, const QUrl &objUrl) {
                if (!obj && url == objUrl)
                    QCoreApplication::exit(-1);
            }, Qt::QueuedConnection);
        engine.load(url);

        return app.exec();
    }

    QCoreApplication app(argc, argv);

    Services::init();

    return app.exec();
}
