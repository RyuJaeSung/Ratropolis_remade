#pragma once
class CCollision
{
public:
	CCollision();
	~CCollision();

public:

	static bool PosRectCkeck(POINT& Point, RECT& Rect);
	static bool ClientRectCkeck(RECT& rClient, RECT& rImage);
};

