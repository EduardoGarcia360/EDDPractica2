#ifndef AVL_H
#define AVL_H

#include "QString"
class avl
{
public:
    avl();

    typedef struct avlnode
    {
        char* placa;
        char* marca;
        char* modelo;
        char* ano;
        char* color;
        int precio;
        char* transmision;

        int bal; /* Factor de balance -1,0,1 */
        struct avlnode *left, *right;
    } nodo, *pnodo;

    //los metodos
    static pnodo lrot(pnodo t);
    static pnodo rrot(pnodo t);
    static void Error(int tipo);
    int Altura();
    pnodo CreaNodo(char* placa);
    pnodo insertR(pnodo t);
    pnodo InsertarAVL(char* placa, char* marca, char* modelo, char* ano, char* color, int precio, char* transmision, pnodo t);
    pnodo espejoR(pnodo t);
    pnodo EspejoAVL(char* placa, char* marca, char* modelo, char* ano, char* color, int precio, char* transmision, pnodo t);
    pnodo Buscar(char* placa, pnodo t);
    pnodo _buscar(char* placa, pnodo t);
    pnodo Modificar(char* placa, char* marca, char* modelo, char* ano, char* color, int precio, char* transmision, pnodo t);
    pnodo _modificar(char* placa, char* marca, char* modelo, char* ano, char* color, int precio, char* transmision, pnodo t);
    pnodo deleteR(pnodo t);
    pnodo DescartarAVL(char* placa, pnodo raiz);
    pnodo deltreeR(pnodo t);
    pnodo deltree(pnodo t);
    void InOrder(pnodo raiz);
    void g(pnodo raiz);
    void inorder(pnodo t, int profundidad);
    QString preorder(pnodo raiz);
    QString preorder_arreglo(pnodo raiz);
    QString preorder_espejo(pnodo raiz);
    int nodos_hoja(pnodo raiz);
};

#endif // AVL_H
