#include "stockoverview.h"
#include "ui_stockoverview.h"

StockOverview::StockOverview(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StockOverview)
{
    ui->setupUi(this);
    con = getConnection();
    //add table-names to drop-down
    ui->comboBoxTables->addItem("bundles");
    ui->comboBoxTables->addItem("client_types");
    ui->comboBoxTables->addItem("clients");
    ui->comboBoxTables->addItem("orders");
    ui->comboBoxTables->addItem("orders_io");
    ui->comboBoxTables->addItem("products");
    ui->comboBoxTables->addItem("shelves");
    ui->comboBoxTables->addItem("slots");
    ui->comboBoxTables->addItem("status_types");
    ui->comboBoxTables->addItem("storage_objects");

    //add a view to the drop-down
    ui->comboBoxTables->addItem("bestand_uebersicht_status20");

    //combobox for filtering
    filterQuery_Produkt = new QSqlQueryModel();
    filterQuery_Produkt->setQuery("SELECT products.product_name FROM products");
    ui->comboBox_Produkt->setModel(filterQuery_Produkt);



    filterQuery_Platz = new QSqlQueryModel();
    filterQuery_Platz->setQuery("SELECT shelves.shelf_label FROM shelves");
    ui->comboBox_Platz->setModel(filterQuery_Platz);



    model_stov_main = new QSqlTableModel();
    model_stov_main->setTable(ui->comboBoxTables->currentText());
    model_stov_main->select();
    ui->tableView_main->setModel(model_stov_main);
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
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
}

void StockOverview::on_pushButton_Close_clicked()
{
    this->close();
}

void StockOverview::on_pushButton_refresh_clicked()
{
  //refresh button for tableView_main
  ui->label_produkt->setText(ui->comboBox_Produkt->currentText());
  model_stov_main = new QSqlTableModel();
  model_stov_main->setTable(ui->comboBoxTables->currentText());
  ui->label_tableName->setText(ui->comboBoxTables->currentText());
  ui->label_platz->setText(ui->comboBox_Platz->currentText());
  model_stov_main->select();
  ui->tableView_main->setModel(model_stov_main);

  //filter by product name
  QString product_name = ui->comboBox_Produkt->currentText();
  model_filterProdukt = new QSqlTableModel();
  model_filterProdukt->setTable("bestand_uebersicht_filter");
  model_filterProdukt->setFilter("product_name = '" +product_name + "'");
  model_filterProdukt->select();
  ui->tableView_produkt->setModel(model_filterProdukt);

  //filter by platz
  QString slot_label = ui->comboBox_Platz->currentText();
  model_filterPlatz = new QSqlTableModel();
  model_filterPlatz->setTable("bestand_uebersicht_filter");
  model_filterPlatz->setFilter("shelf_label = '" +slot_label +"'");
  model_filterPlatz->select();
  ui->tableView_plaetze->setModel(model_filterPlatz);

  //design the table_views
  design(ui->tableView_main);
  design(ui->tableView_produkt);
  design(ui->tableView_plaetze);


}

