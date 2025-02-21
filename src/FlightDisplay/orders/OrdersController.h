//
// Created by zdanek on 20.02.25.
//

#ifndef ORDERS_CONTROLLER_H
#define ORDERS_CONTROLLER_H

#include <QObject>

#include "QGCApplication.h"


Q_DECLARE_LOGGING_CATEGORY(OrdersControllerLog)

class OrdersController : public QObject
{
    Q_OBJECT

public:
    explicit OrdersController(QObject *parent = nullptr);
    virtual ~OrdersController() {}

    Q_INVOKABLE void openOrdersPage();

signals:
    void openPage(QString url);

private:
    void _loadConfig();
    void _saveConfig() const;


    QString _orders_main_page_url;
};


#endif //ORDERS_CONTROLLER_H