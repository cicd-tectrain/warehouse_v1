#include "customerorder.h"
#include "ui_customerorder.h"

CustomerOrder::CustomerOrder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomerOrder)
{
    ui->setupUi(this);
    con = getConnection();
    show_customers();
    show_outgoing_orders();
    ui->tableWidget_create_order->setColumnCount(3);
    ui->tableWidget_create_order->setHorizontalHeaderItem(0, new QTableWidgetItem("suplier_id"));
    ui->tableWidget_create_order->setHorizontalHeaderItem(1, new QTableWidgetItem("product_id"));
    ui->tableWidget_create_order->setHorizontalHeaderItem(2, new QTableWidgetItem("amount"));
}

CustomerOrder::~CustomerOrder()
{
    delete ui;
}


void CustomerOrder::setConnection(QSqlDatabase _con)
{
    con = _con;
}

QSqlDatabase CustomerOrder::getConnection()
{
    return con;
}

void CustomerOrder::design(QTableView* table)
{
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
}

void CustomerOrder::on_pushButton_Close_clicked()
{
    this->close();
}

void CustomerOrder::show_outgoing_orders()
{
    QSqlTableModel* outgoing_orders = new QSqlTableModel();
    outgoing_orders->setTable("orders");
    outgoing_orders->setFilter("status_value = 30");
    outgoing_orders->select();
    ui->tableView_orders->setModel(outgoing_orders);
    design(ui->tableView_orders);
}


void CustomerOrder::show_products()
{
    QSqlTableModel* product = new QSqlTableModel();
    product->setTable("products");
    product->select();
    ui->tableView_products->setModel(product);
    design(ui->tableView_products);
}

void CustomerOrder::show_customers()
{
    QSqlTableModel* customer = new QSqlTableModel();
    customer->setTable("clients");
    customer->setFilter("client_type in (20,30)");
    customer->select();
    ui->tableView_customer->setModel(customer);
    design(ui->tableView_customer);
}

void CustomerOrder::on_pushButton_select_customer_clicked()
{
    int index = ui->tableView_customer->selectionModel()->currentIndex().row();
    int id = ui->tableView_customer->model()->index(index,0).data().toInt();
    QString name = ui->tableView_customer->model()->index(index,1).data().toString();

    ui->plainTextEdit_customer->setPlainText(QString::number(id) + " " + name);
    customer_id = id;
    show_products();



}

void CustomerOrder::on_pushButton_select_product_clicked()
{
    int index = ui->tableView_products->selectionModel()->currentIndex().row();
    int id = ui->tableView_products->model()->index(index,0).data().toInt();
    QString name = ui->tableView_products->model()->index(index,1).data().toString();

    ui->plainTextEdit_product->setPlainText(QString::number(id) + " " + name);
    product_id = id;
}

void CustomerOrder::on_pushButton_add_product_clicked()
{
    ui->plainTextEdit_customer->setDisabled(true);
    if(ui->plainTextEdit_amount->toPlainText().isEmpty())
    {
        msgBox.information(this,"Missing amount","The amount filed is empty");
    }
    else
    {
        double amount = ui->plainTextEdit_amount->toPlainText().toDouble();
        if (amount == NULL)
            return;
        product_amount = amount;
        int row = ui->tableWidget_create_order->rowCount();
        ui->tableWidget_create_order->setRowCount(row+1);
        ui->tableWidget_create_order->setItem(row,0,new QTableWidgetItem(QString::number(customer_id)));
        ui->tableWidget_create_order->setItem(row,1,new QTableWidgetItem(QString::number(product_id)));
        ui->tableWidget_create_order->setItem(row,2,new QTableWidgetItem(QString::number(product_amount)));

    }
}

