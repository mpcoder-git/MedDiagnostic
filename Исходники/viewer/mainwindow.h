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

    int countbolphotos;
    int currentbolphoto;

    int countfbolphotos;
    int currentfbolphoto;

    int countshbolphotos;
    int currentshbolphoto;

public slots:
    void reloadtableelements();
    void reloadtablepropel();
    void elptableclick();
    void propelptableclick();

    void reloadtablebolanalize();
    void reloadtbanalize();
    void tabbolanalizeclick();
    void clearchecks();
    void gotonextbolphoto();
    void gotoprevbolphoto();


    void reloadtableformsbol();
    void tabformsbolclick();
    void gotonextfbolphoto();
    void gotoprevfbolphoto();


    void reloadtableshbol();
    void tabshclick();
    void gotonextshbolphoto();
    void gotoprevshbolphoto();


private:
    Ui::MainWindow *ui;

    QSqlQueryModel *model;
    QSqlQueryModel *model2;
    QSqlQueryModel *model3;

    QSqlQueryModel *modeltb2;

    QSqlQueryModel *modelbolf;
    QSqlQueryModel *modelbolfp;
    QSqlQueryModel *modelbolsh;
    QSqlQueryModel *modelbolp;
    QSqlQueryModel *modelshbolp;

};

#endif // MAINWINDOW_H
