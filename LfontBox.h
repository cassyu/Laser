#pragma once

#include <QFontComboBox>

class LfontBox : public QFontComboBox
{
	Q_OBJECT

public:
	LfontBox(QWidget *parent);
	~LfontBox();
	void dumpStructture(QObject*obj, int spacecount);

	void paintEvent(QPaintEvent *e);
	void wheelEvent(QWheelEvent *e);
};