void CustomerOrder::on_pushButton_finish_order_clicked()
{
    QSqlQuery orders_insert;
    orders_insert.prepare("INSERT INTO orders (orderID, clientID, order_date, status_value) "
                          "VALUES (:orderid, :clientid, :orderdate, :status_value)");
    orders_insert.bindValue(":orderid", NULL);
    orders_insert.bindValue(":clientid", customer_id);
    orders_insert.bindValue(":orderdate", QDateTime::currentDateTime());
    orders_insert.bindValue(":status_value", 30);
    if (orders_insert.exec())
    {
        qDebug() << "Success inserting in order";
    }
    else
    {
        msgBox.information(this,"Failed insert","Failede to insert order");
        return;
    }
    int order_id = orders_insert.lastInsertId().toInt();

    for (int i = 0; i < ui->tableWidget_create_order->rowCount(); ++i) {
        QSqlQuery orders_io_insert;
        orders_io_insert.prepare("INSERT INTO orders_io (orders_ioID, orderID, productID, amount_ordered, status_value)"
                                 "VALUES (:orderioid, :orderid, :productid, :amount_ordered, :status_value)");
        int product_id = ui->tableWidget_create_order->item(i,1)->text().toInt();
        double product_amount = ui->tableWidget_create_order->item(i,2)->text().toDouble();

        orders_io_insert.bindValue(":orderioid", NULL);
        orders_io_insert.bindValue(":orderid", order_id);
        orders_io_insert.bindValue(":productid", product_id);
        orders_io_insert.bindValue(":amount_ordered", product_amount);
        orders_io_insert.bindValue(":status_value", 30);


        if(orders_io_insert.exec())
        {
            qDebug() << "Success inserting in orderio";
            int out_order_ioid = orders_io_insert.lastInsertId().toInt();
            book_storage_objects(out_order_ioid, product_id, product_amount);
        }
        else
        {
            msgBox.information(this,"Failed insert","Failede to insert in orderio");
            return;
        }
    }
    show_outgoing_orders();
    msgBox.information(this,"Success","Order added");

}

void CustomerOrder::book_storage_objects(int out_order_io_id, int product_id, double amount)
{
    QSqlTableModel* bundle = new QSqlTableModel();
    bundle->setTable("bundles");
    bundle->setFilter("productID = " +  QString::number( product_id));
    bundle->select();
    double bundle_size = bundle->record(0).value("amount").toDouble();
    double number_of_full_so = std::floor(amount / bundle_size);
    double amount_commissioned = 0;
    delete(bundle);


    QSqlQuery total_amount_querry;
    total_amount_querry.prepare("SELECT sum(amount) FROM orders_io_storage_object"
                         " WHERE status_value = 20 AND productID = :productID");
    total_amount_querry.bindValue(":productID", product_id);
    if (!total_amount_querry.exec())
    {
        qDebug() << "FAIL select in orders_io_storage_object";
        return;
    }
    total_amount_querry.next();
    double total_amount = total_amount_querry.value(0).toDouble();



    QSqlTableModel* so_join_orders_io = new QSqlTableModel();
    so_join_orders_io->setTable("orders_io_storage_object");
    so_join_orders_io->setFilter("status_value = 20 AND productID = " +  QString::number( product_id));
    so_join_orders_io->select();


    if(total_amount < amount)
    {
        for (int i = 0; i < so_join_orders_io->rowCount(); ++i) {

            double amount_on_so = so_join_orders_io->record(i).value("amount").toDouble();
            long long soID = so_join_orders_io->record(i).value("storage_objectID").toLongLong();
            if(!change_storage_object_status(soID, 30, out_order_io_id))
                return;

            amount_commissioned += amount_on_so;

        }
        change_orders_io_amount(out_order_io_id, total_amount, amount_commissioned);

    }
    else
    {
//        QSqlQuery total_amount_full_querry;
//        total_amount_full_querry.prepare("SELECT sum(amount) FROM orders_io_storage_object"
//                             "WHERE status_value = 20 AND productID = :productID AND amount = :amount");
//        total_amount_full_querry.bindValue(":productID", product_id);
//        total_amount_full_querry.bindValue(":amount", bundle_size);
//        double total_amount_full = total_amount_full_querry.value(0).toDouble();

//        QSqlQuery total_amount_partial_querry;
//        total_amount_partial_querry.prepare("SELECT sum(amount) FROM orders_io_storage_object"
//                             "WHERE status_value = 20 AND productID = :productID AND amount < :amount");
//        total_amount_partial_querry.bindValue(":productID", product_id);
//        total_amount_partial_querry.bindValue(":amount", amount);
//        double total_amount_partial = total_amount_partial_querry.value(0).toDouble();

        double amount_ordered = amount;
        double amount_commissioned = 0;

        for (int i = 0; i < so_join_orders_io->rowCount(); ++i) {

            double amount_on_so = so_join_orders_io->record(i).value("amount").toDouble();
            long long soID = so_join_orders_io->record(i).value("storage_objectID").toLongLong();

            if(amount_ordered < amount_commissioned + amount_on_so)
            {
                double remainder = amount_ordered - amount_commissioned;

                amount_commissioned += remainder;
                create_remainder_so(out_order_io_id, remainder);
                change_amount_on_so(soID, amount_on_so - remainder, 30);
                change_orders_io_amount(out_order_io_id, total_amount, amount_commissioned);
                return;
            }
            else if(amount_ordered == amount_commissioned + amount_on_so)
            {
                amount_commissioned += amount_on_so;
                change_storage_object_status(soID, 30, out_order_io_id);
                change_orders_io_amount(out_order_io_id, total_amount, amount_commissioned);
                return;
            }
            else
            {
                amount_commissioned += amount_on_so;
                change_storage_object_status(soID, 30, out_order_io_id);
            }
        }

    }
}


