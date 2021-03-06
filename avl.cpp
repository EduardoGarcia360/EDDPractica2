#include "avl.h"
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <sstream>
using namespace std;

avl::avl(){}
# define max(A,B) ((A)>(B)?(A):(B)) /* Definición de macros */
# define min(A,B) ((A)>(B)?(B):(A))
int flag; /* Marca para registrar cambios de altura. En rebalance ascendente */

int alto_avl = 0; /* Altura árbol avl. Número de nodos desde la raíz a las hojas.*/
//avl::Datos *data;
std::stringstream enzabezado;
typedef avl::nodo *pnodo;

/* Variable global, para disminuir argumentos */
int edad;
char *nombre;
char *apellido;
char* placa1;
char* marca;
char* modelo;
char* ano;
char* color;
char* transmision;
int precio, alt=1, k=1, hoja=0;
QString pre="", arre="", espe="";
bool laraiz=true;

/* Rotación Izquierda *
 *  A           B
 * / \         / \
 * a  B ==>   A   c
 *   / \     / \
 *   b  c    a  b
 * Sólo cambian los factores de balance de los nodos A y B
 * Los factores de balance de los sub-árboles no cambian. */
pnodo avl::lrot(pnodo t)
{
    pnodo temp;
    int x,y;
    temp = t;
    t = t->right;
    temp->right = t->left;
    t->left = temp;
    //Recalcula factores de balance de los dos nodos
    x = temp->bal; // oldbal(A)
    y = t->bal; // oldbal(B)
    temp->bal = x-1-max(y, 0); // nA
    t->bal = min(x-2+min(y, 0), y-1); // nB
    return t;
}

/* Rotación derecha
 *
 *   A         B
 *  / \       / \
 *  B  c ==> a   A
 * / \          / \
 * a  b        b   c
 *
 */
pnodo avl::rrot(pnodo t)
{
    pnodo temp = t;
    int x,y;
    t = t->left;
    temp->left = t->right;
    t->right = temp;
    x = temp->bal; // oldbal(A)
    y = t->bal;    // oldbal(B)
    temp->bal = x+1-min(y, 0);  // nA
    t->bal = max(x+2+max(y, 0), y+1); //nB
    return t;
}

void avl::Error(int tipo)
{
    if (tipo) printf("\nError en inserción\n");
    else printf("\nError en descarte\n");
}

int avl::Altura(void)
{
    return alto_avl;
}

pnodo avl::CreaNodo(char* placa)
{
    pnodo t;
    t = (pnodo) malloc(sizeof(nodo));
    t->placa=placa;
    t->marca=marca;
    t->modelo=modelo;
    t->ano=ano; //declarado arriba
    t->color=color;
    t->precio=precio;
    t->transmision=transmision;

    t->left=NULL;//puse null en vez de 0
    t->right=NULL;
    return t;
}

pnodo avl::insertR(pnodo t)
{
    if (t == NULL)  /* Llegó a un punto de inserción */
    {
        t = CreaNodo(placa1);
        t->bal = 0; /* Los dos hijos son nulos */
        flag = 1; /* Marca necesidad de revisar balances */
        return t; /* retorna puntero al insertado */
    }
    else if(strcmp(t->placa, placa1)<0)
    {
        //desciende por la derecha
        t->right = insertR(t->right);
        //se pasa por la siguiente línea en la revisión ascendente
        t->bal += flag; /* Incrementa factor de balance */
    }
    else if(strcmp(t->placa, placa1)>0)
    {
        //desciende por la izquierda
        t->left = insertR(t->left);
        //se corrige en el ascenso
        t->bal -= flag; /* Decrementa balance */
    }
    else   /* (t->k == key) Ya estaba en el árbol */
    {
        Error(1);
        flag = 0;
    }

    if (flag == 0) /* No hay que rebalancear. Sigue el ascenso */
        return t;

    /*El código a continuación es el costo adicional para mantener propiedad AVL */
    /* Mantiene árbol balanceado avl. Sólo una o dos rotaciones por inserción */
    if (t->bal < -1)
    {
        /* Quedó desbalanceado por la izquierda. Espejos Casos c y d.*/
        if (t->left->bal > 0)
        /* Si hijo izquierdo está cargado a la derecha */
            t->left = lrot(t->left);
        t = rrot(t);
        flag = 0; /* El subárbol no aumenta su altura */
    }
    else if (t->bal > 1)
    {
        /* Si quedó desbalanceado por la derecha: Casos c y d.*/
        if (t->right->bal < 0)
        /* Si hijo derecho está cargado a la izquierda Caso d.*/
            t->right = rrot(t->right);
        t = lrot(t); /* Caso c.*/
        flag = 0; /* El subárbol no aumenta su altura */
    }
    else if (t->bal == 0)/* La inserción lo balanceo */
        flag = 0; /* El subárbol no aumenta su altura. Caso a*/
    else /* Quedó desbalanceado con -1 ó +1 Caso b */
        flag = 1; /* Propaga ascendentemente la necesidad de rebalancear */
    return t;
}

