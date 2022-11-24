#include "CUI_Damage.h"
#include "CUI_Object.h"

#include "GameInstance.h"
#include "Texture.h"
#include "CFader.h"

HRESULT CUI_Damage::Initialize_Prototype()
{
	m_pHeadShot = CUI_Object::Create();
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
	if (m_fAccTime >= m_fAliveTime)
	{
		DISABLE_GAMEOBJECT(this);
	}
}

void CUI_Damage::OnEnable()
{
	__super::OnEnable();

	vector<_float> vecDigitDmg;

	while (m_iDamageValue != 0)
	{
		_uint iDigitDmg = m_iDamageValue % 10; ;
		vecDigitDmg.push_back(iDigitDmg);
		sort(vecDigitDmg.begin(), vecDigitDmg.end(), greater<_uint>());

		m_iDamageValue /= 10;
	}

	_float fRandPosX = frandom(0.f, 300.f);
	_float fRandPosY = frandom(-300.f, 300.f);

	for (int i = 0; i < vecDigitDmg.size(); ++i)
	{
		GET_COMPONENT_FROM(m_pArrDmgNum[i], CTexture)->Set_CurTextureIndex(vecDigitDmg[i]);

		_float fPosX = fRandPosX + (i * 40.f);
		m_pArrDmgNum[i]->Set_Pos(fPosX, fRandPosY);

		Enable_Fade(m_pArrDmgNum[i], m_fFadeInTime);

		m_pArrDmgNum[i]->DoScale(30.f, 0.5f);
	}

	if (m_bIsHeadShot)
	{
		m_pHeadShot->Set_Pos(fRandPosX - 70.f, fRandPosY);

		Enable_Fade(m_pHeadShot, m_fFadeInTime);

		m_pHeadShot->DoScale(30.f, 0.5f);
	}
}

void CUI_Damage::Enable_Damage(_float fDmg, _bool bHeadShot)
{
	m_iDamageValue = fDmg;
	m_bIsHeadShot = bHeadShot;

	ENABLE_GAMEOBJECT(this);
}

void CUI_Damage::Init_HeadShot()
{
	m_pHeadShot->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/T_HeadshotIcon.dds"));
	m_pHeadShot->Set_Scale(100.f);

	Set_FadeDesc(m_pHeadShot);

	CREATE_GAMEOBJECT(m_pHeadShot, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pHeadShot);
}

void CUI_Damage::Init_DmgNum()
{
	Read_Texture(m_pDmgNum, "/Number", "Num");

	m_pDmgNum->Set_Scale(126.f, 150.f);
	m_pDmgNum->Set_Color(vColorRed);

	Set_FadeDesc(m_pDmgNum);

	GET_COMPONENT_FROM(m_pDmgNum, CTexture)->Remove_Texture(0);

	CREATE_GAMEOBJECT(m_pDmgNum, GROUP_UI);
	DELETE_GAMEOBJECT(m_pDmgNum);

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
