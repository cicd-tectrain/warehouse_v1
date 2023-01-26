#include "orderstock.h"
#include "ui_orderstock.h"

OrderStock::OrderStock(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OrderStock)
{
    ui->setupUi(this);
    con = getConnection();
    show_suplier();
    show_incoming_orders();
    ui->tableWidget_create_order->setColumnCount(3);
    ui->tableWidget_create_order->setHorizontalHeaderItem(0, new QTableWidgetItem("suplier_id"));
    ui->tableWidget_create_order->setHorizontalHeaderItem(1, new QTableWidgetItem("product_id"));
    ui->tableWidget_create_order->setHorizontalHeaderItem(2, new QTableWidgetItem("amount"));

//    show_products();
}

OrderStock::~OrderStock()
{
    delete ui;
}

void OrderStock::setConnection(QSqlDatabase _con)
{
    con = _con;
}

QSqlDatabase OrderStock::getConnection()
{
    return con;
}

void OrderStock::show_incoming_orders()
{
    QSqlTableModel* incoming_orders = new QSqlTableModel();
    incoming_orders->setTable("orders");
    incoming_orders->setFilter("status = 10");
    incoming_orders->select();
    ui->tableView_incoming_orders->setModel(incoming_orders);
    design(ui->tableView_incoming_orders);
}

void OrderStock::design(QTableView* table)
{
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
}

void OrderStock::on_pushButton_Close_clicked()
{
    this->close();
}


void OrderStock::show_products()
{
    QSqlTableModel* product = new QSqlTableModel();
    product->setTable("product");
    product->select();
    ui->tableView_products->setModel(product);
    design(ui->tableView_products);
}

void OrderStock::show_suplier()
{
    QSqlTableModel* suplier = new QSqlTableModel();
    suplier->setTable("clients");
    suplier->setFilter("client_type in (10,30)");
    suplier->select();
    ui->tableView_suplier->setModel(suplier);
    design(ui->tableView_suplier);
}

void OrderStock::on_pushButton_select_suplier_clicked()
{
    int index = ui->tableView_suplier->selectionModel()->currentIndex().row();
    int id = ui->tableView_suplier->model()->index(index,0).data().toInt();
    QString name = ui->tableView_suplier->model()->index(index,1).data().toString();

    ui->plainTextEdit_suplier->setPlainText(QString::number(id) + " " + name);
    suplier_id = id;
    show_products();



}

void OrderStock::on_pushButton_select_product_clicked()
{
    int index = ui->tableView_products->selectionModel()->currentIndex().row();
    int id = ui->tableView_products->model()->index(index,0).data().toInt();
    QString name = ui->tableView_products->model()->index(index,1).data().toString();

    ui->plainTextEdit_product->setPlainText(QString::number(id) + " " + name);
    product_id = id;
}

void OrderStock::on_pushButton_add_product_clicked()
{
    ui->plainTextEdit_suplier->setDisabled(true);
    if(ui->textEdit_amount->toPlainText().isEmpty())
    {
        msgBox.information(this,"Missing amount","The amount filed is empty");
    }
    else
    {
        double amount = ui->textEdit_amount->toPlainText().toDouble();
        if (amount == NULL)
            return;
        product_amount = amount;
        int row = ui->tableWidget_create_order->rowCount();
        ui->tableWidget_create_order->setRowCount(row+1);
        ui->tableWidget_create_order->setItem(row,0,new QTableWidgetItem(QString::number(suplier_id)));
        ui->tableWidget_create_order->setItem(row,1,new QTableWidgetItem(QString::number(product_id)));
        ui->tableWidget_create_order->setItem(row,2,new QTableWidgetItem(QString::number(product_amount)));

    }
}

void OrderStock::on_pushButton_finish_clicked()
{
    QSqlQuery orders_insert;
    orders_insert.prepare("INSERT INTO orders (orderID, clientID, order_date, status) "
                          "VALUES (:orderid, :clientid, :orderdate, :status)");
    orders_insert.bindValue(":orderid", NULL);
    orders_insert.bindValue(":clientid", suplier_id);
    orders_insert.bindValue(":orderdate", QDateTime::currentDateTime());
    orders_insert.bindValue(":status", 10);
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
        orders_io_insert.prepare("INSERT INTO orders_io (orders_ioID, orderID, productID, amount_ordered, status)"
                                 "VALUES (:orderioid, :orderid, :productid, :amount_ordered, :status)");
        int product_id = ui->tableWidget_create_order->item(i,1)->text().toInt();
        double product_amount = ui->tableWidget_create_order->item(i,2)->text().toDouble();

        orders_io_insert.bindValue(":orderioid", NULL);
        orders_io_insert.bindValue(":orderid", order_id);
        orders_io_insert.bindValue(":productid", product_id);
        orders_io_insert.bindValue(":amount_ordered", product_amount);
        orders_io_insert.bindValue(":status", 10);


        if(orders_io_insert.exec())
        {
            qDebug() << "Success inserting in orderio";
            int order_ioid = orders_io_insert.lastInsertId().toInt();
            create_storage_objects(order_ioid, product_id, product_amount);
        }
        else
        {
            msgBox.information(this,"Failed insert","Failede to insert in orderio");
            return;
        }
    }

    msgBox.information(this,"Success","Order added");

}

void OrderStock::create_storage_objects(int order_io_id, int product_id, double amount)
{
    QSqlTableModel* bundle = new QSqlTableModel();
    bundle->setTable("bundle");
    bundle->setFilter("productID = " +  QString::number( product_id));
    bundle->select();
    double bundle_size = bundle->record(0).value("amount").toDouble();
    double storage_object_amount = amount / bundle_size;


    for (int i = 0; i < std::ceil(storage_object_amount); ++i) {
        QSqlQuery storage_object_insert;
        amount -= bundle_size;
        if(amount < 0)
            bundle_size = amount + bundle_size;

        storage_object_insert.prepare("INSERT INTO storage_object (storage_objectID, order_ioID, amount, status)"
                                 "VALUES (:storage_objectID, :order_ioID, :amount, :status)");
        storage_object_insert.bindValue(":storage_objectID", NULL);
        storage_object_insert.bindValue(":order_ioID", order_io_id);
        storage_object_insert.bindValue(":amount", bundle_size);
        storage_object_insert.bindValue(":status", 10);
        if(storage_object_insert.exec())
        {
            qDebug() << "Success inserting in storage_object";
        }
        else
        {
            qDebug() << "FAILED inserting in storage_object";
            return;
        }
    }

}
