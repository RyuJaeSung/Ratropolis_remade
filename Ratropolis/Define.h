#pragma once

#define WINCX 1280
#define WINCY 720

#define TILECX 120
#define TILECY 120
#define TILEX 300
#define TILEY 1
#define SCROLLY 450

#define TEXTIME 3
#define RESETTIME 1
#define MSGDELETE 1.5

#define CRADCX 100
#define CRADCY 110

#define PRICE 20
#define SAFE_DELETE(p) if(p) { delete p; p = nullptr; }
#define ERR_MSG(message) MessageBox(nullptr, message, L"SystemError", MB_OK)


/////////////////////////////////////////////key ¸ñ·Ï/////////////////////////////////////////////
#define KEY_LBUTTON 0x00000001
#define KEY_SPACE	0x00000010
#define KEY_RIGHT	0x00000100
#define KEY_LEFT	0x00001000
#define KEY_ONE		0x00010000
#define KEY_TWO		0x00100000