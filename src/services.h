#ifndef SERVICES_H
#define SERVICES_H

#include <QObject>
#include <QSharedMemory>

#include "upload.h"

class Services : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool visible MEMBER m_visible NOTIFY visibleChanged)
public:
    explicit Services(QObject *parent = nullptr);
    static void init();

public slots:
    void uploading(QList<QUrl> list);
    bool hasFiles(QList<QUrl> list);

signals:
    void visibleChanged();

private slots:
    void updateSettings();

private:
    Upload *m_api = nullptr;
    bool m_visible = true;
};

#endif // SERVICES_H
