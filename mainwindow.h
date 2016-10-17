#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnagregar_clicked();

    void on_btncargar_clicked();
    void cargar_archivo(QString contenido);

    void on_btnbuscar_clicked();

    void on_btneliminar_clicked();

    void on_btnmodificar_clicked();
    void limpiar();
    void limpiar2();
    void paintEvent(QPaintEvent *e);
    void grafica(QString contenido, int n);
    void on_pushButton_clicked();
    void crear_espejo();
    void on_btngraficar_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
