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

    connect (ui->elpaddnewbutton, SIGNAL(clicked()), this, SLOT(addnewelp()));
    connect (ui->elpselectimagebutton, SIGNAL(clicked()), this, SLOT(openelpphoto()));
    connect (ui->elpdeletebutton, SIGNAL(clicked()), this, SLOT(deleteelpline()));
    connect (ui->elptb, SIGNAL(clicked(QModelIndex)), this, SLOT(elptableclick()));
    connect (ui->elpeditbutton, SIGNAL(clicked()), this, SLOT(updateelpline()));

    //=============================================================================

    model2 = new QSqlQueryModel(this);
    model3 = new QSqlQueryModel(this);
    reloadtablepropel();

    connect (ui->pelpaddbuton, SIGNAL(clicked()), this, SLOT(addnewelpproperty()));
    connect (ui->propelptb, SIGNAL(clicked(QModelIndex)), this, SLOT(propelptableclick()));
    connect (ui->pelpdeletebutton, SIGNAL(clicked()), this, SLOT(deletepropelpline()));
    connect (ui->pelpeditbutton, SIGNAL(clicked()), this, SLOT(updatepropelpline()));

    //=============================================================================== tab2

    modelbol = new QSqlQueryModel(this);
    //model3 = new QSqlQueryModel(this);
    reloadtablebolezni();

    connect (ui->addnewbolbutton, SIGNAL(clicked()), this, SLOT(addnewbolezn()));
    connect (ui->boltb, SIGNAL(clicked(QModelIndex)), this, SLOT(bolezntableclick()));
    connect (ui->updatebolbutton, SIGNAL(clicked()), this, SLOT(updatebolezn()));
    connect (ui->delbolbutton, SIGNAL(clicked()), this, SLOT(deletebolezn()));
    connect (ui->selbolphotobutton, SIGNAL(clicked()), this, SLOT(openbolphoto()));

    modelbolp = new QSqlQueryModel(this);
    reloadtablebphotos();

    connect (ui->addbolphotobutton, SIGNAL(clicked()), this, SLOT(addnewbolphoto()));
    connect (ui->delbolphotobutton, SIGNAL(clicked()), this, SLOT(deletebolphoto()));
    connect (ui->bolptb, SIGNAL(clicked(QModelIndex)), this, SLOT(bolphototableclick()));

    //======================================================== tab 3

    reloadtableformsforbol();

    modelbolf = new QSqlQueryModel(this);
    reloadtableformsbol();

    connect (ui->addformbolbutton, SIGNAL(clicked()), this, SLOT(addnewformbol()));
    connect (ui->deleteformbolbutton, SIGNAL(clicked()), this, SLOT(deleteformbol()));
    connect (ui->formsboltab, SIGNAL(clicked(QModelIndex)), this, SLOT(formboltableclick()));
    connect (ui->updateformbolbutton, SIGNAL(clicked()), this, SLOT(updateformbol()));
    connect (ui->tabforformsbol, SIGNAL(clicked(QModelIndex)), this, SLOT(forformboltableclick()));

    modelbolfp = new QSqlQueryModel(this);
    reloadtablefbolphotos();

    connect (ui->selectfbolphotobutton, SIGNAL(clicked()), this, SLOT(openfbolphoto()));
    connect (ui->addfbolphotobutton, SIGNAL(clicked()), this, SLOT(addnewfbolphoto()));
    connect (ui->fbolphotostb, SIGNAL(clicked(QModelIndex)), this, SLOT(fbolphotostableclick()));
    connect (ui->deletefbolphotobutton, SIGNAL(clicked()), this, SLOT(deletefbolphoto()));

    //======================================================== tab 4

    reloadtableshbol1();
    reloadtableshbol2();

    modelbolsh = new QSqlQueryModel(this);
    reloadtableshbol3();

    connect (ui->addshbbutton, SIGNAL(clicked()), this, SLOT(addnewshbol()));
    connect (ui->shboltb1, SIGNAL(clicked(QModelIndex)), this, SLOT(shtable1click()));
    connect (ui->shboltb2, SIGNAL(clicked(QModelIndex)), this, SLOT(shtable2click()));
    connect (ui->shboltb3, SIGNAL(clicked(QModelIndex)), this, SLOT(shtable3click()));
    connect (ui->deleteshbbutton, SIGNAL(clicked()), this, SLOT(deleteshbol()));

    //======================================================== tab 5

    modelbolsl = new QSqlQueryModel(this);
    reloadmodelbolsl();

    reloadtableslbol();
    connect (ui->addnewslbutton, SIGNAL(clicked()), this, SLOT(addnewsl()));
    connect (ui->slboltb, SIGNAL(clicked(QModelIndex)), this, SLOT(sltableclick()));
    connect (ui->deleteslbolbutton, SIGNAL(clicked()), this, SLOT(deleteslbol()));
    connect (ui->updateslbolbutton, SIGNAL(clicked()), this, SLOT(updateslbol()));

    //======================================================== tab 6

    modelbolprop = new QSqlQueryModel(this);

    reloadtablebolel();
    reloadtableels();
    reloadtablepropels();

    connect (ui->elstb, SIGNAL(clicked(QModelIndex)), this, SLOT(elstbclick()));
    connect (ui->addtolistbutton, SIGNAL(clicked()), this, SLOT(addtolist()));
    connect (ui->boleltb, SIGNAL(clicked(QModelIndex)), this, SLOT(boleltbclick()));
    connect (ui->deletetotistbutton, SIGNAL(clicked()), this, SLOT(deletetolist()));

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

    ui->elptb->setColumnWidth(0, 20);
    ui->elptb->setColumnWidth(1, 600);
    ui->elptb->setColumnWidth(2, 40);

}

