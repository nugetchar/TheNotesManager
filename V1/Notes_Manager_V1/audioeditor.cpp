#include "audioeditor.h"
#include "ui_audioeditor.h"
#include "NotesModule.h"
#include "Managers.h"
#include "Workspace.h"
#include "delete_note_foncteur.h"
#include <QFileDialog>
#include <QMessageBox>

AudioEditor::AudioEditor(NotesModule::Audio* ado, QWidget *parent) :QWidget(parent),ui(new Ui::AudioEditor),audio(ado)
{
    ui->setupUi(this);
    sound= new Phonon::VideoPlayer(Phonon::MusicCategory, this);
    sound->setVisible(false);
    ui->volumeSlider->setAudioOutput(sound->audioOutput());
    ui->volumeSlider->setEnabled(false);
    ui->pushButtonDelete->setEnabled(false);

    if (audio){
        ui->lineEditTitle->setText(dynamic_cast<NotesModule::Note*>(audio)->getTitle());
        ui->textEditDescription->setText(dynamic_cast<NotesModule::Multimedia*>(audio)->getDescription());
    }


    QObject::connect(ui->pushButtonSave,SIGNAL(clicked()),this,SLOT(saveAudio()));
    QObject::connect(ui->lineEditTitle,SIGNAL(textEdited(QString)),this,SLOT(activateSave(QString)));
    QObject::connect(ui->textEditDescription,SIGNAL(textChanged()),this,SLOT(activateSave()));
    QObject::connect(ui->pushButtonBrowse,SIGNAL(clicked()),this,SLOT(getAudioPath()));
    QObject::connect(ui->pushButtonPlay,SIGNAL(clicked()),this,SLOT(playSound()));
    QObject::connect(ui->pushButtonStop,SIGNAL(clicked()),this,SLOT(stopSound()));
    QObject::connect(ui->pushButtonPause,SIGNAL(clicked()),this,SLOT(pauseSound()));
    QObject::connect(ui->pushButtonDelete,SIGNAL(clicked()),this,SLOT(delete_note()));
}

AudioEditor::~AudioEditor()
{
    delete ui;
}

void AudioEditor::activateSave(QString){
    ui->pushButtonSave->setEnabled(true);
}

void AudioEditor::saveAudio(){

    updateAudio();
    workspace::Workspace::getInstance()->save_note("audio",audio->getId());
    QMessageBox::information(this, "Save","Your audio has been saved.");
    ui->pushButtonSave->setEnabled(false);
    ui->pushButtonDelete->setEnabled(true);

}

void AudioEditor::updateAudio(){

    if(!audio)
        audio = dynamic_cast<NotesModule::Audio*>(workspace::Workspace::getInstance()->create_note("audio"));
    dynamic_cast<NotesModule::Note*>(audio)->setTitle(ui->lineEditTitle->text());
    dynamic_cast<NotesModule::Multimedia*>(audio)->setDescription(ui->textEditDescription->toPlainText());
    dynamic_cast<NotesModule::Multimedia*>(audio)->setPath(audio_path_);
}

void AudioEditor::getAudioPath(){
    QString path = QFileDialog::getOpenFileName();
    audio_path_ = path;
    if(path!=""){

        sound->play(Phonon::MediaSource(workspace::Workspace::back_slash_to_slash(path)));
        ui->pushButtonPlay->setEnabled(false);
        ui->pushButtonPause->setEnabled(true);
        ui->pushButtonStop->setEnabled(true);
        ui->volumeSlider->setEnabled(true);
    }
}

void AudioEditor::playSound(){
    sound->play();
    ui->pushButtonPlay->setEnabled(false);
    ui->pushButtonStop->setEnabled(true);
    ui->pushButtonPause->setEnabled(true);
}

void AudioEditor::stopSound(){
    sound->stop();
    ui->pushButtonPause->setEnabled(false);
    ui->pushButtonPlay->setEnabled(true);
}

void AudioEditor::pauseSound(){
    sound->pause();
    ui->pushButtonStop->setEnabled(false);
    ui->pushButtonPlay->setEnabled(true);
}

void AudioEditor::delete_note(){
    DeleterForEditors d;
    d(audio);
    audio = 0;
    ui->pushButtonDelete->setEnabled(false);
    ui->pushButtonSave->setEnabled(true);
}



