#ifndef VIDEOEDITOR_H
#define VIDEOEDITOR_H
#include "NotesModule.h"
#include <QWidget>

namespace Ui {
class VideoEditor;
}

class VideoEditor : public QWidget
{
    Q_OBJECT
    
public:
    VideoEditor(NotesModule::Video* vid = 0, QWidget *parent = 0);
    ~VideoEditor();

public slots:
    void saveVideo();
    void updateVideo();
    void getVideoPath();
    void playVideo();
    void stopVideo();
    void pauseVideo();
    void delete_note();
    NotesModule::Video& getVideo(){return *video;}

private slots:
    void activateSave(QString str="");
    
private:
    Ui::VideoEditor *ui;
    NotesModule::Video* video;
    QString video_path;
};

#endif // VIDEOEDITOR_H
