//
// Created by zdanek on 22.02.25.
//

#ifndef BROWSERWINDOW_H
#define BROWSERWINDOW_H

#include <QtWebEngineWidgets/QWebEngineView>
#include <qmainwindow.h>

#include <MewaMed.h>

class BrowserWindow : public QMainWindow
{
public:
    explicit BrowserWindow(QObject *parent);
    ~BrowserWindow() override;
    void loadUrl(const QUrl &url) { _view->setUrl(url); }
    void showUrl(const QUrl &url);
    void show() { setVisible(true); }

public slots:
    void appWindowDestroyed()
    {
        qCDebug(MewaMedLog) << "BrowserWindow::appWindowDestroyed";
        close();
    }


private:
    QWebEngineView *_view;
};


#endif //BROWSERWINDOW_H
