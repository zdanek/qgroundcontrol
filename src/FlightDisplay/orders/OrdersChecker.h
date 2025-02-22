//
// Created by zdanek on 22.02.25.
//

#ifndef ORDERSCHECKER_H
#define ORDERSCHECKER_H

#include "MewaMed.h"

#include <QThread>

#include "OrdersController.h"

class OrdersController;

class OrdersChecker :  public QThread
{
    Q_OBJECT

public:
    OrdersChecker(OrdersController *parent, QString orders_url);

    ~OrdersChecker() override {}

    void shutdown();

signals:
    void newOrdersAvailable();

protected:
    void run() override;

private:

    int _getCurrentSyncPoint();

    QString _orders_url;
    bool _run = true;
    int _latest_sync_point = 0;
    // QMap<QString, QVariant> _orders_map;
};



#endif //ORDERSCHECKER_H
