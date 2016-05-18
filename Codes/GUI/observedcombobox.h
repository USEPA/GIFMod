#ifndef OBSERVEDCOMBOBOX_H
#define OBSERVEDCOMBOBOX_H

#include <QComboBox>

class ObservedComboBox : public QComboBox
{
	Q_OBJECT

public:
	ObservedComboBox(QWidget *parent);
	~ObservedComboBox();

private:
private slots:
	void textChanged(const QString & text);

signals :
};

#endif // OBSERVEDCOMBOBOX_H
