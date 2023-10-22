#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGUI>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlRecord>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void reloadtableelements();
    void addnewelp();
    void openelpphoto();
    void deleteelpline();
    void elptableclick();
    void updateelpline();
    //==================================
    void reloadtablepropel();
    void addnewelpproperty();
    void propelptableclick();
    void deletepropelpline();
    void updatepropelpline();
    //=================================== tab 2
    void reloadtablebolezni();
    void addnewbolezn();
    void deletebolezn();
    void updatebolezn();
    void bolezntableclick();

    void openbolphoto();
    void addnewbolphoto();
    void reloadtablebphotos();
    void deletebolphoto();
    void bolphototableclick();

    //================================== tab 3
    void reloadtableformsforbol();
    void reloadtableformsbol();
    void addnewformbol();
    void deleteformbol();
    void formboltableclick();
    void updateformbol();
    void forformboltableclick();

    void openfbolphoto();
    void addnewfbolphoto();
    void deletefbolphoto();
    void fbolphotostableclick();
    void reloadtablefbolphotos();

    //================================== tab 4

    void reloadtableshbol1();
    void reloadtableshbol2();
    void reloadtableshbol3();
    void addnewshbol();
    void deleteshbol();

    void shtable1click();
    void shtable2click();
    void shtable3click();

    //================================== tab 5

    void reloadtableslbol();
    void reloadmodelbolsl();
    void addnewsl();
    void sltableclick();
    void deleteslbol();
    void updateslbol();

    //================================== tab 6

    void reloadtablebolel();
    void reloadtableels();
    void reloadtablepropels();
    void reloadtablebolprop();
    void elstbclick();
    void addtolist();
    void boleltbclick();
    void deletetolist();



private:
    Ui::MainWindow *ui;

    QSqlQueryModel *model;
    QSqlQueryModel *model2;
    QSqlQueryModel *model3;

    QSqlQueryModel *modelbol;
    QSqlQueryModel *modelbolp;

    QSqlQueryModel *modelbolf;
    QSqlQueryModel *modelbolfp;

    //================================== tab 4
    QSqlQueryModel *modelbolsh;

    //================================== tab 5
    QSqlQueryModel *modelbolsl;

    //================================== tab 6
    QSqlQueryModel *modelbolprop;

};

#endif // MAINWINDOW_H