pnodo avl::espejoR(pnodo t){
    if (t == NULL)  /* Llegó a un punto de inserción */
    {
        t = CreaNodo(placa1);
        t->bal = 0; /* Los dos hijos son nulos */
        flag = 1; /* Marca necesidad de revisar balances */
        return t; /* retorna puntero al insertado */
    }
    else if(strcmp(t->placa, placa1)>0)
    {
        //desciende por la derecha
        t->right = insertR(t->right);
        //se pasa por la siguiente línea en la revisión ascendente
        t->bal += flag; /* Incrementa factor de balance */
    }
    else if(strcmp(t->placa, placa1)<0)
    {
        //desciende por la izquierda
        t->left = insertR(t->left);
        //se corrige en el ascenso
        t->bal -= flag; /* Decrementa balance */
    }
    else   /* (t->k == key) Ya estaba en el árbol */
    {
        Error(1);
        flag = 0;
    }

    if (flag == 0) /* No hay que rebalancear. Sigue el ascenso */
        return t;

    /*El código a continuación es el costo adicional para mantener propiedad AVL */
    /* Mantiene árbol balanceado avl. Sólo una o dos rotaciones por inserción */
    if (t->bal < -1)
    {
        /* Quedó desbalanceado por la izquierda. Espejos Casos c y d.*/
        if (t->left->bal > 0)
        /* Si hijo izquierdo está cargado a la derecha */
            t->left = lrot(t->left);
        t = rrot(t);
        flag = 0; /* El subárbol no aumenta su altura */
    }
    else if (t->bal > 1)
    {
        /* Si quedó desbalanceado por la derecha: Casos c y d.*/
        if (t->right->bal < 0)
        /* Si hijo derecho está cargado a la izquierda Caso d.*/
            t->right = rrot(t->right);
        t = lrot(t); /* Caso c.*/
        flag = 0; /* El subárbol no aumenta su altura */
    }
    else if (t->bal == 0)/* La inserción lo balanceo */
        flag = 0; /* El subárbol no aumenta su altura. Caso a*/
    else /* Quedó desbalanceado con -1 ó +1 Caso b */
        flag = 1; /* Propaga ascendentemente la necesidad de rebalancear */
    return t;
}

pnodo avl::EspejoAVL(char *placa, char *marca1, char *modelo1, char *ano1, char *color1, int precio1, char *transmision1, pnodo t){
    placa1 = placa; //pasa argumento a global.
    marca=marca1;
    modelo=modelo1;
    ano=ano1;
    color=color1;
    precio=precio1;
    transmision=transmision1;

    t = espejoR(t);

    if (flag == 1)
        alto_avl++;
    //si la propagación llega hasta la raíz, aumenta la altura.
    return t;
}

