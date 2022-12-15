#include "CUI_Dead.h"
#include "CUI_Object.h"
#include "GameInstance.h"
#include "Texture.h"
#include "CFader.h"
#include "CPlayerInfo.h"
#include "CPlayer.h"
#include "CShader.h"
#include "CUI_Renderer.h"

#include "CUnit.h"
#include "CUser.h"

#include "CCamera_Follow.h"

HRESULT CUI_Dead::Initialize_Prototype()
{
	Create_DeadUI();
	Create_RevivalUI();

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

	if (!m_bIsFall)
	{
		wstring wstrUnitName = m_pTargetInfo->Get_Player()->Get_PlayerName();
		m_pDeadUI[DU_EnemyName]->Set_FontText(wstrUnitName);

		Fadenable_DeadUI();
	}
	else
	{
		SetActive_RevivalUI(true);
	}
}

void CUI_Dead::OnDisable()
{
	__super::OnDisable();

	SetActive_DeadUI(false);
	SetActive_RevivalUI(false);
}

void CUI_Dead::Toggle_DeadUI(_bool value, _bool isFall)
{
	m_bIsFall = isFall;

	if (value == true)
	{
		ENABLE_GAMEOBJECT(this);
	}
	else
	{
		DISABLE_GAMEOBJECT(this);
	}
}

void CUI_Dead::SetActive_DeadUI(_bool value)
{
	for (int i = 0; i < DU_End; ++i)
	{
		m_pDeadUI[i]->SetActive(value);
	}
}

void CUI_Dead::SetActive_RevivalUI(_bool value)
{
	m_vGaugeColor = _float4(1.f, 1.f, 1.f, 1.f);
	m_pRevivalUI[RU_Bar]->Set_Color(m_vGaugeColor);

	for (int i = 0; i < RU_End; ++i)
	{
		m_pRevivalUI[i]->SetActive(value);
	}
}

void CUI_Dead::My_Tick()
{
	__super::My_Tick();

	if (!m_bIsFall)
	{
		m_fAccTime += fDT(0);

		if (m_fAccTime > m_fDeadUIEnableTime)
		{
			m_fAccTime = 0.f;

			SetActive_DeadUI(false);

			SetActive_RevivalUI(true);

			PLAYER->Get_FollowCam()->Set_FollowTarget(PLAYER);
		}
	}



	if (m_pRevivalUI[RU_Bar]->Is_Valid())
	{
		_float fMinusColorValue = 1.f / m_fRevivalTime * fDT(0);

		m_vGaugeColor.y -= fMinusColorValue;
		if (m_vGaugeColor.y < 0.f)
			m_vGaugeColor.y = 0.f;

		m_vGaugeColor.z -= fMinusColorValue;
		if (m_vGaugeColor.z < 0.f)
			m_vGaugeColor.z = 0.f;

		m_pRevivalUI[RU_Bar]->Set_Color(m_vGaugeColor);

		if (!m_bAbleRevival)
		{
			m_bAbleRevival = true;
			m_pRevivalUI[RU_Bar]->Lerp_ScaleX(306.f, 0.f, m_fRevivalTime);
		}
		else
		{
			/* Oper UI 다시 띄우는 곳 */
			_float fScaleX = m_pRevivalUI[RU_Bar]->Get_Scale().x;
			if (fScaleX < 0.1f)
			{
				m_bAbleRevival = false;
				m_bIsFall = false;

				SetActive_RevivalUI(true);
				Toggle_DeadUI(false);

				CUser::Get_Instance()->SetActive_UnitHUD(false);

				CUser::Get_Instance()->Set_Respawn(true);
				CUser::Get_Instance()->SetActive_OperUI(true);

				CUser::Get_Instance()->Set_FixCursor(false);
				CUser::Get_Instance()->SetActive_Cursor(true);
				GAMEINSTANCE->Change_Camera(L"DefaultCam");
			}
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
	tFadeDesc.fFadeOutTime = 0.f;

	for (int i = 0; i < DU_End; ++i)
	{
		GET_COMPONENT_FROM(m_pDeadUI[i], CFader)->Get_FadeDesc() = tFadeDesc;
	}
}

void CUI_Dead::Fadenable_DeadUI()
{
	for (int i = 0; i < DU_End; ++i)
	{
		Enable_Fade(m_pDeadUI[i], 1.f);
	}
}

void CUI_Dead::Create_RevivalUI()
{
	for (int i = 0; i < RU_End; ++i)
	{
		m_pRevivalUI[i] = CUI_Object::Create();

		if (i < RU_Text)
		{
			m_pRevivalUI[i]->Set_PosY(-200.f);

			if (i == RU_BG)
			{
				m_pRevivalUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Dead/T_ReviveProgressBG.png"));
				m_pRevivalUI[i]->Set_Scale(303.f, 10.f);
				m_pRevivalUI[i]->Set_Sort(0.5f);
				m_pRevivalUI[i]->Set_Color(_float4(0.f, 0.f, 0.f, 0.5f));
			}
			else if (i == RU_Edge)
			{
				m_pRevivalUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Dead/T_ReviveProgressEdge.png"));
				m_pRevivalUI[i]->Set_Scale(310.f, 16.f);
				m_pRevivalUI[i]->Set_Sort(0.5f);
			}
			else if (i == RU_Bar)
			{
				m_pRevivalUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Dead/T_ReviveProgress.png"));
				m_pRevivalUI[i]->Set_Scale(306.f, 12.f);
				m_pRevivalUI[i]->Set_Sort(0.5f);
			}
		}
		else
		{
			if (i == RU_Text)
			{
				m_pRevivalUI[i]->Set_PosY(-175.f);
				m_pRevivalUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Dead/RivivalText.png"));
				m_pRevivalUI[i]->Set_Scale(171.f, 30.f);
				m_pRevivalUI[i]->Set_Sort(0.5f);
			}
			else if (i == RU_Giving)
			{
				m_pRevivalUI[i]->Set_Pos(-43.f, -225.f);
				m_pRevivalUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/KeyIcon/Mouse/T_MouseRightClickWIcon.dds"));
				m_pRevivalUI[i]->Set_Scale(28.f);
				m_pRevivalUI[i]->Set_Sort(0.5f);

				m_pRevivalUI[i]->Set_FontRender(true);
				m_pRevivalUI[i]->Set_FontStyle(true);
				m_pRevivalUI[i]->Set_FontCenter(true);
				m_pRevivalUI[i]->Set_FontScale(0.25f);
				m_pRevivalUI[i]->Set_FontOffset(60.f, 5.f);
				m_pRevivalUI[i]->Set_FontText(TEXT("소생 포기"));
			}
		}

		CREATE_GAMEOBJECT(m_pRevivalUI[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pRevivalUI[i]);
	}
}
