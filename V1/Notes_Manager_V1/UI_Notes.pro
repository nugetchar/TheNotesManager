QT      +=core gui
QT      += phonon

HEADERS += \
    mainwindow.h \
    articleeditor.h \
    audioeditor.h \
    videoeditor.h \
    NotesModule.h \
    Managers.h \
    Workspace.h \
    ExceptionsModule.h \
    ExportModule.h \
    imprimeries.h \
    TYPES_EXPORT.h \
    TrashModule.h \
    imangeeditor.h \
    delete_note_foncteur.h

SOURCES += \
    mainwindow.cpp \
    main.cpp \
    articleeditor.cpp \
    audioeditor.cpp \
    videoeditor.cpp \
    NotesModule.cpp \
    Managers.cpp \
    test_notes_module.cpp \
    Workspace.cpp \
    ExportModule.cpp \
    imprimeries.cpp \
    TYPES_EXPORT.cpp \
    imangeeditor.cpp

FORMS += \
    mainwindow.ui \
    articleeditor.ui \
    audioeditor.ui \
    videoeditor.ui \
    imangeeditor.ui

RESOURCES += \
    resources.qrc
