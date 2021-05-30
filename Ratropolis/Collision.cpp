#include "stdafx.h"
#include "Collision.h"


CCollision::CCollision()
{
}


CCollision::~CCollision()
{
}

bool CCollision::PosRectCkeck(POINT& Point, RECT& Rect)
{
	if (PtInRect(&Rect, Point))
		return true;
	return false;
}

bool CCollision::ClientRectCkeck(RECT & rClient, RECT & rImage)
{
	RECT rc = {};
	if (!IntersectRect(&rc, &rClient, &rImage))
		return true;
	return false;
}
