#ifndef COMMISSIONING_H
#define COMMISSIONING_H

#include <QDialog>
#include<QtSql>
#include<QSqlDatabase>
#include<QTableView>
#include<QMessageBox>

namespace Ui {
class Commissioning;
}

class Commissioning : public QDialog
{
    Q_OBJECT

public:
    explicit Commissioning(QWidget *parent = nullptr);
    ~Commissioning();
    void design(QTableView* table);
    void setConnection(QSqlDatabase _con);
    QSqlDatabase getConnection();
    void show_orders();
    void show_order_io(int id);
    void show_storage_object(int id);
    void show_slots(long long int id);


private slots:
    void on_pushButton_Close_clicked();
    void on_pushButton_select_order_clicked();
    void on_pushButton_select_order_io_clicked();
    void on_pushButton_select_storage_object_clicked();
    void on_pushButton_confirm_commissioning_clicked();


private:
    Ui::Commissioning *ui;
    QSqlDatabase con;
    QMessageBox msgBox;
    int order_id;
    int orders_io_id;
    long long int storage_object_id;
    int slot_id;
};

#endif // COMMISSIONING_H
