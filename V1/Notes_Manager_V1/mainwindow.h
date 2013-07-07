#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "NotesModule.h"
#include "articleeditor.h"
#include "audioeditor.h"
#include "videoeditor.h"
#include "imangeeditor.h"
#include <QLabel>
namespace Ui {
class MainWindow;
}


/**
 * @brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *    ui;
    ArticleEditor*      editorArticle;
    AudioEditor*        editorAudio;
    VideoEditor*        editorVideo;
    ImangeEditor*       editorImage;

    QLabel*             visualisatorText;
    //VisualisatorHtml
    //VisualisatorLatex












    //Methods
    void deleteTabs();
    //void refreshTreeNotes();
   // void addParentToTree(QString name);
    //void addChildToTree(QTreeWidgetItem* parent, QString name);


public slots:
    void changementOngletArticle(int i);
    void changementOngletAudio(int i);
    void changementOngletVideo(int i);
    void changementOngletImage(int i);

    void openNewArticle();
    void openNewAudio();
    void openNewVideo();
    void openNewImage();

    void openWorkspace();

};

#endif // MAINWINDOW_H
