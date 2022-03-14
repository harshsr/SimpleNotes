#include "notepad.h"
#include "ui_notepad.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QFontDialog>


// don't be confused. We simply define the NotePad constructor which demands a parent QWidget pointer to which we attach our NotePad UI
// in this case, NotePad will be the parent widget but that will be defined when we create an object from NotePad (like it is done in main.cpp)
// then we define that NotePad is a child of QMainWindow. This also calls QMainWindow's constructor
// Then we initialize PadUI pointer (it is done in a different way here but I guess we can do this in the constructor body too. I think this is a new cpp feature)
// Then comes the constructor body

NotePad::NotePad(QWidget *parent)
    : QMainWindow(parent)
    , PadUI(new Ui::NotePad)
{
    // sets up ui
    PadUI->setupUi(this);

    // make TextField occupy the whole screen
    //this->setCentralWidget(PadUI->TextField);

    // connect actions defined in notepad.ui (PadUI is the object that points to a notepad.ui) to slots(methods) in NotePad
    // when a UI action is made, QAction::triggered invokes the connected slot

    connect(PadUI->actionNew, &QAction::triggered, this, &NotePad::NewDocument );
    connect(PadUI->actionOpen, &QAction::triggered, this, &NotePad::Open);
    connect(PadUI->actionSave, &QAction::triggered, this, &NotePad::Save);
    connect(PadUI->actionSaveAs, &QAction::triggered, this, &NotePad::SaveAs);
    connect(PadUI->actionPrint, &QAction::triggered, this, &NotePad::Print);
    connect(PadUI->actionExit, &QAction::triggered, this, &NotePad::Exit);
    connect(PadUI->CopyButton,SIGNAL(clicked()),this,SLOT(Copy()));
    connect(PadUI->CutButton, SIGNAL(clicked()), this, SLOT(Cut()));
    connect(PadUI->PasteButton, SIGNAL(clicked()), this, SLOT(Paste()));
    connect(PadUI->UndoButton, SIGNAL(clicked()), this, SLOT(Undo()));
    connect(PadUI->RedoButton, SIGNAL(clicked()), this,SLOT(Redo()));
//    connect(PadUI->actionFont, &QAction::triggered, this, &NotePad::SelectFont);
//    connect(PadUI->actionBold, &QAction::triggered, this, &NotePad::SetFontBold);
//   connect(PadUI->actionUnderline, &QAction::triggered, this, &NotePad::SetFontUnderline);
//    connect(PadUI->actionItalic, &QAction::triggered, this, &NotePad::SetFontItalic);
//    connect(PadUI->actionAbout, &QAction::triggered, this, &NotePad::About);

    // button tooltips
    PadUI->CopyButton->setToolTip("Copy");
    PadUI->CutButton->setToolTip("Cut");
    PadUI->PasteButton->setToolTip("Paste");
    PadUI->UndoButton->setToolTip("Undo");
    PadUI->RedoButton->setToolTip("Redo");


    // disable print action if not supported
#if !defined(QT_PRINTSUPPORT_LIB) || !QT_CONFIG(printer)
    PadUI->actionPrint->setEnabled(false);
#endif

    // disable some more options
#if !QT_CONFIG(clipboard)
    PadUI->actionCut->setEnabled(false);
    PadUI->actionCopy->setEnabled(false);
    PadUI->actionPaste->setEnabled(false);
#endif

}

// destructor to take care of the ui member pointer
NotePad::~NotePad()
{
    delete PadUI;
}

void NotePad::NewDocument()
{
    CurrentFile.clear();
    PadUI->TextField->setText(QString());
}