void MainWindow::addnewelp()
{

    if (ui->elpnameedit->text() != "" && ui->elpphotoedit->text() == "")
       {
        //qDebug() << "123";
        QSqlQuery query;
        QString sqlquery = "insert into elementsp ( Name ) values ('" + this->ui->elpnameedit->text() + "')";
        query.exec(sqlquery);
        reloadtableelements();
       }


    if (ui->elpnameedit->text() != "" && ui->elpphotoedit->text() != "")
        {
            QPixmap pixmap;
            pixmap.load(ui->elpphotoedit->text());
            QByteArray ba;
            QBuffer buf( &ba );
            buf.open( QIODevice::ReadWrite);
            pixmap.save( &buf, "jpg" );

            QSqlQuery query;
            query.prepare("insert into elementsp ( Name, Photo) values (:elname, :photo)");
            query.bindValue(":elname",ui->elpnameedit->text());
            query.bindValue(":photo", ba );
            query.exec();

            reloadtableelements();
            ui->elpphotopreviewlabel->clear();
            ui->elpnameedit->clear();
            ui->elpphotoedit->clear();

            ui->elpphotopreviewlabel->clear();
        }

}

void MainWindow::openelpphoto()
{

    QString fileName = QFileDialog::getOpenFileName(this,
                 tr("Select photo"), "",
                 tr("Photo (*.jpg);;All files (*)"));

        ui->elpphotoedit->setText(fileName);


        QPixmap pixmap;
            pixmap.load(fileName);
            ui->elpphotopreviewlabel->setPixmap( pixmap.scaled(90,90,Qt::KeepAspectRatio,Qt::SmoothTransformation) );
}

//удаление элемента поражения
void MainWindow::deleteelpline()
{

//qDebug() << ui->elptb->currentIndex().row();

    int elptbrow = ui->elptb->currentIndex().row();

    if (elptbrow >= 0)
    {
        model->setQuery("SELECT * FROM elementsp");
        int elpid = model->record(elptbrow).value("Id").toInt();

        model->setQuery("delete FROM elementsp where Id = " + QString::number(elpid));
        model->setQuery("delete FROM propertieselp where Elpid = " + QString::number(elpid));
        ui->elpphoto->clear();
        reloadtableelements();
    }

}

void MainWindow::elptableclick()
{

//qDebug() << ui->elptb->currentIndex().row();

int elptbrow = ui->elptb->currentIndex().row();

if (elptbrow >= 0)
{
    //model->setQuery("SELECT * FROM elementsp");
    QString  elpname = model->record(elptbrow).value("Name").toString();
    ui->elpnameedit->setText(elpname);

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
          ui->elpphoto->setText("No Photo");
        }

        ui->elptb->setFocus();
        ui->elptb->selectRow(elptbrow);


}
reloadtablepropel();
}




void MainWindow::updateelpline()
{

    int elptbrow = ui->elptb->currentIndex().row();

    if (elptbrow >= 0)
    {
        model->setQuery("SELECT * FROM elementsp");
        int elpid = model->record(elptbrow).value("Id").toInt();

        if (ui->elpnameedit->text() != "" && ui->elpphotoedit->text() == "")
        {
            model->setQuery("update  elementsp set Name = '" + ui->elpnameedit->text() +"' where Id = " + QString::number(elpid));
        }
        if (ui->elpnameedit->text() != "" && ui->elpphotoedit->text() != "")
        {

            QPixmap pixmap;
            pixmap.load(ui->elpphotoedit->text());
            QByteArray ba;
            QBuffer buf( &ba );
            buf.open( QIODevice::ReadWrite);
            pixmap.save( &buf, "jpg" );

            QSqlQuery query;
            query.prepare("update elementsp set Name = :elname, Photo = :photo where Id = :elpid ");
            query.bindValue(":elname",ui->elpnameedit->text());
            query.bindValue(":photo", ba );
            query.bindValue(":elpid", elpid );
            query.exec();
        }

        ui->elpnameedit->clear();
        ui->elpphotoedit->clear();

        reloadtableelements();
        ui->elptb->setFocus();
        ui->elptb->selectRow(elptbrow);
    }
ui->elpphotopreviewlabel->clear();
}

//======================================================================

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

    ui->propelptb->setModel(model2);
    ui->propelptb->show();

    ui->propelptb->setColumnWidth(0, 20);
    ui->propelptb->setColumnWidth(1, 30);
    ui->propelptb->setColumnWidth(2, 200);
    ui->propelptb->setColumnWidth(3, 200);
    ui->propelptb->setColumnWidth(4, 150);
    ui->propelptb->setColumnWidth(5, 150);
    ui->propelptb->setColumnWidth(7, 250);

}

void MainWindow::addnewelpproperty()
{

    if (ui->elptb->currentIndex().row() >= 0){

        int elptbrow = ui->elptb->currentIndex().row();
            model->setQuery("SELECT * FROM elementsp");
            int elpid = model->record(elptbrow).value("Id").toInt();


       // if (ui->pelplocalizatia->text() != "" && ui->pelppoverhnost->text() != "" && ui->pelpfon->text() != "" && ui->pelpcvet->text() != "" && ui->pelprazmer->text() != "")
       //{

        QSqlQuery query;
        query.prepare("insert into propertieselp ( Elpid, Localizatiya, Poverhnost, Fon, Cvet, Razmer, Dop) values (:elpid, :localizatia, :poverhnost, :fon, :cvet, :razmer, :dop)");
        query.bindValue(":elpid",elpid);
        query.bindValue(":localizatia", this->ui->pelplocalizatia->text() );
        query.bindValue(":poverhnost", this->ui->pelppoverhnost->text() );
        query.bindValue(":fon", this->ui->pelpfon->text() );
        query.bindValue(":cvet", this->ui->pelpcvet->text() );
        query.bindValue(":razmer", this->ui->pelprazmer->text() );
        query.bindValue(":dop", this->ui->pelpprim->text() );

        query.exec();

        ui->elptb->setFocus();
        ui->elptb->selectRow(elptbrow);
        reloadtablepropel();
       //}
    }
}

