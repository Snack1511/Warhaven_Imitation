#include "CUI_Barracks.h"
#include "CUI_Object.h"
#include "Texture.h"
#include "GameInstance.h"

CUI_Barracks::CUI_Barracks()
{
}

CUI_Barracks::~CUI_Barracks()
{
}

HRESULT CUI_Barracks::Initialize_Prototype()
{
	Create_CharacterPort();

	return S_OK;
}

HRESULT CUI_Barracks::Start()
{
	__super::Start();

	Init_CharacterPort();

	Bind_Btn();

	return S_OK;
}

void CUI_Barracks::On_PointerEnter_Port(const _uint& iEventNum)
{
	Enable_Fade(m_pArrCharacterPort[iEventNum][Port_Highlight], 0.3f);
}

void CUI_Barracks::On_PointerExit_Port(const _uint& iEventNum)
{
	Disable_Fade(m_pArrCharacterPort[iEventNum][Port_Highlight], 0.3f);
}

void CUI_Barracks::On_PointerDown_Port(const _uint& iEventNum)
{
	m_iPrvEventNum = m_iCurEventNum;
	m_iCurEventNum = iEventNum;

	if (m_iPrvEventNum == iEventNum)
		return;

	for (int i = 0; i < Port_Underline; ++i)
	{
		_float4 vPrvPos = m_pArrCharacterPort[m_iPrvEventNum][i]->Get_Pos();
		vPrvPos.y -= 10.f;
		m_pArrCharacterPort[m_iPrvEventNum][i]->DoMove(vPrvPos, m_fDuration, 0);

		_float4 vCurPos = m_pArrCharacterPort[m_iCurEventNum][i]->Get_Pos();
		vCurPos.y += 10.f;
		m_pArrCharacterPort[m_iCurEventNum][i]->DoMove(vCurPos, m_fDuration, 0);
	}

	_float fOriginScaleX = 2.f;
	_float fTargetScaleX = 100.f;

	Disable_Fade(m_pArrCharacterPort[m_iPrvEventNum][Port_Underline], m_fDuration);
	m_pArrCharacterPort[m_iPrvEventNum][Port_Underline]->Lerp_ScaleX(fTargetScaleX, fOriginScaleX, m_fDuration);

	m_pArrCharacterPort[m_iCurEventNum][Port_Underline]->Lerp_ScaleX(fOriginScaleX, fTargetScaleX, m_fDuration);
	Enable_Fade(m_pArrCharacterPort[m_iCurEventNum][Port_Underline], m_fDuration);

	Disable_Fade(m_pArrCharacterPort[m_iPrvEventNum][Port_Outline], 0.3f);
	Enable_Fade(m_pArrCharacterPort[m_iCurEventNum][Port_Outline], 0.3f);
}

void CUI_Barracks::My_Tick()
{
	__super::My_Tick();
}

void CUI_Barracks::My_LateTick()
{
	__super::My_LateTick();
}

void CUI_Barracks::OnEnable()
{
	__super::OnEnable();

	for (int i = 0; i < CLASS_END; ++i)
	{
		for (int j = 0; j < Port_Highlight; ++j)
		{
			Enable_Fade(m_pArrCharacterPort[i][j], m_fDuration);
		}
	}
}

void CUI_Barracks::OnDisable()
{
	__super::OnDisable();

	for (auto& iter : m_pUIList)
		Disable_Fade(iter, 0.3f);
}

