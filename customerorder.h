#ifndef CUSTOMERORDER_H
#define CUSTOMERORDER_H

#include <QDialog>
#include<QtSql>
#include<QSqlDatabase>
#include<QTableView>
#include<QMessageBox>

namespace Ui {
class CustomerOrder;
}

class CustomerOrder : public QDialog
{
    Q_OBJECT

public:
    explicit CustomerOrder(QWidget *parent = nullptr);
    ~CustomerOrder();
    void design(QTableView* table);
    void setConnection(QSqlDatabase _con);
    QSqlDatabase getConnection();
    void show_outgoing_orders();
    void show_products();
    void show_customers();

private slots:
    void on_pushButton_Close_clicked();
    void on_pushButton_select_customer_clicked();
    void on_pushButton_select_product_clicked();
    void on_pushButton_add_product_clicked();
    void on_pushButton_finish_order_clicked();

private:
    Ui::CustomerOrder *ui;
    QSqlDatabase con;
    int customer_id;
    int product_id;
    double product_amount;
    QMessageBox msgBox;
    void book_storage_objects(int out_order_io_id, int product_id, double amount);
    int change_storage_object_status(long long soID, int status, int out_order_ioID);
    int change_orders_io_amount(int ioID, double amount_available, double amount_commissioned);
    int create_remainder_so(int out_order_ioid, double amount);
    int change_amount_on_so(long long soID, double amount, int status);
};

#endif // CUSTOMERORDER_H
