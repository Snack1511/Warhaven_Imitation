#include "CUI_Interact.h"
#include "CUI_Object.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Transform.h"
#include "CUtility_Transform.h"

CUI_Interact::CUI_Interact()
{
}

CUI_Interact::~CUI_Interact()
{
}

HRESULT CUI_Interact::Initialize_Prototype()
{
	Create_InteractUI();
	Create_CircleEffect();

	return S_OK;
}

HRESULT CUI_Interact::Initialize()
{
	return S_OK;
}

HRESULT CUI_Interact::Start()
{
	__super::Start();

	return S_OK;
}

void CUI_Interact::Set_InteractKey(_uint iKeyIndex)
{
	m_pInteractUI->Set_TextureIndex(iKeyIndex);
}

void CUI_Interact::Set_InteractText(wstring wstrText)
{
	m_pInteractUI->Set_FontText(wstrText);
}

void CUI_Interact::Set_InteractTarget(CGameObject* pInteractTarget)
{
	m_pTargetTransform = pInteractTarget->Get_Transform();
}

void CUI_Interact::My_Tick()
{
	__super::My_Tick();

	if (!m_bEnableCircleEffect)
		return;

	m_fAccTime += fDT(0);
	if (m_fAccTime > 0.2f)
	{
		m_fAccTime = 0.f;

		m_pCircleEffect[m_iCircleEffectIdx]->Set_Pos(m_pInteractUI->Get_Pos());
		Enable_Fade(m_pCircleEffect[m_iCircleEffectIdx], 0.3f);
		m_pCircleEffect[m_iCircleEffectIdx]->DoScale(99.f, 0.3f);

		m_iCircleEffectIdx++;
		if (m_iCircleEffectIdx > 2)
			m_bEnableCircleEffect = false;
	}
}

void CUI_Interact::My_LateTick()
{
	__super::My_LateTick();

	if (m_pTargetTransform)
	{
		_float4 vTargetPos = CUtility_Transform::Get_ProjPos(m_pTargetTransform);
		vTargetPos.x -= 15.f;
		vTargetPos.y += 200.f;

		m_pInteractUI->Set_Pos(vTargetPos);
	}
	else
	{
		_float fPosX = m_pInteractUI->Get_FontSizeX() * 0.5f;
		m_pInteractUI->Set_Pos(-fPosX, -70.f);
	}
}

void CUI_Interact::OnEnable()
{
	__super::OnEnable();

	m_pInteractUI->SetActive(true);

	for (int i = 0; i < 3; ++i)
		m_pCircleEffect[i]->Set_Scale(1.f);

	m_iCircleEffectIdx = 0;
	m_bEnableCircleEffect = true;
}

void CUI_Interact::OnDisable()
{
	__super::OnDisable();

	m_pInteractUI->SetActive(false);
}

void CUI_Interact::Create_InteractUI()
{
	m_pInteractUI = CUI_Object::Create();

	GET_COMPONENT_FROM(m_pInteractUI, CTexture)->Remove_Texture(0);
	Read_Texture(m_pInteractUI, "/KeyIcon/Keyboard/White", "Key");

	m_pInteractUI->Set_Scale(20.f);

	m_pInteractUI->Set_FontRender(true);
	m_pInteractUI->Set_FontStyle(true);

	m_pInteractUI->Set_FontOffset(15.f, -15.f);
	m_pInteractUI->Set_FontScale(0.3f);

	CREATE_GAMEOBJECT(m_pInteractUI, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pInteractUI);
}

void CUI_Interact::Create_CircleEffect()
{
	for (int i = 0; i < 3; ++i)
	{
		m_pCircleEffect[i] = CUI_Object::Create();

		m_pCircleEffect[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Circle/T_64CircleStroke3px.dds"));
		m_pCircleEffect[i]->Set_Scale(1.f);
		m_pCircleEffect[i]->Set_Color(RGB(255, 255, 255));
		m_pCircleEffect[i]->Set_FadeDesc(0.f, 0.2f, 0.1f, true);

		CREATE_GAMEOBJECT(m_pCircleEffect[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pCircleEffect[i]);
	}
}
