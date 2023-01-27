#include "managestock.h"
#include "ui_managestock.h"

ManageStock::ManageStock(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManageStock)
{
    ui->setupUi(this);
    con = getConnection();
}

ManageStock::~ManageStock()
{
    delete ui;
}


void ManageStock::setConnection(QSqlDatabase _con)
{
    con = _con;
}

QSqlDatabase ManageStock::getConnection()
{
    return con;
}

void ManageStock::design(QTableView* table)
{
//    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    table->verticalHeader()->setVisible(false);
//    table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
}

void ManageStock::on_pushButton_Close_clicked()
{
    this->close();
}
