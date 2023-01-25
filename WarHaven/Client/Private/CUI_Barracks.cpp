#include "CUI_Barracks.h"
#include "CUI_Object.h"
#include "Texture.h"
#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "CUser.h"
#include "CUI_Cursor.h"
#include "CUI_Main.h"
#include "CPlayerInfo_Main.h"
#include "CPlayerInfo.h"
#include "CMainMenuPlayer.h"
#include "Functor.h"

_bool CUI_Barracks::m_bIsUnlock_RabbitHat = true;
_bool CUI_Barracks::m_bIsUnlock_EpicWarriorClothes = true;

CUI_Barracks::CUI_Barracks()
{
}

CUI_Barracks::~CUI_Barracks()
{
}

HRESULT CUI_Barracks::Initialize_Prototype()
{
	Create_ClassPort();
	Create_ClassInfo();
	Create_ClassBtn();
	Create_TopBtn();
	Create_SkinInfo();
	Create_SkinBtn();
	Crate_Skin();

	return S_OK;
}

HRESULT CUI_Barracks::Start()
{
	__super::Start();

	Init_ClassPort();
	Init_ClassInfo();
	Init_ClassBtn();
	Init_TopBtn();
	Init_SkinInfo();
	Init_SkinBtn();
	Init_Skin();

	Bind_Btn();

	return S_OK;
}

void CUI_Barracks::On_PointerEnter_Port(const _uint& iEventNum)
{
	CFunctor::Play_Sound(L"UI_Btn_Enter_Main", CHANNEL_UI, 1.f);

	Enable_Fade(m_pArrClassPort[iEventNum][Port_Highlight], 0.3f);
}

void CUI_Barracks::On_PointerExit_Port(const _uint& iEventNum)
{
	Disable_Fade(m_pArrClassPort[iEventNum][Port_Highlight], 0.3f);
}

void CUI_Barracks::On_PointerDown_Port(const _uint& iEventNum)
{
	CFunctor::Play_Sound(L"UI_Btn_Down_Main", CHANNEL_UI, 1.f);

	CUser::Get_Instance()->Enable_SkinPopup(0);

	m_iPrvEventNum = m_iCurEventNum;
	m_iCurEventNum = iEventNum;

	if (m_iPrvEventNum == iEventNum)
		return;

	for (int i = 0; i < Port_Underline; ++i)
	{
		_float4 vPrvPos = m_pArrClassPort[m_iPrvEventNum][i]->Get_Pos();
		vPrvPos.y -= 10.f;
		m_pArrClassPort[m_iPrvEventNum][i]->DoMove(vPrvPos, m_fDuration, 0);

		_float4 vCurPos = m_pArrClassPort[m_iCurEventNum][i]->Get_Pos();
		vCurPos.y += 10.f;
		m_pArrClassPort[m_iCurEventNum][i]->DoMove(vCurPos, m_fDuration, 0);
	}

	_float fOriginScaleX = 2.f;
	_float fTargetScaleX = 100.f;

	Disable_Fade(m_pArrClassPort[m_iPrvEventNum][Port_Underline], m_fDuration);
	m_pArrClassPort[m_iPrvEventNum][Port_Underline]->Lerp_ScaleX(fTargetScaleX, fOriginScaleX, m_fDuration);

	m_pArrClassPort[m_iCurEventNum][Port_Underline]->Lerp_ScaleX(fOriginScaleX, fTargetScaleX, m_fDuration);
	Enable_Fade(m_pArrClassPort[m_iCurEventNum][Port_Underline], m_fDuration);

	Disable_Fade(m_pArrClassPort[m_iPrvEventNum][Port_Outline], 0.3f);
	Enable_Fade(m_pArrClassPort[m_iCurEventNum][Port_Outline], 0.3f);

	m_iSelectClass = m_iCurEventNum;
	Set_ClassInfoText(m_iSelectClass);

	CUser::Get_Instance()->Set_MainMenuUnit(m_iSelectClass);
}

void CUI_Barracks::On_PointerEnter_Btn(const _uint& iEventNum)
{
	if (iEventNum > 0)
		return;

	CFunctor::Play_Sound(L"UI_Btn_Enter_Main", CHANNEL_UI, 1.f);

	m_pArrClassBtn[iEventNum][Btn_Outline]->Set_Color(m_vColorGold);
	m_pArrClassBtn[iEventNum][Btn_Deco]->Set_Color(m_vColorGold);
}

void CUI_Barracks::On_PointerStay_Btn(const _uint& iEventNum)
{
	if (iEventNum > 0)
		CUser::Get_Instance()->Get_Cursor()->Set_Mouse(CUI_Cursor::Disable);
}

void CUI_Barracks::On_PointerExit_Btn(const _uint& iEventNum)
{
	if (iEventNum > 0)
		return;

	m_pArrClassBtn[iEventNum][Btn_Outline]->Set_Color(m_vColorOutline);
	m_pArrClassBtn[iEventNum][Btn_Deco]->Set_Color(m_vColorOutline);
}

void CUI_Barracks::On_PointerDown_Btn(const _uint& iEventNum)
{
	if (iEventNum > 0)
		return;

	CFunctor::Play_Sound(L"UI_Btn_Down_Main", CHANNEL_UI, 1.f);

	for (auto& iter : m_pUIList)
		Disable_Fade(iter, m_fDuration);

	m_pTopBtn[0]->Set_FontColor(m_vColorWhite);

	CUser::Get_Instance()->SetActive_MainTopBtn(false);
	CUser::Get_Instance()->Set_TopBtnEffectPosX(-550.f);

	m_bIsSkinEnable = true;
}

void CUI_Barracks::On_PointerEnter_TopBtn(const _uint& iEventNum)
{
	CFunctor::Play_Sound(L"UI_Btn_Enter_Main", CHANNEL_UI, 1.f);
}

