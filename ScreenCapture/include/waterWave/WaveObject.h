#if !defined(WAVE_OBJECT_INCLUDED_)
#define WAVE_OBJECT_INCLUDED_
typedef struct _WAVE_OBJECT
{
	HWND hWnd;
	DWORD dwFlag;
	DWORD hDcRender;
	DWORD hBmpRender;
	DWORD lpDIBitsSource;
	DWORD   lpDIBitsRender;// 用于显示到屏幕的像素数据
	DWORD lpWave1;       // 水波能量数据缓冲1
	DWORD lpWave2;       // 水波能量数据缓冲2
	//********************************************************************
	DWORD dwBmpWidth;
	DWORD dwBmpHeight;
	DWORD dwDIByteWidth;// = (dwBmpWidth * 3 + 3) and ~3
	DWORD dwWaveByteWidth;// = dwBmpWidth * 4
	DWORD dwRandom;
	//********************************************************************
	// 特效参数
	//********************************************************************
	DWORD dwEffectType;
	DWORD dwEffectParam1;
	DWORD dwEffectParam2;
	DWORD dwEffectParam3;
	//********************************************************************
	// 用于行船特效
	//********************************************************************
	DWORD dwEff2X;
	DWORD dwEff2Y;
	DWORD dwEff2XAdd;
	DWORD dwEff2YAdd;
	DWORD dwEff2Flip;
	//********************************************************************
	BITMAPINFO stBmpInfo;// BITMAPINFO <>
} WAVE_OBJECT,*LPWAVE_OBJECT;


//===================================================================================================================

extern "C" void __stdcall _WaveInit(LPWAVE_OBJECT lpWaveObject,HWND hWnd,HBITMAP hBmp,DWORD dwSpeed,DWORD dwType);	//初始化对象; 参数：_lpWaveObject ＝ 指向 WAVE_OBJECT的指针; 返回值 eax = 0 成功、= 1 失败
extern "C" void __stdcall _WaveDropStone(LPWAVE_OBJECT lpWaveObject,DWORD dwPosX,DWORD dwPosY,DWORD dwStoneSize,DWORD dwStoneWeight);	//; 扔一块石头
extern "C" void __stdcall _WaveUpdateFrame(LPWAVE_OBJECT lpWaveObject,HDC hDc,DWORD bIfForce);
extern "C" void __stdcall _WaveFree(LPWAVE_OBJECT lpWaveObject);
extern "C" void __stdcall _WaveEffect(LPWAVE_OBJECT lpWaveObject,DWORD dwType,DWORD dwParam1,DWORD dwParam2,DWORD dwParam3);	// 更改特效

/***********************************************用法举例*******************************************************************
;********************************************************************
; 鼠标左键激起水波
;********************************************************************
		.elseif	eax ==	WM_LBUTTONDOWN
			mov	eax,lParam
			movzx	ecx,ax		; x
			shr	eax,16		; y

			invoke	_WaveDropStone,addr stWaveObj,ecx,eax,2,256
;********************************************************************
; 鼠标右键切换特效
;********************************************************************
		.elseif	eax ==	WM_RBUTTONDOWN
			mov	eax,stWaveObj.dwEffectType
			.if	eax ==	0
				invoke	_WaveEffect,addr stWaveObj,1,5,4,250
			.elseif	eax ==	1
				invoke	_WaveEffect,addr stWaveObj,3,250,4,8
			.elseif	eax ==	3
				invoke	_WaveEffect,addr stWaveObj,2,4,2,180
			.else
				invoke	_WaveEffect,addr stWaveObj,0,0,0,0
			.endif
;********************************************************************
*******************************************************************************************************************************/
#endif