void MainWindow::propelptableclick()
{

    int propelptbrow = ui->propelptb->currentIndex().row();

    if (propelptbrow >= 0)
    {
        //model->setQuery("SELECT * FROM elementsp");
        QString  propelplocalizatiya = model2->record(propelptbrow).value("Localizatiya").toString();
        QString  propelppoverhnost = model2->record(propelptbrow).value("Poverhnost").toString();
        QString  propelpfon = model2->record(propelptbrow).value("Fon").toString();
        QString  propelpcvet = model2->record(propelptbrow).value("Cvet").toString();
        QString  propelprazmer = model2->record(propelptbrow).value("Razmer").toString();
        QString  propelpprim = model2->record(propelptbrow).value("Dop").toString();

        ui->pelplocalizatia->setText(propelplocalizatiya);
        ui->pelppoverhnost->setText(propelppoverhnost);
        ui->pelpfon->setText(propelpfon);
        ui->pelpcvet->setText(propelpcvet);
        ui->pelprazmer->setText(propelprazmer);
        ui->pelpprim->setText(propelpprim);
    }

    ui->propelptb->setFocus();
    ui->propelptb->selectRow(propelptbrow);
}

void MainWindow::deletepropelpline()
{

//qDebug() << ui->elptb->currentIndex().row();

    int pelptbrow = ui->propelptb->currentIndex().row();

    if (pelptbrow >= 0)
    {
        //model->setQuery("SELECT * FROM elementsp");
        int pelpid = model2->record(pelptbrow).value("Id").toInt();

        model2->setQuery("delete FROM propertieselp where Id = " + QString::number(pelpid));
        reloadtablepropel();
    }

}


void MainWindow::updatepropelpline()
{

    int pelptbrow = ui->propelptb->currentIndex().row();

    if (pelptbrow >= 0)
    {
        //model->setQuery("SELECT * FROM elementsp");
        int pelpid = model2->record(pelptbrow).value("Id").toInt();

        if (ui->pelplocalizatia->text() != "" && ui->pelppoverhnost->text() != "" && ui->pelpfon->text() != "" && ui->pelpcvet->text() != "" && ui->pelprazmer->text() != "")
        {


            QSqlQuery query;
            query.prepare("update propertieselp set Localizatiya = :loc, Poverhnost = :pov, Fon = :fon, Cvet = :cvt, Razmer = :raz, Dop = :dop where Id = :propelpid ");
            query.bindValue(":loc",ui->pelplocalizatia->text());
            query.bindValue(":pov",ui->pelppoverhnost->text());
            query.bindValue(":fon",ui->pelpfon->text());
            query.bindValue(":cvt",ui->pelpcvet->text());

            query.bindValue(":raz", ui->pelprazmer->text() );
            query.bindValue(":dop", ui->pelpprim->text() );
            query.bindValue(":propelpid", pelpid );
            query.exec();
        }

        ui->pelplocalizatia->clear();
        ui->pelppoverhnost->clear();
        ui->pelpfon->clear();
        ui->pelpcvet->clear();
        ui->pelprazmer->clear();
        ui->pelpprim->clear();


        reloadtablepropel();
        ui->propelptb->setFocus();
        ui->propelptb->selectRow(pelptbrow);
    }

}

//===================================================================================================================
// Вкладка 2
//====================================================================================================================

void MainWindow::reloadtablebolezni()
{
    //QSqlQueryModel * model = new QSqlQueryModel(this);

    modelbol->setQuery("SELECT * FROM bolezni");



    ui->boltb->setModel(modelbol);
    ui->boltb->show();

    modelbol->setHeaderData( 1, Qt::Horizontal, QObject::tr("Название заболевания") );

    ui->boltb->setColumnWidth(0, 20);
    ui->boltb->setColumnWidth(1, 600);
    //ui->boltb->setColumnWidth(2, 40);

}

void MainWindow::addnewbolezn()
{

    if (ui->bolnameedit->text() != "" )//&& ui->boldescedit->toPlainText() != ""
       {
        //qDebug() << "123";
        QSqlQuery query;
        QString sqlquery = "insert into bolezni ( Name, Desc ) values ('" + this->ui->bolnameedit->text() + "', '" + this->ui->boldescedit->toPlainText() +"')";
        query.exec(sqlquery);
        reloadtablebolezni();
       }

}

void MainWindow::bolezntableclick()
{

    int boltbrow = ui->boltb->currentIndex().row();

    if (boltbrow >= 0)
    {
        //model->setQuery("SELECT * FROM elementsp");
        QString  bolname = modelbol->record(boltbrow).value("Name").toString();
        QString  boldesc = modelbol->record(boltbrow).value("Desc").toString();

        ui->bolnameedit->setText(bolname);
        ui->boldescedit->setText(boldesc);

    }

    ui->boltb->setFocus();
    ui->boltb->selectRow(boltbrow);

    reloadtablebphotos();
    ui->previewbphoto->clear();
}

