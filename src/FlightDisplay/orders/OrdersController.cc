//
// Created by zdanek on 20.02.25.
//

#include "OrdersController.h"

#include <QObject>

#include <MewaMed.h>

class QWebEngineView;
QGC_LOGGING_CATEGORY(OrdersControllerLog, "OrdersControllerLog")

OrdersController::OrdersController(QObject *parent)
    : QObject(parent)
{
    // connect(this, &OrdersController::showWebPage, this, &OrdersController::showWebPage);

    _loadConfig();
    _saveConfig();
}

OrdersController::~OrdersController() {}

void OrdersController::hideOrdersPanel()
{
    _orders_panel_visible = false;
    emit ordersPanelVisibleChanged(_orders_panel_visible);
}

void OrdersController::openOrdersPanel()
{
    qCDebug(OrdersControllerLog) << "OrdersController::openOrdersPage:" << _orders_main_page_url;
    // emit showWebPage(_orders_main_page_url);
    emit urlChanged(_orders_main_page_url);
    _orders_panel_visible = true;
    emit ordersPanelVisibleChanged(_orders_panel_visible);
}

void OrdersController::_loadConfig()
{
    qCDebug(OrdersControllerLog) << "OrdersController::_loadConfig()";
    QSettings settings;
    settings.beginGroup("MewaMed");

    _orders_main_page_url = settings.value("orders_main_page_url", "http://84.10.54.242:25200/orders/").toString();

    qCDebug(OrdersControllerLog) << "Orders main page url: " << _orders_main_page_url;
    settings.endGroup();
}

void OrdersController::_saveConfig() const
{
    qCDebug(OrdersControllerLog) << "OrdersController::_saveConfig()";
    QSettings settings;
    settings.beginGroup("MewaMed");
    settings.setValue("orders_main_page_url", _orders_main_page_url);
    settings.endGroup();
}