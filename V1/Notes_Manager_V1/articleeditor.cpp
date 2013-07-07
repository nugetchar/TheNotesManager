#include "articleeditor.h"
#include "ui_articleeditor.h"
#include "Managers.h"
#include "NotesModule.h"
#include <QMessageBox>
#include "Workspace.h"
#include "delete_note_foncteur.h"
ArticleEditor::ArticleEditor(NotesModule::Article* art, QWidget *parent): QWidget(parent), ui(new Ui::ArticleEditor), article(art)
{
    ui->setupUi(this);
    ui->pushButtonDelete->setEnabled(false);
    if(article){
        ui->lineEditTitle->setText((article)->getTitle());
        ui->textEditContent->setText(article->getText());
    }

    //connections
    QObject::connect(ui->pushButtonSave,SIGNAL(clicked()),this,SLOT(saveArticle()));
    QObject::connect(ui->lineEditTitle,SIGNAL(textEdited(QString)),this,SLOT(activateSave(QString)));
    QObject::connect(ui->textEditContent,SIGNAL(textChanged()),this,SLOT(activateSave()));
    QObject::connect(ui->pushButtonDelete,SIGNAL(clicked()),this,SLOT(delete_note()));
}

ArticleEditor::~ArticleEditor()
{
    delete ui;
}

void ArticleEditor::activateSave(QString){
    ui->pushButtonSave->setEnabled(true);
}

void ArticleEditor::saveArticle(){
    updateArticle();
    workspace::Workspace::getInstance()->save_note("article",article->getId());
    QMessageBox::information(this, "Save","Your article has been saved at the location: "
                             + article->getId());
    ui->pushButtonSave->setEnabled(false);
    ui->pushButtonDelete->setEnabled(true);
}


void ArticleEditor::updateArticle(){

    if(!article)   article = dynamic_cast<NotesModule::Article*>(workspace::Workspace::getInstance()->create_note("article"));
    (article)->setTitle(ui->lineEditTitle->text());
    article->setText(ui->textEditContent->toPlainText());

}

void ArticleEditor::delete_note(){
    DeleterForEditors d;
    d(article);
    article = 0;
    ui->pushButtonDelete->setEnabled(false);
    ui->pushButtonSave->setEnabled(true);
}



