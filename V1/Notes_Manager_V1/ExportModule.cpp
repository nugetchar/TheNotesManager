#include "ExportModule.h"
#include <QDebug>
int export_notes::ExportWrapper::nbInstances = 0;
QMap<TYPE_EXPORT, export_notes::AbstractGetterExport*>* export_notes::ExportWrapper::exporters = 0;

const char* export_notes::FatherOfExporters::types_export_str[3]={".html",".tex",".txt"};

//EXPORTER HTML
void export_notes::ExporterHTML::start_header(const NotesModule::Note& n, QTextStream& out) const{
    out << "<html>" << '\n'
        << "<head>" << '\n'
        << "<title>" << n.getTitle() << "</title>" << '\n';
}
void export_notes::ExporterHTML::end_header(QTextStream& out) const{
    out << "</head>" << '\n';
}

void export_notes::ExporterHTML::header(const NotesModule::Note& note, QTextStream& out,
                                        unsigned int titleLevel) const{
    if(titleLevel<1)
        titleLevel = 1;
    else
        if(titleLevel>4)
            titleLevel = 4;

    out << "<h" << titleLevel << ">" << note.getTitle() << "</h" << titleLevel <<">"
        << '\n';
}
void export_notes::ExporterHTML::footer(QTextStream& out) const{
    out << "<footer>ENTREZ ICI VOTRE FOOTER</footer></html>" << '\n';
}


void export_notes::ExporterHTML::start_body(QTextStream& out) const{
    out << "<body>" << '\n';
}
void export_notes::ExporterHTML::end_body(QTextStream& out) const{
    out << "</body>" << '\n';
}

void export_notes::ExporterHTML::body(const NotesModule::Article &n, QTextStream& out,
                                      unsigned int titleLevel) const{
    out << "<p id='" << n.getId() << "'>" << '\n'
        << n.getText() << '\n'
        << "</p>" << '\n';

}
void export_notes::ExporterHTML::body(const NotesModule::Document &n, QTextStream& out,
                                      unsigned int titleLevel) const{
    out << "<p id='" << n.getId() << "'>" << '\n';
    NotesModule::Document::ConstIterator it = n.getConstIterator();
    while(!it.isDone()){
        out << "<br/>" << '\n';
        if(++titleLevel > 4)
            titleLevel--;
        this->header(it.currentItem(),out,titleLevel);
        it.currentItem().exportAsPart(out,HTML,titleLevel+1);
        it.next();
    }
    out << '\n'
        << "</p>" << '\n';
}

void export_notes::ExporterHTML::body(const NotesModule::Audio &n, QTextStream &out,
                                      unsigned int titleLevel) const{
    out << "<audio src='" << n.getPath() << "' id='" << n.getId()
        << "' controls>" << "<!-- Supprimez cette balise si vous ne voulez pas que le son puisse être téléchargé-->"
        << "<a href='" << n.getPath() << "'>Download song</a><br /><p>"
        << n.getDescription() << "</p></audio>" << '\n';
}

void export_notes::ExporterHTML::body(const NotesModule::Video &n, QTextStream &out,
                                      unsigned int titleLevel) const{
    out << "<video id='" << n.getId()
        << "' controls>";
    if(n.getExtension() == ".webm")
        out << "<source src='" << n.getPath() << "' type='video/webm; codecs=\"vp8.0, vorbis\"'/>";
    else{
        if(n.getExtension() == ".ogg")
            out << "<source src='" << n.getPath() << "' type='video/ogg; codecs=\"theora, vorbis\"'/>";
        else
            out << "<source src='" << n.getPath() << "' type='video/mp4; codecs=\"avc1.4D401E, mp4a.40.2\"'/>";
    }
    out << "<!-- Supprimez cette balise si vous ne voulez pas que la video puisse être téléchargée-->"
        << "<a href='" << n.getPath() << "'>Download video</a><br /><p>"
        << n.getDescription() << "</p></video>" << '\n';
}


void export_notes::ExporterHTML::body(const NotesModule::Image &n, QTextStream &out,
                                      unsigned int titleLevel) const{
    out << "<img id='" << n.getId() << "' src='" << n.getPath()
        << "' alt='" << n.getDescription() << "'/>" << '\n';
}