/* Mantiene variable global con el alto del árbol. */
pnodo avl::InsertarAVL(char* placa, char* marca1, char* modelo1, char* ano1, char* color1, int precio1, char* transmision1, pnodo t)
{
    placa1 = placa; //pasa argumento a global.
    marca=marca1;
    modelo=modelo1;
    ano=ano1;
    color=color1;
    precio=precio1;
    transmision=transmision1;

    t = insertR(t);

    if (flag == 1)
        alto_avl++;
    //si la propagación llega hasta la raíz, aumenta la altura.
    return t;
}

pnodo avl::deleteR(pnodo t)
{
    pnodo p;
    bool r=false;
    if (t == NULL)  /* No encontró nodo a descartar */
    {
        Error(0);
        flag = 0;
    }
    else if (t->placa < placa1)
    {
        //Comienza el descenso por la derecha
        t->right = deleteR(t->right);
        //aquí se llega en el retorno ascendente.
        t->bal -= flag; /* Se descartó por la derecha. Disminuye factor */
        //Se retorna después de la revisión de los factores
    }
    else if (t->placa > placa1)
    {
        //Desciende por la izquierda
        t->left = deleteR(t->left);
        //o se llega por esta vía si se descartó por la izquierda.
        t->bal += flag; /* se descartó por la izq. Aumenta factor de balance */
    }
    else   /* (t->placa == placa1) */
    {
        cout<<"t.placa igual placa1"<<endl;
        /* Encontró el nodo a descartar */
        if(t->left==0 && t->right==0){
            cout<<"raiz"<<endl;
            free(t);
            flag = 0;
            r=true;
            return NULL;
        }else if (t->left == NULL)   /*Si hay hijo derecho debe ser hoja, por ser AVL */
        {
            p = t;
            t = t->right;
            free(p);
            flag = 1; /* Debe seguir revisando factores de balance */
            return t; /* ascendentemente */
        }
        else if (t->right == NULL)   /*Si hay hijo izquierdo debe ser hoja */
        {
            p = t;
            t = t->left;
            free(p);
            flag = 1; /* Asciende revisando factores de balance */
            return t; /* Corrigiendo */
        }
        else   /* Tiene dos hijos */
        {
            if (t->bal<0)
            {
                /* Si cargado a la izquierda, elimina mayor descendiente hijo izq */
                p = t->left;
                while (p->right != NULL) p = p->right;
                t->placa = p->placa;
                placa1 = p->placa; //busca hoja a eliminar
                t->left = deleteR(t->left);
                t->bal += flag; /* incrementa factor de balance */
            }
            else   /* Si cargado a la derecha, elimina menor descendiente hijo der */
            {
                p = t->right;
                while (p->left != NULL)
                    p = p->left;
                t->placa = p->placa;
                placa1 = p->placa;
                t->right = deleteR(t->right);
                t->bal -= flag; /* decrementa balance */
            }
        }
    }

    /* Mantiene árbol balanceado avl. Sólo una o dos rotaciones por descarte */
    if(flag==0 && r==true){
        return NULL;
    }else
    if (flag == 0 ){ /* No hay que rebalancear. Sigue el ascenso, sin rebalancear */
        return t;
     }

    /* Hay que revisar factores de balance en el ascenso*/
    if (t->bal < -1)  /* Si quedó desbalanceado por la izquierda y dejó de ser AVL */
    {
        if (t->left->bal > 0)  /*espejos casos c, d y e */
        {
            /* Si el hijo izquierdo está cargado a la derecha */
            t->left = lrot(t->left);
            flag = 1; /*Continuar revisando factores */
        }
        else if (t->left->bal == 0)
            flag = 0; /*No debe seguir el rebalance */
        else
            flag = 1;/* Debe seguir revisando factores de balance */
        t = rrot(t);
    }
    else if (t->bal > 1)  /* Si quedó desbalaceado por la derecha */
    {
        if (t->right->bal < 0)
        {
            /* Si hijo derecho está cargado a la izquierda */
            t->right = rrot(t->right);
            flag = 1; //debe seguir revisando. Caso d.
        }
        else if (t->right->bal == 0)
            flag = 0; /* No debe seguir el rebalance. Caso c. */
        else //positivo
            flag = 1;/* Debe seguir revisando factores de balance. Caso e. */
        t = lrot(t);
    }
    else if (t->bal == 0) /* Si estaba en +1 ó -1 y queda en cero */
        flag = 1; /* Debe seguir corrigiendo. Caso b.*/
    else /* Si estaba en cero y queda en -1 ó +1 */
        flag = 0; /* No debe seguir rebalanceando. Caso a.*/
    return t;
}

