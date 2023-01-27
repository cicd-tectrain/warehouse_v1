#include "stockoverview.h"
#include "ui_stockoverview.h"

StockOverview::StockOverview(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StockOverview)
{
    ui->setupUi(this);
    con = getConnection();
}

StockOverview::~StockOverview()
{
    delete ui;
}


void StockOverview::setConnection(QSqlDatabase _con)
{
    con = _con;
}

QSqlDatabase StockOverview::getConnection()
{
    return con;
}

void StockOverview::design(QTableView* table)
{
//    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    table->verticalHeader()->setVisible(false);
//    table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
}

void StockOverview::on_pushButton_Close_clicked()
{
    this->close();
}
