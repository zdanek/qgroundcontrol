//
// Created by zdanek on 20.02.25.
//

#include "OrdersController.h"

#include <QObject>
#include <QSettings>

#include "OrdersChecker.h"

QGC_LOGGING_CATEGORY(OrdersControllerLog, "OrdersControllerLog")

OrdersController::OrdersController(QObject *parent)
    : QObject(parent)
{
    _loadConfig();
    _saveConfig();

    _orders_checker = new OrdersChecker(this, _orders_api_url);
    _orders_checker->start(QThread::LowPriority);
}

OrdersController::~OrdersController()
{
     _orders_checker->shutdown();
     _orders_checker->deleteLater();
}

void OrdersController::hideOrdersPanel()
{
    _has_new_orders = false;
    emit hasNewOrdersChanged(_has_new_orders);
    _orders_panel_visible = false;
    emit ordersPanelVisibleChanged(_orders_panel_visible);
}

void OrdersController::openOrdersPanel()
{
    qCDebug(OrdersControllerLog) << "OrdersController::openOrdersPage:" << _orders_main_page_url;
    _has_new_orders = false;
    emit hasNewOrdersChanged(_has_new_orders);

    _orders_panel_visible = true;
    emit ordersPanelVisibleChanged(_orders_panel_visible);
}

void OrdersController::newOrdersAvailable()
{
    _has_new_orders = true;
    emit hasNewOrdersChanged(_has_new_orders);
}

void OrdersController::_loadConfig()
{
    qCDebug(OrdersControllerLog) << "OrdersController::_loadConfig()";
    QSettings settings;
    settings.beginGroup("MewaMed");

    _orders_main_page_url = settings.value("orders_main_page_url", "http://84.10.54.242:25200/orders/").toString();
    _orders_api_url = settings.value("orders_api_url", "http://84.10.54.242:25200/rest/").toString();

    qCDebug(OrdersControllerLog) << "Orders main page url: " << _orders_main_page_url;
    qCDebug(OrdersControllerLog) << "Orders API url: " << _orders_api_url;
    settings.endGroup();
}

void OrdersController::_saveConfig() const
{
    qCDebug(OrdersControllerLog) << "OrdersController::_saveConfig()";
    QSettings settings;
    settings.beginGroup("MewaMed");
    settings.setValue("orders_main_page_url", _orders_main_page_url);
    settings.setValue("orders_api_url", _orders_api_url);
    settings.endGroup();
}