void CUI_Barracks::Create_CharacterPort()
{
	for (int i = 0; i < Port_End; ++i)
	{
		m_pCharacterPort[i] = CUI_Object::Create();

		m_pCharacterPort[i]->Set_FadeDesc(0.3f);

		m_pCharacterPort[i]->Set_PosY(-250.f);
		m_pCharacterPort[i]->Set_Scale(100.f, 140.f);
		m_pCharacterPort[i]->Set_Sort(0.5f);

		switch (i)
		{
		case Port_BG:
			m_pCharacterPort[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/CharacterWindow/BG.dds"));
			m_pCharacterPort[i]->Set_MouseTarget(true);
			break;

		case Port_Char:
			GET_COMPONENT_FROM(m_pCharacterPort[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pCharacterPort[i], "/CharacterWindow", "Portrait");

			m_pCharacterPort[i]->Set_Sort(0.49f);
			break;

		case Port_Highlight:
			m_pCharacterPort[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/Barracks/T_GradientSmall3.dds"));
			m_pCharacterPort[i]->Set_Color(_float4(1.f, 1.f, 1.f, 0.3f));
			m_pCharacterPort[i]->Set_Sort(0.47f);
			m_pCharacterPort[i]->Set_IsSlice(true);
			m_pCharacterPort[i]->Set_SliceRatio(_float4(0.f, 0.f, 0.f, 0.9f));
			break;

		case Port_Class:
			GET_COMPONENT_FROM(m_pCharacterPort[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pCharacterPort[i], "/CharacterWindow", "Icon");

			m_pCharacterPort[i]->Set_PosY(-195.f);
			m_pCharacterPort[i]->Set_Scale(20.f);
			m_pCharacterPort[i]->Set_Sort(0.48f);
			break;

		case Port_Outline:
			m_pCharacterPort[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/Barracks/T_2pxStrokeBox.png"));
			m_pCharacterPort[i]->Set_Color(m_vColorGold);
			m_pCharacterPort[i]->Set_Scale(102.f, 142.f);
			m_pCharacterPort[i]->Set_Sort(0.48f);
			m_pCharacterPort[i]->Set_IsSlice(true);
			m_pCharacterPort[i]->Set_TextureSzie(_float2(32.f, 32.f));
			m_pCharacterPort[i]->Set_SliceRatio(_float4(0.5f, 0.5f, 0.5f, 0.5f));
			break;

		case Port_Underline:
			m_pCharacterPort[i]->Set_Color(m_vColorGold);
			m_pCharacterPort[i]->Set_PosY(-318.f);
			m_pCharacterPort[i]->Set_Scale(2.f);
			break;
		}

		for (int j = 0; j < CLASS_END; ++j)
		{
			m_pArrCharacterPort[j][i] = m_pCharacterPort[i]->Clone();
		}
	}
}

void CUI_Barracks::Init_CharacterPort()
{
	for (int i = 0; i < Port_End; ++i)
	{
		CREATE_GAMEOBJECT(m_pCharacterPort[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pCharacterPort[i]);

		for (int j = 0; j < CLASS_END; ++j)
		{
			CREATE_GAMEOBJECT(m_pArrCharacterPort[j][i], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pArrCharacterPort[j][i]);

			_float fPosX = -540.f + (j * 120.f);
			m_pArrCharacterPort[j][i]->Set_PosX(fPosX);

			switch (i)
			{
			case Port_BG:
				m_pUIList.push_back(m_pArrCharacterPort[j][i]);
				break;

			case Port_Char:
				m_pUIList.push_back(m_pArrCharacterPort[j][i]);
				break;

			case Port_Class:
			{
				m_pUIList.push_back(m_pArrCharacterPort[j][i]);

				_float fClassPosX = fPosX + 35.f;
				m_pArrCharacterPort[j][Port_Class]->Set_PosX(fClassPosX);
			}
			break;

			case Port_Outline:
				m_pUIList.push_back(m_pArrCharacterPort[j][i]);
				break;

			case Port_Underline:

				cout << j << " : " << m_pArrCharacterPort[j][i]->Get_PosX() << endl;

				m_pUIList.push_back(m_pArrCharacterPort[j][i]);

				break;
			}
		}
	}

	for (int i = 0; i < CLASS_END; ++i)
	{
		m_pArrCharacterPort[i][Port_Char]->Set_TextureIndex(i);
		m_pArrCharacterPort[i][Port_Class]->Set_TextureIndex(i);
	}
}

void CUI_Barracks::Bind_Btn()
{
	for (int i = 0; i < CLASS_END; ++i)
	{
		m_pArrCharacterPort[i][Port_BG]->CallBack_PointEnter += bind(&CUI_Barracks::On_PointerEnter_Port, this, i);
		m_pArrCharacterPort[i][Port_BG]->CallBack_PointExit += bind(&CUI_Barracks::On_PointerExit_Port, this, i);
		m_pArrCharacterPort[i][Port_BG]->CallBack_PointDown += bind(&CUI_Barracks::On_PointerDown_Port, this, i);
	}
}
