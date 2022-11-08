#include "CUI_Object.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Loading_Manager.h"
#include "Transform.h"
#include "Functor.h"
#include "Renderer.h"

CUI_Object::CUI_Object()
{
}

CUI_Object::CUI_Object(const CUI_Object& Prototype)
	: CUI(Prototype)
{
}

CUI_Object::~CUI_Object()
{
}

HRESULT CUI_Object::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	SetTexture(TEXT("../Bin/Resources/Textures/White.png"));

	GET_COMPONENT(CRenderer)->Set_Pass(VTXTEX_PASS_ALPHA);

	Set_Pos(0.f, 0.f);
	Set_Scale(100.f);

	m_wstrUIName = TEXT("UI_Object");

	return S_OK;
}

HRESULT CUI_Object::Initialize()
{
	return S_OK;
}

HRESULT CUI_Object::Start()
{
	__super::Start();

	return S_OK;
}

void CUI_Object::Set_Texture(const _tchar* pFilePath)
{
	if (m_bIsMultiTex)
	{
		CTexture* pTexture = GET_COMPONENT(CTexture);
		pTexture->Add_Texture(pFilePath);
		pTexture->Next_Texture();
	}
	else
	{
		CTexture* pTexture = GET_COMPONENT(CTexture);
		pTexture->Add_Texture(pFilePath);
		pTexture->Next_Texture();
		pTexture->Remove_Texture(0);
		pTexture->Set_CurTextureIndex(0);
	}
}

void CUI_Object::SetUp_ShaderResource(CShader* pShader, const char* pConstName)
{
}

void CUI_Object::OnEnable()
{
	__super::OnEnable();
}

void CUI_Object::OnDisable()
{
	__super::OnDisable();
}

void CUI_Object::My_Tick()
{
	__super::My_Tick();

	if (m_bIsMouseTarget)
		MouseEvent();
}

void CUI_Object::My_LateTick()
{
	__super::My_LateTick();
}

void CUI_Object::MouseEvent()
{
	CheckInRect();

	m_bIsOnMouse = PtInRect(&m_tRect, m_ptMouse) ? true : false;

	if (m_bIsOnMouse)
	{
		if (CLoading_Manager::Get_Instance()->Get_LoadLevel() == LEVEL_TEST)
		{
			if (KEY(LBUTTON, HOLD))
			{
				_float4 vPos = CFunctor::To_Window(_float4(m_ptMouse.x, m_ptMouse.y, 0.f));
				Set_Pos(vPos.x, -vPos.y);
			}
		}

		OnMouseEnter();

		if (m_pButton)
		{
			// 버튼 이벤트 함수 호출

			/*if (KEY(LBUTTON, TAP))
			{
				OnMouseClick();
			}*/
		}
	}
	else
	{
		OnMouseExit();
	}
}
