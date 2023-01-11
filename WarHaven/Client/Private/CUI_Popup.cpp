#include "CUI_Popup.h"
#include "GameInstance.h"
#include "CUI_Object.h"
#include "Texture.h"
#include "CFader.h"
#include "CUser.h"
#include "Loading_Manager.h"

CUI_Popup::CUI_Popup()
{
}

CUI_Popup::~CUI_Popup()
{
}

HRESULT CUI_Popup::Initialize_Prototype()
{
	Create_ConquestPopup();
	Create_KillPopup();
	Create_SkinPopup();

	return S_OK;
}

HRESULT CUI_Popup::Start()
{
	__super::Start();

	for (int i = 0; i < Skin_End; ++i)
	{
		CREATE_GAMEOBJECT(m_pSKinPopup[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pSKinPopup[i]);
	}

	return S_OK;
}

void CUI_Popup::Enable_Popup(ePOPUP_TYPE ePopupType)
{
	switch (ePopupType)
	{
	case Client::CUI_Popup::eConquest:
		Enable_ConquestPopup(L"거점 점령", ePopupType);
		break;
	case Client::CUI_Popup::eKILL:
		Enable_KillPopup(L"2연속 처치", ePopupType);
		break;
	case Client::CUI_Popup::eKILL2:
		Enable_KillPopup(L"3연속 처치", ePopupType);
		break;
	case Client::CUI_Popup::eKILL3:
		Enable_KillPopup(L"4연속 처치", ePopupType);
		break;
	case Client::CUI_Popup::eKILL4:
		Enable_KillPopup(L"전장의 화신", ePopupType);
		break;
	case Client::CUI_Popup::eBURGERKING:
		Enable_ConquestPopup(L"버거킹", ePopupType);
		break;
	case Client::CUI_Popup::eHEADHUNTER:
		Enable_ConquestPopup(L"헤드 헌터", ePopupType);
		break;
	case Client::CUI_Popup::eKILLELITE:
		Enable_ConquestPopup(L"영웅 처치", ePopupType);
		break;
	case Client::CUI_Popup::eCNT:
		break;
	default:
		break;
	}

	for (int i = 0; i < CP_End; ++i)
	{
		m_pConquestPopup[i]->Set_FadeDesc(m_fFadeTime, m_fFadeTime, m_fEnableTime, true);
	}
}

void CUI_Popup::Enable_ConquestPopup(wstring Text, _uint iIconIndex)
{
	_bool bIsFontRender = m_pConquestPopup[CP_BG]->Get_FontRender();
	if (bIsFontRender)
		m_pConquestPopup[CP_BG]->Set_FontText(Text);

	GET_COMPONENT_FROM(m_pConquestPopup[CP_ICON], CTexture)->Set_CurTextureIndex(iIconIndex);

	for (int i = 0; i < CP_End; ++i)
	{
		Enable_Fade(m_pConquestPopup[i], m_fFadeTime);
	}
}

void CUI_Popup::Enable_KillPopup(wstring Text, _uint iIconIndex)
{
	_bool bIsFontRender = m_pKillPopup[Kill_Icon]->Get_FontRender();
	if (bIsFontRender)
		m_pKillPopup[Kill_Icon]->Set_FontText(Text);

	GET_COMPONENT_FROM(m_pKillPopup[Kill_Icon], CTexture)->Set_CurTextureIndex(iIconIndex);

	for (int i = 0; i < Kill_End; ++i)
	{
		Enable_Fade(m_pKillPopup[i], m_fFadeTime);
	}
}

void CUI_Popup::Enable_SkinPopup(_uint iSkin)
{
	m_iSkinIdx = iSkin;
	switch (m_iSkinIdx)
	{
	case 0:
		m_pSKinPopup[Skin_Item]->Set_TextureIndex(iSkin);
		m_pSKinPopup[Skin_Text0]->Set_FontText(TEXT("축하합니다."));
		m_pSKinPopup[Skin_Text1]->Set_FontText(TEXT("훈련장을 완료하여 토끼탈을 드립니다."));
		break;

	case 1:
		m_pSKinPopup[Skin_Item]->Set_TextureIndex(iSkin);
		m_pSKinPopup[Skin_Text0]->Set_FontText(TEXT("축하합니다."));
		m_pSKinPopup[Skin_Text1]->Set_FontText(TEXT("점령전을 완료하여 영웅 갑옷을 드립니다."));
		break;
	}

	SetActive_SkinPopup(true);

	m_bEnableSkinPopup = true;
}

void CUI_Popup::My_Tick()
{
	__super::My_Tick();

	if (m_bEnableSkinPopup)
	{
		m_fAccTime += fDT(0);
		if (m_fAccTime > 1.f)
		{
			m_fAccTime = 0.f;

			m_bEnableSkinPopup = false;
			m_bFadePopup = true;
		}
	}

	if (m_bFadePopup)
	{
		if (KEY(SPACE, TAP))
		{
			m_bFadePopup = false;

			switch (m_iSkinIdx)
			{
			case 0:
				CUser::Get_Instance()->Unlock_RabbitHat();
				break;

			case 1:
				CUser::Get_Instance()->Unlock_EpicWarriorClothes();
				CLoading_Manager::Get_Instance()->Reserve_Load_Level(LEVEL_MAINMENU);
				break;
			}
			SetActive_SkinPopup(false);
			DISABLE_GAMEOBJECT(this);
		}
	}
}

void CUI_Popup::OnEnable()
{
	__super::OnEnable();
}

void CUI_Popup::OnDisable()
{
	__super::OnDisable();
}

void CUI_Popup::Create_ConquestPopup()
{
	for (int i = 0; i < CP_End; ++i)
	{
		m_pConquestPopup[i] = CUI_Object::Create();

		m_pConquestPopup[i]->Set_PosY(200.f);

		switch (i)
		{
		case CP_Line:

			m_pConquestPopup[i]->Set_UIShaderFlag(SH_UI_HARDBLOOM);

			m_pConquestPopup[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Popup/T_PopupLine.png"));

			m_pConquestPopup[i]->Set_Scale(300.f, 51.f);
			m_pConquestPopup[i]->Set_Sort(0.55f);

			break;

		case CP_Blur:

			m_pConquestPopup[i]->Set_UIShaderFlag(SH_UI_HARDBLOOM);

			m_pConquestPopup[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Popup/T_PopupBlur.dds"));

			m_pConquestPopup[i]->Set_Color(_float4(1.f, 0.9f, 0.8f, 1.f));

			m_pConquestPopup[i]->Set_Scale(160.f);
			m_pConquestPopup[i]->Set_Sort(0.55f);

			break;

		case CP_BG:

			m_pConquestPopup[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Popup/T_PopupIconbox.dds"));

			m_pConquestPopup[i]->Set_Scale(80.f);
			m_pConquestPopup[i]->Set_Sort(0.54f);

			m_pConquestPopup[i]->Set_FontRender(true);
			m_pConquestPopup[i]->Set_FontStyle(true);
			m_pConquestPopup[i]->Set_FontCenter(true);
			m_pConquestPopup[i]->Set_FontOffset(3.f, 75.f);
			m_pConquestPopup[i]->Set_FontScale(0.3f);

			break;

		case CP_ICON:

			m_pConquestPopup[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Popup/T_GlowFoothold.dds"));
			GET_COMPONENT_FROM(m_pConquestPopup[i], CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Popup/Kill_0.png"));
			GET_COMPONENT_FROM(m_pConquestPopup[i], CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Popup/Kill_2.png"));
			GET_COMPONENT_FROM(m_pConquestPopup[i], CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Popup/Kill_3.png"));
			GET_COMPONENT_FROM(m_pConquestPopup[i], CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Popup/Kill_4.png"));
			GET_COMPONENT_FROM(m_pConquestPopup[i], CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Popup/T_GlowBlock.dds"));
			GET_COMPONENT_FROM(m_pConquestPopup[i], CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Popup/T_GlowHeadshot.dds"));
			GET_COMPONENT_FROM(m_pConquestPopup[i], CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Popup/T_GlowKill.dds"));
			GET_COMPONENT_FROM(m_pConquestPopup[i], CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Popup/T_GlowKIllElite.dds"));
			GET_COMPONENT_FROM(m_pConquestPopup[i], CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Popup/T_GlowRevenge.dds"));
			GET_COMPONENT_FROM(m_pConquestPopup[i], CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Popup/T_HighValueTargetPlayer.dds"));

			m_pConquestPopup[i]->Set_Scale(60.f);
			m_pConquestPopup[i]->Set_Sort(0.53f);

			m_pConquestPopup[i]->Set_FontRender(true);
			m_pConquestPopup[i]->Set_FontStyle(true);
			m_pConquestPopup[i]->Set_FontCenter(true);
			m_pConquestPopup[i]->Set_FontOffset(3.f, 70.f);
			m_pConquestPopup[i]->Set_FontScale(0.3f);

			break;
		}

		CREATE_GAMEOBJECT(m_pConquestPopup[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pConquestPopup[i]);
	}
}

void CUI_Popup::Create_KillPopup()
{
	for (int i = 0; i < Kill_End; ++i)
	{
		m_pKillPopup[i] = CUI_Object::Create();

		m_pKillPopup[i]->Set_FadeDesc(m_fFadeTime, m_fFadeTime, m_fEnableTime, true);

		m_pKillPopup[i]->Set_UIShaderFlag(SH_UI_HARDBLOOM);

		switch (i)
		{
		case Kill_Icon:

			m_pKillPopup[i]->Set_Sort(0.49f);

			GET_COMPONENT_FROM(m_pKillPopup[i], CTexture)->Remove_Texture(0);
			GET_COMPONENT_FROM(m_pKillPopup[i], CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Popup/Kill_0.png"));
			GET_COMPONENT_FROM(m_pKillPopup[i], CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Popup/Kill_2.png"));
			GET_COMPONENT_FROM(m_pKillPopup[i], CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Popup/Kill_3.png"));
			GET_COMPONENT_FROM(m_pKillPopup[i], CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Popup/Kill_4.png"));

			m_pKillPopup[i]->Set_PosY(155.f);
			m_pKillPopup[i]->Set_Scale(160.f, 116.f);

			m_pKillPopup[i]->Set_FontRender(true);
			m_pKillPopup[i]->Set_FontStyle(true);
			m_pKillPopup[i]->Set_FontCenter(true);
			m_pKillPopup[i]->Set_FontOffset(5.f, 70.f);
			m_pKillPopup[i]->Set_FontScale(0.3f);

			break;

		case Kill_Line:

			m_pKillPopup[i]->Set_Sort(0.5f);

			m_pKillPopup[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Popup/T_PopupLine.png"));

			m_pKillPopup[i]->Set_PosY(111.f);
			m_pKillPopup[i]->Set_Scale(300.f, 100.f);

			break;
		}

		CREATE_GAMEOBJECT(m_pKillPopup[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pKillPopup[i]);
	}
}

void CUI_Popup::Create_SkinPopup()
{
	for (int i = 0; i < Skin_End; ++i)
	{
		m_pSKinPopup[i] = CUI_Object::Create();

		m_pSKinPopup[i]->Set_FadeDesc(m_fFadeTime, m_fFadeTime);

		switch (i)
		{
		case Skin_PopupBG:
			m_pSKinPopup[i]->Set_Sort(0.14f);
			m_pSKinPopup[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Popup/T_DecoBox.png"));
			m_pSKinPopup[i]->Set_IsSlice(true);
			m_pSKinPopup[i]->Set_SliceRatio(_float4(0.5f, 0.5f, 0.5f, 0.5f));
			m_pSKinPopup[i]->Set_TextureSzie(_float2(50.f, 50.f));
			m_pSKinPopup[i]->Set_Scale(550.f, 300.f);
			break;
		case Skin_Out:
			m_pSKinPopup[i]->Set_Sort(0.13f);
			m_pSKinPopup[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/KDA/T_MinimapFrame.dds"));
			m_pSKinPopup[i]->Set_PosY(65.f);
			m_pSKinPopup[i]->Set_Scale(130.f);
			m_pSKinPopup[i]->Set_Color(RGB(0.5f, 0.5f, 0.5f));
			break;
		case Skin_BG:
			m_pSKinPopup[i]->Set_Sort(0.13f);
			m_pSKinPopup[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/Barracks/Skin/T_ItemBG4.dds"));
			m_pSKinPopup[i]->Set_PosY(65.f);
			m_pSKinPopup[i]->Set_Scale(100.f);
			break;
		case Skin_Item:
			m_pSKinPopup[i]->Set_Sort(0.12f);
			m_pSKinPopup[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/Barracks/Skin/Hat/T_SkinHatCommon1002.dds"));
			m_pSKinPopup[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/Barracks/Skin/Clothes/Clothes02.dds"));

			m_pSKinPopup[i]->Set_PosY(65.f);
			m_pSKinPopup[i]->Set_Scale(100.f);
			break;
		case Skin_Line0:
			m_pSKinPopup[i]->Set_Sort(0.13f);
			m_pSKinPopup[i]->Set_Scale(435.f, 1.f);
			m_pSKinPopup[i]->Set_PosY(-20.f);
			break;
		case Skin_Line1:
			m_pSKinPopup[i]->Set_Sort(0.13f);
			m_pSKinPopup[i]->Set_Scale(435.f, 1.f);
			m_pSKinPopup[i]->Set_PosY(-100.f);
			break;
		case Skin_Deco:
			m_pSKinPopup[i]->Set_Sort(0.13f);
			m_pSKinPopup[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Popup/T_DecoLam03.png"));
			m_pSKinPopup[i]->Set_Scale(70.f, 10.f);
			m_pSKinPopup[i]->Set_PosY(-25.f);
			break;
		case Skin_Text0:
			m_pSKinPopup[i]->Set_Sort(0.13f);
			m_pSKinPopup[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Alpha0.png"));
			m_pSKinPopup[i]->Set_PosY(-50.f);
			m_pSKinPopup[i]->Set_FontRender(true);
			m_pSKinPopup[i]->Set_FontCenter(true);
			m_pSKinPopup[i]->Set_FontOffset(3.f, 3.f);

			m_pSKinPopup[i]->Set_FontScale(0.3f);
			break;
		case Skin_Text1:
			m_pSKinPopup[i]->Set_Sort(0.13f);
			m_pSKinPopup[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Alpha0.png"));
			m_pSKinPopup[i]->Set_PosY(-80.f);
			m_pSKinPopup[i]->Set_FontRender(true);
			m_pSKinPopup[i]->Set_FontCenter(true);
			m_pSKinPopup[i]->Set_FontOffset(3.f, 3.f);
			m_pSKinPopup[i]->Set_FontScale(0.3f);
			break;
		case Skin_Esc:
			m_pSKinPopup[i]->Set_Sort(0.13f);
			m_pSKinPopup[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/KeyIcon/Keyboard/White/T_WhiteSpaceKeyIcon.png"));
			m_pSKinPopup[i]->Set_Pos(-41.f, -125.f);
			m_pSKinPopup[i]->Set_Scale(24.f);
			m_pSKinPopup[i]->Set_FontRender(true);
			m_pSKinPopup[i]->Set_FontStyle(true);
			m_pSKinPopup[i]->Set_FontOffset(15.f, -15.f);
			m_pSKinPopup[i]->Set_FontScale(0.3f);
			m_pSKinPopup[i]->Set_FontText(TEXT("창 닫기"));
			break;
		}
	}
}

void CUI_Popup::SetActive_SkinPopup(_bool value)
{
	if (value == true)
	{
		for (int i = 0; i < Skin_End; ++i)
			Enable_Fade(m_pSKinPopup[i], m_fFadeTime);
	}
	else
	{
		for (int i = 0; i < Skin_End; ++i)
			Disable_Fade(m_pSKinPopup[i], m_fFadeTime);
	}
}
