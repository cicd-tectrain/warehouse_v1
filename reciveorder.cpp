#include "reciveorder.h"
#include "ui_reciveorder.h"

ReciveOrder::ReciveOrder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReciveOrder)
{
    ui->setupUi(this);
    con = getConnection();
    show_orders();
    ui->pushButton_select_order_io->setDisabled(true);
    ui->pushButton_select_storage_object->setDisabled(true);
    ui->pushButton_confirm_storage->setDisabled(true);
}

ReciveOrder::~ReciveOrder()
{
    delete ui;
}


void ReciveOrder::setConnection(QSqlDatabase _con)
{
    con = _con;
}

QSqlDatabase ReciveOrder::getConnection()
{
    return con;
}

void ReciveOrder::design(QTableView* table)
{
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
}

void ReciveOrder::on_pushButton_Close_clicked()
{
    this->close();
}

void ReciveOrder::show_orders()
{
    QSqlTableModel* orders = new QSqlTableModel();
    orders->setTable("orders");
    orders->setFilter("status = 10");
    orders->select();
    ui->tableView_orders->setModel(orders);
    design(ui->tableView_orders);
}

void ReciveOrder::show_order_io(int id)
{
    QSqlTableModel* orders_io = new QSqlTableModel();
    orders_io->setTable("orders_io");
    orders_io->setFilter("status = 10 AND orderID = " + QString::number(id));
    orders_io->select();
    ui->tableView_orders_io->setModel(orders_io);
    design(ui->tableView_orders_io);

}

void ReciveOrder::show_storage_object(int id)
{
    QSqlTableModel* storage_object = new QSqlTableModel();
    storage_object->setTable("storage_object");
    storage_object->setFilter("status = 10 AND order_ioID = " + QString::number(id));
    storage_object->select();
    ui->tableView_storage_object->setModel(storage_object);
    design(ui->tableView_storage_object);
}

void ReciveOrder::show_slots(long long int id)
{
    QSqlTableModel* slot = new QSqlTableModel();
    slot->setTable("show_empty_slots");
    slot->setFilter("storage_objectID is NULL LIMIT 1");
    slot->select();
    ui->tableView_slot->setModel(slot);
    design(ui->tableView_slot);
    slot_id =  slot->record(0).value("slotID").toInt();

}

void ReciveOrder::on_pushButton_select_order_clicked()
{
    int index = ui->tableView_orders->selectionModel()->currentIndex().row();
    int id = ui->tableView_orders->model()->index(index,0).data().toInt();
    //QString name = ui->tableView_orders->model()->index(index,1).data().toString();
    order_id = id;

    ui->pushButton_select_order_io->setDisabled(false);
    show_order_io(id);

}
void ReciveOrder::on_pushButton_select_order_io_clicked()
{
    int index = ui->tableView_orders_io->selectionModel()->currentIndex().row();
    int id = ui->tableView_orders_io->model()->index(index,0).data().toInt();
    //QString name = ui->tableView_orders->model()->index(index,1).data().toString();
    orders_io_id = id;

    ui->pushButton_select_storage_object->setDisabled(false);
    show_storage_object(id);

}
void ReciveOrder::on_pushButton_select_storage_object_clicked()
{
    int index = ui->tableView_storage_object->selectionModel()->currentIndex().row();
    long long int id = ui->tableView_storage_object->model()->index(index,0).data().toLongLong();
    //QString name = ui->tableView_orders->model()->index(index,1).data().toString();
    storage_object_id = id;

    ui->pushButton_confirm_storage->setDisabled(false);
    show_slots(id);

}
void ReciveOrder::on_pushButton_confirm_storage_clicked()
{
    QSqlQuery update_slot;
    QSqlQuery update_storage_object;
    update_slot.prepare("UPDATE slot "
                        "SET storage_objectID = :storage_object_id "
                        "WHERE slotID = :slot_id");
    update_slot.bindValue(":storage_object_id", storage_object_id);
    update_slot.bindValue(":slot_id", slot_id);
    if(!update_slot.exec())
    {
        qDebug() << "FAIL updating in slot";
        return;
    }
    update_storage_object.prepare("UPDATE storage_object "
                        "SET status = :status "
                        "WHERE storage_objectID = :storage_object_id");
    update_storage_object.bindValue(":status", 20);
    update_storage_object.bindValue(":storage_object_id", storage_object_id);
    if(!update_storage_object.exec())
    {
        qDebug() << "FAIL updating in storage object";
        return;
    }

    show_storage_object(orders_io_id);

    ui->tableView_slot->setModel(NULL);

    QSqlTableModel* storage_object = new QSqlTableModel();
    storage_object->setTable("storage_object");
    storage_object->setFilter("status = 10 AND order_ioID = " + QString::number(orders_io_id));
    storage_object->select();

    if(storage_object->rowCount() == 0)
    {
        QSqlQuery update_orders_io;
        update_orders_io.prepare("UPDATE orders_io "
                            "SET status = :status "
                            "WHERE orders_ioID = :orders_io_id");
        update_orders_io.bindValue(":orders_io_id", orders_io_id);
        update_orders_io.bindValue(":status", 20);
        if(!update_orders_io.exec())
        {
            qDebug() << "FAIL updateing in orders_io";
            return;
        }

        show_order_io(order_id);

        QSqlTableModel* orders_io = new QSqlTableModel();
        orders_io->setTable("orders_io");
        orders_io->setFilter("status = 10 AND order_ioID = " + QString::number(order_id));
        orders_io->select();

        if(orders_io->rowCount() == 0)
        {
            QSqlQuery update_orders;
            update_orders.prepare("UPDATE orders "
                                "SET status = :status "
                                "WHERE orderID = :order_id");
            update_orders.bindValue(":order_id", order_id);
            update_orders.bindValue(":status", 20);
            if(!update_orders.exec())
            {
                qDebug() << "FAIL updateing in orders_io";
                return;
            }

            show_orders();
        }
    }
}