void MainWindow::updatebolezn()
{

    int boltbrow = ui->boltb->currentIndex().row();

    if (boltbrow >= 0)
    {
        //model->setQuery("SELECT * FROM elementsp");
        int bolid = modelbol->record(boltbrow).value("Id").toInt();

        if (ui->bolnameedit->text() != "" )
        {
            QSqlQuery query;
            query.prepare("update bolezni set Name = :nm, Desc = :ds where Id = :bolid ");
            query.bindValue(":nm",ui->bolnameedit->text());
            query.bindValue(":ds",ui->boldescedit->toPlainText());
            query.bindValue(":bolid", bolid );
            query.exec();
        }

        ui->bolnameedit->clear();
        ui->boldescedit->clear();

        reloadtablebolezni();
        ui->boltb->setFocus();
        ui->boltb->selectRow(boltbrow);
    }
}

//удаление болезни и всех связей
void MainWindow::deletebolezn()
{
    int boltbrow = ui->boltb->currentIndex().row();

    if (boltbrow >= 0)
    {
        int bolid = modelbol->record(boltbrow).value("Id").toInt();

        modelbol->setQuery("delete FROM bolezni where Id = " + QString::number(bolid));
        modelbol->setQuery("delete FROM bphotos where Bid = " + QString::number(bolid));

        //формы болезней и их фотографии
        modelbol->setQuery("delete FROM formsbolezni where Bolid = " + QString::number(bolid));
        modelbol->setQuery("delete FROM fbphotos where Bolid = " + QString::number(bolid));

        //похожие болезни
        modelbol->setQuery("delete FROM pbolezni where Bolid = " + QString::number(bolid));

        //схемы лечения
        modelbol->setQuery("delete FROM shemalecheniya where Bolid = " + QString::number(bolid));

        //схемы лечения
        modelbol->setQuery("delete FROM bolprop where Bolid = " + QString::number(bolid));


        reloadtablebolezni();
    }
}


void MainWindow::openbolphoto()
{

    QString fileName = QFileDialog::getOpenFileName(this,
                 tr("Select photo"), "",
                 tr("Photo (*.jpg);;All files (*)"));

        ui->bolphotoedit->setText(fileName);

        QPixmap pixmap;
            pixmap.load(fileName);
            ui->bolphotopreview->setPixmap( pixmap.scaled(50,50,Qt::KeepAspectRatio,Qt::SmoothTransformation) );
}

void MainWindow::addnewbolphoto()
{

    int boltbrow = ui->boltb->currentIndex().row();

    if (boltbrow >= 0)
    {
        int bolid = modelbol->record(boltbrow).value("Id").toInt();

    if (ui->bolphotoedit->text() != "")
        {
            QPixmap pixmap;
            pixmap.load(ui->bolphotoedit->text());
            QByteArray ba;
            QBuffer buf( &ba );
            buf.open( QIODevice::ReadWrite);
            pixmap.save( &buf, "jpg" );

            QSqlQuery query;
            query.prepare("insert into bphotos ( Bid, Photo) values (:bolid, :photo)");
            query.bindValue(":bolid",bolid);
            query.bindValue(":photo", ba );
            query.exec();

            reloadtablebphotos();
            ui->bolphotopreview->clear();
            ui->bolphotoedit->clear();

        }
    }
}

void MainWindow::reloadtablebphotos()
{
    int boltbrow = ui->boltb->currentIndex().row();
int bolid;
    if (boltbrow >= 0)
    {
         bolid = modelbol->record(boltbrow).value("Id").toInt();
    }
    if (boltbrow < 0)
    {
         bolid = 0;
    }

//qDebug() << bolid;

    modelbolp->setQuery("SELECT * FROM bphotos where bid = " + QString::number(bolid) );
    ui->bolptb->setModel(modelbolp);
    ui->bolptb->show();

    ui->bolptb->setColumnWidth(0, 20);
    ui->bolptb->setColumnWidth(1, 20);
    //ui->boltb->setColumnWidth(2, 40);

}

void MainWindow::deletebolphoto()
{

    int bolptbrow = ui->bolptb->currentIndex().row();

    if (bolptbrow >= 0)
    {
        int bolpid = modelbolp->record(bolptbrow).value("Id").toInt();

        modelbolp->setQuery("delete FROM bphotos where Id = " + QString::number(bolpid));
        ui->previewbphoto->clear();
        reloadtablebphotos();
    }
}


void MainWindow::bolphototableclick()
{

    int bolptbrow = ui->bolptb->currentIndex().row();

    if (bolptbrow >= 0)
    {
        QByteArray showphoto;
        if (!modelbolp->record(bolptbrow).value("Photo").isNull())
        {
        showphoto = modelbolp->record(bolptbrow).value("Photo").toByteArray();
        }
        if (modelbolp->record(bolptbrow).value("Photo").isNull())
        {
        showphoto.clear();
        }

        if( !showphoto.isEmpty() )
        {
            QPixmap pixmap;
            pixmap.loadFromData(showphoto);
            ui->previewbphoto->setPixmap(pixmap.scaled(111,111,Qt::KeepAspectRatio,Qt::SmoothTransformation));
        }
    }

    ui->bolptb->setFocus();
    ui->bolptb->selectRow(bolptbrow);

}

//===================================================================================================================
// Вкладка 3
//====================================================================================================================

void MainWindow::reloadtableformsforbol()
{


    //modelbol->setQuery("SELECT * FROM bolezni");
    ui->tabforformsbol->setModel(modelbol);
    ui->tabforformsbol->show();

    ui->tabforformsbol->setColumnWidth(0, 20);
    ui->tabforformsbol->setColumnWidth(1, 500);
    ui->tabforformsbol->setColumnHidden(2, true);
    //ui->boltb->setColumnWidth(2, 40);

}

