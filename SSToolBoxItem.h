#pragma once
#ifndef SSTOOLBOXITEM_H
#define SSTOOLBOXITEM_H

#include <QObject>
#include <QLabel>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>

class SSToolBox;
class SSToolBoxItem : public QWidget
{
	Q_OBJECT
    enum
	{
		BAR_HEIGHT = 28
	};
public:
	explicit  SSToolBoxItem(QWidget *parent= nullptr,QString name = tr(""));

	void setContentExpend(bool expand);
	void setFlag(int flag);
	void paintEvent(QPaintEvent *event);
	int  getFlag() const ;
	inline    void setBarHeight(int height)
	{
		m_pBar->setFixedHeight(height);
	}
	inline  void setContentLayout(QLayout* playout =nullptr)
	{
		m_pContent->setLayout(playout);
	}

public :
	void  stateSwitch();

private:
	void  createCtrl();
	void  initLayout();
	void  setIcon();
	//void  stateSwitch();

protected:
	void mousePressEvent(QMouseEvent *event);

private:
	int        m_iCurrentFlag;
	QString    m_sName;
	QLabel    *m_pName;
	QLabel    *m_pIcon;
	QWidget   *m_pBar;
	QWidget   *m_pContent;
	SSToolBox *m_pToolBox;
	bool       m_bExpand;
};

#endif 
