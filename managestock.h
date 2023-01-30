#ifndef MANAGESTOCK_H
#define MANAGESTOCK_H

#include <QDialog>
#include<QtSql>
#include<QSqlDatabase>
#include<QTableView>
#include<QMessageBox>

namespace Ui {
class ManageStock;
}

class ManageStock : public QDialog
{
    Q_OBJECT

public:
    explicit ManageStock(QWidget *parent = nullptr);
    ~ManageStock();
    void design(QTableView* table);
    void setConnection(QSqlDatabase _con);
    QSqlDatabase getConnection();
    void show_storage_objects();
    void show_free_slots();

private slots:
    void on_pushButton_Close_clicked();
    void on_pushButton_select_storage_object_clicked();
    void on_pushButton_select_slot_clicked();
    void on_pushButton_change_slot_clicked();
    void on_pushButton_change_amount_clicked();
    void on_pushButton_delete_so_clicked();

private:
    Ui::ManageStock *ui;
    QSqlDatabase con;
    QMessageBox msgBox;
    long long storage_objectID;
    int old_slotID;
    int new_slotID = NULL;
    double amount;
    int order_ioID;
};

#endif // MANAGESTOCK_H
