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

void CUI_Damage::OnEnable()
{
	__super::OnEnable();

	if (m_bIsHeadShot)
	{
		Enable_Fade(m_pHeadShot, m_fFadeInTime);
	}

	_uint iDigit = 0;
	while (m_iDamageValue != 0)
	{		
		_uint iTextureNum = m_iDamageValue % 10; ;
		GET_COMPONENT_FROM(m_pArrDmgNum[iDigit], CTexture)->Set_CurTextureIndex(iTextureNum);
		iDigit++;

		m_vecDigitDmg.push_back(iTextureNum);
		sort(m_vecDigitDmg.begin(), m_vecDigitDmg.end(), greater<_uint>());

		m_iDamageValue /= 10;
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

	Set_FadeDesc(m_pHeadShot);
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

	tFadeDesc.fFadeOutStartTime = 1.f;
	tFadeDesc.fFadeOutTime = m_fFadeOutTime;

	GET_COMPONENT_FROM(pOther, CFader)->Get_FadeDesc() = tFadeDesc;
}
