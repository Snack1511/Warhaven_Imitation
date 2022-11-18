#include "CUI_Renderer.h"
#include "CustomFont.h"
#include "GameObject.h"
#include "Transform.h"
#include "GameInstance.h"

CUI_Renderer::CUI_Renderer(_uint iGroupID)
	: CRenderer(iGroupID)
{
}

CUI_Renderer::~CUI_Renderer()
{
	Release();
}

CUI_Renderer* CUI_Renderer::Create(_uint iGroupID, const RENDER_GROUP& eRenderGroup, const _uint& iCurPass, const _float4& vOffsetPos)
{
	CUI_Renderer* pRenderer = new CUI_Renderer(iGroupID);

	pRenderer->m_iCurPass = iCurPass;
	pRenderer->m_vOffsetPos = vOffsetPos;
	pRenderer->m_eRenderGroup = eRenderGroup;

	if (FAILED(pRenderer->Initialize_Prototype()))
	{
		SAFE_DELETE(pRenderer);
		Call_MsgBox(L"Failed to Initialize_Prototype : CUI_Renderer");
		return nullptr;
	}

	return pRenderer;
}

HRESULT CUI_Renderer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Renderer::Initialize()
{
	return S_OK;
}

void CUI_Renderer::Start()
{
	__super::Start();
}

void CUI_Renderer::Tick()
{
	__super::Tick();
}

void CUI_Renderer::Late_Tick()
{
	__super::Late_Tick();
}

HRESULT CUI_Renderer::Render()
{
	__super::Render();

	// 유아이 위치
	if (m_bIsRenderText)
	{
		_float4 vUIPos = m_pUI->Get_Transform()->Get_MyWorld(WORLD_POS);
		_float2 vFontPos = { vUIPos.x + m_vOffset.x, -vUIPos.y + m_vOffset.y };

		const _tchar* wszText = m_wstrText.c_str();

		if (m_bIsBold)
		{
			CGameInstance::Get_Instance()->Render_Font(TEXT("WarKR_Bold"), wszText, vFontPos, m_vColor, m_fFontScale);
		}
		else
		{
			CGameInstance::Get_Instance()->Render_Font(TEXT("War_Regular"), wszText, vFontPos, m_vColor, m_fFontScale);
		}
	}

	return S_OK;
}

void CUI_Renderer::Release()
{
	__super::Release();
}

void CUI_Renderer::Set_Text(_bool value, wstring wstrText, _float4 vOffset, _float4 vColor, _float fScale)
{
	vOffset.x += 640.f;
	vOffset.y += 360.f;

	m_bIsBold = value;
	m_wstrText = wstrText;
	m_vOffset = vOffset;
	m_vColor = vColor;
	m_fFontScale = fScale;
}