void CUI_Barracks::On_PointerDown_TopBtn(const _uint& iEventNum)
{
	m_iPrvSelectSkin = m_iCurSelectSkin;
	m_iCurSelectSkin = iEventNum;

	if (m_iPrvSelectSkin == iEventNum)
		return;

	CFunctor::Play_Sound(L"UI_Btn_Down_Main", CHANNEL_UI, 1.f);

	_float fPosX = -550.f + (iEventNum * 95.f);
	CUser::Get_Instance()->Set_TopBtnEffectPosX(fPosX);

	m_pTopBtn[m_iPrvSelectSkin]->Set_IsClick(false);
	m_pTopBtn[m_iPrvSelectSkin]->Set_FontColor(_float4(0.5f, 0.5f, 0.5f, 1.f));

	m_pTopBtn[m_iCurSelectSkin]->Set_IsClick(true);
	m_pTopBtn[m_iCurSelectSkin]->Set_FontColor(m_vColorWhite);

	m_bTickDisable = true;
}

void CUI_Barracks::On_PointerStay_SkinBG(const _uint& iEventNum)
{
	if (m_iCurSelectSkin == Skin::Hat)
	{
		if (m_bIsUnlock_RabbitHat)
		{
			if (iEventNum > 1)
				CUser::Get_Instance()->Get_Cursor()->Set_Mouse(CUI_Cursor::Disable);
		}
		else
		{
			if (iEventNum > 0)
				CUser::Get_Instance()->Get_Cursor()->Set_Mouse(CUI_Cursor::Disable);
		}
	}
	else
	{
		if (m_bIsUnlock_EpicWarriorClothes)
		{
			if (iEventNum == 1)
				CUser::Get_Instance()->Get_Cursor()->Set_Mouse(CUI_Cursor::Disable);
		}
		else
		{
			if (iEventNum > 0)
				CUser::Get_Instance()->Get_Cursor()->Set_Mouse(CUI_Cursor::Disable);
		}
	}
}

void CUI_Barracks::On_PointerDown_SkinBG(const _uint& iEventNum)
{
	if (iEventNum == 0)
		CFunctor::Play_Sound(L"UI_Btn_Down_Skin", CHANNEL_UI, 1.f);

	if (m_iCurSelectSkin == Skin::Clothes)
	{
		if (m_iSelectClass != WARRIOR)
			return;

		if (!m_bIsUnlock_EpicWarriorClothes)
			return;

		for (int j = 0; j < 2; ++j)
		{
			for (int i = SB_Outline; i < SB_Lock; ++i)
				Disable_Fade(m_pArrSkinBtn[j][i], m_fDuration);
		}

		switch (iEventNum)
		{
		case 0:
			CFunctor::Play_Sound(L"UI_Btn_Down_Skin", CHANNEL_UI, 1.f);
			m_pSkinInfo[Skin_Name]->Set_FontText(TEXT("훈련복"));
			m_pSkinInfo[Skin_Tier]->Set_FontText(TEXT("일반"));

			for (int i = SB_Outline; i < SB_Lock; ++i)
				Enable_Fade(m_pArrSkinBtn[iEventNum][i], m_fDuration);

			CUser::Get_Instance()->Get_MainPlayerInfo()->Set_CustomBody((CLASS_TYPE)m_iSelectClass, CPlayerInfo::eCUSTOM_BODY::eBODY1);
			CUser::Get_Instance()->Change_ModelParts(m_iSelectClass, MODEL_PART_BODY);
			break;

		case 2:
			CFunctor::Play_Sound(L"UI_Btn_Down_Skin", CHANNEL_UI, 1.f);
			m_pSkinInfo[Skin_Name]->Set_FontText(TEXT("영웅의 갑옷"));
			m_pSkinInfo[Skin_Tier]->Set_FontText(TEXT("영웅"));
			m_pSkinInfo[Skin_Tier]->Set_FontColor(RGB(90, 60, 130));

			for (int i = SB_Outline; i < SB_Lock; ++i)
				Enable_Fade(m_pArrSkinBtn[iEventNum][i], m_fDuration);

			CUser::Get_Instance()->Get_MainPlayerInfo()->Set_CustomBody((CLASS_TYPE)m_iSelectClass, CPlayerInfo::eCUSTOM_BODY::eBODY2);
			CUser::Get_Instance()->Change_ModelParts(m_iSelectClass, MODEL_PART_BODY);
			break;
		}
	}
	else if (m_iCurSelectSkin == Skin::Hat)
	{
		if (m_bIsUnlock_RabbitHat)
		{
			for (int j = 0; j < 2; ++j)
			{
				for (int i = SB_Outline; i < SB_Lock; ++i)
					Disable_Fade(m_pArrSkinBtn[j][i], m_fDuration);
			}

			switch (iEventNum)
			{
			case 0:
				CFunctor::Play_Sound(L"UI_Btn_Down_Skin", CHANNEL_UI, 1.f);
				m_pSkinInfo[Skin_Name]->Set_FontText(TEXT("미착용"));
				m_pSkinInfo[Skin_Tier]->Set_FontRender(false);

				for (int i = SB_Outline; i < SB_Lock; ++i)
					Enable_Fade(m_pArrSkinBtn[iEventNum][i], m_fDuration);

				CUser::Get_Instance()->Get_MainPlayerInfo()->Set_CustomHead((CLASS_TYPE)m_iSelectClass, CPlayerInfo::eCUSTOM_HEAD::eHEAD1);
				CUser::Get_Instance()->Change_ModelParts(m_iSelectClass, MODEL_PART_HEAD);
				break;

			case 1:
				CFunctor::Play_Sound(L"UI_Btn_Down_Skin", CHANNEL_UI, 1.f);
				m_pSkinInfo[Skin_Name]->Set_FontText(TEXT("토끼탈"));
				m_pSkinInfo[Skin_Tier]->Set_FontRender(true);
				m_pSkinInfo[Skin_Tier]->Set_FontText(TEXT("고급"));
				m_pSkinInfo[Skin_Tier]->Set_FontColor(RGB(60, 100, 200));

				for (int i = SB_Outline; i < SB_Lock; ++i)
					Enable_Fade(m_pArrSkinBtn[iEventNum][i], m_fDuration);

				CUser::Get_Instance()->Get_MainPlayerInfo()->Set_CustomHead((CLASS_TYPE)m_iSelectClass, CPlayerInfo::eCUSTOM_HEAD::eRABBIT);
				CUser::Get_Instance()->Change_ModelParts(m_iSelectClass, MODEL_PART_HEAD);
				break;
			}
		}
		else
		{
			if (iEventNum > 0)
				return;
		}
	}
	else
	{
		if (iEventNum > 0)
			return;
	}
}

