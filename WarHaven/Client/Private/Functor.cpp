#include "stdafx.h"
#include "..\Public\Functor.h"

#include "GameInstance.h"
#include "Transform.h"
#include "GameObject.h"

#include "CUser.h"

_float4 CFunctor::Get_MousePos(void)
{
	POINT pt{};

	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	return _float4(float(pt.x), float(pt.y), 0.f, 1.f);
}

_float4 CFunctor::OrthoToRealPosition(const _float4& vPosition)
{
	_float4 vScreenPosition = _float4(
		vPosition.x - (g_iWinCX * 0.5f),
		-vPosition.y + (g_iWinCY * 0.5f),
		vPosition.z,
		1.f);

	return vScreenPosition;
}

_float4 CFunctor::RealToOrthoPosition(const _float4& vPosition)
{
	_float4 vScreenPosition = _float4(
		vPosition.x + (g_iWinCX * 0.5f),
		-vPosition.y + (g_iWinCY * 0.5f),
		vPosition.z,
		1.f);

	return vScreenPosition;
}

_float CFunctor::Get_DegreeFromDot(const _float4& V1, const _float4& V2)
{
	_float4	vLeft = V1;
	_float4	vRight = V2;

	vLeft.Normalize();
	vRight.Normalize();

	_float fCosTheta = vLeft.Dot(V2);
	_float fTheta = acosf(fCosTheta);

	if (vLeft.y < 0)
		fTheta *= -1.f;

	_float fDegree = XMConvertToDegrees(fTheta);

	return fDegree;
}

string CFunctor::To_String(wstring wstrText)
{
	return string(wstrText.begin(), wstrText.end());
}

wstring CFunctor::To_Wstring(string wstrText)
{
	return wstring(wstrText.begin(), wstrText.end());
}

string	CFunctor::Remove_String(string str, _bool value)
{
	if (value == true)
	{
		_int iFind = (_int)str.rfind("/") + 1;
		string strFileName = str.substr(iFind, str.length() - iFind);
	}
	else
	{
		_int iFind = (_int)str.rfind("/") + 1;
		string strFileName = str.substr(iFind, str.length() - iFind);

		
	}
	
	return str;
}

_float CFunctor::Lerp_Float(const _float& fSour, const _float& fDest, const _float& fRatio)
{
	return (fSour * (1.f - fRatio)) + (fDest * fRatio);
}

_float4 CFunctor::To_Descartes(_float4 vPos)
{
	vPos.x -= g_iWinCX * 0.5f;
	vPos.y -= g_iWinCY * 0.5f;

	return _float4(vPos.x, vPos.y, vPos.z);
}

_float4 CFunctor::To_Window(_float4 vPos)
{
	vPos.x += g_iWinCX * 0.5f;
	vPos.y += g_iWinCY * 0.5f;

	return _float4(vPos.x, vPos.y, vPos.z);
}

//void	CFunctor::Play_Sound(wstring wstrFileName, _uint iGroupIndex, _float4 vPosition, _float fVolume)
//{
//#define SOUND_MAX_RANGE	30.f
//#define SOUND_MIN_RANGE	4.f
//
//	_float fRatio = 0.f;
//
//	_float4 vPlayerPos = PLAYER->Get_Transform()->Get_World(WORLD_POS);
//	
//	_float fLength = (vPlayerPos - vPosition).Length();
//	_float fMinRatio = 0.1f;
//
//	if (iGroupIndex == CHANNEL_VOICE)
//		fMinRatio = 0.5f;
//
//
//	if (fLength <= SOUND_MIN_RANGE)
//		fRatio = 1.f;
//	else
//	{
//		fRatio = max(1.f - (fLength / SOUND_MAX_RANGE), fMinRatio);
//	}
//
//	//Length�� ���� ���� �Ҹ��� Ŀ����.
//	//�Ÿ��� 5���� ������ rATIO�� 1�� ����
//
//	GAMEINSTANCE->Play_Sound(wstrFileName.c_str(), (CHANNEL_GROUP)iGroupIndex, fRatio * fVolume);
//}