//EXPORTER TeX
void export_notes::ExporterTex::header(const NotesModule::Note& note, QTextStream& out,
                                       unsigned int titleLevel) const{
    if(titleLevel <= 1){
        out << "\\chapter{" << note.getTitle() << "}"<< '\n';
        return;
    }
    if(titleLevel == 2){
        out << "\\section{" << note.getTitle() << "}" << '\n';
        return;
    }
    if(titleLevel == 3){
        out << "\\subsection{" << note.getTitle() << "}" << '\n';
        return;
    }
    if(titleLevel == 4){
        out << "\\subsubsection{" << note.getTitle() << "}" << '\n';
        return;
    }
    if(titleLevel > 4)
        out << "\\paragraph{" << note.getTitle() << "}" << '\n';
}
void export_notes::ExporterTex::footer(QTextStream& out) const{
    out << "\\end{document}";
}

void export_notes::ExporterTex::body(const NotesModule::Article &n, QTextStream& out,
                                     unsigned int titleLevel) const{

    out << n.getText() << '\n';
    qDebug() << "Body Latex Article" << '\n';

}
void export_notes::ExporterTex::body(const NotesModule::Document &n, QTextStream& out,
                                     unsigned int titleLevel) const{
    NotesModule::Document::ConstIterator it = n.getConstIterator();
    while(!it.isDone()){
        if(++titleLevel > 5)
            titleLevel--;
        this->header(it.currentItem(),out,titleLevel);
        it.currentItem().exportAsPart(out,LATEX,titleLevel+1);
        it.next();
    }
}

void export_notes::ExporterTex::body(const NotesModule::Audio &n, QTextStream &out,
                                     unsigned int titleLevel) const{
    out << "Type: " << n.getType() << '\n'
        << "Ressource: "<< n.getPath() << '\n'
        << "Description: " << n.getDescription() << '\n';
}
void export_notes::ExporterTex::body(const NotesModule::Video &n, QTextStream &out,
                                     unsigned int titleLevel) const{
    out << "Type: " << n.getType() << '\n'
        << "Ressource: "<< n.getPath() << '\n'
        << "Description: " << n.getDescription() << '\n';
}
void export_notes::ExporterTex::body(const NotesModule::Image &n, QTextStream &out,
                                     unsigned int titleLevel) const{
    out << "\\begin{center}" << '\n'
        << "\\includegrahics{" << n.getPath() << "}" << '\n'
        << "\\end{center}" << '\n';
}

//EXPORTER Text
void export_notes::ExporterText::header(const NotesModule::Note& note, QTextStream& out,
                                        unsigned int titleLevel) const{
    out << note.getTitle() << '\n';
}
void export_notes::ExporterText::footer(QTextStream& out) const{
}

void export_notes::ExporterText::body(const NotesModule::Article &n, QTextStream& out,
                                      unsigned int titleLevel) const{
    out << n.getText() << '\n';

}
void export_notes::ExporterText::body(const NotesModule::Document &n, QTextStream& out,
                                      unsigned int titleLevel) const{
    NotesModule::Document::ConstIterator it = n.getConstIterator();
    while(!it.isDone()){
        this->header(it.currentItem(),out,titleLevel);
        it.currentItem().exportAsPart(out,TEXT,titleLevel+1);
        it.next();
    }
}

void export_notes::ExporterText::body(const NotesModule::Audio &n, QTextStream &out,
                                      unsigned int titleLevel) const{
    out << "Type: " << n.getType() << '\n'
        << "Ressource: "<< n.getPath() << '\n'
        << "Description: " << n.getDescription() << '\n';
}
void export_notes::ExporterText::body(const NotesModule::Video &n, QTextStream &out,
                                      unsigned int titleLevel) const{
    out << "Type: " << n.getType() << '\n'
        << "Ressource: "<< n.getPath() << '\n'
        << "Description: " << n.getDescription() << '\n';
}

void export_notes::ExporterText::body(const NotesModule::Image &n, QTextStream &out,
                                      unsigned int titleLevel) const{
    out << "Type: " << n.getType() << '\n'
        << "Ressource: "<< n.getPath() << '\n'
        << "Description: " << n.getDescription() << '\n';
}
