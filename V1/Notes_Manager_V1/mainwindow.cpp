#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "articleeditor.h"
#include "audioeditor.h"
#include <QMessageBox>
#include <QFileDialog>
#include "Managers.h"
#include "Workspace.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //addParentToTree("hanane");
    //addParentToTree("mezzine"");


    editorArticle=0;
    editorAudio=0;
    editorVideo=0;
    editorImage=0;
    visualisatorText=0;
    //visualisatorHtml;
    //visualisatorLatex;

    //connections
    QObject::connect(ui->actionExit_2, SIGNAL(triggered()), qApp, SLOT(quit()));
    QObject::connect(ui->actionNewArticle, SIGNAL(triggered()),this, SLOT(openNewArticle()));
    QObject::connect(ui->actionNewAudio, SIGNAL(triggered()),this, SLOT(openNewAudio()));
    QObject::connect(ui->actionNewVideo, SIGNAL(triggered()),this, SLOT(openNewVideo()));
    QObject::connect(ui->actionNewPicture, SIGNAL(triggered()),this, SLOT(openNewImage()));
    QObject::connect(ui->actionWorkspace, SIGNAL(triggered()),this, SLOT(openWorkspace()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openNewArticle(){

    ui->tabWidgetOnglets->clear();

    deleteTabs();

    editorArticle=new ArticleEditor(0,ui->tabWidgetOnglets);
    ui->tabWidgetOnglets->addTab(editorArticle, "Editor");

    visualisatorText = new QLabel("",ui->tabWidgetOnglets);
    ui->tabWidgetOnglets->addTab(visualisatorText, "Text");

    QObject::connect(ui->tabWidgetOnglets,SIGNAL(currentChanged(int)),this,SLOT(changementOngletArticle(int)));

}

void MainWindow::openNewAudio() {

    ui->tabWidgetOnglets->clear();

    deleteTabs();

    editorAudio=new AudioEditor(0,ui->tabWidgetOnglets);
    ui->tabWidgetOnglets->addTab(editorAudio, "Editor");

    visualisatorText = new QLabel("",ui->tabWidgetOnglets);
    ui->tabWidgetOnglets->addTab(visualisatorText, "Text");
    QObject::connect(ui->tabWidgetOnglets,SIGNAL(currentChanged(int)),this,SLOT(changementOngletAudio(int)));

}

void MainWindow::openNewVideo() {

    ui->tabWidgetOnglets->clear();

    deleteTabs();

    editorVideo=new VideoEditor(0,ui->tabWidgetOnglets);
    ui->tabWidgetOnglets->addTab(editorVideo, "Editor");

    visualisatorText = new QLabel("",ui->tabWidgetOnglets);

    ui->tabWidgetOnglets->addTab(visualisatorText, "Text");


    QObject::connect(ui->tabWidgetOnglets,SIGNAL(currentChanged(int)),this,SLOT(changementOngletVideo(int)));

}

void MainWindow::openNewImage() {

    ui->tabWidgetOnglets->clear();

    deleteTabs();

    editorImage=new ImangeEditor(0,ui->tabWidgetOnglets);
    ui->tabWidgetOnglets->addTab(editorImage, "Editor");

    visualisatorText = new QLabel("",ui->tabWidgetOnglets);
    ui->tabWidgetOnglets->addTab(visualisatorText, "Text");

    QObject::connect(ui->tabWidgetOnglets,SIGNAL(currentChanged(int)),this,SLOT(changementOngletImage(int)));

}

void MainWindow::changementOngletArticle(int i){
    qDebug()<<"je suis venue ici";
    if (i==1){
        editorArticle->updateArticle();
        QString str=editorArticle->getArticle().getTitle()+"\n\n"+editorArticle->getArticle().getText()+"\n";
        visualisatorText->setText(str);
    }
    // i=2 export Html
    // i=3 export Latex
}

void MainWindow::changementOngletAudio(int i){
    if (i==1){
        editorAudio->updateAudio();
        QString str=editorAudio->getAudio().getTitle()+"\n\n"+editorAudio->getAudio().getDescription()+"\n\n"+editorAudio->getAudio().getPath();
        visualisatorText->setText(str);

    }
    // i=2 export Html
    // i=3 export Latex
}

void MainWindow::changementOngletVideo(int i){
    if (i==1){
        editorVideo->updateVideo();
        QString str=editorVideo->getVideo().getTitle()+"\n\n"+editorVideo->getVideo().getDescription()+"\n\n"+editorVideo->getVideo().getPath();
        visualisatorText->setText(str);

    }
    // i=2 export Html
    // i=3 export Latex
}

void MainWindow::changementOngletImage(int i){
    if (i==1){
        editorImage->updateImage();
        QString str=editorImage->getImage().getTitle()+"\n\n"+editorImage->getImage().getDescription()+"\n\n"+editorImage->getImage().getPath();
        visualisatorText->setText(str);


    }
    // i=2 export Html
    // i=3 export Latex
}

void MainWindow::deleteTabs(){

    if (editorArticle) delete editorArticle; editorArticle=0;
    if (editorAudio)   delete editorAudio;   editorAudio=0;
    if (editorVideo)   delete editorVideo;   editorVideo=0;
    if (editorImage)   delete editorImage;   editorImage=0;

    if (visualisatorText)     delete visualisatorText;   visualisatorText=0;
    //if (visualisatorHtml)   delete visualisatorHtml;     visualisatorHtml=0;
    //if (visualisatorLatex)   delete visualisatorLatex;     visualisatorLatex=0;

    QObject::disconnect(ui->tabWidgetOnglets,SIGNAL(currentChanged(int)),this,SLOT(changementOngletArticle(int)));
    QObject::disconnect(ui->tabWidgetOnglets,SIGNAL(currentChanged(int)),this,SLOT(changementOngletAudio(int)));
    QObject::disconnect(ui->tabWidgetOnglets,SIGNAL(currentChanged(int)),this,SLOT(changementOngletVideo(int)));
    QObject::disconnect(ui->tabWidgetOnglets,SIGNAL(currentChanged(int)),this,SLOT(changementOngletImage(int)));




}

void MainWindow::openWorkspace(){
    QString path=QFileDialog::getExistingDirectory();
    if(path!=""){
        try{
            workspace::Workspace::change_workspace(path);
        }catch(WorkspaceException e){
            qDebug() << e.getInfo() << '\n';
        }catch(NotesManagerException e1){
            qDebug() << e1.getInfo() << '\n';
        }
        //refreshTreeNotes();
    }

}
/*
void MainWindow::refreshTreeNotes(){
    QSet doc = workspace::Workspace::getInstance()->get_Note_by_type("document");
    for(QSet::ConstIterator it=doc.constBegin(); it<doc.constEnd(); ++it){

    }
}

void MainWindow::addParentToTree(QString name){
    QTreeWidgetItem *note = new QTreeWidgetItem(ui->treeWidget);
    note->setText(0,name);

}

void MainWindow::addChildToTree(QTreeWidgetItem* parent, QString name){
    QTreeWidgetItem *note = new QTreeWidgetItem(ui->treeWidget);
    note->setText(0,name);
    parent->addChild(note);
}
*/
