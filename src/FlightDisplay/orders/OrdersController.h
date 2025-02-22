//
// Created by zdanek on 20.02.25.
//

#ifndef ORDERS_CONTROLLER_H
#define ORDERS_CONTROLLER_H

#include <QObject>

#include "QGCApplication.h"

#include <BrowserWindow.h>

Q_DECLARE_LOGGING_CATEGORY(OrdersControllerLog)

class OrdersController : public QObject
{
    Q_OBJECT

public:
    explicit OrdersController(QObject *parent = nullptr);
    ~OrdersController() override;

    Q_PROPERTY(QString url READ url NOTIFY urlChanged)
    Q_PROPERTY(bool ordersPanelVisible READ isOrdersPanelVisible NOTIFY ordersPanelVisibleChanged)
    Q_PROPERTY(bool hasNewOrders READ hasNewOrders NOTIFY hasNewOrdersChanged)

    Q_INVOKABLE void hideOrdersPanel();
    Q_INVOKABLE bool isOrdersPanelVisible() const { return _orders_panel_visible; }
    Q_INVOKABLE bool hasNewOrders() const { return _has_new_orders; }
    Q_INVOKABLE QString url() const { return _orders_main_page_url; }
    Q_INVOKABLE void openOrdersPanel();

signals:
    void urlChanged(QString &url);
    void ordersPanelVisibleChanged(bool visible);
    void hasNewOrdersChanged(bool hasNewOrders);

private:
    void _loadConfig();
    void _saveConfig() const;
    // void _openWebPage(const QString &url) const;

    QString _orders_main_page_url;
    bool _orders_panel_visible = false;
    bool _has_new_orders = false;
};


#endif //ORDERS_CONTROLLER_H