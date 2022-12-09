#include "destination.h"
#include "ui_destination.h"
#include "voyage.h"
#include"mainwindow.h"
#include <QString>
#include<QValidator>
#include<QMessageBox>
#include<QSqlQueryModel>
#include<QIntValidator>
#include<QtPrintSupport>
#include<QFileDialog>
#include"qrcodegen.h"
#include "arduino.h"




destination::destination(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::destination)
{
    ui->setupUi(this);
    ui->tab_voyage->setModel(V.afficher());
    C.connect_arduino();

}

destination::~destination()
{
    delete ui;
}

void destination::on_pb_ajouter_clicked()
{
    QString CODE=ui->le_code->text();
    QString DESTINATION=ui->le_dest->text();
    QString HEBERGEMENT=ui->le_hebergement->text();
    QString DUREE=ui->le_dure->text();
    QString PRIX=ui->le_prix->text();

        voyage P(CODE ,DESTINATION ,HEBERGEMENT ,DUREE ,PRIX);
        bool test=P.ajouter();
           QMessageBox msgBox;
           if (test)
                {msgBox.setText("Ajout avec succes");
           ui->tab_voyage->setModel(V.afficher());
           }
           else
                msgBox.setText("Echec de l'ajout");
                msgBox.exec();

}



void destination::on_le_code_supp_clicked()
{
    voyage V1; V1.setcode(ui->supprimer->text());
    bool test=V1.supprimer(V1.getcode());
    QMessageBox msgBox;

    if(test){
    msgBox.setText("supression avec succés.");
    ui->tab_voyage->setModel(V.afficher());
     }
    else{
        msgBox.setText("Echec de supression");
        msgBox.exec();
    }
}

void destination::on_pb_modifier_clicked()
{
           QString code=ui->le_code->text();
           QString destination=ui->le_dest->text();
           QString hebergement=ui->le_hebergement->text();
           QString duree=ui->le_dure->text();
           QString prix=ui->le_prix->text();


            voyage P(code ,destination ,hebergement ,duree ,prix);
            if(P.modifier_voyage(code))
            {
            QMessageBox::information(nullptr, QObject::tr("Modifier un voyage"), QObject::tr("Vol modifié.\n"), QMessageBox::Cancel);
                ui->tab_voyage->setModel(P.afficher());
 }
 else
            {QMessageBox::critical(nullptr, QObject::tr("modifier un voyage"), QObject::tr("Erreur !!!\n"), QMessageBox::Cancel);

                ui->le_code->text().toInt();
                ui->le_dest->text();
                ui->le_hebergement->text();
                ui->le_dure->text();
                ui->le_prix->text().toFloat();
            }

}



void destination::on_pb_rechercher_textChanged(const QString &arg1)
{
    ui->tab_voyage->setModel(V.rechercherCode(arg1));
}

void destination::on_comboBox_currentIndexChanged(int index)
{
    if(index == 1)
           ui->tab_voyage->setModel(V.Triecode());
       else if(index == 2)
           ui->tab_voyage->setModel(V.Triedest());
       else
           ui->tab_voyage->setModel(V.afficher());
}

void destination::on_pushButton_6_clicked()
{
    voyage V;
    QString text=V.exporter();
    ui->print->setText(text);
    QPrinter printer ;
    printer.setPrinterName("imprim");
    QPrintDialog dialog (&printer,this);
    if(dialog.exec()==QDialog::Rejected) return ;
    ui->print->print(&printer);

}

void destination::on_pushButton_14_clicked()
{
    {
        QString text ="detail du voyage  :"+ ui->tab_voyage->model()->data(ui->tab_voyage->model()->index(ui->tab_voyage->currentIndex().row(),3)).toString()
                   +" "+ui->tab_voyage->model()->data(ui->tab_voyage->model()->index(ui->tab_voyage->currentIndex().row(),2)).toString()
                   +" "+ui->tab_voyage->model()->data(ui->tab_voyage->model()->index(ui->tab_voyage->currentIndex().row(),3)).toString()
                   +" "+ui->tab_voyage->model()->data(ui->tab_voyage->model()->index(ui->tab_voyage->currentIndex().row(),4)).toString()
                   +" "+ui->tab_voyage->model()->data(ui->tab_voyage->model()->index(ui->tab_voyage->currentIndex().row(),5)).toString();


           using namespace qrcodegen;
             QrCode qr = QrCode::encodeText( text.toUtf8().data(), QrCode::Ecc::MEDIUM );
             qint32 sz = qr.getSize();
             QImage im(sz,sz, QImage::Format_RGB32);
               QRgb black = qRgb(  0,  0,  0);
               QRgb white = qRgb(255,255,255);
             for (int y = 0; y < sz; y++)
               for (int x = 0; x < sz; x++)
                 im.setPixel(x,y,qr.getModule(x, y) ? black : white );
             ui->qrcode->setPixmap( QPixmap::fromImage(im.scaled(200,200,Qt::KeepAspectRatio,Qt::FastTransformation),Qt::MonoOnly) );
    }
}



void destination::on_refreshButton_released()
{

}

void destination::on_refreshButton_clicked()
{
    QString data = C.read_from_arduino();
    int result = data.split( "," )[1].toInt();
    if (result == 0 )
        ui->fireLabel->setText("FIRE");
    else
        ui->fireLabel->setText("NO FIRE");
}

void destination::on_offButton_pressed()
{

}

void destination::on_offButton_clicked()
{
    C.write_to_arduino("s");
}