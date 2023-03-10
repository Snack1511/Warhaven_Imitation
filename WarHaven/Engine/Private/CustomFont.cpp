#include "..\Public\CustomFont.h"

#include "GameInstance.h"

CCustomFont::CCustomFont()
{
}

CCustomFont::~CCustomFont()
{
	Release();
}

CCustomFont* CCustomFont::Create(const _tchar* pFontFilePath)
{
	CCustomFont* pInstance = new CCustomFont();

	if (FAILED(pInstance->Initialize(pFontFilePath)))
	{
		Call_MsgBox(L"Failed to Initialize : CCustomFont");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CCustomFont::Initialize(const _tchar* pFontFilePath)
{
	m_pBatch = new SpriteBatch(PDEVICE_CONTEXT);
	m_pFont = new SpriteFont(PDEVICE, pFontFilePath);

	if (!m_pBatch || !m_pFont)
		return E_FAIL;

	return S_OK;
}



HRESULT CCustomFont::Render(const _tchar* pString, const _float2& vPosition, const _float4& vColor, _float fScale, _bool bCenter)
{
	if (nullptr == m_pBatch || nullptr == m_pFont)
		return E_FAIL;

	m_pBatch->Begin();

	if (!bCenter)
	{
		m_pFont->DrawString
		(
			m_pBatch,
			pString,
			vPosition,
			XMLoadFloat4(&vColor),
			0,
			XMFLOAT2(0.f, 0.f),
			fScale
		);
	}
	else
	{
		XMVECTOR size = m_pFont->MeasureString(pString);

		_float fPosX = (XMVectorGetX(size) * 0.5f) * fScale;
		_float fPosY = (XMVectorGetY(size) * 0.5f) * fScale;

		_float fResultX = (vPosition.x - 7.f) - fPosX;
		_float fResultY = (vPosition.y - 5.f) - fPosY;

		_float2 vResult = _float2(fResultX, fResultY);

		m_pFont->DrawString
		(
			m_pBatch,
			pString,
			vResult,
			XMLoadFloat4(&vColor),
			0,
			XMFLOAT2(0.f, 0.f),
			fScale
		);
	}

	m_pBatch->End();

	return S_OK;
}

_float CCustomFont::Get_FontSize(const _tchar* pString, _float fScale)
{
	XMVECTOR size = m_pFont->MeasureString(pString) * fScale;

	return size.m128_f32[0];
}





void CCustomFont::Release()
{
	SAFE_DELETE(m_pBatch);
	SAFE_DELETE(m_pFont);
}