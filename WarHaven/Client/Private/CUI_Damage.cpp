#include "CUI_Damage.h"
#include "CUI_Object.h"

#include "Easing_Utillity.h"
#include "GameInstance.h"
#include "Texture.h"
#include "CFader.h"
#include "CShader.h"

CUI_Damage::CUI_Damage()
{
}

CUI_Damage::~CUI_Damage()
{
}

HRESULT CUI_Damage::Initialize_Prototype()
{
	m_pDmgIcon = CUI_Object::Create();
	m_pDmgNum = CUI_Object::Create();

	return S_OK;
}

HRESULT CUI_Damage::Initialize()
{
	return S_OK;
}

HRESULT CUI_Damage::Start()
{
	__super::Start();

	Init_HeadShot();
	Init_DmgNum();

	return S_OK;
}

void CUI_Damage::My_Tick()
{
	m_fAccTime += fDT(0);

	_float m_fAliveTime = m_fFadeInTime + m_fFadeOutTime + m_fMaintainTime;

	if (m_fAccTime >= m_fScaleDownTime)
	{
		if (m_bIsScaleDown)
		{
			m_bIsScaleDown = false;
			for (int i = 0; i < 3; ++i)
			{
				m_pArrDmgNum[i]->DoScale(-m_fScaleValue, m_fScaleDownTime);
			}

			if (m_pDmgIcon->Is_Valid())
			{
				m_pDmgIcon->DoScale(-m_fScaleValue, m_fScaleUpTime);
			}
		}
	}

	if (m_fAccTime >= m_fAliveTime)
	{
		m_fAccTime = 0.f;
		DISABLE_GAMEOBJECT(this);
	}
}

void CUI_Damage::OnEnable()
{
	__super::OnEnable();

	m_vecDigitDmg.clear();
	while (1)
	{
		_uint iDigitDmg = m_iDamageValue % 10;
		m_vecDigitDmg.push_back(iDigitDmg);

		if (m_iDamageValue < 10)
			break;

		m_iDamageValue /= 10;
	}

	sort(m_vecDigitDmg.begin(), m_vecDigitDmg.end(), greater<_uint>());

	_float fRandPosX = (_float)random(0, 300);
	_float fRandPosY = (_float)random(-300, 300);

	for (int i = 0; i < m_vecDigitDmg.size(); ++i)
	{
		GET_COMPONENT_FROM(m_pArrDmgNum[i], CTexture)->Set_CurTextureIndex(m_vecDigitDmg[i]);

		_float fPosX = fRandPosX + (i * 20.f);
		m_pArrDmgNum[i]->Set_Pos(fPosX, fRandPosY);

		Enable_Fade(m_pArrDmgNum[i], m_fFadeInTime);

		m_pArrDmgNum[i]->Set_Scale(m_vFontScale);
		m_pArrDmgNum[i]->DoScale(m_fScaleValue, m_fScaleUpTime);
		m_pArrDmgNum[i]->Set_Color(m_vColorWhite);
	}

	if (m_eDamageIcon != Default)
	{
		for (int i = 0; i < m_vecDigitDmg.size(); ++i)
		{
			if (m_eDamageIcon == Head)
			{
				m_pArrDmgNum[i]->Set_Color(m_vColorRed);
			}
			else if (m_eDamageIcon == Guard)
			{
				m_pArrDmgNum[i]->Set_Color(m_vColorGray);
			}
			else if (m_eDamageIcon == Heal)
			{
				m_pArrDmgNum[i]->Set_Color(RGB(0, 255, 0));
			}
		}

		m_pDmgIcon->Set_Pos(fRandPosX - 40.f, fRandPosY);
		m_pDmgIcon->Set_Scale(m_vHeadShotScale);
		m_pDmgIcon->DoScale(m_fScaleValue, m_fScaleUpTime);

		GET_COMPONENT_FROM(m_pDmgIcon, CTexture)->Set_CurTextureIndex(m_eDamageIcon);

		Enable_Fade(m_pDmgIcon, m_fFadeInTime);
	}

	m_bIsScaleDown = true;
}

void CUI_Damage::Enable_Damage(_uint eIcon, _float fDmg)
{
	if (eIcon > Default)
		return;

	m_iDamageValue = eIcon == 2 ? (_uint)fDmg : (_uint)(fDmg * -1.f);
	m_eDamageIcon = (DamageIcon)eIcon;

	ENABLE_GAMEOBJECT(this);
}

void CUI_Damage::Init_HeadShot()
{
	m_pDmgIcon->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/T_HeadshotIcon.dds"));
	GET_COMPONENT_FROM(m_pDmgIcon, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/T_IconGuardShield.dds"));

	m_pDmgIcon->Set_Scale(100.f);

	Set_FadeDesc(m_pDmgIcon);

	CREATE_GAMEOBJECT(m_pDmgIcon, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pDmgIcon);
}

void CUI_Damage::Init_DmgNum()
{
	Read_Texture(m_pDmgNum, "/Number", "Num");

	m_pDmgNum->Set_Scale(m_vFontScale);

	Set_FadeDesc(m_pDmgNum);

	GET_COMPONENT_FROM(m_pDmgNum, CTexture)->Remove_Texture(0);


	CREATE_GAMEOBJECT(m_pDmgNum, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pDmgNum);

	for (int i = 0; i < 3; ++i)
	{
		m_pArrDmgNum[i] = m_pDmgNum->Clone();

		CREATE_GAMEOBJECT(m_pArrDmgNum[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pArrDmgNum[i]);
	}
}

void CUI_Damage::Set_FadeDesc(CUI_Object* pOther)
{
	FADEDESC tFadeDesc;
	ZeroMemory(&tFadeDesc, sizeof(FADEDESC));

	tFadeDesc.eFadeOutType = FADEDESC::FADEOUT_DISABLE;
	tFadeDesc.eFadeStyle = FADEDESC::FADE_STYLE_DEFAULT;

	tFadeDesc.bFadeInFlag = FADE_NONE;
	tFadeDesc.bFadeOutFlag = FADE_TIME;

	tFadeDesc.fFadeInStartTime = 0.f;
	tFadeDesc.fFadeInTime = m_fFadeInTime;

	tFadeDesc.fFadeOutStartTime = m_fMaintainTime;
	tFadeDesc.fFadeOutTime = m_fFadeOutTime;

	GET_COMPONENT_FROM(pOther, CFader)->Get_FadeDesc() = tFadeDesc;
}
