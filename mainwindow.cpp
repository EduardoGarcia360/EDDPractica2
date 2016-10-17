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
avl::pnodo carroespejo;
avl arbol_carros;
avl arbol_espejo;

bool graficar=false;
QString algo="";
int num=0;
QButtonGroup* gra_arbol = new QButtonGroup;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    gra_arbol->addButton(ui->rbtnarbol);
    gra_arbol->addButton(ui->rbtnespejo);
    gra_arbol->addButton(ui->rbtnarreglo);
    ui->cboxtransmision->addItem("Automatica");
    ui->cboxtransmision->addItem("Mecanica");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *e){
    if(graficar){
        grafica(algo, num);
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

    if(placa.isNull()||marca.isNull()||modelo.isNull()||ano.isNull()||color.isNull()||precio.isNull()||transmision.isNull()||
       placa.isEmpty()||marca.isEmpty()||modelo.isEmpty()||ano.isEmpty()||color.isEmpty()||precio.isEmpty()||transmision.isNull()){
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
    //limpiar();
    QString ruta = QFileDialog::getOpenFileName(this,"Elija un Archivo.","/home/eduardo/Escritorio/cargar.txt", "Txt files (*.txt*)");
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

    algo=arbol_carros.preorder(carro);
    graficar=true;
    update();
    //grafica(algo);
}

void MainWindow::on_btnbuscar_clicked()
{
    limpiar();
    QString placa = ui->txtplaca->text();
    if(placa.isNull()||placa.isEmpty()){
        //QMessageBox::information(this,"Error","Para buscar debes ingresar una placa.");
        int h = arbol_carros.nodos_hoja(carro);
        cout<<h<<endl;
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

void MainWindow::grafica(QString contenido, int n){
    QPixmap pix(871,551);
    pix.fill(Qt::white);
    QPainter paint(&pix);

    QStringList NodoSeparados = contenido.split("#");
    int altura_del_arbol=0;
    if(n==1){
        altura_del_arbol = arbol_carros.Altura();
    }else{
        altura_del_arbol = arbol_espejo.Altura();
    }

    int altura_del_label = ui->lblgrafica1->height();
    int ancho_del_label = ui->lblgrafica1->width();

    int nivel1 = altura_del_label/altura_del_arbol;
    int nivel2 = nivel1*2;
    int nivel3 = nivel1*3;
    int nivel4 = nivel1*4;

    for(int i=0;i<NodoSeparados.length()-1;i++){
        QString linea = NodoSeparados.value(i);
        QStringList Datos = linea.split(";");
        if(Datos.length()==3){
            //Izq;111ryt;3# -> [izq][111ryt][3]
            if(Datos.value(0) == "Ra"){
                //cuadro
                int medio = ancho_del_label / 2;
                paint.drawRect(medio-35,0,70,50);
                //linea izq
                paint.drawLine(medio-35,50,medio-100,70);
                //linea der
                paint.drawLine(medio+35,50,medio+100,70);
                //ui->lblgrafica1->setPixmap(pix);
                ui->lblgrafica2->setPixmap(pix);
            }else if(Datos.value(0) == "Izq"){
                //cuadro

            }else{

            }
        }else{
            //Izq;Der;Der;256ptx;3# -> [Izq][Der][Der][256ptx][3]
        }
    }
}

void MainWindow::crear_espejo(){
    QString todo = arbol_carros.preorder_espejo(carro);

    char* pla;
    char* mar;
    char* mod;
    char* an;
    char* col;
    int pre, caso=0;
    char* tra;
    QStringList lineas = todo.split("#"); //se separan las lineas por el salto de linea.
    for(int h=0; h<lineas.length(); h++){
        QString tmp = lineas.value(h);
        QStringList algo = tmp.split(";");
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
                carroespejo = arbol_espejo.EspejoAVL(pla, mar, mod, an, col, pre, tra, carroespejo);
                break;
            }
        }
    }
}

void MainWindow::on_btngraficar_clicked()
{
    if(ui->rbtnarbol->isChecked()){
        int a = arbol_carros.Altura();
        ui->lblaltura->setText(QString::number(a));
        algo=arbol_carros.preorder(carro);
        graficar=true;
        num=1;
        update();
    }
    if(ui->rbtnarreglo->isChecked()){

    }
    if(ui->rbtnespejo->isChecked()){
        crear_espejo();
        int a = arbol_espejo.Altura();
        ui->lblaltura->setText(QString::number(a));
        algo=arbol_espejo.preorder(carroespejo);
        graficar=true;
        update();
    }

}
