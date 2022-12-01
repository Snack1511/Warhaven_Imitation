#include "CUI_Dead.h"
#include "CUI_Object.h"
#include "GameInstance.h"
#include "Texture.h"
#include "CFader.h"
#include "CPlayerInfo.h"
#include "CPlayer.h"

HRESULT CUI_Dead::Initialize_Prototype()
{
	Create_DeadUI();

	return S_OK;
}

HRESULT CUI_Dead::Initialize()
{
	return S_OK;
}

HRESULT CUI_Dead::Start()
{
	__super::Start();

	Set_FadeDeadUI();

	return S_OK;
}

void CUI_Dead::OnEnable()
{
	__super::OnEnable();

	wstring wstrUnitName = m_pTargetInfo->Get_Player()->Get_PlayerName();
	m_pDeadUI[DU_EnemyName]->Set_FontText(wstrUnitName);
	for (int i = 0; i < DU_End; ++i)
	{
		Enable_Fade(m_pDeadUI[i], 1.f);
	}
}

void CUI_Dead::OnDisable()
{
	__super::OnDisable();

	for (int i = 0; i < DU_End; ++i)
	{
		Disable_Fade(m_pDeadUI[i], 1.f);
	}
}

void CUI_Dead::Enable_DeadUI()
{
	ENABLE_GAMEOBJECT(this);
}

void CUI_Dead::My_Tick()
{
	__super::My_Tick();

	if (Is_Valid())
	{
		m_fAccTime += fDT(0);
		if (m_fAccTime > 5.f)
		{
			m_fAccTime = 0.f;

			DISABLE_GAMEOBJECT(this);
		}
	}
}

void CUI_Dead::My_LateTick()
{
	__super::My_LateTick();
}

void CUI_Dead::Create_DeadUI()
{
	for (int i = 0; i < DU_End; ++i)
	{
		m_pDeadUI[i] = CUI_Object::Create();

		CREATE_GAMEOBJECT(m_pDeadUI[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pDeadUI[i]);
	}

	GET_COMPONENT_FROM(m_pDeadUI[DU_Profile], CTexture)->Remove_Texture(0);
	Read_Texture(m_pDeadUI[DU_Profile], "/Dead", "Profile");

	m_pDeadUI[DU_Profile]->Set_PosX(-450.f);
	m_pDeadUI[DU_Profile]->Set_Scale(222.f, 500.f);

	GET_COMPONENT_FROM(m_pDeadUI[DU_EnemyName], CTexture)->Remove_Texture(0);
	Read_Texture(m_pDeadUI[DU_EnemyName], "/Dead", "Tier");

	m_pDeadUI[DU_EnemyName]->Set_PosY(-200.f);
	m_pDeadUI[DU_EnemyName]->Set_Scale(100.f);

	m_pDeadUI[DU_EnemyName]->Set_FontRender(true);
	m_pDeadUI[DU_EnemyName]->Set_FontStyle(true);
	m_pDeadUI[DU_EnemyName]->Set_FontCenter(true);
	m_pDeadUI[DU_EnemyName]->Set_FontScale(0.5f);

	m_pDeadUI[DU_KillText]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Dead/KillEnermy.png"));
	m_pDeadUI[DU_KillText]->Set_PosY(-150.f);
	m_pDeadUI[DU_KillText]->Set_Scale(172.f, 50.f);
	m_pDeadUI[DU_KillText]->Set_Color(_float4(0.9f, 0.f, 0.f, 1.f));
}

void CUI_Dead::Set_FadeDeadUI()
{
	FADEDESC tFadeDesc;
	ZeroMemory(&tFadeDesc, sizeof(FADEDESC));
	tFadeDesc.eFadeOutType = FADEDESC::FADEOUT_DISABLE;
	tFadeDesc.eFadeStyle = FADEDESC::FADE_STYLE_DEFAULT;
	tFadeDesc.bFadeInFlag = FADE_NONE;
	tFadeDesc.bFadeOutFlag = FADE_NONE;
	tFadeDesc.fFadeInStartTime = 0.f;
	tFadeDesc.fFadeInTime = 1.f;
	tFadeDesc.fFadeOutStartTime = 0.f;
	tFadeDesc.fFadeOutTime = 1.f;

	for (int i = 0; i < DU_End; ++i)
	{
		GET_COMPONENT_FROM(m_pDeadUI[i], CFader)->Get_FadeDesc() = tFadeDesc;
	}
}