void CUI_Barracks::Unlock_RabbitHat()
{
	m_bIsUnlock_RabbitHat = true;
}

void CUI_Barracks::Unlock_EpicWarriorClothes()
{
	m_bIsUnlock_EpicWarriorClothes = true;
}

void CUI_Barracks::My_Tick()
{
	__super::My_Tick();

	Late_Enable();
	Late_SkinEnable();

	Disable_SkinWindow();

	if (m_bTickDisable)
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = SB_Outline; j < SB_End; ++j)
				Disable_Fade(m_pArrSkinBtn[i][j], m_fDuration);

			for (int j = 0; j < Skin::End; ++j)
				Disable_Fade(m_pArrSkin[i][j], m_fDuration);
		}

		m_bTickDisable = false;
		m_bLateTickEnable = true;
	}
}

void CUI_Barracks::My_LateTick()
{
	__super::My_LateTick();

	if (m_bLateTickEnable)
	{
		Set_SkinIdx((CLASS_TYPE)m_iSelectClass);

		m_bLateTickEnable = false;
	}
}

void CUI_Barracks::OnEnable()
{
	__super::OnEnable();

	m_bIsEnable = true;

	for (int i = 0; i < Port_Underline; ++i)
	{
		m_pArrClassPort[m_iSelectClass][i]->Set_PosY(-240.f);

		if (i == Port_Class)
			m_pArrClassPort[m_iSelectClass][i]->Set_PosY(-185.f);
	}

	Enable_Fade(m_pArrClassPort[m_iSelectClass][Port_Outline], m_fDuration);
	Enable_Fade(m_pArrClassPort[m_iSelectClass][Port_Underline], m_fDuration);

	m_pArrClassPort[m_iSelectClass][Port_Underline]->Set_Scale(100.f, 2.f);
}

void CUI_Barracks::OnDisable()
{
	__super::OnDisable();

	for (auto& iter : m_pUIList)
		Disable_Fade(iter, m_fDuration);
}

