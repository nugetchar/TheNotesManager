#ifndef IMANGEEDITOR_H
#define IMANGEEDITOR_H

#include <QWidget>
#include "Workspace.h"

namespace Ui {
class ImangeEditor;
}

/**
 * @brief The ImangeEditor class
 */
class ImangeEditor : public QWidget
{
    Q_OBJECT
    
public:
    ImangeEditor(NotesModule::Image* img = 0, QWidget *parent = 0);
    ~ImangeEditor();

public slots:
    void saveImage();
    void updateImage();
    void getImagePath();
    void delete_note();
    NotesModule::Image& getImage(){return *image;}

private slots:
    void activateSave(QString str="");
    
private:
    Ui::ImangeEditor *ui;
    NotesModule::Image* image;
    QString image_path;
};

#endif // IMANGEEDITOR_H
