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

	Create_HeadShotIcon();

	return S_OK;
}

HRESULT CUI_Damage::Initialize()
{
	return S_OK;
}

HRESULT CUI_Damage::Start()
{
	__super::Start();

	CREATE_GAMEOBJECT(m_pScratch, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pScratch);

	for (int i = 0; i < Head_End; ++i)
	{
		CREATE_GAMEOBJECT(m_pHeadShotIcon[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pHeadShotIcon[i]);
	}

	Init_HeadShot();
	Init_DmgNum();

	return S_OK;
}

void CUI_Damage::My_Tick()
{
	if (m_bIsScaleDown)
	{
		m_fAccTime += fDT(0);
		if (m_fAccTime >= m_fScaleDownTime)
		{
			m_fAccTime = 0.f;
			m_bIsScaleDown = false;

			for (int i = 0; i < 3; ++i)
				m_pArrDmgNum[i]->DoScale(-m_fScaleValue, m_fScaleDownTime);

			if (m_pDmgIcon->Is_Valid())
				m_pDmgIcon->DoScale(-m_fScaleValue, m_fScaleUpTime);

			if (m_eDamageIcon != Head)
				m_bDisable = true;
		}
	}
	else
	{
		m_fHeadShotTime += fDT(0);
		if (m_fHeadShotTime >= m_fScaleUpTime)
		{
			m_fHeadShotTime = 0.f;
			m_bScratch = true;

			if (m_eDamageIcon == Head)
			{
				m_vHeadPos.x -= 5.f;

				for (int i = 0; i < Head_End; ++i)
				{
					m_pHeadShotIcon[i]->Set_Pos(m_vHeadPos);
					Enable_Fade(m_pHeadShotIcon[i], 0.3f);
				}

				m_pDmgIcon->SetActive(false);
			}
		}
	}

	if (m_eDamageIcon == Head)
	{
		if (m_bScratch)
		{
			m_fScratchTime += fDT(0);
			if (m_fScratchTime >= 0.3f)
			{
				m_fScratchTime = 0.f;
				m_bScratch = false;
				m_bDisable = true;

				m_pScratch->Set_Pos(m_vHeadPos);
				Enable_Fade(m_pScratch, 0.3f);

				_float fDuration = 0.1f;

				m_pScratch->DoScale(25.f, fDuration);

				_float4 vUpPos = _float4(m_vHeadPos.x - 10.f, m_vHeadPos.y - 7.f, 0.f);
				_float4 vDownPos = _float4(m_vHeadPos.x + 10.f, m_vHeadPos.y + 7.f, 0.f);

				m_pHeadShotIcon[Head_Up]->DoMove(vUpPos, fDuration);
				m_pHeadShotIcon[Head_Down]->DoMove(vDownPos, fDuration);
			}
		}
	}

	if (m_bDisable)
	{
		m_fAccTime += fDT(0);
		if (m_fAccTime > 0.3f)
		{
			m_bScratch = false;

			Disable_Fade(m_pScratch, 0.3f);

			for (int i = 0; i < Head_End; ++i)
				Disable_Fade(m_pHeadShotIcon[i], 0.3f);
		}

		if (m_fAccTime > 0.1f)
		{
			m_fAccTime = 0.f;
			m_bDisable = false;
			DISABLE_GAMEOBJECT(this);
		}
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

	reverse(m_vecDigitDmg.begin(), m_vecDigitDmg.end());

	_float fRandPosX = (_float)random(0, 300);
	_float fRandPosY = (_float)random(-300, 300);

	for (int i = 0; i < m_vecDigitDmg.size(); ++i)
	{
		GET_COMPONENT_FROM(m_pArrDmgNum[i], CTexture)->Set_CurTextureIndex(m_vecDigitDmg[i]);

		_float fPosX = fRandPosX + (i * 20.f);
		m_pArrDmgNum[i]->Set_Pos(fPosX, fRandPosY);

		Enable_Fade(m_pArrDmgNum[i], m_fFadeInTime);

		m_pArrDmgNum[i]->Set_Color(m_vColorWhite);
		m_pArrDmgNum[i]->Set_Scale(m_vFontScale);
		m_pArrDmgNum[i]->DoScale(m_fScaleValue, m_fScaleUpTime);
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
				m_pArrDmgNum[i]->Set_Color(m_vColorGreen);
			}
		}

		m_pDmgIcon->Set_Pos(fRandPosX - 40.f, fRandPosY);
		m_vHeadPos = m_pDmgIcon->Get_Pos();
		m_pDmgIcon->Set_Scale(m_vHeadShotScale);
		m_pDmgIcon->DoScale(m_fScaleValue, m_fScaleUpTime);

		if (m_eDamageIcon < Default)
			GET_COMPONENT_FROM(m_pDmgIcon, CTexture)->Set_CurTextureIndex(m_eDamageIcon);

		Enable_Fade(m_pDmgIcon, m_fFadeInTime);
	}

	m_bIsScaleDown = true;
}

void CUI_Damage::OnDisable()
{
	__super::OnDisable();
}

void CUI_Damage::Enable_Damage(_uint eIcon, _float fDmg)
{
	if (eIcon > Default)
		return;

	m_iDamageValue = eIcon == 2 ? (_uint)(fDmg) : (_uint)(fDmg * -1.f);
	if (m_iDamageValue > 999)
		m_iDamageValue = 999;

	m_eDamageIcon = (DamageIcon)eIcon;
}

void CUI_Damage::Create_HeadShotIcon()
{
	m_pScratch = CUI_Object::Create();
	m_pScratch->Set_Texture(L"../Bin/Resources/Textures/UI/HUD/T_IconIndicatorScratch.dds");
	m_pScratch->Set_Scale(50.f);
	m_pScratch->Set_Sort(0.5f);

	m_pScratch->Set_FadeDesc(0.f, 0.3f, 0.3f, true);

	for (int i = 0; i < Head_End; ++i)
	{
		m_pHeadShotIcon[i] = CUI_Object::Create();

		m_pHeadShotIcon[i]->Set_Scale(50.f);
		m_pHeadShotIcon[i]->Set_Sort(0.5f);

		m_pHeadShotIcon[i]->Set_FadeDesc(0.f, 0.3f, 0.3f, true);

		switch (i)
		{
		case Head_Up:
			m_pHeadShotIcon[i]->Set_Texture(L"../Bin/Resources/Textures/UI/HUD/T_IconIndicatorHeadshotTop.dds");
			break;

		case Head_Down:
			m_pHeadShotIcon[i]->Set_Texture(L"../Bin/Resources/Textures/UI/HUD/T_IconIndicatorHeadshotBottom.dds");
			break;
		}
	}
}

void CUI_Damage::Init_HeadShot()
{
	m_pDmgIcon->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/T_HeadshotIcon.png"));
	GET_COMPONENT_FROM(m_pDmgIcon, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/T_IconGuardShield.dds"));
	GET_COMPONENT_FROM(m_pDmgIcon, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/T_HeartIcon.png"));

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
