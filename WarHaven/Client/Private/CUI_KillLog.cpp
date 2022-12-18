#include "CUI_KillLog.h"
#include "GameInstance.h"
#include "CUI_Object.h"
#include "CPlayer.h"
#include "CTeamConnector.h"
#include "Texture.h"

#include "CUI_Renderer.h"

CUI_KillLog::CUI_KillLog()
{
}

CUI_KillLog::~CUI_KillLog()
{
}

HRESULT CUI_KillLog::Initialize_Prototype()
{
	Create_KillLog();

	return S_OK;
}

HRESULT CUI_KillLog::Initialize()
{
	return S_OK;
}

HRESULT CUI_KillLog::Start()
{
	__super::Start();

	for (_uint i = 0; i < Kill_End; ++i)
	{
		m_pAttacker[i]->Set_Pos(m_vStartPosition);
		m_pVictim[i]->Set_Pos(m_vStartPosition);
	}
	m_pDeadByIcon->Set_Pos(m_vStartPosition);

	return S_OK;
}


void CUI_KillLog::Set_OriginPosY()
{
	m_pDeadByIcon->Set_PosY(m_fKillLogPosY);
}

void CUI_KillLog::Set_LogName(CPlayer* attacker, CPlayer* victim)
{
	m_fAccTime = 0.f;

	ENABLE_GAMEOBJECT(this);
	//SetActive_KillLog(true);

	wstring attackerName = attacker->Get_PlayerInfo()->Get_PlayerName();
	wstring victimName = victim->Get_PlayerInfo()->Get_PlayerName();

	Init_VictimText(victimName);
	Init_AttackerText(attackerName);

	if (!attacker->Get_Team())
		return;

	_bool bIsAttackerTeam = attacker->Get_Team()->IsMainPlayerTeam();
	if (bIsAttackerTeam)
	{
		if (attacker->Get_OutlineType() == CPlayer::eSQUADMEMBER)
		{
			m_pAttacker[Kill_Name]->Set_FontColor(vColorGreen);
			m_pVictim[Kill_Name]->Set_FontColor(vColorRed);

			m_pAttacker[Kill_Icon]->Set_Color(vColorGreen);
			m_pVictim[Kill_Icon]->Set_Color(vColorRed);
		}
		else
		{
			m_pAttacker[Kill_Name]->Set_FontColor(vColorBlue);
			m_pVictim[Kill_Name]->Set_FontColor(vColorRed);

			m_pAttacker[Kill_Icon]->Set_Color(vColorBlue);
			m_pVictim[Kill_Icon]->Set_Color(vColorRed);
		}
	}
	else
	{
		if (victim->Get_OutlineType() == CPlayer::eSQUADMEMBER)
		{
			m_pAttacker[Kill_Name]->Set_FontColor(vColorRed);
			m_pVictim[Kill_Name]->Set_FontColor(vColorGreen);

			m_pAttacker[Kill_Icon]->Set_Color(vColorRed);
			m_pVictim[Kill_Icon]->Set_Color(vColorGreen);
		}
		else
		{
			m_pAttacker[Kill_Name]->Set_FontColor(vColorRed);
			m_pVictim[Kill_Name]->Set_FontColor(vColorBlue);

			m_pAttacker[Kill_Icon]->Set_Color(vColorRed);
			m_pVictim[Kill_Icon]->Set_Color(vColorBlue);
		}
	}

	CLASS_TYPE attackerClass = attacker->Get_CurClass();
	CLASS_TYPE victimClass = victim->Get_CurClass();

	GET_COMPONENT_FROM(m_pAttacker[Kill_Icon], CTexture)->Set_CurTextureIndex(attackerClass);
	GET_COMPONENT_FROM(m_pVictim[Kill_Icon], CTexture)->Set_CurTextureIndex(victimClass);

	_bool IsDeadByHeadshot = victim->IsDeadByHeadshot();
	GET_COMPONENT_FROM(m_pDeadByIcon, CTexture)->Set_CurTextureIndex(IsDeadByHeadshot);


}

void CUI_KillLog::Enable_KillUI()
{
	SetActive_KillLog(true);
}

void CUI_KillLog::MoveUp_KillLog()
{
	if (!m_pDeadByIcon)
		return;

	_float4 vPos = m_pDeadByIcon->Get_Pos();
	vPos.y += 35.f;
	m_pDeadByIcon->DoMove(vPos, 0.5f, 0);
}

void CUI_KillLog::My_Tick()
{
	__super::My_Tick();

	_float4 vTargetPos = m_vStartPosition;
	vTargetPos.y -= (m_fStepY * (_float)m_iCurIndex);
	
	_float fCurPosY = m_pDeadByIcon->Get_PosY();

	if (fCurPosY > vTargetPos.y)
	{
		fCurPosY -= fDT(0) * m_fStepY * 2.f;

		if (fCurPosY < vTargetPos.y)
			fCurPosY = vTargetPos.y;
	}

	m_pDeadByIcon->Set_PosY(fCurPosY);

	for (_uint i = 0; i < Kill_End; ++i)
	{
		m_pVictim[i]->Set_PosY(fCurPosY);
		m_pAttacker[i]->Set_PosY(fCurPosY);
	}


	if (!m_bIsDisable)
	{
		m_fAccTime += fDT(0);
		if (m_fAccTime > m_fDisableTime)
		{
			m_fAccTime = 0.f;
			m_bIsDisable = true;

			SetActive_KillLog(false);
		}
	}
	else
	{
		m_fAccTime += fDT(0);
		if (m_fAccTime > m_fFadeTime)
		{
			m_fAccTime = 0.f;
			SetActive(false);
		}
	}
}