pnodo avl::DescartarAVL(char* placa, pnodo t)
{
    pnodo p;
    placa1 = placa;
    p = deleteR(t);
    if (flag == 1) alto_avl--;
    return p;
}

pnodo avl::deltreeR(pnodo t)
{
    if (t != NULL)
    {
        t->left = deltreeR(t->left);
        t->right = deltreeR(t->right);
        free(t); //borra la raíz subárbol
    }
    return NULL;
}

pnodo avl::deltree(pnodo t)
{
    alto_avl = 0;
    return deltreeR(t);
}

void avl::g(pnodo raiz){
   enzabezado.str("");

   InOrder(raiz);
   char *ruta="dot -Tpng arbol.dot -o arbol.png";

   std::string tod=enzabezado.str(); //se concatena todo en una variable string
   char *pass=new char[tod.length()+1]; //se declara un char* aux q xontendra todo lo de la variable "todo"
   strcpy(pass,tod.c_str());

   std::stringstream cuerpo;
    cuerpo <<"digraph G {node[shape=circle, style=filled];\n edge[color=blue];rankdir=UD \n ""\n"; //Encabezado (siempre va esto)

    cuerpo<<pass;
    cuerpo<< "\n}";
     std::string todo=cuerpo.str(); //se concatena todo en una variable string
     char *pass1=new char[todo.length()+1]; //se declara un char* aux q xontendra todo lo de la variable "todo"
     strcpy(pass1,todo.c_str()); //se igualan los valores

        //todo esto necesario para escribir un archivo
        FILE * pFile;
        pFile = fopen ("arbol.dot","w");
        if (pFile!=NULL)
        {
           fputs (pass1,pFile);
           fclose (pFile);

           system(ruta);
           system ("gthumb arbol.png");
          }
}

void avl::InOrder(pnodo raiz)
{
    if(raiz)
   {
        InOrder(raiz->left);
        printf("llave: %p", raiz->placa);
        enzabezado<<"\""<<raiz->placa
                  <<"\"  [ label = \" Placa: "<<raiz->placa
                  <<' '<<"\n Marca: "<<raiz->marca
                  <<' '<<"\n Modelo: "<<raiz->modelo
                  <<' '<<"\n Ano: "<<raiz->ano
                  <<' '<<"\n Color: "<<raiz->color
                  <<' '<<"\n Precio: "<<raiz->precio
                  <<' '<<"\n Transmision: "<<raiz->transmision
                  <<' '<<"  \" shape = \"ellipse\" ] ; \n";

        if(raiz->left){
            printf("left child: %p ", raiz->left->placa);

            enzabezado<<"\""<<raiz->placa<<"\"->\""<<raiz->left->placa<<"\";\n";
         }
        if(raiz->right){
            printf("right child: %p ", raiz->right->placa);
            enzabezado<<"\""<<raiz->placa<<"\"->\""<<raiz->right->placa<<"\";\n";
         }
      printf("\n");
        InOrder(raiz->right);
   }
}

QString avl::preorder(pnodo raiz){
    if(laraiz){
        pre="Ra;";
        laraiz=false;
    }
    if(raiz){
        char* dato_nodo = (char*)malloc(6);
        strcpy(dato_nodo, raiz->placa);
        QString texto_n = QString::fromStdString(dato_nodo);
        pre += texto_n + ";" + QString::number(alt)+"#";
        alt++;
        pre+="Izq;";
        preorder(raiz->left);
        preorder(raiz->right);
        pre+="Der;";
        alt--;
        return pre;
    }
}

