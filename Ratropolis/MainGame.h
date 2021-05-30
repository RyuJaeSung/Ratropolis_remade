#pragma once

class CMainGame final
{
public:
	CMainGame();
	~CMainGame();

public:
	void Initialize();
	bool Update();
	void Late_Update();
	void Render();
	void Release();
};

