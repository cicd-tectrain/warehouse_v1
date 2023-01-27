#ifndef STOCKOVERVIEW_H
#define STOCKOVERVIEW_H

#include <QDialog>
#include<QtSql>
#include<QSqlDatabase>
#include<QTableView>
#include<QMessageBox>

namespace Ui {
class StockOverview;
}

class StockOverview : public QDialog
{
    Q_OBJECT

public:
    explicit StockOverview(QWidget *parent = nullptr);
    ~StockOverview();
    void design(QTableView* table);
    void setConnection(QSqlDatabase _con);
    QSqlDatabase getConnection();

private slots:
    void on_pushButton_Close_clicked();

private:
    Ui::StockOverview *ui;
    QSqlDatabase con;
};

#endif // STOCKOVERVIEW_H