int CustomerOrder::change_storage_object_status(long long soID, int status, int out_order_ioID)
{
    QSqlQuery storage_object_update;

    storage_object_update.prepare("UPDATE storage_objects "
                                  "SET out_order_ioID = :out_order_ioID , status_value = :status_value "
                                  "WHERE storage_objectID = :storage_objectID");
    storage_object_update.bindValue(":out_order_ioID", out_order_ioID);
    storage_object_update.bindValue(":storage_objectID", soID);
    storage_object_update.bindValue(":status_value", status);
    if(storage_object_update.exec())
    {
        qDebug() << "Success updating status in storage_object";
        return 1;
    }
    else
    {
        qDebug() << "FAILED updating status in storage_object";
        return 0;
    }
}

int CustomerOrder::change_orders_io_amount(int ioID, double amount_available, double amount_commissioned)
{
    QSqlQuery orders_io_update;

    orders_io_update.prepare("UPDATE orders_io "
                                  "SET amount_available = :amount_available , amount_commissioned = :amount_commissioned "
                                  "WHERE orders_ioID = :orders_ioID");
    orders_io_update.bindValue(":amount_available", amount_available);
    orders_io_update.bindValue(":amount_commissioned", amount_commissioned);
    orders_io_update.bindValue(":orders_ioID", ioID);
    if(orders_io_update.exec())
    {
        qDebug() << "Success updating amount in orders_io";
        return 1;
    }
    else
    {
        qDebug() << "FAILED updating amount in orders_io";
        return 0;
    }
}


int CustomerOrder::create_remainder_so(int out_order_ioid, double amount)
{
    QSqlQuery storage_object_insert;
    storage_object_insert.prepare("INSERT INTO storage_objects (storage_objectID, order_ioID, out_order_ioID, amount, status_value)"
                             "VALUES (:storage_objectID, :order_ioID, :out_order_ioID , :amount, :status_value)");
    storage_object_insert.bindValue(":storage_objectID", NULL);
    storage_object_insert.bindValue(":order_ioID", NULL);
    storage_object_insert.bindValue(":out_order_ioID", out_order_ioid);
    storage_object_insert.bindValue(":amount", amount);
    storage_object_insert.bindValue(":status_value", 30);

    if(storage_object_insert.exec())
    {
        qDebug() << "Success inserting remainder in storage_object";
        return 1;
    }
    else
    {
        qDebug() << "FAILED inserting remainder in storage_object";
        return 0;
    }
}


int CustomerOrder::change_amount_on_so(long long soID, double amount, int status)
{
    QSqlQuery storage_object_update;

    storage_object_update.prepare("UPDATE storage_objects "
                                  "SET amount = :amount , status_value = :status_value "
                                  "WHERE storage_objectID = :storage_objectID");
    storage_object_update.bindValue(":amount", amount);
    storage_object_update.bindValue(":storage_objectID", soID);
    storage_object_update.bindValue(":status_value", status);
    if(storage_object_update.exec())
    {
        qDebug() << "Success updating rest amount in storage_object";
        return 1;
    }
    else
    {
        qDebug() << "FAILED updating rest amount in storage_object";
        return 0;
    }
}
