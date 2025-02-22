//
// Created by zdanek on 22.02.25.
//

#include "BrowserWindow.h"

#include <QQuickWindow>
#include <QtWebEngine/QtWebEngine>
#include <QtWebEngineWidgets/QWebEngineView>

#include "QGCApplication.h"
#include <MewaMed.h>

BrowserWindow::BrowserWindow(QObject *parent)
{
    qCDebug(MewaMedLog) << "Creating BrowserWindow";
    _view = new QWebEngineView(this);
    setWindowTitle("MewaMed Orders");
    setCentralWidget(_view);
    resize(1024, 768);
    setVisible(false);

    connect(qgcApp(), &QGCApplication::applicationShutdown, this, &BrowserWindow::appWindowDestroyed);

    // connect(, &QObject::destroyed, this, &BrowserWindow::appWindowDestroyed);
}

BrowserWindow::~BrowserWindow()
{
    qCDebug(MewaMedLog) << "Destroying BrowserWindow";
    // QMainWindow::close();
    // QMainWindow::~QMainWindow();
}

void BrowserWindow::showUrl(const QUrl &url)
{
    qCDebug(MewaMedLog) << "BrowserWindow::showUrl:" << url;
    _view->setUrl(url);
    setFocus(Qt::FocusReason::PopupFocusReason);
    showNormal();
    show();
}
