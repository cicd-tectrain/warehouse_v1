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

private slots:
    void on_pushButton_Close_clicked();

private:
    Ui::Commissioning *ui;
    QSqlDatabase con;
};

#endif // COMMISSIONING_H
