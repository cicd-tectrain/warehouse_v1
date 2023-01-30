#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QtSql>
#include<QSqlDatabase>
#include<QMessageBox>
#include<QDebug>
#include<QTableView>
#include <QSqlQuery>
#include <QComboBox>
#include<QSqlQueryModel>
#include<QSqlTableModel>

#include"OrderStock.h"
#include"reciveorder.h"
#include"stockoverview.h"
#include"managestock.h"
#include"customerorder.h"
#include"commissioning.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void connection();
    void design(QTableView* table);
    void show_main_window(); //show_Leser();

private slots:
    void on_pushButton_order_stock_clicked();
    void on_pushButton_recieve_order_clicked();

    void on_pushButton_stock_overview_clicked();
    void on_pushButton_manage_stock_clicked();
    void on_pushButton_create_customer_order_clicked();
    void on_pushButton_commissioning_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QMessageBox msgBox;
    QSqlTableModel* model_main; //model_leser;
    OrderStock* dialog_order_stock;
    ReciveOrder* dialog_recive_order;
    StockOverview* dialog_stock_overview;
    ManageStock* dialog_manage_stock;
    CustomerOrder* dialog_customer_order;
    Commissioning* dialog_commissioning;
    QSqlQueryModel* filterQuery_Produkt;
    QSqlQueryModel* filterQuery_Platz;
    QSqlTableModel* model_filterProdukt;
    QSqlTableModel* model_filterPlatz;
    QComboBox* comboBox_test;
};
#endif // MAINWINDOW_H
