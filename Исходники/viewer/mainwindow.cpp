#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //соединяемся с базой
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("mbase.sqlite");
    if (!db.open())
    {
      qDebug() << db.lastError().text();
    }

    //делаем запрос для наполенния таблицы
    model = new QSqlQueryModel(this);
    reloadtableelements();

    connect (ui->elptb, SIGNAL(clicked(QModelIndex)), this, SLOT(elptableclick()));

    model2 = new QSqlQueryModel(this);
    model3 = new QSqlQueryModel(this);
    reloadtablepropel();

    connect (ui->propelptb, SIGNAL(clicked(QModelIndex)), this, SLOT(propelptableclick()));

    //=================================== tab2

    modeltb2 = new QSqlQueryModel(this);
    modelbolp = new QSqlQueryModel(this);

    reloadtablebolanalize();

    connect (ui->tabanalizeButton, SIGNAL(clicked()), this, SLOT(reloadtbanalize()));
    connect (ui->tabbolanalize, SIGNAL(clicked(QModelIndex)), this, SLOT(tabbolanalizeclick()));
    connect (ui->clearchecksButton, SIGNAL(clicked()), this, SLOT(clearchecks()));

    connect (ui->prevbolphotoButton, SIGNAL(clicked()), this, SLOT(gotoprevbolphoto()));
    connect (ui->nextbolphotoButton, SIGNAL(clicked()), this, SLOT(gotonextbolphoto()));
    //=================================== tab3
    modelbolf = new QSqlQueryModel(this);
    modelbolfp = new QSqlQueryModel(this);
    connect (ui->tbfb, SIGNAL(clicked(QModelIndex)), this, SLOT(tabformsbolclick()));

    connect (ui->prevfbolphotoButton, SIGNAL(clicked()), this, SLOT(gotoprevfbolphoto()));
    connect (ui->nextfbolphotoButton, SIGNAL(clicked()), this, SLOT(gotonextfbolphoto()));
    //=================================== tab4
    modelbolsh = new QSqlQueryModel(this);
    modelshbolp = new QSqlQueryModel(this);

    connect (ui->tbsh, SIGNAL(clicked(QModelIndex)), this, SLOT(tabshclick()));

    connect (ui->prevshbolphotoButton, SIGNAL(clicked()), this, SLOT(gotoprevshbolphoto()));
    connect (ui->nextshbolphotoButton, SIGNAL(clicked()), this, SLOT(gotonextshbolphoto()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::reloadtableelements()
{
    //QSqlQueryModel * model = new QSqlQueryModel(this);

    model->setQuery("SELECT * FROM elementsp");

    model->setHeaderData( 1, Qt::Horizontal, QObject::tr("Название элемента поражения") );

    ui->elptb->setModel(model);
    ui->elptb->show();

    //ui->elptb->setColumnWidth(0, 20);
    ui->elptb->setColumnHidden(0, true);
    ui->elptb->setColumnWidth(1, 600);
    ui->elptb->setColumnHidden(2, true);
    //ui->elptb->setColumnWidth(2, 40);

}

void MainWindow::reloadtablepropel()
{
    int rownum;

    if (ui->elptb->currentIndex().row() < 0){
       rownum = 0;
    }

    if (ui->elptb->currentIndex().row() >= 0){
        model3->setQuery("SELECT * FROM elementsp");
        rownum = model3->record(ui->elptb->currentIndex().row()).value("Id").toInt();
    }

    model2->setQuery("SELECT * FROM propertieselp where elpid="+QString::number(rownum));

    model2->setHeaderData( 2, Qt::Horizontal, QObject::tr("Локализация") );
    model2->setHeaderData( 3, Qt::Horizontal, QObject::tr("Поверхность") );
    model2->setHeaderData( 4, Qt::Horizontal, QObject::tr("Фон") );
    model2->setHeaderData( 5, Qt::Horizontal, QObject::tr("Цвет") );
    model2->setHeaderData( 6, Qt::Horizontal, QObject::tr("Размер") );
    model2->setHeaderData( 7, Qt::Horizontal, QObject::tr("Примечание") );
    model2->setHeaderData( 8, Qt::Horizontal, QObject::tr("Выбор") );



    ui->propelptb->setModel(model2);
    ui->propelptb->show();

    //ui->propelptb->setColumnWidth(0, 20);
    ui->propelptb->setColumnHidden(0, true);
    //ui->propelptb->setColumnWidth(1, 30);
    ui->propelptb->setColumnHidden(1, true);
    ui->propelptb->setColumnWidth(2, 200);
    ui->propelptb->setColumnWidth(3, 200);
    ui->propelptb->setColumnWidth(4, 150);
    ui->propelptb->setColumnWidth(5, 150);
    ui->propelptb->setColumnWidth(7, 250);
    ui->propelptb->setColumnWidth(8, 60);

}

void MainWindow::elptableclick()
{

int elptbrow = ui->elptb->currentIndex().row();

if (elptbrow >= 0)
{

    QString  elpname = model->record(elptbrow).value("Name").toString();


    QByteArray showphoto;

        if (!model->record(elptbrow).value("Photo").isNull())
        {
        showphoto = model->record(elptbrow).value("Photo").toByteArray();
        }
        if (model->record(elptbrow).value("Photo").isNull())
        {
        showphoto.clear();
        }


        if( !showphoto.isEmpty() )
        {
            QPixmap pixmap;
            pixmap.loadFromData(showphoto);
            ui->elpphoto->setPixmap(pixmap.scaled(200,200,Qt::KeepAspectRatio,Qt::SmoothTransformation));
        }

        if( showphoto.isEmpty() )
        {
          ui->elpphoto->setText("Нет фотографии");
        }

        ui->elptb->setFocus();
        ui->elptb->selectRow(elptbrow);


}
reloadtablepropel();
}

void MainWindow::propelptableclick()
{

    int propelptbrow = ui->propelptb->currentIndex().row();

    if (propelptbrow >= 0)
    {
        //model->setQuery("SELECT * FROM elementsp");
        int  propelpid = model2->record(propelptbrow).value("Id").toInt();
        QString  propelpcheck = model2->record(propelptbrow).value("Checked").toString();
//qDebug() << propelpcheck;

        if (propelpcheck == ""){

            model2->setQuery("update  propertieselp set Checked = 'X' where Id = " + QString::number(propelpid));
            //qDebug() << propelpcheck;
        }
        if (propelpcheck == "X"){
           model2->setQuery("update  propertieselp set Checked = NULL where Id = " + QString::number(propelpid));
        }

        reloadtablepropel();

    }

    ui->propelptb->setFocus();
    ui->propelptb->selectRow(propelptbrow);
}


//===================================================== tab 2

void MainWindow::reloadtablebolanalize()
{


    modeltb2->setQuery("select  t1.Name, count(t1.Id) as cnt, t2.Id, t2.bolid, t2.Elpid, t2.Pelpid, t3.Id, t3.Localizatiya, t3.Checked from bolezni t1, bolprop t2, propertieselp t3 where t1.Id = t2.Bolid and t2.Pelpid = t3.Id and t3.Checked = 'X' group by t1.Name order by cnt desc");

    modeltb2->setHeaderData( 0, Qt::Horizontal, QObject::tr("Название заболевания") );
    modeltb2->setHeaderData( 1, Qt::Horizontal, QObject::tr("Совпадений") );

    ui->tabbolanalize->setModel(modeltb2);
    ui->tabbolanalize->show();

    //ui->tabbolanalize->setColumnWidth(0, 20);
    //ui->tabbolanalize->setColumnHidden(0, true);
    ui->tabbolanalize->setColumnWidth(0, 500);
    ui->tabbolanalize->setColumnWidth(1, 100);

    ui->tabbolanalize->setColumnHidden(2, true);
    ui->tabbolanalize->setColumnHidden(3, true);
    ui->tabbolanalize->setColumnHidden(4, true);
    ui->tabbolanalize->setColumnHidden(5, true);
    ui->tabbolanalize->setColumnHidden(6, true);
    ui->tabbolanalize->setColumnHidden(7, true);
    ui->tabbolanalize->setColumnHidden(8, true);

    //ui->tabbolanalize->setColumnWidth(2, 40);

}



void MainWindow::clearchecks()
{
    QSqlQuery query;
    QString sqlquery = "update  propertieselp set Checked = NULL where Checked = 'X' " ;
    query.exec(sqlquery);
    reloadtablebolanalize();
}




void MainWindow::reloadtbanalize()
{
reloadtablebolanalize();
}


void MainWindow::tabbolanalizeclick()
{
    ui->descbol->clear();
    ui->descfb->clear();
    ui->descsh->clear();
    ui->shemalecheniya->clear();


    int tbarow = ui->tabbolanalize->currentIndex().row();

    if (tbarow >= 0)
    {
        //model->setQuery("SELECT * FROM elementsp");
        int  tbaid = modeltb2->record(tbarow).value(3).toInt();


        QSqlQuery query;
        QString sqlquery = "select * from bolezni where Id = " + QString::number(tbaid);
        query.exec(sqlquery);
        if (query.next()) {
        QString tbadesc = query.value(2).toString();
        ui->descbol->setText(tbadesc);
        }

        //QSqlQuery query2;
        sqlquery = "select * from shemalecheniya where Bolid = " + QString::number(tbaid);
        query.exec(sqlquery);
        if (query.next()) {
        QString tbshldesc = query.value(2).toString();
        ui->shemalecheniya->setText(tbshldesc);
        }

        //фотографии для болезней
        sqlquery = "select count(*) from bphotos where Bid = " + QString::number(tbaid);
        query.exec(sqlquery);
        if (query.next()) {
        countbolphotos = query.value(0).toInt();

        if (countbolphotos == 0){
           ui->bolphoto->clear();
           ui->label_10->clear();
        }

        if (countbolphotos > 0){
                 currentbolphoto = 0;
//qDebug() << currentbolphoto;
                 modelbolp->setQuery("SELECT * FROM bphotos where bid = " + QString::number(tbaid) );

                         QByteArray showphoto;
                         if (!modelbolp->record(currentbolphoto).value("Photo").isNull())
                         {
                         showphoto = modelbolp->record(currentbolphoto).value("Photo").toByteArray();
                         }
                         if (modelbolp->record(currentbolphoto).value("Photo").isNull())
                         {
                         showphoto.clear();
                         }

                         if( !showphoto.isEmpty() )
                         {
                             QPixmap pixmap;
                             pixmap.loadFromData(showphoto);
                             ui->bolphoto->setPixmap(pixmap.scaled(200,200,Qt::KeepAspectRatio,Qt::SmoothTransformation));
                         }

                         if( showphoto.isEmpty() )
                         {
                           ui->bolphoto->setText("Нет фотографии");
                         }


                        ui->label_10->setText("Фотография " + QString::number(currentbolphoto+1) + " из " + QString::number(countbolphotos));
             }


            }

        ui->fbolphoto->clear();
        ui->label_11->clear();
        ui->shbolphoto->clear();
        ui->label_12->clear();

    }

    ui->tabbolanalize->setFocus();
    ui->tabbolanalize->selectRow(tbarow);

    reloadtableformsbol();
    reloadtableshbol();
}



void MainWindow::gotoprevbolphoto()
{

    int tbarow = ui->tabbolanalize->currentIndex().row();

    if (tbarow >= 0)
    {

        int  tbaid = modeltb2->record(tbarow).value(3).toInt();

    if (countbolphotos > 0){
    int futurecurphoto = currentbolphoto - 1;

    if (futurecurphoto+1 > 0 && futurecurphoto+1 <= countbolphotos){

        currentbolphoto = futurecurphoto;

        modelbolp->setQuery("SELECT * FROM bphotos where bid = " + QString::number(tbaid) );

                QByteArray showphoto;
                if (!modelbolp->record(currentbolphoto).value("Photo").isNull())
                {
                showphoto = modelbolp->record(currentbolphoto).value("Photo").toByteArray();
                }
                if (modelbolp->record(currentbolphoto).value("Photo").isNull())
                {
                showphoto.clear();
                }

                if( !showphoto.isEmpty() )
                {
                    QPixmap pixmap;
                    pixmap.loadFromData(showphoto);
                    ui->bolphoto->setPixmap(pixmap.scaled(200,200,Qt::KeepAspectRatio,Qt::SmoothTransformation));
                }

                if( showphoto.isEmpty() )
                {
                  ui->bolphoto->setText("Нет фотографии");

                }
                ui->label_10->setText("Фотография " + QString::number(currentbolphoto+1) + " из " + QString::number(countbolphotos));
    }

}
}
}

void MainWindow::gotonextbolphoto()
{

    int tbarow = ui->tabbolanalize->currentIndex().row();

    if (tbarow >= 0)
    {

        int  tbaid = modeltb2->record(tbarow).value(3).toInt();

    if (countbolphotos > 0){
    int futurecurphoto = currentbolphoto + 1;

    if (futurecurphoto+1 > 0 && futurecurphoto+1 <= countbolphotos){

        currentbolphoto = futurecurphoto;

        modelbolp->setQuery("SELECT * FROM bphotos where bid = " + QString::number(tbaid) );

                QByteArray showphoto;
                if (!modelbolp->record(currentbolphoto).value("Photo").isNull())
                {
                showphoto = modelbolp->record(currentbolphoto).value("Photo").toByteArray();
                }
                if (modelbolp->record(currentbolphoto).value("Photo").isNull())
                {
                showphoto.clear();
                }

                if( !showphoto.isEmpty() )
                {
                    QPixmap pixmap;
                    pixmap.loadFromData(showphoto);
                    ui->bolphoto->setPixmap(pixmap.scaled(200,200,Qt::KeepAspectRatio,Qt::SmoothTransformation));
                }

                if( showphoto.isEmpty() )
                {
                  ui->bolphoto->setText("Нет фотографии");
                }
                ui->label_10->setText("Фотография " + QString::number(currentbolphoto+1) + " из " + QString::number(countbolphotos));
    }


}
}
}


//=============================================== tab 3


void MainWindow::reloadtableformsbol()
{


    int bolfid;
    int bolftbrow = ui->tabbolanalize->currentIndex().row();

    if (bolftbrow < 0)
    {
      bolfid = 0;
    }


    if (bolftbrow >= 0)
    {
         bolfid = modeltb2->record(bolftbrow).value(3).toInt();
    }
//qDebug() << bolfid;
    modelbolf->setQuery("SELECT * FROM formsbolezni where Bolid = " + QString::number(bolfid) );

    modelbolf->setHeaderData( 2, Qt::Horizontal, QObject::tr("Название формы заболевания") );

    ui->tbfb->setModel(modelbolf);
ui->tbfb->show();

ui->tbfb->setColumnHidden(0, true);
ui->tbfb->setColumnHidden(1, true);
ui->tbfb->setColumnHidden(3, true);

ui->tbfb->setColumnWidth(2, 500);


}


void MainWindow::tabformsbolclick()
{

    int tbfbrow = ui->tbfb->currentIndex().row();

    if (tbfbrow >= 0)
    {

        int  tbfbid = modelbolf->record(tbfbrow).value(1).toInt();
        QString tbfbdesc = modelbolf->record(tbfbrow).value(3).toString();
//qDebug() << tbfbrow;
        ui->descfb->setText(tbfbdesc);




        //фотографии для болезней
        QSqlQuery query;
        QString sqlquery = "select count(*) from fbphotos where fid = " + QString::number(tbfbid);
        query.exec(sqlquery);
        if (query.next()) {
        countfbolphotos = query.value(0).toInt();

        if (countfbolphotos == 0){
           ui->fbolphoto->clear();
           ui->label_11->clear();
        }

        if (countfbolphotos > 0){
                 currentfbolphoto = 0;

                 modelbolfp->setQuery("SELECT * FROM fbphotos where fid = " + QString::number(tbfbid) );

                         QByteArray showphoto;
                         if (!modelbolfp->record(currentfbolphoto).value("Photo").isNull())
                         {
                         showphoto = modelbolfp->record(currentfbolphoto).value("Photo").toByteArray();
                         }
                         if (modelbolfp->record(currentfbolphoto).value("Photo").isNull())
                         {
                         showphoto.clear();
                         }

                         if( !showphoto.isEmpty() )
                         {
                             QPixmap pixmap;
                             pixmap.loadFromData(showphoto);
                             ui->fbolphoto->setPixmap(pixmap.scaled(200,200,Qt::KeepAspectRatio,Qt::SmoothTransformation));
                         }

                         if( showphoto.isEmpty() )
                         {
                           ui->fbolphoto->setText("Нет фотографии");
                         }


                        ui->label_11->setText("Фотография " + QString::number(currentfbolphoto+1) + " из " + QString::number(countfbolphotos));
             }


            }




    }

    ui->tbfb->setFocus();
    ui->tbfb->selectRow(tbfbrow);

}





void MainWindow::gotoprevfbolphoto()
{

    int tbfbrow = ui->tbfb->currentIndex().row();

    if (tbfbrow >= 0)
    {

        int  tbfbid = modelbolf->record(tbfbrow).value(1).toInt();

    if (countfbolphotos > 0){
    int futurecurphoto = currentfbolphoto - 1;

    if (futurecurphoto+1 > 0 && futurecurphoto+1 <= countfbolphotos){

        currentfbolphoto = futurecurphoto;

        modelbolfp->setQuery("SELECT * FROM fbphotos where fid = " + QString::number(tbfbid) );

                QByteArray showphoto;
                if (!modelbolfp->record(currentfbolphoto).value("Photo").isNull())
                {
                showphoto = modelbolfp->record(currentfbolphoto).value("Photo").toByteArray();
                }
                if (modelbolfp->record(currentfbolphoto).value("Photo").isNull())
                {
                showphoto.clear();
                }

                if( !showphoto.isEmpty() )
                {
                    QPixmap pixmap;
                    pixmap.loadFromData(showphoto);
                    ui->fbolphoto->setPixmap(pixmap.scaled(200,200,Qt::KeepAspectRatio,Qt::SmoothTransformation));
                }

                if( showphoto.isEmpty() )
                {
                  ui->fbolphoto->setText("Нет фотографии");
                }


               ui->label_11->setText("Фотография " + QString::number(currentfbolphoto+1) + " из " + QString::number(countfbolphotos));
    }

}
}
}

void MainWindow::gotonextfbolphoto()
{

    int tbfbrow = ui->tbfb->currentIndex().row();

    if (tbfbrow >= 0)
    {

        int  tbfbid = modelbolf->record(tbfbrow).value(1).toInt();
//qDebug() << tbfbid;
    if (countfbolphotos > 0){
    int futurecurphoto = currentfbolphoto + 1;

    if (futurecurphoto+1 > 0 && futurecurphoto+1 <= countfbolphotos){

        currentfbolphoto = futurecurphoto;

        modelbolfp->setQuery("SELECT * FROM fbphotos where fid = " + QString::number(tbfbid) );

                QByteArray showphoto;
                if (!modelbolfp->record(currentfbolphoto).value("Photo").isNull())
                {
                showphoto = modelbolfp->record(currentfbolphoto).value("Photo").toByteArray();
                }
                if (modelbolfp->record(currentfbolphoto).value("Photo").isNull())
                {
                showphoto.clear();
                }

                if( !showphoto.isEmpty() )
                {
                    QPixmap pixmap;
                    pixmap.loadFromData(showphoto);
                    ui->fbolphoto->setPixmap(pixmap.scaled(200,200,Qt::KeepAspectRatio,Qt::SmoothTransformation));
                }

                if( showphoto.isEmpty() )
                {
                  ui->fbolphoto->setText("Нет фотографии");
                }


               ui->label_11->setText("Фотография " + QString::number(currentfbolphoto+1) + " из " + QString::number(countfbolphotos));
    }


}
}
}





//=============================================== tab 4
void MainWindow::reloadtableshbol()
{

    int sbolid;
        int sboltbrow = ui->tabbolanalize->currentIndex().row();

        if (sboltbrow < 0)
        {
          sbolid = 0;
        }

        if (sboltbrow >= 0)
        {
          sbolid = modeltb2->record(sboltbrow).value(3).toInt();
        }

        modelbolsh->setQuery("SELECT t1.Id, t1.Bolid, t1.Sbolid, t2.Id, t2.Name FROM pbolezni t1, bolezni t2 where t1.Bolid = " + QString::number(sbolid) + " and t1.Sbolid = t2.Id" );

        modelbolsh->setHeaderData( 4, Qt::Horizontal, QObject::tr("Название заболевания") );

        ui->tbsh->setModel(modelbolsh);
    ui->tbsh->show();


   ui->tbsh->setColumnHidden(0, true);
    ui->tbsh->setColumnHidden(1, true);
   ui->tbsh->setColumnHidden(2, true);
   ui->tbsh->setColumnHidden(3, true);
    ui->tbsh->setColumnWidth(4, 500);

}

void MainWindow::tabshclick()
{

    int tbshrow = ui->tbsh->currentIndex().row();

    if (tbshrow >= 0)
    {

        int  tbshid = modelbolsh->record(tbshrow).value(3).toInt();


        QSqlQuery query;
        QString sqlquery = "select * from bolezni where Id = " + QString::number(tbshid);
        query.exec(sqlquery);
        if (query.next()) {

        QString tbshdesc = query.value(2).toString();
        ui->descsh->setText(tbshdesc);

        }



        //фотографии для болезней
        sqlquery = "select count(*) from bphotos where Bid = " + QString::number(tbshid);
        query.exec(sqlquery);
        if (query.next()) {
        countshbolphotos = query.value(0).toInt();

        if (countshbolphotos == 0){
           ui->shbolphoto->clear();
           ui->label_12->clear();
        }

        if (countshbolphotos > 0){
                 currentshbolphoto = 0;
//qDebug() << currentbolphoto;
                 modelshbolp->setQuery("SELECT * FROM bphotos where bid = " + QString::number(tbshid) );

                         QByteArray showphoto;
                         if (!modelshbolp->record(currentshbolphoto).value("Photo").isNull())
                         {
                         showphoto = modelshbolp->record(currentshbolphoto).value("Photo").toByteArray();
                         }
                         if (modelshbolp->record(currentshbolphoto).value("Photo").isNull())
                         {
                         showphoto.clear();
                         }

                         if( !showphoto.isEmpty() )
                         {
                             QPixmap pixmap;
                             pixmap.loadFromData(showphoto);
                             ui->shbolphoto->setPixmap(pixmap.scaled(200,200,Qt::KeepAspectRatio,Qt::SmoothTransformation));
                         }

                         if( showphoto.isEmpty() )
                         {
                           ui->shbolphoto->setText("Нет фотографии");
                         }


                        ui->label_12->setText("Фотография " + QString::number(currentshbolphoto+1) + " из " + QString::number(countshbolphotos));
             }


            }





    }

    ui->tbsh->setFocus();
    ui->tbsh->selectRow(tbshrow);

}





void MainWindow::gotoprevshbolphoto()
{

    int tbshrow = ui->tbsh->currentIndex().row();

    if (tbshrow >= 0)
    {

        int  tbshid = modelbolsh->record(tbshrow).value(3).toInt();

    if (countshbolphotos > 0){
    int futurecurphoto = currentshbolphoto - 1;

    if (futurecurphoto+1 > 0 && futurecurphoto+1 <= countshbolphotos){

        currentshbolphoto = futurecurphoto;

        modelshbolp->setQuery("SELECT * FROM bphotos where bid = " + QString::number(tbshid) );

                QByteArray showphoto;
                if (!modelshbolp->record(currentshbolphoto).value("Photo").isNull())
                {
                showphoto = modelshbolp->record(currentshbolphoto).value("Photo").toByteArray();
                }
                if (modelshbolp->record(currentshbolphoto).value("Photo").isNull())
                {
                showphoto.clear();
                }

                if( !showphoto.isEmpty() )
                {
                    QPixmap pixmap;
                    pixmap.loadFromData(showphoto);
                    ui->shbolphoto->setPixmap(pixmap.scaled(200,200,Qt::KeepAspectRatio,Qt::SmoothTransformation));
                }

                if( showphoto.isEmpty() )
                {
                  ui->shbolphoto->setText("Нет фотографии");

                }
                ui->label_12->setText("Фотография " + QString::number(currentshbolphoto+1) + " из " + QString::number(countshbolphotos));
    }

}
}
}

void MainWindow::gotonextshbolphoto()
{

    int tbshrow = ui->tbsh->currentIndex().row();

    if (tbshrow >= 0)
    {

        int  tbshid = modelbolsh->record(tbshrow).value(3).toInt();

    if (countshbolphotos > 0){
    int futurecurphoto = currentshbolphoto + 1;

    if (futurecurphoto+1 > 0 && futurecurphoto+1 <= countshbolphotos){

        currentshbolphoto = futurecurphoto;

        modelshbolp->setQuery("SELECT * FROM bphotos where bid = " + QString::number(tbshid) );

                QByteArray showphoto;
                if (!modelshbolp->record(currentshbolphoto).value("Photo").isNull())
                {
                showphoto = modelshbolp->record(currentshbolphoto).value("Photo").toByteArray();
                }
                if (modelshbolp->record(currentshbolphoto).value("Photo").isNull())
                {
                showphoto.clear();
                }

                if( !showphoto.isEmpty() )
                {
                    QPixmap pixmap;
                    pixmap.loadFromData(showphoto);
                    ui->shbolphoto->setPixmap(pixmap.scaled(200,200,Qt::KeepAspectRatio,Qt::SmoothTransformation));
                }

                if( showphoto.isEmpty() )
                {
                  ui->shbolphoto->setText("Нет фотографии");
                }
                ui->label_12->setText("Фотография " + QString::number(currentshbolphoto+1) + " из " + QString::number(countshbolphotos));
    }


}
}
}
