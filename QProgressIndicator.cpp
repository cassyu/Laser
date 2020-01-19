﻿#include "QProgressIndicator.h"

#include <QPainter>
#include <QMainWindow>
#include "mainwindow.h"

QProgressIndicator::QProgressIndicator(QWidget *parent)
	: QWidget(parent),
	m_angle(0),
	m_timerId(-1),
	m_delay(40),
	m_displayedWhenStopped(false),
	m_color(Qt::red)
{
	//setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	setFocusPolicy(Qt::NoFocus);
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	//setWindowFlags(Qt::FramelessWindowHint);
	
	setFixedSize(300, 300);
	setParent(MainWindow::GetMainWindow());
}

QProgressIndicator::~QProgressIndicator()
{
}

bool QProgressIndicator::isAnimated() const
{
	return m_timerId != -1;
}

bool QProgressIndicator::isDisplayedWhenStopped() const
{
	return m_displayedWhenStopped;
}

QSize QProgressIndicator::sizeHint() const
{
	return QSize(100, 100);
}

int QProgressIndicator::heightForWidth(int w) const
{
	return w;
}

void QProgressIndicator::startAnimation()
{
	m_angle = 0;
	if (m_timerId == -1)
		m_timerId = startTimer(m_delay);
}

void QProgressIndicator::stopAnimation()
{
	if (m_timerId != -1)
		killTimer(m_timerId);

	m_timerId = -1;
	update();
}

void QProgressIndicator::setAnimationDelay(int delay)
{
	if (m_timerId != -1)
		killTimer(m_timerId);

	m_delay = delay;

	if (m_timerId != -1)
		m_timerId = startTimer(m_delay);
}

void QProgressIndicator::setDisplayedWhenStopped(bool state)
{
	m_displayedWhenStopped = state;

	update();
}

void QProgressIndicator::setColor(const QColor & color)
{
	m_color = color;
	update();
}

void QProgressIndicator::timerEvent(QTimerEvent * event)
{
	m_angle = (m_angle + 30) % 360;

	update();
}

void QProgressIndicator::paintEvent(QTimerEvent * event)
{
	if (!m_displayedWhenStopped && !isAnimated())
		return;

	int width = qMin(this->width(), this->height());

	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing);

	int outerRadius = (width - 1)*0.5;
	int innerRadius = (width - 1)*0.5*0.38;

	int capsuleHeight = outerRadius - innerRadius;
	int capsuleWidth = (width > 32) ? capsuleHeight * 0.23 : capsuleHeight * 0.35;
	int capsuleRadius = capsuleWidth / 2;

	for (int i = 0; i < 12; i++)
	{
		QColor color = m_color;
		color.setAlphaF(1.0f - (i / 12.0f));
		p.setPen(Qt::red);
		p.setBrush(color);
		p.save();
		p.translate(rect().center());
		p.rotate(m_angle - i * 30.0f);
		p.drawRoundedRect(-capsuleWidth * 0.5, -(innerRadius + capsuleHeight), capsuleWidth, capsuleHeight, capsuleRadius, capsuleRadius);
		p.restore();
	}
}
