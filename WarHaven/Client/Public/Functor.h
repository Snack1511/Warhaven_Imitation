#pragma once
#include "Client_Defines.h"

class CFunctor
{
public:
	static _float4	Get_MousePos(void);

	static _float4	OrthoToRealPosition(const _float4& vPosition);
	static _float4	RealToOrthoPosition(const _float4& vPosition);

	static _float	Get_DegreeFromDot(const _float4& V1, const _float4& V2);

	static string	To_String(wstring wstrText);
	static string	To_String_UTF8(wstring wstrText);
	static wstring	To_Wstring(string wstrText);

	// value : 뒤에서부터 ? 앞에서부터
	static string	Remove_String(string str, _bool value = true);

	static _float	Lerp_Float(const _float& fSour, const _float& fDest, const _float& fRatio);

	static _float4	To_Descartes(_float4 vPos);
	static _float4	To_Window(_float4 vPos);

	static _float	Lerp(_float fDest, _float fSour, _float fRatio);

	/* 1인자 : 사운드 이름, 2인자 : 채널 enum, 3인자 : 사운드 발생한 월드 포지션, 4인자 : 볼륨 크기 (default : 1)*/
	static void		Play_Sound(wstring wstrFileName, _uint iGroupIndex, _float4 vPosition, _float fVolume = 1.f);
	/* 1인자 : 사운드 이름, 2인자 : 채널 enum, 4인자 : 볼륨 크기 (default : 1)*/
	static void		Play_Sound(wstring wstrFileName, _uint iGroupIndex, _float fVolume = 1.f);

	static _int		Play_LoopSound(wstring wstrFileName, _uint iGroupIndex, _float4 vPosition, _float fVolume = 1.f);
	static _int		Play_LoopSound(wstring wstrFileName, _uint iGroupIndex, _float fVolume = 1.f);

	static void		Stop_Sound(_uint iGroupIndex);

public:
	static string	Get_FileName(string strFilePath);
};

