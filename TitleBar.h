#pragma once
#ifndef TITLE_BAR
#define TITLE_BAR 
#include <QWidget>

class QLabel;
class QPushButton;

class TitleBar : public QWidget
{
	Q_OBJECT

public:
	 explicit TitleBar(QWidget *parent = 0);
	~TitleBar();

protected:
	virtual void mouseDoubleClickEvent(QMouseEvent *event);

	//进行鼠标界面的拖动
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);

	void paintEvent(QPaintEvent *event);

	//设置界面标题与图表
	virtual bool eventFilter(QObject* obj,QEvent* event);

private slots:
	void onClicked();

private:
	void updateMaximize();

private:
	QPoint        dragPosition;
	//QLabel *m_pIconLabel;
	QPushButton*  m_pIconLabel;
	QLabel *      m_pTitleLabel;
	QPushButton * m_pMinimizeButton;
	QPushButton * m_pMaximizeButton;
	QPushButton * m_pCloseButton;
	bool          m_mousePress;

};


#endif 