void MainWindow::reloadtableformsbol()
{
    int bolfid;
    int bolftbrow = ui->tabforformsbol->currentIndex().row();

    if (bolftbrow < 0)
    {
      bolfid = 0;
    }


    if (bolftbrow >= 0)
    {
         bolfid = modelbol->record(bolftbrow).value("Id").toInt();
    }

    modelbolf->setQuery("SELECT * FROM formsbolezni where Bolid = " + QString::number(bolfid) );

    modelbolf->setHeaderData( 2, Qt::Horizontal, QObject::tr("Название формы заболевания") );

    ui->formsboltab->setModel(modelbolf);
ui->formsboltab->show();

ui->formsboltab->setColumnWidth(0, 20);
ui->formsboltab->setColumnWidth(1, 30);
ui->formsboltab->setColumnWidth(2, 400);
//ui->tabforformsbol->setColumnHidden(2, true);
//ui->boltb->setColumnWidth(2, 40);
}

void MainWindow::addnewformbol()
{

    int bolftbrow = ui->tabforformsbol->currentIndex().row();

    if (bolftbrow >= 0)
    {
        int bolid = modelbol->record(bolftbrow).value("Id").toInt();

    if (ui->formboledit->text() != "" )//&& ui->boldescedit->toPlainText() != ""
       {
        //qDebug() << "123";
        QSqlQuery query;
        QString sqlquery = "insert into formsbolezni ( Bolid, Name, Desc ) values (" + QString::number(bolid) + ",'" + this->ui->formboledit->text() + "', '" + this->ui->formboldesc->toPlainText() +"')";
        query.exec(sqlquery);
        reloadtableformsbol();
       }
    }
}

//удаление формы болезни
void MainWindow::deleteformbol()
{

    int bolftbrow = ui->formsboltab->currentIndex().row();

    if (bolftbrow >= 0)
    {
        int bolfid = modelbolf->record(bolftbrow).value("Id").toInt();

        modelbolf->setQuery("delete FROM formsbolezni where Id = " + QString::number(bolfid));
        modelbolf->setQuery("delete FROM fbphotos where Fid = " + QString::number(bolfid));
        reloadtableformsbol();
    }
}

void MainWindow::formboltableclick()
{

    int bolftbrow = ui->formsboltab->currentIndex().row();

    if (bolftbrow >= 0)
    {
        QString  fbolname = modelbolf->record(bolftbrow).value("Name").toString();
        QString  fboldesc = modelbolf->record(bolftbrow).value("Desc").toString();

        ui->formboledit->setText(fbolname);
        ui->formboldesc->setText(fboldesc);

    }

    ui->formsboltab->setFocus();
    ui->formsboltab->selectRow(bolftbrow);

    reloadtablefbolphotos();
    //reloadtablebphotos();
    ui->fbolphotoview->clear();
}


void MainWindow::updateformbol()
{

    int bolftbrow = ui->formsboltab->currentIndex().row();

    if (bolftbrow >= 0)
    {
        //model->setQuery("SELECT * FROM elementsp");
        int bolfid = modelbolf->record(bolftbrow).value("Id").toInt();

        if (ui->formboledit->text() != "" )
        {
            QSqlQuery query;
            query.prepare("update formsbolezni set Name = :nm, Desc = :ds where Id = :bolfid ");
            query.bindValue(":nm",ui->formboledit->text());
            query.bindValue(":ds",ui->formboldesc->toPlainText());
            query.bindValue(":bolfid", bolfid );
            query.exec();
        }

        ui->formboledit->clear();
        ui->formboldesc->clear();

        reloadtableformsbol();
        ui->formsboltab->setFocus();
        ui->formsboltab->selectRow(bolftbrow);
    }
}

void MainWindow::forformboltableclick()
{
int bolftbrow = ui->tabforformsbol->currentIndex().row();

    ui->tabforformsbol->setFocus();
    ui->tabforformsbol->selectRow(bolftbrow);

    reloadtableformsbol();
    reloadtablefbolphotos();

    ui->fbolphotoview->clear();

}

void MainWindow::openfbolphoto()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                 tr("Select photo"), "",
                 tr("Photo (*.jpg);;All files (*)"));

        ui->fbolpathedit->setText(fileName);

        QPixmap pixmap;
            pixmap.load(fileName);
            ui->fbolppreview->setPixmap( pixmap.scaled(50,50,Qt::KeepAspectRatio,Qt::SmoothTransformation) );

}

void MainWindow::addnewfbolphoto()
{

    int fboltbrow = ui->formsboltab->currentIndex().row();
    int bolftbrow = ui->tabforformsbol->currentIndex().row();


    if (fboltbrow >= 0 && bolftbrow >= 0)
    {
        int fbolid = modelbolf->record(fboltbrow).value("Id").toInt();
        int bolid = modelbol->record(bolftbrow).value("Id").toInt();

    if (ui->fbolpathedit->text() != "")
        {
            QPixmap pixmap;
            pixmap.load(ui->fbolpathedit->text());
            QByteArray ba;
            QBuffer buf( &ba );
            buf.open( QIODevice::ReadWrite);
            pixmap.save( &buf, "jpg" );

            QSqlQuery query;
            query.prepare("insert into fbphotos ( Fid, Bolid, Photo) values (:fbolid, :bolid, :photo)");
            query.bindValue(":fbolid",fbolid);
            query.bindValue(":bolid",bolid);
            query.bindValue(":photo", ba );
            query.exec();

           reloadtablefbolphotos();
            ui->fbolppreview->clear();
            ui->fbolpathedit->clear();

        }
    }
}

