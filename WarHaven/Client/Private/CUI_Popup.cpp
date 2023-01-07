#include "CUI_Popup.h"
#include "GameInstance.h"
#include "CUI_Object.h"
#include "Texture.h"
#include "CFader.h"
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

	return S_OK;
}

HRESULT CUI_Popup::Start()
{
	__super::Start();

	return S_OK;
}

void CUI_Popup::Enable_Popup(ePOPUP_TYPE ePopupType)
{
	switch (ePopupType)
	{
	case Client::CUI_Popup::eConquest:
		Enable_ConquestPopup(L"���� ����", ePopupType);
		break;
	case Client::CUI_Popup::eKILL:
		Enable_KillPopup(L"2���� óġ", ePopupType);
		break;
	case Client::CUI_Popup::eKILL2:
		Enable_KillPopup(L"3���� óġ", ePopupType);
		break;
	case Client::CUI_Popup::eKILL3:
		Enable_KillPopup(L"4���� óġ", ePopupType);
		break;
	case Client::CUI_Popup::eKILL4:
		Enable_KillPopup(L"������ ȭ��", ePopupType);
		break;
	case Client::CUI_Popup::eBURGERKING:
		Enable_ConquestPopup(L"����ŷ", ePopupType);
		break;
	case Client::CUI_Popup::eHEADHUNTER:
		Enable_ConquestPopup(L"��� ����", ePopupType);
		break;
	case Client::CUI_Popup::eKILLELITE:
		Enable_ConquestPopup(L"���� óġ", ePopupType);
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

		switch (i)
		{
		case Skin_PopupBG:
			// ���ν����̽� ������ 50 ���� 0.5
			// ũ�� 550 300
			break;
		case Skin_Out:
			// Y 65
			// ������ 130
			// �� ���
			break;
		case Skin_BG:
			// Y 65
			// ������ 100
			break;
		case Skin_Item:
			// Y 65
			// ������ 100
			break;
		case Skin_Line0:
			// ������ 435 1
			// Y 20
			break;
		case Skin_Line1:
			// ������ 435 1
			// Y -100
			break;
		case Skin_Deco:
			// ������ 70 10
			// Y -25
			break;
		case Skin_Text0:
			// ��ġ -50
			// �߾�����
			// ��Ʈ ������ 3 3
			// ��Ʈ ũ�� 0.3
			break;
		case Skin_Text1:
			// ��ġ -80
			// �߾�����
			// ��Ʈ ������ 3 3
			// ��Ʈ ũ�� 0.3
			break;
		case Skin_Esc:
			// ��ġ -41 -125
			// ũ�� 24
			// ��Ʈ ������ 15 -15
			// ��Ʈ ũ�� 0.3
			break;
		}
	}
}
