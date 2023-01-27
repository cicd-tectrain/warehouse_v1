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

private slots:
    void on_pushButton_Close_clicked();

private:
    Ui::CustomerOrder *ui;
    QSqlDatabase con;
};

#endif // CUSTOMERORDER_H
