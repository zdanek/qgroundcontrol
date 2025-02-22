//
// Created by zdanek on 20.02.25.
//

#ifndef ORDERS_CONTROLLER_H
#define ORDERS_CONTROLLER_H

#include <QObject>

#include "OrdersChecker.h"

Q_DECLARE_LOGGING_CATEGORY(OrdersControllerLog)

class OrdersChecker;

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

public slots:
    void newOrdersAvailable();

private:
    void _loadConfig();
    void _saveConfig() const;

    QString _orders_main_page_url;
    QString _orders_api_url;

    bool _orders_panel_visible = false;
    bool _has_new_orders = false;
    OrdersChecker *_orders_checker;
};


#endif //ORDERS_CONTROLLER_H