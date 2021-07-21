#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class NotePad; }
QT_END_NAMESPACE

class NotePad : public QMainWindow
{
    Q_OBJECT

public:
    NotePad(QWidget *parent = nullptr);
    ~NotePad();

private:
    Ui::NotePad *PadUI;
    QString CurrentFile;

private slots:
    void NewDocument();
    void Open();
    void Save();
    void SaveAs();
    void Print();
    void Exit();
    void Copy();
    void Cut();
    void Paste();
    void Undo();
    void Redo();
    void SelectFont();
    void SetFontBold(bool Bold);
    void SetFontUnderline(bool Underline);
    void SetFontItalic(bool Italic);
    void About();
};
#endif // NOTEPAD_H
