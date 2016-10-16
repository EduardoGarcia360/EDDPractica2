#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QString"
#include "string"
#include "QFile"
#include "QTextStream"
#include "QMessageBox"
#include "QFileDialog"
#include "avl.h"
#include "iostream"
#include "QPainter"

using namespace std;

avl::pnodo carro;
avl arbol_carros;
bool graficar=false;

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

void MainWindow::paintEvent(QPaintEvent *e){
    if(graficar){
        QPixmap pix(591,291);
        pix.fill(Qt::white);
        QPainter paint(&pix);
        paint.drawLine(0,0,15,15);
        ui->lblgrafica1->setPixmap(pix);
    }else{
        QPixmap pix(591,291);
        pix.fill(Qt::white);
        ui->lblgrafica1->setPixmap(pix);
    }
}

void MainWindow::on_btnagregar_clicked()
{
    limpiar();
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
        char* pla = new char[placa.size()+1];
        char* mar = new char[marca.size()+1];
        char* mod = new char[modelo.size()+1];
        char* an = new char[ano.size()+1];
        char* col = new char[color.size()+1];
        char* tra = new char[transmision.size()+1];
        strcpy(pla, placa.toLatin1().data());
        strcpy(mar, marca.toLatin1().data());
        strcpy(mod, modelo.toLatin1().data());
        strcpy(an, ano.toLatin1().data());
        strcpy(col, color.toLatin1().data());
        strcpy(tra, transmision.toLatin1().data());
        int pre = precio.toInt();
        carro = arbol_carros.InsertarAVL(pla,mar,mod,an,col,pre,tra,carro);
        ui->listWidget->addItem("* Se incerto el carro con placa: "+placa);
        limpiar2();
    }
}

void MainWindow::on_btncargar_clicked()
{
    limpiar();
    QString ruta = QFileDialog::getOpenFileName(this,tr("Elija un Archivo."), "/home/eduardo/", "Txt files (*.txt*)");
    QFile archivo(ruta);
    if(!archivo.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::information(this,"Error","No se pudo abrir el archivo.");
    }else{
        QTextStream contenido(&archivo);
        QString texto = contenido.readAll();
        cargar_archivo(texto);
        archivo.close();
        ui->listWidget->addItem("* Se cargaron datos mediante un archivo.");
    }
}

void MainWindow::cargar_archivo(QString contenido){
    char* pla;
    char* mar;
    char* mod;
    char* an;
    char* col;
    int pre, caso=0;
    char* tra;
    QStringList lineas = contenido.split("\n"); //se separan las lineas por el salto de linea.
    for(int h=0; h<lineas.length(); h++){
        QString tmp = lineas.value(h);
        QStringList algo = tmp.split(":");
        for(int w=0; w<algo.length(); w++){
            QString tmp2 = algo.value(w);
            switch(caso){
            case 0:
                pla = new char[tmp2.size()+1];
                strcpy(pla, tmp2.toLatin1().data());
                caso=1;
                break;
            case 1:
                mar = new char[tmp2.size()+1];
                strcpy(mar, tmp2.toLatin1().data());
                caso=2;
                break;
            case 2:
                mod = new char[tmp2.size()+1];
                strcpy(mod, tmp2.toLatin1().data());
                caso=3;
                break;
            case 3:
                an = new char[tmp2.size()+1];
                strcpy(an, tmp2.toLatin1().data());
                caso=4;
                break;
            case 4:
                col = new char[tmp2.size()+1];
                strcpy(col, tmp2.toLatin1().data());
                caso=5;
                break;
            case 5:
                pre = tmp2.toInt();
                caso=6;
                break;
            case 6:
                tra = new char[tmp2.size()+1];
                strcpy(tra, tmp2.toLatin1().data());
                caso=0;
                carro = arbol_carros.InsertarAVL(pla, mar, mod, an, col, pre, tra, carro);
                break;
            }
        }
    }

}

