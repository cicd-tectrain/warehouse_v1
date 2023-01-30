#ifndef STOCKOVERVIEW_H
#define STOCKOVERVIEW_H

#include <QDialog>
#include<QtSql>
#include<QSqlDatabase>
#include<QTableView>
#include<QMessageBox>
#include<QComboBox>
#include<QSqlQueryModel>
#include<QSqlTableModel>
#include<QString>

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

    void on_pushButton_refresh_clicked();

private:
    Ui::StockOverview *ui;
    QSqlDatabase con;
    QComboBox* comboBoxTables;
    QComboBox* comboBox_Platz;
    QComboBox* comboBox_Produkt;
    QSqlQueryModel* filterQuery_Produkt;
    QSqlQueryModel* filterQuery_Platz;
    QSqlTableModel* model_filterProdukt;
    QSqlTableModel* model_filterPlatz;
    QSqlTableModel* model_stov_main;


};

#endif // STOCKOVERVIEW_H