void MainWindow::reloadtablefbolphotos()
{
    int fbolid;
    int fboltbrow = ui->formsboltab->currentIndex().row();

    if (fboltbrow < 0)
    {
      fbolid = 0;
    }

    if (fboltbrow >= 0)
    {
         fbolid = modelbolf->record(fboltbrow).value("Id").toInt();
    }

    modelbolfp->setQuery("SELECT * FROM fbphotos where Fid = " + QString::number(fbolid) );
ui->fbolphotostb->setModel(modelbolfp);
ui->fbolphotostb->show();

ui->fbolphotostb->setColumnWidth(0, 20);
ui->fbolphotostb->setColumnWidth(1, 20);
ui->fbolphotostb->setColumnWidth(2, 40);
//ui->fbolphotostb->setColumnHidden(2, true);

}

void MainWindow::fbolphotostableclick()
{

    int fbolptbrow = ui->fbolphotostb->currentIndex().row();

    if (fbolptbrow >= 0)
    {
        QByteArray showphoto;
        if (!modelbolfp->record(fbolptbrow).value("Photo").isNull())
        {
        showphoto = modelbolfp->record(fbolptbrow).value("Photo").toByteArray();
        }
        if (modelbolfp->record(fbolptbrow).value("Photo").isNull())
        {
        showphoto.clear();
        }

        if( !showphoto.isEmpty() )
        {
            QPixmap pixmap;
            pixmap.loadFromData(showphoto);
            ui->fbolphotoview->setPixmap(pixmap.scaled(121,121,Qt::KeepAspectRatio,Qt::SmoothTransformation));
        }
    }

    ui->fbolphotostb->setFocus();
    ui->fbolphotostb->selectRow(fbolptbrow);

}

void MainWindow::deletefbolphoto()
{

    int fbolptbrow = ui->fbolphotostb->currentIndex().row();

    if (fbolptbrow >= 0)
    {
        int fbolpid = modelbolfp->record(fbolptbrow).value("Id").toInt();

        modelbolfp->setQuery("delete FROM fbphotos where Id = " + QString::number(fbolpid));
        reloadtablefbolphotos();
        ui->fbolphotoview->clear();
    }
}

//===================================================================================================================
// Вкладка 4
//====================================================================================================================

void MainWindow::reloadtableshbol1()
{

    ui->shboltb1->setModel(modelbol);
    ui->shboltb1->show();

    ui->shboltb1->setColumnWidth(0, 20);
    ui->shboltb1->setColumnWidth(1, 500);
    ui->shboltb1->setColumnHidden(2, true);
    //ui->boltb->setColumnWidth(2, 40);

}

void MainWindow::reloadtableshbol2()
{

    ui->shboltb2->setModel(modelbol);
    ui->shboltb2->show();

    ui->shboltb2->setColumnWidth(0, 20);
    ui->shboltb2->setColumnWidth(1, 500);
    ui->shboltb2->setColumnHidden(2, true);
    //ui->boltb->setColumnWidth(2, 40);

}

void MainWindow::reloadtableshbol3()
{

    int sbolid;
        int sboltbrow = ui->shboltb1->currentIndex().row();

        if (sboltbrow < 0)
        {
          sbolid = 0;
        }

        if (sboltbrow >= 0)
        {
          sbolid = modelbol->record(sboltbrow).value("Id").toInt();
        }

        modelbolsh->setQuery("SELECT t1.Id, t1.Bolid, t1.Sbolid, t2.Id, t2.Name FROM pbolezni t1, bolezni t2 where t1.Bolid = " + QString::number(sbolid) + " and t1.Sbolid = t2.Id" );

        modelbolsh->setHeaderData( 4, Qt::Horizontal, QObject::tr("Название заболевания") );

        ui->shboltb3->setModel(modelbolsh);
    ui->shboltb3->show();

    ui->shboltb3->setColumnWidth(0, 20);
    //ui->shboltb3->setColumnWidth(1, 20);
    //ui->fbolphotostb->setColumnWidth(2, 50);
   ui->shboltb3->setColumnHidden(1, true);
   ui->shboltb3->setColumnHidden(2, true);
   ui->shboltb3->setColumnHidden(3, true);
    ui->shboltb3->setColumnWidth(4, 500);

}

void MainWindow::addnewshbol()
{
    int boltb1row = ui->shboltb1->currentIndex().row();
    int boltb2row = ui->shboltb2->currentIndex().row();

    if (boltb1row >= 0 && boltb2row >= 0)
    {
        int bolid1 = modelbol->record(boltb1row).value("Id").toInt();
        int bolid2 = modelbol->record(boltb2row).value("Id").toInt();

        QSqlQuery queryd;
        QString sqlqueryd = "select count(*) from pbolezni where Bolid=" + QString::number(bolid1) + " and Sbolid=" +QString::number(bolid2)  ;//where Bolid=" + QString::number(bolid1) + " and Sbolid=" +QString::number(bolid2)
        queryd.exec(sqlqueryd);

        int cntqr = 0;
        if (queryd.next())
        cntqr = queryd.value(0).toInt();


        if (cntqr == 0){

        QSqlQuery query;
        QString sqlquery = "insert into pbolezni ( Bolid, Sbolid ) values (" + QString::number(bolid1) + "," +QString::number(bolid2) + ")";//
        query.exec(sqlquery);
        reloadtableshbol3();
        }
    }
}

void MainWindow::shtable1click()
{
int shboltb1row = ui->shboltb1->currentIndex().row();

    ui->shboltb1->setFocus();
    ui->shboltb1->selectRow(shboltb1row);

   reloadtableshbol3();

}

void MainWindow::shtable2click()
{
int shboltb2row = ui->shboltb2->currentIndex().row();

    ui->shboltb2->setFocus();
    ui->shboltb2->selectRow(shboltb2row);

}

void MainWindow::shtable3click()
{
int shboltb3row = ui->shboltb3->currentIndex().row();

    ui->shboltb3->setFocus();
    ui->shboltb3->selectRow(shboltb3row);

}

