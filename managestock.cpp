#include "managestock.h"
#include "ui_managestock.h"

ManageStock::ManageStock(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManageStock)
{
    ui->setupUi(this);
    con = getConnection();
    show_storage_objects();
    show_free_slots();
    ui->pushButton_change_amount->setDisabled(true);
    ui->pushButton_delete_so->setDisabled(true);
}

ManageStock::~ManageStock()
{
    delete ui;
}


void ManageStock::setConnection(QSqlDatabase _con)
{
    con = _con;
}

QSqlDatabase ManageStock::getConnection()
{
    return con;
}

void ManageStock::design(QTableView* table)
{
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
}

void ManageStock::on_pushButton_Close_clicked()
{
    this->close();
}


void ManageStock::show_storage_objects()
{
    QSqlTableModel* storage_object = new QSqlTableModel();
    storage_object->setTable("storage_object_slot");
    storage_object->setFilter("status_value = 20");
    storage_object->select();
    ui->tableView_storage_object->setModel(storage_object);
    design(ui->tableView_storage_object);
}

void ManageStock::show_free_slots()
{
    QSqlTableModel* slot = new QSqlTableModel();
    slot->setTable("slots");
    slot->setFilter("storage_objectID IS NULL");
    slot->select();
    ui->tableView_free_slot->setModel(slot);
    design(ui->tableView_free_slot);
}

void ManageStock::on_pushButton_select_storage_object_clicked()
{
    int index = ui->tableView_storage_object->selectionModel()->currentIndex().row();
    long long int id = ui->tableView_storage_object->model()->index(index,0).data().toLongLong();
    int slot_id = ui->tableView_storage_object->model()->index(index,2).data().toInt();
    double _amount = ui->tableView_storage_object->model()->index(index,4).data().toDouble();
    int order_io_id = ui->tableView_storage_object->model()->index(index,3).data().toInt();

    storage_objectID = id;
    old_slotID = slot_id;
    amount = _amount;
    order_ioID = order_io_id;

    ui->plainTextEdit_so_id->setPlainText(QString::number(id));
    ui->plainTextEdit_slot_id->setPlainText(QString::number(slot_id));
    ui->plainTextEdit_amount->setPlainText(QString::number(_amount));

    ui->pushButton_change_amount->setDisabled(false);
    ui->pushButton_delete_so->setDisabled(false);

}

void ManageStock::on_pushButton_select_slot_clicked()
{
    int index = ui->tableView_free_slot->selectionModel()->currentIndex().row();
    int id = ui->tableView_free_slot->model()->index(index,0).data().toLongLong();


    new_slotID = id;
    ui->plainTextEdit_slot_id->setPlainText(QString::number(id));
}

void ManageStock::on_pushButton_change_slot_clicked()
{
    if(!new_slotID)
        return;

    QSqlQuery update_old_slot;
    update_old_slot.prepare("UPDATE slots "
                        "SET storage_objectID = :storage_object_id "
                        "WHERE slotID = :slot_id");
    update_old_slot.bindValue(":storage_object_id", QVariant(QMetaType::fromType<QString>()));
    update_old_slot.bindValue(":slot_id", old_slotID);
    if(!update_old_slot.exec())
    {
        qDebug() << "FAIL updating in old slot";
        return;
    }

    QSqlQuery update_new_slot;
    update_new_slot.prepare("UPDATE slots "
                        "SET storage_objectID = :storage_object_id "
                        "WHERE slotID = :slot_id");
    update_new_slot.bindValue(":storage_object_id", storage_objectID);
    update_new_slot.bindValue(":slot_id", new_slotID);
    if(!update_new_slot.exec())
    {
        qDebug() << "FAIL updating in new slot";
        return;
    }
    show_storage_objects();
    show_free_slots();

}

void ManageStock::on_pushButton_change_amount_clicked()
{
    QSqlQuery update_amount;
    double amount = 0;

    if(ui->plainTextEdit_amount->toPlainText().isEmpty())
    {
        msgBox.information(this,"Missing amount","The amount filed is empty");
        return;
    }
    else
    {
        amount = ui->plainTextEdit_amount->toPlainText().toDouble();
        if (amount == NULL)
            return;

    }
    update_amount.prepare("UPDATE storage_objects "
                        "SET amount = :amount "
                        "WHERE storage_objectID = :storage_objectID");
    update_amount.bindValue(":amount", amount);
    update_amount.bindValue(":storage_objectID", storage_objectID);
    if(!update_amount.exec())
    {
        qDebug() << "FAIL updating amount in old storage object";
        return;
    }

    show_storage_objects();
}

void ManageStock::on_pushButton_delete_so_clicked()
{


    QSqlQuery delete_so;
    delete_so.prepare("DELETE FROM storage_objects "
                        "WHERE storage_objectID = :storage_objectID");
    delete_so.bindValue(":storage_objectID", storage_objectID);
    if(!delete_so.exec())
    {
        qDebug() << "FAIL delete in new storage object";
        return;
    }
     show_storage_objects();
     show_free_slots();
     ui->plainTextEdit_slot_id->clear();
     ui->plainTextEdit_so_id->clear();
     ui->plainTextEdit_amount->clear();

     ui->pushButton_change_amount->setDisabled(true);
     ui->pushButton_delete_so->setDisabled(true);

//     QSqlTableModel* storage_object = new QSqlTableModel();
//     storage_object->setTable("storage_objects");
//     storage_object->setFilter("status_value = 10 AND order_ioID = " + QString::number(order_ioID));
//     storage_object->select();

//     if(storage_object->rowCount() == 0)
//     {
//         QSqlQuery update_orders_io;
//         update_orders_io.prepare("UPDATE orders_io "
//                             "SET status_value = :status_value "
//                             "WHERE orders_ioID = :orders_io_id");
//         update_orders_io.bindValue(":orders_io_id", order_ioID);
//         update_orders_io.bindValue(":status_value", 20);
//         if(!update_orders_io.exec())
//         {
//             qDebug() << "FAIL updateing in orders_io";
//             return;
//         }

//         QSqlTableModel* order_id_get = new QSqlTableModel();
//         order_id_get->setTable("orders_io");
//         order_id_get->setFilter("order_ioID = " + QString::number(order_ioID));
//         order_id_get->select();
//         int order_id = ui->tableView_storage_object->model()->index(0,1).data().toInt();
//         delete(order_id_get);

//         QSqlTableModel* orders_io = new QSqlTableModel();
//         orders_io->setTable("orders_io");
//         orders_io->setFilter("status_value = 10 AND orderID = " + QString::number(order_id));
//         orders_io->select();

//         if(orders_io->rowCount() == 0)
//         {
//             QSqlQuery update_orders;
//             update_orders.prepare("UPDATE orders "
//                                 "SET status_value = :status_value "
//                                 "WHERE orderID = :order_id");
//             update_orders.bindValue(":order_id", order_id);
//             update_orders.bindValue(":status_value", 20);
//             if(!update_orders.exec())
//             {
//                 qDebug() << "FAIL updateing in orders_io";
//                 return;
//             }
//         }
//    }
}

