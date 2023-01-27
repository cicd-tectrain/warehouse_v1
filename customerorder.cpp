#include "customerorder.h"
#include "ui_customerorder.h"

CustomerOrder::CustomerOrder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomerOrder)
{
    ui->setupUi(this);
    con = getConnection();
}

CustomerOrder::~CustomerOrder()
{
    delete ui;
}


void CustomerOrder::setConnection(QSqlDatabase _con)
{
    con = _con;
}

QSqlDatabase CustomerOrder::getConnection()
{
    return con;
}

void CustomerOrder::design(QTableView* table)
{
//    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    table->verticalHeader()->setVisible(false);
//    table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
}

void CustomerOrder::on_pushButton_Close_clicked()
{
    this->close();
}
