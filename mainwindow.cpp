#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QString"
#include "string.h"
#include "QFile"
#include "QTextStream"
#include "QMessageBox"
#include "QFileDialog"

using namespace std;

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

void MainWindow::on_btncargar_clicked()
{
    QString ruta = QFileDialog::getOpenFileName(this,tr("Elija un Archivo."), "/home/eduardo/", "Txt files (*.txt*)");
    QFile archivo(ruta);
    if(!archivo.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::information(this,"Error","No se pudo abrir el archivo.");
    }else{
        QTextStream contenido(&archivo);
        QString texto = contenido.readAll();
        cargar_archivo(texto);
        archivo.close();
    }
}

void MainWindow::cargar_archivo(QString contenido){
    QStringList lineas = contenido.split("\n"); //se separan las lineas por el salto de linea.
    for(int i=0;i<lineas.length()-1;i++){
        QString linea_actual = lineas.value(i);
        if(!linea_actual.isNull() || !linea_actual.isEmpty()){ //si no esta vacia o nula
            int caso=1;
            QString dato="";
            for(int j=0;j<linea_actual.length()-1;j++){
                QString caracter = linea_actual.at(j);
                if(caracter!=";"){
                    if(caracter!=":"){
                        dato+=caracter;
                    }else{
                        if(caso==1){
                            //placa
                            caso++;
                        }else if(caso==2){
                            //marca
                            caso++;
                        }else if(caso==3){
                            //modelo
                            caso++;
                        }else if(caso==4){
                            //ano
                            caso++;
                        }else if(caso==5){
                            //color
                            caso++;
                        }else if(caso==6){
                            //precio
                            caso++;
                        }else if(caso==7){
                            //transmision
                            caso-=6;
                        }
                    }
                }
            }
        }
    }
}
