#include "commissioning.h"
#include "ui_commissioning.h"

Commissioning::Commissioning(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Commissioning)
{
    ui->setupUi(this);
    con = getConnection();
}

Commissioning::~Commissioning()
{
    delete ui;
}

void Commissioning::setConnection(QSqlDatabase _con)
{
    con = _con;
}

QSqlDatabase Commissioning::getConnection()
{
    return con;
}

void Commissioning::design(QTableView* table)
{
//    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    table->verticalHeader()->setVisible(false);
//    table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
}

void Commissioning::on_pushButton_Close_clicked()
{
    this->close();
}
