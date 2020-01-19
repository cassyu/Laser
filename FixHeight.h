#pragma once
#ifndef FIXHEIGHT
#define FIXHEIGHT

class FixHeight
{
public:
	virtual void  setCusfixSizeHeight(int h) = 0;
	virtual int   GetFixedHeight() = 0;
	virtual int   fixSizeHeight() = 0;
protected:
	int   m_Height;
};

#endif 