void CUI_KillLog::OnEnable()
{
	__super::OnEnable();

	Enable_KillUI();
}

void CUI_KillLog::OnDisable()
{
	__super::OnDisable();

	m_bIsDisable = false;
}

void CUI_KillLog::SetActive_KillLog(_bool value)
{
	for (int i = 0; i < Kill_End; ++i)
	{
		if (value == true)
		{
			Enable_Fade(m_pDeadByIcon, m_fFadeTime);
			Enable_Fade(m_pAttacker[i], m_fFadeTime);
			Enable_Fade(m_pVictim[i], m_fFadeTime);
		}
		else
		{
			Disable_Fade(m_pDeadByIcon, m_fFadeTime);
			Disable_Fade(m_pAttacker[i], m_fFadeTime);
			Disable_Fade(m_pVictim[i], m_fFadeTime);
		}
	}
}

void CUI_KillLog::Init_VictimText(wstring Text)
{
	m_pVictim[Kill_Name]->Set_FontText(Text);

	_float fTextHalfSize = m_pVictim[Kill_Name]->Get_FontSizeX() * 0.5f;
	_float fVictimPosX = m_vStartPosition.x - fTextHalfSize;
	m_pVictim[Kill_Name]->Set_PosX(fVictimPosX);

	_float IconPos = fVictimPosX - fTextHalfSize - m_fIconBlank;
	m_pVictim[Kill_Icon]->Set_PosX(IconPos);

	m_pDeadByIcon->Set_PosX(IconPos - m_fIconBlank);
}

void CUI_KillLog::Init_AttackerText(wstring Text)
{
	_float fAttackerIconPosX = m_pDeadByIcon->Get_PosX() - m_fIconBlank;
	m_pAttacker[Kill_Icon]->Set_PosX(fAttackerIconPosX);


	m_pAttacker[Kill_Name]->Set_FontText(Text);
	_float fTextHalfSize = m_pAttacker[Kill_Name]->Get_FontSizeX() * 0.5f;
	_float fAttackerNamePos = fAttackerIconPosX - m_fIconBlank - fTextHalfSize;

	m_pAttacker[Kill_Name]->Set_PosX(fAttackerNamePos);
}

void CUI_KillLog::Create_KillLog()
{
	m_pDeadByIcon = CUI_Object::Create();

	m_pDeadByIcon->Set_FadeDesc(m_fFadeTime);

	GET_COMPONENT_FROM(m_pDeadByIcon, CTexture)->Remove_Texture(0);
	Read_Texture(m_pDeadByIcon, "/KillLog", "DeadBy");

	m_pDeadByIcon->Set_Scale(20.f);
	m_pDeadByIcon->Set_PosX(450.f);
	m_pDeadByIcon->Set_Sort(0.5f);

	CREATE_GAMEOBJECT(m_pDeadByIcon, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pDeadByIcon);

	for (int i = 0; i < Kill_End; ++i)
	{
		m_pAttacker[i] = CUI_Object::Create();
		m_pVictim[i] = CUI_Object::Create();

		m_pAttacker[i]->Set_FadeDesc(m_fFadeTime);
		m_pVictim[i]->Set_FadeDesc(m_fFadeTime);

		m_pAttacker[i]->Set_Scale(20.f);
		m_pAttacker[i]->Set_Sort(0.5f);

		m_pVictim[i]->Set_Scale(20.f);
		m_pVictim[i]->Set_Sort(0.5f);

		switch (i)
		{
		case Kill_Icon:

			GET_COMPONENT_FROM(m_pAttacker[Kill_Icon], CTexture)->Remove_Texture(0);
			Read_Texture(m_pAttacker[Kill_Icon], "/KillLog", "Class");

			GET_COMPONENT_FROM(m_pVictim[Kill_Icon], CTexture)->Remove_Texture(0);
			Read_Texture(m_pVictim[Kill_Icon], "/KillLog", "Class");

			break;

		case Kill_Name:

			m_pAttacker[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Alpha0.png"));
			m_pAttacker[i]->Set_FontRender(true);
			m_pAttacker[i]->Set_FontStyle(true);
			m_pAttacker[i]->Set_FontCenter(true);
			m_pAttacker[i]->Set_FontOffset(5.f, 5.f);
			m_pAttacker[i]->Set_FontScale(0.15f);

			m_pVictim[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Alpha0.png"));
			m_pVictim[i]->Set_FontRender(true);
			m_pVictim[i]->Set_FontStyle(true);
			m_pVictim[i]->Set_FontCenter(true);
			m_pVictim[i]->Set_FontOffset(5.f, 5.f);
			m_pVictim[i]->Set_FontScale(0.15f);

			break;
		}

		CREATE_GAMEOBJECT(m_pAttacker[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pAttacker[i]);

		CREATE_GAMEOBJECT(m_pVictim[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pVictim[i]);
	}
}
