#include "imangeeditor.h"
#include "ui_imangeeditor.h"
#include "delete_note_foncteur.h"
#include <QFileDialog>
#include <QMessageBox>

ImangeEditor::ImangeEditor(NotesModule::Image* img, QWidget *parent) :QWidget(parent), ui(new Ui::ImangeEditor), image(img)
{
    ui->setupUi(this);
    ui->pushButtonDelete->setEnabled(false);

    if (image){
        ui->lineEditTitle->setText(dynamic_cast<NotesModule::Note*>(image)->getTitle());
        ui->textEditDescription->setText(dynamic_cast<NotesModule::Multimedia*>(image)->getDescription());
        ui->pushButtonDelete->setEnabled(false);
    }

    QObject::connect(ui->pushButtonSave,SIGNAL(clicked()),this,SLOT(saveImage()));
    QObject::connect(ui->lineEditTitle,SIGNAL(textEdited(QString)),this,SLOT(activateSave(QString)));
    QObject::connect(ui->textEditDescription,SIGNAL(textChanged()),this,SLOT(activateSave()));
    QObject::connect(ui->pushButtonBrowse,SIGNAL(clicked()),this,SLOT(getImagePath()));
    QObject::connect(ui->pushButtonDelete,SIGNAL(clicked()),this,SLOT(delete_note()));
}

ImangeEditor::~ImangeEditor()
{
    delete ui;
}

void ImangeEditor::activateSave(QString){
    ui->pushButtonSave->setEnabled(true);
}

void ImangeEditor::saveImage(){

    updateImage();
    workspace::Workspace::getInstance()->save_note("image",image->getId());
    QMessageBox::information(this, "Save","Your picture has been saved at this location: "
                             + workspace::Workspace::getInstance()->get_path() + "\\" + image->getId());
    ui->pushButtonSave->setEnabled(false);
ui->pushButtonDelete->setEnabled(true);
}

void ImangeEditor::updateImage(){

    if(!image)
        image = dynamic_cast<NotesModule::Image*>(workspace::Workspace::getInstance()->create_note("image"));
    dynamic_cast<NotesModule::Note*>(image)->setTitle(ui->lineEditTitle->text());
    dynamic_cast<NotesModule::Multimedia*>(image)->setDescription(ui->textEditDescription->toPlainText());
    dynamic_cast<NotesModule::Multimedia*>(image)->setPath(image_path);
}

void ImangeEditor::getImagePath(){
    QString path = QFileDialog::getOpenFileName();
    image_path = path;
    QString s="<img src='" + path + "' />";
    ui->labelImage->setText(s);
}

void ImangeEditor::delete_note(){
    DeleterForEditors d;
    d(image);
    image = 0;
    ui->pushButtonDelete->setEnabled(false);
    ui->pushButtonSave->setEnabled(true);
}