void MainWindow::on_btnbuscar_clicked()
{
    limpiar();
    QString placa = ui->txtplaca->text();
    if(placa.isNull()||placa.isEmpty()){
        QMessageBox::information(this,"Error","Para buscar debes ingresar una placa.");
    }else{
        avl::pnodo ubicado;
        char* pla = new char[placa.size()+1];
        strcpy(pla, placa.toLatin1().data());

        ubicado = arbol_carros.Buscar(pla, carro);
        /*
        cout<<"buscar"<<endl;
        char* dato_nodo = (char*)malloc(10);
        strcpy(dato_nodo, ubicado->modelo);
        QString texto_n = QString::fromStdString(dato_nodo);
        cout<<"contenido del nodo"<<endl;
        cout<<texto_n.toStdString()<<endl;
        **/
        if(ubicado==NULL){
            QMessageBox::information(this,"Error","Dato no encontrado.");
        }else{
            /*char* a qstring*/
            QString texto_nodo;
            char* dato_nodo = (char*)malloc(50);
            strcpy(dato_nodo, ubicado->marca);
            texto_nodo = QString::fromStdString(dato_nodo);
            ui->txtmarca->setPlaceholderText(texto_nodo);

            strcpy(dato_nodo, ubicado->modelo);
            texto_nodo = QString::fromStdString(dato_nodo);
            ui->txtmodelo->setPlaceholderText(texto_nodo);

            strcpy(dato_nodo, ubicado->ano);
            texto_nodo = QString::fromStdString(dato_nodo);
            ui->txtano->setPlaceholderText(texto_nodo);

            strcpy(dato_nodo, ubicado->color);
            texto_nodo = QString::fromStdString(dato_nodo);
            ui->txtcolor->setPlaceholderText(texto_nodo);

            texto_nodo = QString::number(ubicado->precio);
            ui->txtprecio->setPlaceholderText(texto_nodo);
            ui->listWidget->addItem("* Se busco el auto con placa: "+placa);
        }

    }
}

void MainWindow::on_btneliminar_clicked()
{
    limpiar();
    QString placa = ui->txtplaca->text();
    if(placa.isNull()||placa.isEmpty()){
        QMessageBox::information(this,"Error","Para eliminar debes ingresar una placa.");
    }else{
        avl::pnodo ubicado;
        char* pla = new char[placa.size()+1];
        strcpy(pla, placa.toLatin1().data());
        ubicado = arbol_carros.DescartarAVL(pla, carro);
        ui->listWidget->addItem("* Se elimino el auto con placa: "+ placa);
    }
}

void MainWindow::on_btnmodificar_clicked()
{
    limpiar();
    QString placa = ui->txtplaca->text();
    if(placa.isNull()||placa.isEmpty()){
        QMessageBox::information(this,"Error","Para modificar debes ingresar una placa.");
    }else{
        avl::pnodo ubicado;
        char* pla = new char[placa.size()+1];
        strcpy(pla, placa.toLatin1().data());
        ubicado = arbol_carros.Buscar(pla, carro);
        if(ubicado==NULL){
            QMessageBox::information(this,"Error","Placa no encontrada.");
        }else{
            QString marca, modelo, ano, color, precio, transmision;
            int precio1=0;
            marca=ui->txtmarca->text();
            modelo=ui->txtmodelo->text();
            ano=ui->txtano->text();
            color=ui->txtcolor->text();
            precio=ui->txtprecio->text();
            transmision=ui->cboxtransmision->currentText();
            char* mar;
            char* mod;
            char* an;
            char* col;
            char* tra;
            if(marca.isNull()||marca.isEmpty()){
                mar = new char[1];
                strcpy(mar, "n");
            }else{
                mar = new char[marca.size()+1];
                strcpy(mar, marca.toLatin1().data());
            }
            if(modelo.isNull()||modelo.isEmpty()){
                mod = new char[1];
                strcpy(mod, "n");
            }else{
                mod = new char[modelo.size()+1];
                strcpy(mod, modelo.toLatin1().data());
            }
            if(ano.isNull()||ano.isEmpty()){
                an = new char[1];
                strcpy(an, "n");
            }else{
                an = new char[ano.size()+1];
                strcpy(an, ano.toLatin1().data());
            }
            if(color.isNull()||color.isEmpty()){
                col = new char[1];
                strcpy(col, "n");
            }else{
                col = new char[color.size()+1];
                strcpy(col, color.toLatin1().data());
            }
            if(precio.isNull()||precio.isEmpty()){
                precio1=0;
            }else{
                precio1 = precio.toInt();
            }
            tra = new char[transmision.size()+1];
            strcpy(tra, transmision.toLatin1().data());

            carro = arbol_carros.Modificar(pla, mar, mod, an, col, precio1, tra, carro);
            ui->listWidget->addItem("* Se modifico el auto con placa: "+placa);
            limpiar2();
        }
    }
}

void MainWindow::limpiar(){
    ui->txtmarca->setPlaceholderText("");
    ui->txtmodelo->setPlaceholderText("");
    ui->txtprecio->setPlaceholderText("");
    ui->txtano->setPlaceholderText("");
    ui->txtcolor->setPlaceholderText("");
}

void MainWindow::limpiar2(){
    ui->txtplaca->setText("");
    ui->txtmarca->setText("");
    ui->txtmodelo->setText("");
    ui->txtano->setText("");
    ui->txtcolor->setText("");
    ui->txtprecio->setText("");
}

void MainWindow::on_pushButton_clicked()
{
    arbol_carros.preorder(carro);
}
