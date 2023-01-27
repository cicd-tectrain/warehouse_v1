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

private slots:
    void on_pushButton_Close_clicked();

private:
    Ui::ManageStock *ui;
    QSqlDatabase con;
};

#endif // MANAGESTOCK_H
