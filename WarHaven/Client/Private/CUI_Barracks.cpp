#include "CUI_Barracks.h"
#include "CUI_Object.h"
#include "Texture.h"
#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "CUser.h"
#include "CUI_Cursor.h"
#include "CUI_Main.h"

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
	Enable_Fade(m_pArrClassPort[iEventNum][Port_Highlight], 0.3f);
}

void CUI_Barracks::On_PointerExit_Port(const _uint& iEventNum)
{
	Disable_Fade(m_pArrClassPort[iEventNum][Port_Highlight], 0.3f);
}

void CUI_Barracks::On_PointerDown_Port(const _uint& iEventNum)
{
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
}

void CUI_Barracks::On_PointerEnter_Btn(const _uint& iEventNum)
{
	if (iEventNum > 0)
		return;

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

	for (auto& iter : m_pUIList)
		Disable_Fade(iter, m_fDuration);

	m_pTopBtn[0]->Set_FontColor(m_vColorWhite);

	m_bIsSkinEnable = true;
}

void CUI_Barracks::On_PointerDown_TopBtn(const _uint& iEventNum)
{
	m_iPrvSelectSkin = m_iCurSelectSkin;
	m_iCurSelectSkin = iEventNum;

	if (m_iPrvSelectSkin == iEventNum)
		return;

	m_pTopBtn[m_iPrvSelectSkin]->Set_IsClick(false);
	m_pTopBtn[m_iPrvSelectSkin]->Set_FontColor(_float4(0.5f, 0.5f, 0.5f, 1.f));

	m_pTopBtn[m_iCurSelectSkin]->Set_IsClick(true);
	m_pTopBtn[m_iCurSelectSkin]->Set_FontColor(m_vColorWhite);

	m_bTickDisable = true;
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
		m_pTopBtn[i]->Set_Pos(fPosX, 200.f);
		m_pTopBtn[i]->Set_Scale(75.f, 35.f);
		m_pTopBtn[i]->Set_Sort(0.5f);

		m_pTopBtn[i]->Set_MouseTarget(true);

		m_pTopBtn[i]->Set_FontRender(true);
		m_pTopBtn[i]->Set_FontStyle(true);
		m_pTopBtn[i]->Set_FontCenter(true);
		m_pTopBtn[i]->Set_FontScale(0.4f);
		m_pTopBtn[i]->Set_FontColor(_float4(0.5f, 0.5f, 0.5f, 1.f));

		switch (i)
		{
		case 0:
			m_pTopBtn[i]->Set_FontText(TEXT("�ǻ�"));
			break;

		case 1:
			m_pTopBtn[i]->Set_FontText(TEXT("����"));
			break;

		case 2:
			m_pTopBtn[i]->Set_FontText(TEXT("����"));
			break;

		case 3:
			m_pTopBtn[i]->Set_FontText(TEXT("�۶��̴�"));
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
		m_pSkinInfo[i]->Set_FontText(TEXT("�׽�Ʈ"));
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
	m_pArrClassBtn[0][Btn_Text]->Set_FontText(TEXT("��Ų"));
	m_pArrClassBtn[0][Btn_LockText]->Set_FontText(TEXT("�ǻ󡤹��⡤���ڡ��۶��̴�"));

	m_pArrClassBtn[1][Btn_Text]->Set_PosX(-540.f);
	m_pArrClassBtn[1][Btn_Text]->Set_FontText(TEXT("Ư��"));
	m_pArrClassBtn[1][Btn_LockText]->Set_FontText(TEXT("���"));

	m_pArrClassBtn[2][Btn_Text]->Set_PosX(-540.f);
	m_pArrClassBtn[2][Btn_Text]->Set_FontText(TEXT("�ǻ����"));
	m_pArrClassBtn[2][Btn_LockText]->Set_FontText(TEXT("���"));
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
		m_pTopBtn[i]->CallBack_PointDown += bind(&CUI_Barracks::On_PointerDown_TopBtn, this, i);
	}
}

void CUI_Barracks::Set_ClassInfoText(_uint iEventNum)
{
	wstring wstrBGText = iEventNum < 6 ? TEXT("����") : TEXT("����");
	m_pClassInfo[Info_BG]->Set_FontText(wstrBGText);

	wstring wstrClassText;

	switch (iEventNum)
	{
	case 0:
		wstrClassText = TEXT("���̵�");
		break;
	case 1:
		wstrClassText = TEXT("������ũ");
		break;
	case 2:
		wstrClassText = TEXT("��ġ");
		break;
	case 3:
		wstrClassText = TEXT("�����");
		break;
	case 4:
		wstrClassText = TEXT("����ũ");
		break;
	case 5:
		wstrClassText = TEXT("���ظ�");
		break;
	case 6:
		wstrClassText = TEXT("�ǿ���");
		break;
	case 7:
		wstrClassText = TEXT("���");
		break;
	case 8:
		wstrClassText = TEXT("ȣ��Ʈ");
		break;
	case 9:
		wstrClassText = TEXT("����");
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

				// m_pArrClassPort[m_iSelectClass][j]
			}

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
	}
}

void CUI_Barracks::Set_SkinIdx(CLASS_TYPE eClass)
{
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
		m_pArrSkinBtn[0][SB_Lock]->SetActive(false);
		m_pArrSkinBtn[0][SB_Blind]->SetActive(false);

		for (int i = 1; i < 3; ++i)
		{
			m_pArrSkinBtn[i][SB_Outline]->SetActive(false);
			m_pArrSkinBtn[i][SB_Select]->SetActive(false);
		}
	}
}
