#include <QApplication>
#include <mainwindow.h>
#include <articleeditor.h>
#include <audioeditor.h>
#include <videoeditor.h>
#include <imangeeditor.h>
#include "Managers.h"
#include "Workspace.h"
#include <QDir>
#include <QFileDialog>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);


        QFile f("current_workspace.txt");
        if (!f.open(QIODevice::ReadWrite | QIODevice::Text))
            return -1;
        QTextStream in(&f);
        QString wspace = in.readLine();

        f.close();
        QDir dir(wspace);
        if(!dir.exists()) wspace = "";

        QString new_wspace;

        //new_wspace = QFileDialog::getExistingDirectory(0, "Choose a workspace", wspace,QFileDialog::);
        if(new_wspace == "") return -1;

        workspace::Workspace::releaseTheWorkspace(new_wspace);

        if(new_wspace != wspace){
            f.remove();
            if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
                return -1;
            QTextStream out(&f);
            out << new_wspace << '\n';
            f.close();
        }
    MainWindow window;
    window.show();

    return app.exec();

}
