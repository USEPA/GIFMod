#pragma once
#include "qlineedit.h"
#include "qevent.h"
#include "qcompleter.h"
#include "qtextedit.h"

class commandLineEdit : public QTextEdit
{
	Q_OBJECT

public:
	explicit commandLineEdit(QWidget *parent = Q_NULLPTR);
	void setCompleter(QCompleter *c); //updates the completer
	QCompleter *completer() const;
	QString text();
protected:
	void keyPressEvent(QKeyEvent *e) override;
	void focusInEvent(QFocusEvent *e) override;

	private slots:
	void insertCompletion(const QString &completion);

private:
	QString textUnderCursor() const;

private:
	QCompleter *c;
	


private slots:
	void keyReleaseEvent(QKeyEvent *e)
	{
		if (e->key() == Qt::Key_Up || e->key() == Qt::Key_Down)
			emit navigate(e);
		if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
			emit returnPressed(e);
		if (e->text() == ".")
			emit DotPressed(e);
		if (e->key() == Qt::Key_Space)
			emit SpacePressed(e);
		if (e->text() == ",")
			emit CommaPressed(e);
	}
	
signals:
	void navigate(QKeyEvent*);
	void returnPressed(QKeyEvent*);
	void DotPressed(QKeyEvent*);
	void SpacePressed(QKeyEvent *);
	void CommaPressed(QKeyEvent *);

};