#ifndef ARTICLEEDITOR_H
#define ARTICLEEDITOR_H

#include <QWidget>
#include <NotesModule.h>

namespace Ui {
class ArticleEditor;
}

/**
 * @brief The ArticleEditor class
 */
class ArticleEditor : public QWidget
{
    Q_OBJECT
    
public:
    explicit ArticleEditor(NotesModule::Article* art = 0, QWidget *parent = 0);
    ~ArticleEditor();



    
private:
    Ui::ArticleEditor *ui;
    NotesModule::Article* article;

public slots:
    void saveArticle();
    void updateArticle();
    void delete_note();
    NotesModule::Article& getArticle(){return *article;}

private slots:
    void activateSave(QString str="");


};

#endif // ARTICLEEDITOR_H