void NotePad::Open()
{
    // getOpenFileName opens the open file window. We store the selected file's compete path in FileName
    QString FileName = QFileDialog::getOpenFileName(this,"Open the file","D:/","Text File (*.txt)");

    // define a QFile object with Filename
    QFile MyFile(FileName);
    //QMessageBox::information(this,FileName,FileName);

    // store Filename in CurrenFile for later use
    CurrentFile = FileName;

    // open QFile File defined above with File.open. It is readonly text file.
    if (!MyFile.open(QIODevice::ReadOnly | QFile::Text))
    {
        // if cannot be opened, show a warning
        QMessageBox::warning(this,"Warning","Cannot open file: "+ MyFile.errorString());
        return;
    }

    //
    setWindowTitle(FileName);
    // read everything from File. Store it in MyText. This works kind of similar to standard c++ file read and write. Learn that first.
    QTextStream Input(&MyFile);
    QString MyText = Input.readAll();

    // copy MyText to our UI TextField for display.
    PadUI->TextField->setText(MyText);
    MyFile.close();
}

void NotePad::Save()
{
    QString FileName;
    // if CurrentFile is empty, set it
    if(CurrentFile.isEmpty())
    {
        //similar to what we did in Open(). This opens a window to select save location and file name and stores the complete path in FileName
        FileName = QFileDialog::getSaveFileName(this,"Save","D:/","text (*.txt)");
        CurrentFile = FileName;
    }
    else
    {
        FileName = CurrentFile;
    }

    QFile MyFile(FileName);
    if(!MyFile.open(QIODevice::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this,"Warning","Cannot save file:" + MyFile.errorString());
        return;
    }

    setWindowTitle(FileName);

    // This works kind of similar to standard c++ file read and write. Learn that first.
    QTextStream Out(&MyFile);
    QString MyText = PadUI->TextField->toPlainText();
    Out<<MyText;
    MyFile.close();
}

void NotePad::SaveAs()
{
    // this is not save as another file type. It's just save a text file to another path witha nother name without checking if its the CurrentFile
    QString FileName = QFileDialog::getSaveFileName(this,"Save as","D:/","text (*.txt)");
    QFile MyFile(FileName);

    if(!MyFile.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this,"warning","Cannot save file :" + MyFile.errorString());
        return;
    }

    CurrentFile = FileName;
    setWindowTitle(FileName);

    // This works kind of similar to standard c++ file read and write. Learn that first.
    QTextStream Out(&MyFile);
    QString MyText = PadUI->TextField->toPlainText();
    Out<<MyText;
    MyFile.close();
}

void NotePad::Print()
{
    /*
    // Nedd to add( QT       += printsupport )to the project file for print support
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printer)
    QPrinter printDev;
#if QT_CONFIG(printdialog)
    QPrintDialog dialog(&printDev, this);
    if (dialog.exec() == QDialog::Rejected)
        return;
#endif // QT_CONFIG(printdialog)
    PadUI->TextField->print(&printDev);
#endif // QT_CONFIG(printer)
*/
}

void NotePad::Exit()
{
    QCoreApplication::quit();
}

void NotePad::Copy()
{
#if QT_CONFIG(clipboard)
    PadUI->TextField->copy();
#endif
}

void NotePad::Cut()
{
#if QT_CONFIG(clipboard)
    PadUI->TextField->cut();
#endif
}

void NotePad::Paste()
{
#if QT_CONFIG(clipboard)
    PadUI->TextField->paste();
#endif
}

void NotePad::Undo()
{
    PadUI->TextField->undo();
}

void NotePad::Redo()
{
    PadUI->TextField->redo();
}

void NotePad::SelectFont()
{
    bool FontSelected;
    QFont Font = QFontDialog::getFont(&FontSelected,this);
    if(FontSelected)
        PadUI->TextField->setFont(Font);
}

void NotePad::SetFontBold(bool Bold)
{

}

void NotePad::SetFontUnderline(bool Underline)
{
    PadUI->TextField->setFontUnderline(Underline);
}

void NotePad::SetFontItalic(bool Italic)
{
    PadUI->TextField->setFontItalic(Italic);
}

void NotePad::About()
{

}
