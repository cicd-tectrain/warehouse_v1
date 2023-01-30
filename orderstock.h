#ifndef ORDERSTOCK_H
#define ORDERSTOCK_H

#include <QDialog>
#include<QtSql>
#include<QSqlDatabase>
#include<QTableView>
#include<QMessageBox>

namespace Ui {
class OrderStock;
}

class OrderStock : public QDialog
{
    Q_OBJECT

public:
    explicit OrderStock(QWidget *parent = nullptr);
    ~OrderStock();
    void design(QTableView* table);
    void setConnection(QSqlDatabase _con);
    QSqlDatabase getConnection();
    void show_incoming_orders();
    void show_products();
    void show_suplier();


private slots:
    void on_pushButton_Close_clicked();
    void on_pushButton_select_suplier_clicked();
    void on_pushButton_select_product_clicked();
    void on_pushButton_add_product_clicked();
    void on_pushButton_finish_clicked();

private:
    Ui::OrderStock *ui;
    QSqlDatabase con;       //connection
    //QSqlTableModel* model_buecher;
    int suplier_id;
    int product_id;
    double product_amount;
    QMessageBox msgBox;
    void create_storage_objects(int order_io_id, int product_id, double amount);
};



#endif // ORDERSTOCK_H
