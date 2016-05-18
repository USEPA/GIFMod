#ifndef ExpressionEditor_H
#define ExpressionEditor_H

#include <QLineEdit>

struct ExpressionEditorPri;
class StatusViewer;
class ReactionTableModel;

class ExpressionEditor : public QLineEdit
{
	Q_OBJECT
public:
    explicit ExpressionEditor(ReactionTableModel* model, StatusViewer* statusbar, QWidget* p = 0);
    ~ExpressionEditor();

protected:
    void focusInEvent(QFocusEvent * e);
    void focusOutEvent(QFocusEvent * e);
    void keyPressEvent(QKeyEvent*);

private slots:
    void onCompletorActivated(const QString& item);

private:
    void setupCompleter();
    void replaceWordUnderCursorWith(const QString& c);
    QString cursorWord(const QString &sentence) const;

private:
    ExpressionEditorPri* d;
};

#endif // ExpressionEditor_H
