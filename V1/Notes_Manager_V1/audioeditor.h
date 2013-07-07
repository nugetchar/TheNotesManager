#ifndef AUDIOEDITOR_H
#define AUDIOEDITOR_H

#include <QWidget>
#include "NotesModule.h"
#include <phonon/MediaObject>
#include <phonon/MediaSource>
#include <phonon/VideoPlayer>

namespace Ui {
class AudioEditor;
}

/**
 * @brief The AudioEditor class
 */
class AudioEditor : public QWidget
{
    Q_OBJECT
    
public:
     AudioEditor(NotesModule::Audio* ado = 0,QWidget *parent = 0);
    ~AudioEditor();
    
public slots:
     void saveAudio();
     void updateAudio();
     void getAudioPath();
     void playSound();
     void stopSound();
     void pauseSound();
     void delete_note();
     NotesModule::Audio& getAudio(){return *audio;}

private slots:
    void activateSave(QString str="");

private:
    Ui::AudioEditor* ui;
    NotesModule::Audio* audio;
    Phonon::VideoPlayer* sound;
    QString audio_path_;
};

#endif // AUDIOEDITOR_H