void MainWindow::deleteshbol()
{

    int shboltb3row = ui->shboltb3->currentIndex().row();
qDebug() << shboltb3row;
    if (shboltb3row >= 0)
    {
        int shbolid = modelbolsh->record(shboltb3row).value("Id").toInt();
//qDebug() << shbolid;
        modelbolsh->setQuery("delete FROM pbolezni where Id = " + QString::number(shbolid));
        reloadtableshbol3();

    }
}

//===================================================================================================================
// Вкладка 5
//====================================================================================================================

void MainWindow::reloadtableslbol()
{

    ui->slboltb->setModel(modelbol);
    ui->slboltb->show();

    ui->slboltb->setColumnWidth(0, 20);
    ui->slboltb->setColumnWidth(1, 500);
    ui->slboltb->setColumnHidden(2, true);
    //ui->boltb->setColumnWidth(2, 40);

}


void MainWindow::addnewsl()
{
    int slboltbrow = ui->slboltb->currentIndex().row();

    if (slboltbrow >= 0)
    {
        int bolid = modelbol->record(slboltbrow).value("Id").toInt();

        QSqlQuery queryd;
        QString sqlqueryd = "select count(*) from shemalecheniya where Bolid=" + QString::number(bolid)   ;
        queryd.exec(sqlqueryd);

        int cntqr = 0;
        if (queryd.next())
        cntqr = queryd.value(0).toInt();

        if (cntqr == 0){

        QSqlQuery query;
        QString sqlquery = "insert into shemalecheniya ( Bolid, Desc ) values (" + QString::number(bolid) + ", '" + ui->sldescedit->toPlainText() + "')";//
        query.exec(sqlquery);
        //reloadtableslbol();
        }
    }
}

void MainWindow::sltableclick()
{
int slboltbrow = ui->slboltb->currentIndex().row();

    ui->slboltb->setFocus();
    ui->slboltb->selectRow(slboltbrow);

    reloadmodelbolsl();

int  bolid = modelbol->record(slboltbrow).value("Id").toInt();
QString  sldesc = "";

QSqlQuery queryd;
QString sqlqueryd = "select count(*) from shemalecheniya where Bolid=" + QString::number(bolid)   ;
queryd.exec(sqlqueryd);

int cntsl = 0;
if (queryd.next())
cntsl = queryd.value(0).toInt();

if (cntsl > 0)
{
    QSqlQuery querysl;
    QString sqlquerysl = "select * from shemalecheniya where Bolid=" + QString::number(bolid)   ;
    querysl.exec(sqlquerysl);
    if (querysl.next())
      sldesc = querysl.value(2).toString();

    ui->sltextBrowser->setText(sldesc);
    ui->sldescedit->setText(sldesc);
}

if (cntsl == 0)
{
    ui->sltextBrowser->setText("");
    ui->sldescedit->setText("");
}

}

void MainWindow::reloadmodelbolsl()
{

    modelbolsl->setQuery("SELECT * FROM shemalecheniya " );
}

void MainWindow::deleteslbol()
{

    int slboltbrow = ui->slboltb->currentIndex().row();

    if (slboltbrow >= 0)
    {
        int slbolid = modelbol->record(slboltbrow).value("Id").toInt();

        modelbolsl->setQuery("delete FROM shemalecheniya where Bolid = " + QString::number(slbolid));
        reloadmodelbolsl();
        ui->sltextBrowser->setText("");
        ui->sldescedit->setText("");
    }
}


void MainWindow::updateslbol()
{

    int bolsltbrow = ui->slboltb->currentIndex().row();
    int slid = 0;
    QString sldesc = "";

    if (bolsltbrow >= 0)
    {

        int bolid = modelbol->record(bolsltbrow).value("Id").toInt();

        QSqlQuery queryd;
        QString sqlqueryd = "select count(*) from shemalecheniya where Bolid=" + QString::number(bolid)   ;
        queryd.exec(sqlqueryd);

        int cntsl = 0;
        if (queryd.next())
        cntsl = queryd.value(0).toInt();

        if (cntsl == 1)
        {

            QSqlQuery querysl;
            QString sqlquerysl = "select * from shemalecheniya where Bolid=" + QString::number(bolid)   ;
            querysl.exec(sqlquerysl);
            if (querysl.next())
              slid = querysl.value(0).toInt();

            QSqlQuery query;
            query.prepare("update shemalecheniya set  Desc = :ds where Id = :bolslid ");
            query.bindValue(":ds",ui->sldescedit->toPlainText());
            query.bindValue(":bolslid", slid );
            query.exec();

            ui->sldescedit->clear();
            ui->sltextBrowser->clear();

            reloadmodelbolsl();

            QSqlQuery querysl2;
            QString sqlquerysl2 = "select * from shemalecheniya where Id=" + QString::number(slid)   ;
            querysl2.exec(sqlquerysl2);
            if (querysl2.next())
              sldesc = querysl2.value(2).toString();
            ui->sltextBrowser->setText(sldesc);


        }


    }
}

//===================================================================================================================
// Вкладка 6
//====================================================================================================================

void MainWindow::reloadtablebolel()
{

    ui->boleltb->setModel(modelbol);
    ui->boleltb->show();

    ui->boleltb->setColumnWidth(0, 20);
    ui->boleltb->setColumnWidth(1, 500);
    ui->boleltb->setColumnHidden(2, true);
    //ui->boltb->setColumnWidth(2, 40);

}