void CUI_Barracks::Create_ClassPort()
{
	for (int i = 0; i < Port_End; ++i)
	{
		m_pClassPort[i] = CUI_Object::Create();

		m_pClassPort[i]->Set_FadeDesc(0.3f);

		m_pClassPort[i]->Set_PosY(-250.f);
		m_pClassPort[i]->Set_Scale(100.f, 140.f);
		m_pClassPort[i]->Set_Sort(0.5f);

		switch (i)
		{
		case Port_BG:
			m_pClassPort[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/CharacterWindow/BG.dds"));
			m_pClassPort[i]->Set_MouseTarget(true);
			break;

		case Port_Char:
			GET_COMPONENT_FROM(m_pClassPort[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pClassPort[i], "/CharacterWindow", "Portrait");

			m_pClassPort[i]->Set_Sort(0.49f);
			break;

		case Port_Highlight:
			m_pClassPort[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/Barracks/T_GradientSmall3.dds"));
			m_pClassPort[i]->Set_Color(_float4(1.f, 1.f, 1.f, 0.3f));
			m_pClassPort[i]->Set_Sort(0.47f);
			m_pClassPort[i]->Set_IsSlice(true);
			m_pClassPort[i]->Set_SliceRatio(_float4(0.f, 0.f, 0.f, 0.9f));
			break;

		case Port_Class:
			GET_COMPONENT_FROM(m_pClassPort[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pClassPort[i], "/CharacterWindow", "Icon");

			m_pClassPort[i]->Set_PosY(-195.f);
			m_pClassPort[i]->Set_Scale(20.f);
			m_pClassPort[i]->Set_Sort(0.48f);
			break;

		case Port_Outline:
			m_pClassPort[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/Barracks/T_2pxStrokeBox.png"));
			m_pClassPort[i]->Set_Color(m_vColorGold);
			m_pClassPort[i]->Set_Scale(102.f, 142.f);
			m_pClassPort[i]->Set_Sort(0.48f);
			m_pClassPort[i]->Set_IsSlice(true);
			m_pClassPort[i]->Set_TextureSzie(_float2(32.f, 32.f));
			m_pClassPort[i]->Set_SliceRatio(_float4(0.5f, 0.5f, 0.5f, 0.5f));
			break;

		case Port_Underline:
			m_pClassPort[i]->Set_Color(m_vColorGold);
			m_pClassPort[i]->Set_PosY(-318.f);
			m_pClassPort[i]->Set_Scale(2.f);
			break;
		}

		for (int j = 0; j < CLASS_END; ++j)
		{
			m_pArrClassPort[j][i] = m_pClassPort[i]->Clone();
		}
	}
}

void CUI_Barracks::Create_ClassInfo()
{
	for (int i = 0; i < Info_End; ++i)
	{
		m_pClassInfo[i] = CUI_Object::Create();

		m_pClassInfo[i]->Set_FadeDesc(0.3f);

		m_pClassInfo[i]->Set_PosX(-565.f);
		m_pClassInfo[i]->Set_Sort(0.5f);

		m_pClassInfo[i]->Set_FontRender(true);
		m_pClassInfo[i]->Set_FontStyle(true);

		switch (i)
		{
		case Info_BG:
			m_pClassInfo[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/CharacterWindow/ClassInfo_BG.dds"));
			m_pClassInfo[i]->Set_Scale(50.f, 110.f);
			m_pClassInfo[i]->Set_PosY(200.f);

			m_pClassInfo[i]->Set_FontOffset(37.f, 0.f);
			m_pClassInfo[i]->Set_FontScale(0.25f);
			break;

		case Info_Class:
			GET_COMPONENT_FROM(m_pClassInfo[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pClassInfo[i], "/CharacterWindow", "Icon");

			m_pClassInfo[i]->Set_Scale(32.f);
			m_pClassInfo[i]->Set_PosY(213.f);

			m_pClassInfo[i]->Set_FontOffset(33, -30.f);
			m_pClassInfo[i]->Set_FontScale(0.5f);
			break;
		}
	}
}

void CUI_Barracks::Create_ClassBtn()
{
	for (int i = 0; i < Btn_End; ++i)
	{
		m_pClassBtn[i] = CUI_Object::Create();

		m_pClassBtn[i]->Set_FadeDesc(0.3f);
		m_pClassBtn[i]->Set_Sort(0.49f);
		m_pClassBtn[i]->Set_PosX(-490.f);

		switch (i)
		{
		case Btn_BG:
			m_pClassBtn[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Rect/White.png"));
			m_pClassBtn[i]->Set_Scale(200.f, 50.f);
			m_pClassBtn[i]->Set_Sort(0.5f);
			m_pClassBtn[i]->Set_Color(m_vColorBG);
			m_pClassBtn[i]->Set_MouseTarget(true);
			break;

		case Btn_Outline:
			m_pClassBtn[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/Barracks/T_2pxStrokeBox.png"));
			m_pClassBtn[i]->Set_Scale(202.f, 52.f);
			m_pClassBtn[i]->Set_Color(m_vColorOutline);

			m_pClassBtn[i]->Set_IsSlice(true);
			m_pClassBtn[i]->Set_TextureSzie(_float2(32.f, 32.f));
			m_pClassBtn[i]->Set_SliceRatio(_float4(0.5f, 0.5f, 0.5f, 0.5f));
			break;

		case Btn_Deco:
			m_pClassBtn[i]->Set_PosX(-585.f);
			m_pClassBtn[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/Barracks/T_DecoLam01.dds"));
			m_pClassBtn[i]->Set_Scale(7.f, 48.f);
			m_pClassBtn[i]->Set_Color(m_vColorOutline);
			break;

		case Btn_Text:
			m_pClassBtn[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Alpha0.png"));

			m_pClassBtn[i]->Set_FontRender(true);
			m_pClassBtn[i]->Set_FontStyle(true);
			m_pClassBtn[i]->Set_FontOffset(-26.f, -15.f);
			m_pClassBtn[i]->Set_FontScale(0.23f);
			m_pClassBtn[i]->Set_FontColor(m_vColorGold);
			break;

		case Btn_LockText:
			m_pClassBtn[i]->Set_PosX(-555.f);
			m_pClassBtn[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Alpha0.png"));

			m_pClassBtn[i]->Set_FontRender(true);
			m_pClassBtn[i]->Set_FontOffset(-26.f, -15.f);
			m_pClassBtn[i]->Set_FontScale(0.2f);
			break;

		case Btn_Lock:
			m_pClassBtn[i]->Set_PosX(-572.f);
			m_pClassBtn[i]->Set_Scale(20.f);
			m_pClassBtn[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/Barracks/T_Lock.dds"));
			break;
		}
	}
}

void CUI_Barracks::Create_TopBtn()
{
	for (int i = 0; i < 4; ++i)
	{
		m_pTopBtn[i] = CUI_Object::Create();

		m_pTopBtn[i]->Set_FadeDesc(0.3f);

		m_pTopBtn[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Alpha0.png"));

		_float fPosX = -560.f + (i * 95.f);
		m_pTopBtn[i]->Set_Pos(fPosX, 300.f);
		m_pTopBtn[i]->Set_Scale(75.f, 35.f);
		m_pTopBtn[i]->Set_Sort(0.5f);

		m_pTopBtn[i]->Set_MouseTarget(true);

		m_pTopBtn[i]->Set_FontRender(true);
		m_pTopBtn[i]->Set_FontStyle(true);
		m_pTopBtn[i]->Set_FontCenter(true);
		m_pTopBtn[i]->Set_FontScale(0.4f);
		m_pTopBtn[i]->Set_FontOffset(5.f, 5.f);
		m_pTopBtn[i]->Set_FontColor(_float4(0.5f, 0.5f, 0.5f, 1.f));

		switch (i)
		{
		case 0:
			m_pTopBtn[i]->Set_FontText(TEXT("의상"));
			break;

		case 1:
			m_pTopBtn[i]->Set_FontText(TEXT("무기"));
			break;

		case 2:
			m_pTopBtn[i]->Set_FontText(TEXT("모자"));
			break;

		case 3:
			m_pTopBtn[i]->Set_FontText(TEXT("날틀"));
			break;
		}
	}
}

void CUI_Barracks::Crate_Skin()
{
	for (int i = 0; i < Skin::End; ++i)
	{
		m_pSkin[i] = CUI_Object::Create();

		m_pSkin[i]->Set_Sort(0.49f);
		m_pSkin[i]->Set_FadeDesc(0.3f);
		m_pSkin[i]->Set_Scale(125.f);
		m_pSkin[i]->Set_PosY(175.f);

		GET_COMPONENT_FROM(m_pSkin[i], CTexture)->Remove_Texture(0);

		switch (i)
		{
		case CUI_Barracks::Clothes:
			Read_Texture(m_pSkin[i], "/Lobby/Barracks/Skin/Clothes", "Clothes");
			break;

		case CUI_Barracks::Weapon:
			Read_Texture(m_pSkin[i], "/Lobby/Barracks/Skin/Weapon", "Weapon");
			break;

		case CUI_Barracks::Hat:
			Read_Texture(m_pSkin[i], "/Lobby/Barracks/Skin/Hat", "Hat");
			break;

		case CUI_Barracks::Glider:
			Read_Texture(m_pSkin[i], "/Lobby/Barracks/Skin/Glider", "Glider");
			break;
		}
	}
}

void CUI_Barracks::Create_SkinInfo()
{
	for (int i = 0; i < Skin_End; ++i)
	{
		m_pSkinInfo[i] = CUI_Object::Create();

		m_pSkinInfo[i]->Set_PosX(-575.f);
		m_pSkinInfo[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Alpha0.png"));

		m_pSkinInfo[i]->Set_FontRender(true);
		m_pSkinInfo[i]->Set_FontText(TEXT("테스트"));
		m_pSkinInfo[i]->Set_Sort(0.5f);

		switch (i)
		{
		case Skin_Name:
			m_pSkinInfo[i]->Set_PosY(225.f);
			m_pSkinInfo[i]->Set_FontStyle(true);
			m_pSkinInfo[i]->Set_FontOffset(-20.f, -25.f);
			m_pSkinInfo[i]->Set_FontScale(0.5f);
			break;

		case Skin_Tier:
			m_pSkinInfo[i]->Set_PosY(195.f);
			m_pSkinInfo[i]->Set_FontOffset(-17.f, -15.f);
			m_pSkinInfo[i]->Set_FontScale(0.3f);
			break;
		}
	}
}

void CUI_Barracks::Create_SkinBtn()
{
	for (int i = 0; i < SB_End; ++i)
	{
		m_pSkinBtn[i] = CUI_Object::Create();

		m_pSkinBtn[i]->Set_FadeDesc(0.3f);
		m_pSkinBtn[i]->Set_Scale(125.f);
		m_pSkinBtn[i]->Set_PosY(175.f);

		switch (i)
		{
		case SB_BG:
			GET_COMPONENT_FROM(m_pSkinBtn[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pSkinBtn[i], "/Lobby/Barracks/Skin", "T_ItemBG");
			m_pSkinBtn[i]->Set_Sort(0.5f);
			m_pSkinBtn[i]->Set_MouseTarget(true);
			break;

		case SB_Outline:
			m_pSkinBtn[i]->Set_Color(m_vColorGold);
			m_pSkinBtn[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/Barracks/T_2pxStrokeBox.png"));
			m_pSkinBtn[i]->Set_IsSlice(true);
			m_pSkinBtn[i]->Set_TextureSzie(_float2(32.f, 32.f));
			m_pSkinBtn[i]->Set_SliceRatio(_float4(0.5f, 0.5f, 0.5f, 0.5f));
			m_pSkinBtn[i]->Set_Sort(0.49f);
			m_pSkinBtn[i]->Set_Scale(136.f);
			break;

		case SB_Select:
			m_pSkinBtn[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/Barracks/T_Check2.dds"));
			m_pSkinBtn[i]->Set_PosY(220.f);
			m_pSkinBtn[i]->Set_Sort(0.47f);
			m_pSkinBtn[i]->Set_Scale(24.f);
			break;

		case SB_Lock:
			m_pSkinBtn[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/Barracks/T_Lock.dds"));
			m_pSkinBtn[i]->Set_PosY(220.f);
			m_pSkinBtn[i]->Set_Sort(0.47f);
			m_pSkinBtn[i]->Set_Scale(24.f);
			break;

		case SB_Blind:
			m_pSkinBtn[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/Barracks/T_GradientSmall3.dds"));
			m_pSkinBtn[i]->Set_Sort(0.48f);
			m_pSkinBtn[i]->Set_Scale(125.f);
			m_pSkinBtn[i]->Set_Color(_float4(0.f, 0.f, 0.f, 1.f));

			m_pSkinBtn[i]->Set_IsSlice(true);
			m_pSkinBtn[i]->Set_TextureSzie(_float2(80.f, 80.f));
			m_pSkinBtn[i]->Set_SliceRatio(_float4(0.5f, 0.5f, 0.5f, 0.5f));
			break;
		}
	}
}

void CUI_Barracks::Init_ClassPort()
{
	for (int i = 0; i < Port_End; ++i)
	{
		CREATE_GAMEOBJECT(m_pClassPort[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pClassPort[i]);

		for (int j = 0; j < CLASS_END; ++j)
		{
			CREATE_GAMEOBJECT(m_pArrClassPort[j][i], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pArrClassPort[j][i]);

			_float fPosX = -540.f + (j * 120.f);
			m_pArrClassPort[j][i]->Set_PosX(fPosX);

			switch (i)
			{
			case Port_BG:
				m_pUIList.push_back(m_pArrClassPort[j][i]);
				break;

			case Port_Char:
				m_pUIList.push_back(m_pArrClassPort[j][i]);
				break;

			case Port_Class:
			{
				m_pUIList.push_back(m_pArrClassPort[j][i]);

				_float fClassPosX = fPosX + 35.f;
				m_pArrClassPort[j][Port_Class]->Set_PosX(fClassPosX);
			}
			break;

			case Port_Outline:
				m_pUIList.push_back(m_pArrClassPort[j][i]);
				break;

			case Port_Underline:
				m_pUIList.push_back(m_pArrClassPort[j][i]);
				break;
			}
		}
	}

	for (int i = 0; i < CLASS_END; ++i)
	{
		m_pArrClassPort[i][Port_Char]->Set_TextureIndex(i);
		m_pArrClassPort[i][Port_Class]->Set_TextureIndex(i);
	}
}

void CUI_Barracks::Init_ClassInfo()
{
	for (int i = 0; i < Info_End; ++i)
	{
		CREATE_GAMEOBJECT(m_pClassInfo[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pClassInfo[i]);

		m_pUIList.push_back(m_pClassInfo[i]);
	}
}

void CUI_Barracks::Init_ClassBtn()
{
	for (int i = 0; i < Btn_End; ++i)
	{
		CREATE_GAMEOBJECT(m_pClassBtn[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pClassBtn[i]);

		for (int j = 0; j < 3; ++j)
		{
			m_pArrClassBtn[j][i] = m_pClassBtn[i]->Clone();

			CREATE_GAMEOBJECT(m_pArrClassBtn[j][i], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pArrClassBtn[j][i]);

			m_pUIList.push_back(m_pArrClassBtn[j][i]);

			_float fPosY = 100.f - (60.f * j);
			m_pArrClassBtn[j][i]->Set_PosY(fPosY);

			switch (i)
			{
			case Btn_Text:
			{
				_float fTextPosY = fPosY + 5.f;
				m_pArrClassBtn[j][i]->Set_PosY(fTextPosY);
			}
			break;

			case Btn_LockText:
			{
				_float fLockTextPosY = fPosY - 15.f;
				m_pArrClassBtn[j][i]->Set_PosY(fLockTextPosY);
			}
			break;

			case Btn_Lock:
			{
				_float fLockPosY = fPosY + 9.f;
				m_pArrClassBtn[j][i]->Set_PosY(fLockPosY);
			}
			break;
			}
		}
	}

	m_pArrClassBtn[0][Btn_Text]->Set_PosX(-555.f);
	m_pArrClassBtn[0][Btn_Text]->Set_FontText(TEXT("스킨"));
	m_pArrClassBtn[0][Btn_LockText]->Set_FontText(TEXT("의상·무기·모자·날틀"));

	m_pArrClassBtn[1][Btn_Text]->Set_PosX(-540.f);
	m_pArrClassBtn[1][Btn_Text]->Set_FontText(TEXT("특성"));
	m_pArrClassBtn[1][Btn_LockText]->Set_FontText(TEXT("잠김"));

	m_pArrClassBtn[2][Btn_Text]->Set_PosX(-540.f);
	m_pArrClassBtn[2][Btn_Text]->Set_FontText(TEXT("의사소통"));
	m_pArrClassBtn[2][Btn_LockText]->Set_FontText(TEXT("잠김"));
}

void CUI_Barracks::Init_TopBtn()
{
	for (int i = 0; i < 4; ++i)
	{
		CREATE_GAMEOBJECT(m_pTopBtn[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pTopBtn[i]);
	}
}

void CUI_Barracks::Init_SkinInfo()
{
	for (int i = 0; i < Skin_End; ++i)
	{
		CREATE_GAMEOBJECT(m_pSkinInfo[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pSkinInfo[i]);

		m_pUIList.push_back(m_pSkinInfo[i]);
	}
}

void CUI_Barracks::Init_Skin()
{
	for (int i = 0; i < Skin::End; ++i)
	{
		CREATE_GAMEOBJECT(m_pSkin[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pSkin[i]);
	}

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < Skin::End; ++j)
		{
			m_pArrSkin[i][j] = m_pSkin[j]->Clone();

			CREATE_GAMEOBJECT(m_pArrSkin[i][j], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pArrSkin[i][j]);

			m_pUIList.push_back(m_pArrSkin[i][j]);

			_float fPosX = 235.f + (i * 140.f);
			m_pArrSkin[i][j]->Set_PosX(fPosX);
		}
	}
}

void CUI_Barracks::Init_SkinBtn()
{
	for (int i = 0; i < SB_End; ++i)
	{
		CREATE_GAMEOBJECT(m_pSkinBtn[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pSkinBtn[i]);
	}

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < SB_End; ++j)
		{
			m_pArrSkinBtn[i][j] = m_pSkinBtn[j]->Clone();

			CREATE_GAMEOBJECT(m_pArrSkinBtn[i][j], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pArrSkinBtn[i][j]);

			m_pUIList.push_back(m_pArrSkinBtn[i][j]);

			_float fPosX = 235.f + (i * 140.f);
			m_pArrSkinBtn[i][j]->Set_PosX(fPosX);

			switch (j)
			{
			case SB_BG:
				m_pArrSkinBtn[i][j]->Set_TextureIndex(i);
				break;

			case SB_Select:
			{
				_float fSelectPosX = fPosX + 45.f;
				m_pArrSkinBtn[i][j]->Set_PosX(fSelectPosX);
			}
			break;

			case SB_Lock:
			{
				_float fLockPosX = fPosX + 45.f;
				m_pArrSkinBtn[i][j]->Set_PosX(fLockPosX);
			}
			break;
			}
		}
	}
}

void CUI_Barracks::Bind_Btn()
{
	for (int i = 0; i < CLASS_END; ++i)
	{
		m_pArrClassPort[i][Port_BG]->CallBack_PointEnter += bind(&CUI_Barracks::On_PointerEnter_Port, this, i);
		m_pArrClassPort[i][Port_BG]->CallBack_PointExit += bind(&CUI_Barracks::On_PointerExit_Port, this, i);
		m_pArrClassPort[i][Port_BG]->CallBack_PointDown += bind(&CUI_Barracks::On_PointerDown_Port, this, i);
	}

	for (int i = 0; i < 3; ++i)
	{
		m_pArrClassBtn[i][Btn_BG]->CallBack_PointEnter += bind(&CUI_Barracks::On_PointerEnter_Btn, this, i);
		m_pArrClassBtn[i][Btn_BG]->CallBack_PointStay += bind(&CUI_Barracks::On_PointerStay_Btn, this, i);
		m_pArrClassBtn[i][Btn_BG]->CallBack_PointExit += bind(&CUI_Barracks::On_PointerExit_Btn, this, i);
		m_pArrClassBtn[i][Btn_BG]->CallBack_PointDown += bind(&CUI_Barracks::On_PointerDown_Btn, this, i);
	}

	for (int i = 0; i < 4; ++i)
	{
		m_pTopBtn[i]->CallBack_PointEnter += bind(&CUI_Barracks::On_PointerEnter_TopBtn, this, i);
		m_pTopBtn[i]->CallBack_PointDown += bind(&CUI_Barracks::On_PointerDown_TopBtn, this, i);
	}

	for (int i = 0; i < 3; ++i)
	{
		m_pArrSkinBtn[i][SB_BG]->CallBack_PointStay += bind(&CUI_Barracks::On_PointerStay_SkinBG, this, i);
		m_pArrSkinBtn[i][SB_BG]->CallBack_PointDown += bind(&CUI_Barracks::On_PointerDown_SkinBG, this, i);
	}
}

void CUI_Barracks::Set_ClassInfoText(_uint iEventNum)
{
	wstring wstrBGText = iEventNum < 6 ? TEXT("병사") : TEXT("영웅");
	m_pClassInfo[Info_BG]->Set_FontText(wstrBGText);

	wstring wstrClassText;

	switch (iEventNum)
	{
	case 0:
		wstrClassText = TEXT("블레이드");
		break;
	case 1:
		wstrClassText = TEXT("스파이크");
		break;
	case 2:
		wstrClassText = TEXT("아치");
		break;
	case 3:
		wstrClassText = TEXT("가디언");
		break;
	case 4:
		wstrClassText = TEXT("스모크");
		break;
	case 5:
		wstrClassText = TEXT("워해머");
		break;
	case 6:
		wstrClassText = TEXT("피오나");
		break;
	case 7:
		wstrClassText = TEXT("콴다");
		break;
	case 8:
		wstrClassText = TEXT("호에트");
		break;
	case 9:
		wstrClassText = TEXT("랜서");
		break;
	}

	m_pClassInfo[Info_Class]->Set_FontText(wstrClassText);
	m_pClassInfo[Info_Class]->Set_TextureIndex(iEventNum);
}

void CUI_Barracks::Late_Enable()
{
	if (m_bIsEnable)
	{
		m_fEnableTime += fDT(0);
		if (m_fEnableTime > m_fEnableMaxTime)
		{
			m_fEnableTime = 0.f;
			m_bIsEnable = false;

			Set_ClassInfoText(m_iSelectClass);

			for (int i = 0; i < CLASS_END; ++i)
			{
				for (int j = 0; j < Port_Highlight; ++j)
					Enable_Fade(m_pArrClassPort[i][j], m_fDuration);
			}

			for (int j = 0; j < Port_Highlight; ++j)
				m_pArrClassPort[m_iSelectClass][j]->SetActive(true);

			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < Btn_End; ++j)
				{
					if (i == 0)
					{
						if (j == Btn_Lock)
							continue;
					}

					Enable_Fade(m_pArrClassBtn[i][j], m_fDuration);
				}
			}

			for (int i = 0; i < Info_End; ++i)
				Enable_Fade(m_pClassInfo[i], m_fDuration);
		}
	}
}

void CUI_Barracks::Late_SkinEnable()
{
	if (m_bIsSkinEnable)
	{
		m_fSkinEnableTime += fDT(0);
		if (m_fSkinEnableTime > m_fSkinEnableMaxTime)
		{
			m_fSkinEnableTime = 0.f;
			m_bIsSkinEnable = false;

			for (int i = 0; i < 4; ++i)
				Enable_Fade(m_pTopBtn[i], m_fDuration);

			for (int i = 0; i < Skin_End; ++i)
				Enable_Fade(m_pSkinInfo[i], m_fDuration);

			Set_SkinIdx((CLASS_TYPE)m_iSelectClass);

			m_bIsSkinWindow = true;
		}
	}
}

void CUI_Barracks::Disable_SkinWindow()
{
	if (KEY(ESC, TAP))
	{
		if (!m_bIsSkinWindow)
			return;

		Enable_Fade(m_pArrClassPort[m_iSelectClass][Port_Outline], m_fDuration);
		Enable_Fade(m_pArrClassPort[m_iSelectClass][Port_Underline], m_fDuration);

		m_pArrClassPort[m_iSelectClass][Port_Underline]->Set_Scale(100.f, 2.f);

		m_bIsSkinWindow = false;
		m_bIsEnable = true;

		m_pTopBtn[m_iCurSelectSkin]->Set_FontColor(_float4(0.5f, 0.5f, 0.5f, 1.f));
		m_iCurSelectSkin = Skin::Clothes;

		for (int i = 0; i < 4; ++i)
			Disable_Fade(m_pTopBtn[i], m_fDuration);

		for (int i = 0; i < Skin_End; ++i)
			Disable_Fade(m_pSkinInfo[i], m_fDuration);

		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < SB_End; ++j)
				Disable_Fade(m_pArrSkinBtn[i][j], m_fDuration);

			for (int j = 0; j < Skin::End; ++j)
				Disable_Fade(m_pArrSkin[i][j], m_fDuration);
		}

		CUser::Get_Instance()->SetActive_MainTopBtn(true);
		CUser::Get_Instance()->Set_TopBtnEffectPosX(-455.f);
	}
}

void CUI_Barracks::Set_SkinIdx(CLASS_TYPE eClass)
{
	if (!m_pSkinInfo[Skin_Tier]->Get_FontRender())
		m_pSkinInfo[Skin_Tier]->Set_FontRender(true);

	m_pSkinInfo[Skin_Tier]->Set_FontColor(RGB(255, 255, 255));

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < SB_End; ++j)
			Enable_Fade(m_pArrSkinBtn[i][j], m_fDuration);

		m_pArrSkin[i][m_iCurSelectSkin]->Set_TextureIndex(i);
		Enable_Fade(m_pArrSkin[i][m_iCurSelectSkin], m_fDuration);
	}

	_uint iNum = 0;
	switch (m_iSelectClass)
	{
	case WARRIOR:	iNum = 0;	break;
	case SPEAR:		iNum = 3;	break;
	case ARCHER:	iNum = 6;	break;
	case PALADIN:	iNum = 9;	break;
	case PRIEST:	iNum = 12;	break;
	case ENGINEER:	iNum = 15;	break;
	case FIONA:		iNum = 18;	break;
	case QANDA:		iNum = 19;	break;
	case HOEDT:		iNum = 20;	break;
	case LANCER:	iNum = 21;	break;
	}

	if (m_iCurSelectSkin == Skin::Clothes)
	{
		if (m_iSelectClass < FIONA)
		{
			switch (m_iSelectClass)
			{
			case WARRIOR:
				if (m_bIsUnlock_EpicWarriorClothes)
				{
					for (int i = SB_Lock; i < SB_End; ++i)
						m_pArrSkinBtn[2][i]->SetActive(false);
				}
				break;
			}

			m_pSkinInfo[Skin_Name]->Set_FontText(TEXT("훈련복"));
			m_pSkinInfo[Skin_Tier]->Set_FontText(TEXT("일반"));
		}
		else
		{
			switch (m_iSelectClass)
			{
			case FIONA:
				m_pSkinInfo[Skin_Name]->Set_FontText(TEXT("피오나 일반복"));
				m_pSkinInfo[Skin_Tier]->Set_FontText(TEXT("일반"));
				break;

			case QANDA:
				m_pSkinInfo[Skin_Name]->Set_FontText(TEXT("콴다 일반복"));
				m_pSkinInfo[Skin_Tier]->Set_FontText(TEXT("일반"));
				break;

			case HOEDT:
				m_pSkinInfo[Skin_Name]->Set_FontText(TEXT("호에트 일반복"));
				m_pSkinInfo[Skin_Tier]->Set_FontText(TEXT("일반"));
				break;

			case LANCER:
				m_pSkinInfo[Skin_Name]->Set_FontText(TEXT("랜서 일반복"));
				m_pSkinInfo[Skin_Tier]->Set_FontText(TEXT("일반"));
				break;
			}
		}

		if (m_iSelectClass < QANDA)
		{
			m_pArrSkinBtn[0][SB_Lock]->SetActive(false);
			m_pArrSkinBtn[0][SB_Blind]->SetActive(false);

			for (int i = 0; i < 3; ++i)
				m_pArrSkin[i][m_iCurSelectSkin]->Set_TextureIndex(iNum + i);

			for (int i = 1; i < 3; ++i)
			{
				m_pArrSkinBtn[i][SB_Outline]->SetActive(false);
				m_pArrSkinBtn[i][SB_Select]->SetActive(false);
			}
		}
		else
		{
			switch (m_iSelectClass)
			{
			case QANDA:		iNum = 21;	break;
			case HOEDT:		iNum = 22;	break;
			case LANCER:	iNum = 23;	break;
			}

			m_pArrSkin[2][m_iCurSelectSkin]->Set_TextureIndex(iNum);

			for (int i = 0; i < 2; ++i)
			{
				m_pArrSkin[i][m_iCurSelectSkin]->SetActive(false);

				for (int j = 0; j < SB_End; ++j)
					m_pArrSkinBtn[i][j]->SetActive(false);
			}

			m_pArrSkinBtn[2][SB_Lock]->SetActive(false);
			m_pArrSkinBtn[2][SB_Blind]->SetActive(false);
		}
	}

	if (m_iCurSelectSkin == Skin::Weapon)
	{
		if (m_iSelectClass < FIONA)
		{
			switch (m_iSelectClass)
			{
			case Client::WARRIOR:
				m_pSkinInfo[Skin_Name]->Set_FontText(TEXT("훈련용 검"));
				m_pSkinInfo[Skin_Tier]->Set_FontText(TEXT("일반"));
				break;
			case Client::SPEAR:
				m_pSkinInfo[Skin_Name]->Set_FontText(TEXT("훈련용 창"));
				m_pSkinInfo[Skin_Tier]->Set_FontText(TEXT("일반"));
				break;
			case Client::ARCHER:
				m_pSkinInfo[Skin_Name]->Set_FontText(TEXT("훈련용 활"));
				m_pSkinInfo[Skin_Tier]->Set_FontText(TEXT("일반"));
				break;
			case Client::PALADIN:
				m_pSkinInfo[Skin_Name]->Set_FontText(TEXT("훈련용 메이스"));
				m_pSkinInfo[Skin_Tier]->Set_FontText(TEXT("일반"));
				break;
			case Client::PRIEST:
				m_pSkinInfo[Skin_Name]->Set_FontText(TEXT("훈련용 지팡이"));
				m_pSkinInfo[Skin_Tier]->Set_FontText(TEXT("일반"));
				break;
			case Client::ENGINEER:
				m_pSkinInfo[Skin_Name]->Set_FontText(TEXT("훈련용 해머"));
				m_pSkinInfo[Skin_Tier]->Set_FontText(TEXT("일반"));
				break;
			}

			m_pArrSkinBtn[0][SB_Lock]->SetActive(false);
			m_pArrSkinBtn[0][SB_Blind]->SetActive(false);

			for (int i = 0; i < 3; ++i)
				m_pArrSkin[i][m_iCurSelectSkin]->Set_TextureIndex(iNum + i);

			for (int i = 1; i < 3; ++i)
			{
				m_pArrSkinBtn[i][SB_Outline]->SetActive(false);
				m_pArrSkinBtn[i][SB_Select]->SetActive(false);
			}
		}
		else
		{
			switch (m_iSelectClass)
			{
			case FIONA:
				m_pSkinInfo[Skin_Name]->Set_FontText(TEXT("피오나의 검"));
				m_pSkinInfo[Skin_Tier]->Set_FontText(TEXT("일반"));
				break;

			case QANDA:
				m_pSkinInfo[Skin_Name]->Set_FontText(TEXT("콴다의 지팡이"));
				m_pSkinInfo[Skin_Tier]->Set_FontText(TEXT("일반"));
				break;

			case HOEDT:
				m_pSkinInfo[Skin_Name]->Set_FontText(TEXT("호에트의 지팡이"));
				m_pSkinInfo[Skin_Tier]->Set_FontText(TEXT("일반"));
				break;

			case LANCER:
				m_pSkinInfo[Skin_Name]->Set_FontText(TEXT("랜서의 창"));
				m_pSkinInfo[Skin_Tier]->Set_FontText(TEXT("일반"));
				break;
			}

			m_pArrSkin[2][m_iCurSelectSkin]->Set_TextureIndex(iNum);

			for (int i = 0; i < 2; ++i)
			{
				m_pArrSkin[i][m_iCurSelectSkin]->SetActive(false);

				for (int j = 0; j < SB_End; ++j)
					m_pArrSkinBtn[i][j]->SetActive(false);
			}

			m_pArrSkinBtn[2][SB_Lock]->SetActive(false);
			m_pArrSkinBtn[2][SB_Blind]->SetActive(false);
		}
	}

	if (m_iCurSelectSkin >= Skin::Hat)
	{
		if (m_iCurSelectSkin == Skin::Hat)
		{
			if (m_bIsUnlock_RabbitHat)
			{
				for (int i = SB_Lock; i < SB_End; ++i)
					m_pArrSkinBtn[1][i]->SetActive(false);
			}

			m_pSkinInfo[Skin_Name]->Set_FontText(TEXT("미착용"));
			m_pSkinInfo[Skin_Tier]->Set_FontRender(false);
		}
		else
		{
			m_pSkinInfo[Skin_Name]->Set_FontText(TEXT("훈련용 날틀"));
			m_pSkinInfo[Skin_Tier]->Set_FontText(TEXT("일반"));
		}

		m_pArrSkinBtn[0][SB_Lock]->SetActive(false);
		m_pArrSkinBtn[0][SB_Blind]->SetActive(false);

		for (int i = 1; i < 3; ++i)
		{
			m_pArrSkinBtn[i][SB_Outline]->SetActive(false);
			m_pArrSkinBtn[i][SB_Select]->SetActive(false);
		}
	}
}
