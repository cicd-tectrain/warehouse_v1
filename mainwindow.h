#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QtSql>
#include<QSqlDatabase>
#include<QMessageBox>
#include<QDebug>
#include<QTableView>
#include <QSqlQuery>

#include"OrderStock.h"
#include"reciveorder.h"


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
    void show_main_window();

private slots:
    void on_pushButton_order_stock_clicked();
    void on_pushButton_recieve_order_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QMessageBox msgBox;
    QSqlTableModel* model_main;
    OrderStock* dialog_order_stock;
    ReciveOrder* dialog_recive_order;
};
#endif // MAINWINDOW_H
