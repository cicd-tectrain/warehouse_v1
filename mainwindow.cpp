#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connection();

    show_main_window();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connection()
{
    db = QSqlDatabase::addDatabase("QODBC");
    QString connString = "Driver={MySql ODBC 8.0 ANSI Driver};";
            connString += "SERVER=localhost:3307;DATABASE=mydb;";
            connString += "USER=root;PASSWORD=1234;";

    db.setDatabaseName(connString);
}

//void MainWindow::design(QTableView* table)
//{
//    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    table->verticalHeader()->setVisible(false);
//    table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
//}

void MainWindow::show_main_window()
{
//    if(db.open())
//    {
//        //msgBox.information(this,"Info","Your are connected");
//        //qDebug() << "Your are connected";
//        model_main = new QSqlTableModel();
//        model_main->setTable("clients");
//        model_main->select();
//        ui->tableView_main->setModel(model_main);
//        design(ui->tableView_main);
//        connect(ui->tableView_main->selectionModel(),
//               SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
//                SLOT(on_leser_verleih()));

//    }
//    else
//    {
//        //msgBox.information(this,"Failed","The connection failed");
//        qDebug() << "The connection failed";
//    }
}

void MainWindow::on_pushButton_order_stock_clicked()
{
    dialog_order_stock = new OrderStock(this);
    dialog_order_stock->setModal(true);
    dialog_order_stock->exec();
    dialog_order_stock->setConnection(db);

}

void MainWindow::on_pushButton_recieve_order_clicked()
{
    dialog_recive_order = new ReciveOrder(this);
    dialog_recive_order->setModal(true);
    dialog_recive_order->exec();
    dialog_recive_order->setConnection(db);

}