void MainWindow::reloadtablebolprop()
{

    int bolid;

    if (ui->boleltb->currentIndex().row() < 0){
       bolid = 0;
    }

    if (ui->boleltb->currentIndex().row() >= 0){
        bolid = modelbol->record(ui->boleltb->currentIndex().row()).value("Id").toInt();
    }

    modelbolprop->setQuery("SELECT t1.Id, t1.Bolid, t1.Elpid, t1.Pelpid, t2.Id, t2.Name, t3.Id, t3.Localizatiya, t3.Poverhnost, t3.Fon, t3.Cvet, t3.Razmer FROM bolprop t1, elementsp t2, propertieselp t3 where t1.Bolid = " + QString::number(bolid) + " and t2.Id = t1.Elpid and t3.Id = t1.Pelpid");

    modelbolprop->setHeaderData( 5, Qt::Horizontal, QObject::tr("Название элемента поражения") );
    modelbolprop->setHeaderData( 7, Qt::Horizontal, QObject::tr("Локализация") );
    modelbolprop->setHeaderData( 8, Qt::Horizontal, QObject::tr("Поверхность") );
    modelbolprop->setHeaderData( 9, Qt::Horizontal, QObject::tr("Фон") );
    modelbolprop->setHeaderData( 10, Qt::Horizontal, QObject::tr("Цвет") );
    modelbolprop->setHeaderData( 11, Qt::Horizontal, QObject::tr("Размер") );


    ui->bolpropelstb->setModel(modelbolprop);
    ui->bolpropelstb->show();

    ui->bolpropelstb->setColumnWidth(0, 20);
    ui->bolpropelstb->setColumnWidth(1, 30);
    ui->bolpropelstb->setColumnWidth(2, 30);
    ui->bolpropelstb->setColumnWidth(3, 40);
    ui->bolpropelstb->setColumnWidth(4, 20);
    ui->bolpropelstb->setColumnWidth(5, 200);
    ui->bolpropelstb->setColumnWidth(6, 20);
    ui->bolpropelstb->setColumnWidth(7, 200);
    ui->bolpropelstb->setColumnWidth(8, 200);
    ui->bolpropelstb->setColumnWidth(9, 150);
    ui->bolpropelstb->setColumnWidth(10, 150);
    ui->bolpropelstb->setColumnWidth(11, 50);



    //ui->bolpropelstb->setColumnWidth(1, 500);
    //ui->boleltb->setColumnHidden(2, true);
    //ui->boltb->setColumnWidth(2, 40);

}



void MainWindow::reloadtableels()
{
    //QSqlQueryModel * model = new QSqlQueryModel(this);

    //model->setQuery("SELECT * FROM elementsp");
    ui->elstb->setModel(model);
    ui->elstb->show();

    ui->elstb->setColumnWidth(0, 20);
    ui->elstb->setColumnWidth(1, 450);
    ui->elstb->setColumnWidth(2, 40);

}

void MainWindow::reloadtablepropels()
{
    int rownum;

    if (ui->elstb->currentIndex().row() < 0){
       rownum = 0;
    }

    if (ui->elstb->currentIndex().row() >= 0){
        model3->setQuery("SELECT * FROM elementsp");
        rownum = model3->record(ui->elstb->currentIndex().row()).value("Id").toInt();
    }

    model2->setQuery("SELECT * FROM propertieselp where elpid="+QString::number(rownum));
    ui->propelstb->setModel(model2);
    ui->propelstb->show();

    ui->propelstb->setColumnWidth(0, 20);
    ui->propelstb->setColumnWidth(1, 30);
    ui->propelstb->setColumnWidth(2, 200);
    ui->propelstb->setColumnWidth(3, 200);
    ui->propelstb->setColumnWidth(4, 150);
    ui->propelstb->setColumnWidth(5, 150);


}

void MainWindow::elstbclick()
{

    int elstbrow = ui->elstb->currentIndex().row();

    ui->elstb->setFocus();
        ui->elstb->selectRow(elstbrow);

reloadtablepropels();
}


void MainWindow::addtolist()
{
    int boltbrow = ui->boleltb->currentIndex().row();
    int propelstbrow = ui->propelstb->currentIndex().row();
    int elstbrow = ui->elstb->currentIndex().row();

    if (boltbrow >= 0 && propelstbrow >= 0  && elstbrow >= 0)
    {
        int bolid = modelbol->record(boltbrow).value("Id").toInt();
        int elpid = model2->record(propelstbrow).value("Id").toInt();
        int elspid = model->record(elstbrow).value("Id").toInt();

        QSqlQuery queryd;
        QString sqlqueryd = "select count(*) from bolprop where Bolid=" + QString::number(bolid) + " and Pelpid=" + QString::number(elpid)  ;
        queryd.exec(sqlqueryd);

        int cntqr = 0;
        if (queryd.next())
        cntqr = queryd.value(0).toInt();

        //qDebug() << cntqr;

        if (cntqr == 0){

        QSqlQuery query;
        QString sqlquery = "insert into bolprop ( Bolid, Elpid, Pelpid ) values (" + QString::number(bolid) + ", " + QString::number(elspid) + ", " + QString::number(elpid) + ")";//
        //qDebug() << sqlquery;
        query.exec(sqlquery);
        reloadtablebolprop();
        }
    }
}

void MainWindow::boleltbclick()
{

    int boleltbrow = ui->boleltb->currentIndex().row();

    ui->boleltb->setFocus();
        ui->boleltb->selectRow(boleltbrow);

reloadtablebolprop();
}


void MainWindow::deletetolist()
{

    int bolpropelstbrow = ui->bolpropelstb->currentIndex().row();

    if (bolpropelstbrow >= 0)
    {
        int bpid = modelbolprop->record(bolpropelstbrow).value("Id").toInt();

        modelbolsl->setQuery("delete FROM bolprop where Id = " + QString::number(bpid));
        reloadtablebolprop();

    }
}