int avl::nodos_hoja(pnodo raiz){
    if(raiz){
        nodos_hoja(raiz->left);
        hoja++;
        nodos_hoja(raiz->right);
        return hoja;
    }
}

QString avl::preorder_espejo(pnodo raiz){
    if(raiz){
        char* dato_nodo = (char*)malloc(6);
        strcpy(dato_nodo, raiz->placa);
        QString pla = QString::fromStdString(dato_nodo);

        char* dato_ma = (char*)malloc(10);
        strcpy(dato_ma, raiz->marca);
        QString mar = QString::fromStdString(dato_ma);

        char* dato_mod = (char*)malloc(10);
        strcpy(dato_mod, raiz->modelo);
        QString mod = QString::fromStdString(dato_mod);

        char* dato_an = (char*)malloc(4);
        strcpy(dato_an, raiz->ano);
        QString an = QString::fromStdString(dato_an);

        char* dato_col = (char*)malloc(10);
        strcpy(dato_col, raiz->color);
        QString col = QString::fromStdString(dato_col);

        int pre = raiz->precio;

        char* dato_tra = (char*)malloc(10);
        strcpy(dato_tra, raiz->transmision);
        QString tra = QString::fromStdString(dato_tra);

        espe+=pla+";"+mar+";"+mod+";"+an+";"+col+";"+QString::number(pre)+";"+tra+"#";
        preorder_espejo(raiz->left);
        preorder_espejo(raiz->right);
        return espe;
    }
}

QString avl::preorder_arreglo(pnodo raiz){
    if(raiz){
        char* dato_nodo = (char*)malloc(6);
        strcpy(dato_nodo, raiz->placa);
        QString t_n = QString::fromStdString(dato_nodo);
        arre+=QString::number(k)+";"+t_n+"#";

        int tmp = 2*k;
        k = tmp;
        preorder_arreglo(raiz->left);
        preorder_arreglo(raiz->right);
        int tmp2 = 2*k+1;
        k = tmp2;
        return arre;
    }
}

pnodo avl::Buscar(char *placa, pnodo t){
    pnodo p;
    p = _buscar(placa, t);
    return p;
}

pnodo avl::_buscar(char* placa, pnodo t){
    pnodo actual = t;
    while(actual!=NULL){
        if(strcmp(placa, actual->placa)>0){
            actual = actual->right;
        }else if(strcmp(placa, actual->placa)<0){
            actual = actual->left;
        }else if(strcmp(placa, actual->placa)==0){
            return actual;
            break;
        }
    }
    if(actual==NULL){
        return NULL;
    }
}

pnodo avl::Modificar(char *placa1, char *marca1, char *modelo1, char *ano1, char *color1, int precio1, char *transmision1, pnodo t){
    pnodo p;
    p = _modificar(placa1, marca1, modelo1, ano1, color1, precio1, transmision1, t);
    return p;
}

pnodo avl::_modificar(char *placa1, char *marca1, char *modelo1, char *ano1, char *color1, int precio1, char *transmision1, pnodo t){
    pnodo actual = t;
    while(actual!=NULL){
        if(strcmp(placa1, actual->placa)>0){
            actual = actual->right;
        }else if(strcmp(placa1, actual->placa)<0){
            actual = actual->left;
        }else{
            if(strcmp(marca1,"n")!=0){
                actual->marca = marca1;
            }

            if(strcmp(modelo1,"n")!=0){
                actual->modelo = modelo1;
            }

            if(strcmp(ano1,"n")!=0){
                actual->ano = ano1;
            }

            if(strcmp(color1,"n")!=0){
                actual->color = color1;
            }

            if(precio1!=0){
                actual->precio = precio1;
            }

            if(strcmp(transmision1,"n")!=0){
                actual->transmision = transmision1;
            }
            return actual;
            break;
        }
    }
}


