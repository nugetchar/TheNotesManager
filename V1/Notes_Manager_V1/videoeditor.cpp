#include "videoeditor.h"
#include "ui_videoeditor.h"
#include "Workspace.h"
#include "delete_note_foncteur.h"
#include <QFileDialog>
#include <QMessageBox>


VideoEditor::VideoEditor(NotesModule::Video* vid ,QWidget *parent) :QWidget(parent), ui(new Ui::VideoEditor), video(vid)
{
    ui->setupUi(this);
    ui->videoPlayer->setMinimumHeight(170);
    ui->volumeSlider->setAudioOutput(ui->videoPlayer->audioOutput());
    ui->volumeSlider->setEnabled(false);
    ui->pushButtonDelete->setEnabled(false);
    if (video){
        ui->lineEditTitle->setText(dynamic_cast<NotesModule::Note*>(video)->getTitle());
        ui->textEditDescription->setText(dynamic_cast<NotesModule::Multimedia*>(video)->getDescription());

    }

    QObject::connect(ui->pushButtonSave,SIGNAL(clicked()),this,SLOT(saveVideo()));
    QObject::connect(ui->lineEditTitle,SIGNAL(textEdited(QString)),this,SLOT(activateSave(QString)));
    QObject::connect(ui->textEditDescription,SIGNAL(textChanged()),this,SLOT(activateSave()));
    QObject::connect(ui->pushButtonBrowse,SIGNAL(clicked()),this,SLOT(getVideoPath()));
    QObject::connect(ui->pushButtonPlay,SIGNAL(clicked()),this,SLOT(playVideo()));
    QObject::connect(ui->pushButtonStop,SIGNAL(clicked()),this,SLOT(stopVideo()));
    QObject::connect(ui->pushButtonPause,SIGNAL(clicked()),this,SLOT(pauseVideo()));
    QObject::connect(ui->pushButtonDelete,SIGNAL(clicked()),this,SLOT(delete_note()));
}

VideoEditor::~VideoEditor()
{
    delete ui;
}

void VideoEditor::activateSave(QString){
    ui->pushButtonSave->setEnabled(true);
}

void VideoEditor::saveVideo(){

    updateVideo();
    workspace::Workspace::getInstance()->save_note("video",video->getId());
    QMessageBox::information(this, "Save","Your video has been saved.");
    ui->pushButtonSave->setEnabled(false);

    ui->pushButtonDelete->setEnabled(true);
}

void VideoEditor::updateVideo(){

    if(!video)
        video = dynamic_cast<NotesModule::Video*>(workspace::Workspace::getInstance()->create_note("video"));
    dynamic_cast<NotesModule::Note*>(video)->setTitle(ui->lineEditTitle->text());
    dynamic_cast<NotesModule::Multimedia*>(video)->setDescription(ui->textEditDescription->toPlainText());
    dynamic_cast<NotesModule::Multimedia*>(video)->setPath(video_path);
}

void VideoEditor::getVideoPath(){
    QString path = QFileDialog::getOpenFileName();
    video_path = path;
    if(path!=""){
        ui->videoPlayer->play(Phonon::MediaSource(path));
        ui->pushButtonPlay->setEnabled(false);
        ui->pushButtonPause->setEnabled(true);
        ui->pushButtonStop->setEnabled(true);
        ui->volumeSlider->setEnabled(true);
    }
}

void VideoEditor::playVideo(){
    //QString path = workspace::Workspace::back_slash_to_slash(video->getPath());
    //QString p("C:\Users\azerty\Desktop\LO21\UI_Notes\resources\audio.mp3");
    ui->videoPlayer->play();
    ui->pushButtonPlay->setEnabled(false);
    ui->pushButtonStop->setEnabled(true);
    ui->pushButtonPause->setEnabled(true);
}

void VideoEditor::stopVideo(){
    ui->videoPlayer->stop();
    ui->pushButtonPause->setEnabled(false);
    ui->pushButtonPlay->setEnabled(true);
}

void VideoEditor::pauseVideo(){
    ui->videoPlayer->pause();
    ui->pushButtonStop->setEnabled(false);
    ui->pushButtonPlay->setEnabled(true);
}

void VideoEditor::delete_note(){
    DeleterForEditors d;
    d(video);
    video = 0;
    ui->pushButtonDelete->setEnabled(false);
    ui->pushButtonSave->setEnabled(true);
}
