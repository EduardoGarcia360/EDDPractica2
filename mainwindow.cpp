#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QString"
#include "QMessageBox"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->cboxtransmision->addItem("Automatica");
    ui->cboxtransmision->addItem("Mecanica");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnagregar_clicked()
{
    QString placa, marca, modelo, ano, color, precio, transmision;
    placa=ui->txtplaca->text();
    marca=ui->txtmarca->text();
    modelo=ui->txtmodelo->text();
    ano=ui->txtano->text();
    color=ui->txtcolor->text();
    precio=ui->txtprecio->text();
    transmision=ui->cboxtransmision->currentText();
    if(placa.isNull()||marca.isNull()||modelo.isNull()||ano.isNull()||color.isNull()||precio.isNull()||transmision.isNull()){
        QMessageBox::information(this,"Error","Debes llenar todas las casillas.");
    }else{
        //insertar en el arbol
    }
}
