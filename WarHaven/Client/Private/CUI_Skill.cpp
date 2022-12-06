#include "CUI_Skill.h"
#include "GameInstance.h"
#include "CUI_Object.h"
#include "Transform.h"
#include "Texture.h"
#include "Functor.h"
#include "CShader.h"
#include "CUI_Renderer.h"
#include "Easing_Utillity.h"

CUI_Skill::CUI_Skill()
{
}

CUI_Skill::~CUI_Skill()
{
}

HRESULT CUI_Skill::Initialize_Prototype()
{
	Create_SkillUI();
	Create_Outline();

	return S_OK;
}

HRESULT CUI_Skill::Initialize()
{
	return S_OK;
}

HRESULT CUI_Skill::Start()
{
	__super::Start();

	Set_SkillUI(WARRIOR);

	return S_OK;
}

void CUI_Skill::Set_SkillUI(_uint iClass)
{
	m_iPrvClass = m_iCurClass;
	m_iCurClass = iClass;

	/*if (m_iPrvClass == iClass)
		return;*/

	switch (iClass)
	{
	case WARRIOR:
		m_iIndex = 2;
		m_iSkillNum = 0;
		break;

	case ENGINEER:
		m_iIndex = 3;
		m_iSkillNum = 14;
		break;

	case FIONA:
		m_iIndex = 3;
		m_iSkillNum = 17;
		break;
	}
}

void CUI_Skill::SetActive_SkillUI(_bool value)
{
	for (int i = 0; i < SU_End; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			m_pArrSkillUI[i][j]->SetActive(value);
		}
	}
}

void CUI_Skill::SetActive_Outline(_bool value)
{
	for (int i = 0; i < Outline_End; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			m_pArrOutline[i][j]->SetActive(value);
		}
	}
}

void CUI_Skill::Enable_AllSkillUI()
{
	// 일단 모든 UI 비활성화
	SetActive_SkillUI(false);
	SetActive_Outline(false);

	// 해당 클래스의 할당된 인덱스 만큼의 UI 활성화
	for (int i = 0; i < SU_End; ++i)
	{
		for (int j = 0; j < m_iIndex; ++j)
		{
			GET_COMPONENT_FROM(m_pArrSkillUI[SU_Icon][j], CTexture)->Set_CurTextureIndex(m_iSkillNum + j);

			m_pArrSkillUI[i][j]->SetActive(true);
		}
	}

	for (int i = 0; i < Outline_End; ++i)
	{
		for (int j = 0; j < m_iIndex; ++j)
		{
			m_pArrOutline[i][j]->SetActive(true);
		}
	}
}

void CUI_Skill::Create_SkillUI()
{
	for (int i = 0; i < SU_End; ++i)
	{
		m_pSkillUI[i] = CUI_Object::Create();

		m_pSkillUI[i]->Set_PosY(-315.f);

		if (i == SU_BG)
		{
			m_pSkillUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/Skill/BG1.dds"));
			m_pSkillUI[i]->Set_Scale(50.f);
			m_pSkillUI[i]->Set_Sort(0.5f);
		}
		else if (i == SU_Icon)
		{
			GET_COMPONENT_FROM(m_pSkillUI[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pSkillUI[i], "/HUD/Skill/Icon", "Skill");

			m_pSkillUI[i]->Set_Scale(28.f);
			m_pSkillUI[i]->Set_Sort(0.49f);
		}
		else if (i == SU_Key)
		{
			GET_COMPONENT_FROM(m_pSkillUI[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pSkillUI[i], "/KeyIcon/Keyboard/Black", "Key");

			m_pSkillUI[i]->Set_PosY(-275.f);

			m_pSkillUI[i]->Set_Scale(15.f);
			m_pSkillUI[i]->Set_Sort(0.5f);
		}

		CREATE_GAMEOBJECT(m_pSkillUI[i], GROUP_UI);
		DELETE_GAMEOBJECT(m_pSkillUI[i]);

		for (int j = 0; j < 3; ++j)
		{
			m_pArrSkillUI[i][j] = m_pSkillUI[i]->Clone();

			_float fPosX = 480.f - (55.f * j);
			m_pArrSkillUI[i][j]->Set_PosX(fPosX);

			CREATE_GAMEOBJECT(m_pArrSkillUI[i][j], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pArrSkillUI[i][j]);
		}
	}
}

void CUI_Skill::Create_Outline()
{
	for (int i = 0; i < Outline_End; ++i)
	{
		m_pOutline[i] = CUI_Object::Create();

		m_pOutline[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/Skill/Outline.png"));

		m_pOutline[i]->Set_PosY(-315.f);
		m_pOutline[i]->Set_Scale(40.f);
		m_pOutline[i]->Set_Sort(0.49f);

		CREATE_GAMEOBJECT(m_pOutline[i], GROUP_UI);
		DELETE_GAMEOBJECT(m_pOutline[i]);

		for (int j = 0; j < 3; ++j)
		{
			m_pArrOutline[i][j] = m_pOutline[i]->Clone();

			_float fPosX = 480.f - (55.f * j);
			m_pArrOutline[i][j]->Set_PosX(fPosX);

			CREATE_GAMEOBJECT(m_pArrOutline[i][j], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pArrOutline[i][j]);
		}
	}
}

void CUI_Skill::OnEnable()
{
	__super::OnEnable();

	Enable_AllSkillUI();
}

void CUI_Skill::OnDisable()
{
	__super::OnDisable();

	SetActive_SkillUI(false);
	SetActive_Outline(false);
}

void CUI_Skill::My_Tick()
{
	__super::My_Tick();

	/*Enable_Outline();

	for (int i = 0; i < SkillEnd; ++i)
	{
		_tchar  szSkill[MAX_STR] = {};
		swprintf_s(szSkill, TEXT("%.1f"), m_fCoolTime[i]);
		m_pSkillCoolTextArr[i]->Set_FontText(szSkill);

		if (m_fCoolTime[i] <= 0.05f)
		{
			DISABLE_GAMEOBJECT(m_pSkillCoolTextArr[i]);
			DISABLE_GAMEOBJECT(m_pSkillCoolBGArr[i]);
		}
	}*/
}


void CUI_Skill::Set_Shader_SkillGauge1(CShader* pShader, const char* pConstName)
{
	//pShader->Set_RawValue("g_fValue", &m_fSkillGauge[Skill1], sizeof(_float));
}

void CUI_Skill::Set_Shader_SkillGauge2(CShader* pShader, const char* pConstName)
{
	//pShader->Set_RawValue("g_fValue", &m_fSkillGauge[Skill2], sizeof(_float));
}

void CUI_Skill::Set_Shader_SkillGauge3(CShader* pShader, const char* pConstName)
{
	//pShader->Set_RawValue("g_fValue", &m_fSkillGauge[Skill3], sizeof(_float));
}

void CUI_Skill::Set_SkillHUD(_uint iIndex)
{
	m_iPrvSkill = m_iCurSkill;
	m_iCurSkill = iIndex;

	m_bAbleOutline = true;

	switch (iIndex)
	{
	case WARRIOR:

		Set_SkillBtn(2);
		Set_SkillBtn(1, 44, 25, false);
		Set_SkillBtn(0, 9, 24, false);

		break;

	case SPEAR:

		Set_SkillBtn(3);
		Set_SkillBtn(2, 44, 23, false);
		Set_SkillBtn(1, 9, 22, false);
		Set_SkillBtn(0, 99, 21, false);

		break;

	case ARCHER:

		Set_SkillBtn(3);
		Set_SkillBtn(2, 44, 2, false);
		Set_SkillBtn(1, 9, 1, false);
		Set_SkillBtn(0, 74, 0, false);

		break;

	case PALADIN:

		Set_SkillBtn(3);
		Set_SkillBtn(2, 44, 15, false);
		Set_SkillBtn(1, 9, 14, false);
		Set_SkillBtn(0, 76, 13, false);

		break;

	case PRIEST:

		Set_SkillBtn(3);
		Set_SkillBtn(2, 44, 18, false);
		Set_SkillBtn(1, 9, 17, false);
		Set_SkillBtn(0, 46, 16, false);

		break;

	case ENGINEER:

		Set_SkillBtn(3);
		Set_SkillBtn(2, 44, 5, false);
		Set_SkillBtn(1, 9, 4, false);
		Set_SkillBtn(0, 45, 3, false);

		break;

	case FIONA:


		Set_SkillBtn(3);
		Set_SkillBtn(2, 44, 8, false);
		Set_SkillBtn(1, 9, 7, false);
		Set_SkillBtn(0, 99, 6, false);

		//ENABLE_GAMEOBJECT(m_arrSkillUI[1][HeroKey]);

		break;

	case QANDA:


		Set_SkillBtn(2);
		Set_SkillBtn(1, 44, 20, false);
		Set_SkillBtn(0, 9, 19, false);

		//ENABLE_GAMEOBJECT(m_arrSkillUI[0][HeroKey]);

		break;

	case HOEDT:

		Active_SkillHUD(4);

		Set_SkillBtn(3);
		Set_SkillBtn(2, 44, 11, false);
		Set_SkillBtn(1, 68, 10, false);
		Set_SkillBtn(0, 9, 9, false);

		//ENABLE_GAMEOBJECT(m_arrSkillUI[1][HeroKey]);

		break;

	case LANCER:

		Active_SkillHUD(2);

		Set_SkillBtn(1);
		Set_SkillBtn(0, 68, 12, false);

		break;
	}
}

void CUI_Skill::Set_CoolTime(_uint iSkillType, _float fCoolTime, _float fMaxCoolTime)
{
	//m_fCoolTime[iSkillType] = fCoolTime;
	//m_fMaxCoolTime[iSkillType] = fMaxCoolTime;

	/*for (_uint i = 0; i < SkillEnd; ++i)
	{
		if (m_fCoolTime[iSkillType] > 0.f)
		{
			m_fSkillGauge[iSkillType] = m_fCoolTime[iSkillType] / m_fMaxCoolTime[iSkillType];
		}
	}

	if (m_fCoolTime[iSkillType] > 0.01f)
	{
		ENABLE_GAMEOBJECT(m_pSkillCoolTextArr[iSkillType]);
		ENABLE_GAMEOBJECT(m_pSkillCoolBGArr[iSkillType]);
	}*/
}

void CUI_Skill::Active_SkillHUD(_uint iIndex)
{
	//m_iBtnCount = iIndex;

	//for (_uint i = 0; i < 4; ++i)
	//{
	//	for (_uint j = 0; j < Type_End; ++j)
	//	{
	//		DISABLE_GAMEOBJECT(m_arrSkillUI[i][j]);
	//	}
	//}

	//_float m_fLerpSpeed = 0.3f;

	//for (_uint i = 0; i < iIndex; ++i)
	//{
	//	float fPosX = 480.f - (55.f * i);

	//	//m_arrSkillUI[i][BG]->Set_Sort(0.3f);
	//	//m_arrSkillUI[i][Outline0]->Set_Sort(0.2f);
	//	//m_arrSkillUI[i][Outline1]->Set_Sort(0.2f);
	//	//m_arrSkillUI[i][Outline2]->Set_Sort(0.2f);

	//	for (_uint j = 0; j < Type_End; ++j)
	//	{
	//		m_arrSkillUI[i][j]->Set_PosX(fPosX);

	//		if (m_iLerpCount > 0)
	//		{
	//			if (i < iIndex - 1)
	//			{
	//				//if (j == HeroKey)
	//				//{
	//				//	m_arrSkillUI[i][j]->Set_Pos(fPosX - 15.f, -295.f);

	//				//	continue;
	//				//}

	//				//// m_arrSkillUI[i][j]->Lerp_Scale(125.f, 50.f, m_fLerpSpeed);

	//				//if (j == Outline0)
	//				//{
	//				//	// m_arrSkillUI[i][j]->Lerp_Scale(125.f, 49.f, m_fLerpSpeed);
	//				//}

	//				//if (j == Icon)
	//				//{
	//				//	// m_arrSkillUI[i][j]->Lerp_Scale(125.f, 40.f, m_fLerpSpeed);
	//				//}
	//			}
	//		}

	//		/*if (j < HeroKey)
	//		{
	//			ENABLE_GAMEOBJECT(m_arrSkillUI[i][j]);

	//			if (m_iLerpCount < 1)
	//			{
	//				DISABLE_GAMEOBJECT(m_arrSkillUI[i][Outline1]);
	//				DISABLE_GAMEOBJECT(m_arrSkillUI[i][Outline2]);
	//			}
	//		}*/
	//	}
	//}

	//m_iLerpCount++;
}

void CUI_Skill::Set_SkillBtn(_uint iIndex, _uint iKeyIdx, _uint iIconIdx, bool bRelic)
{
	if (bRelic == true)
	{
		/*for (int i = 0; i < 4; ++i)
		{
			DISABLE_GAMEOBJECT(m_arrSkillUI[iIndex][i]);
		}

		if (iIndex < 3)
		{
			DISABLE_GAMEOBJECT(m_pSkillCoolTextArr[iIndex]);
		}*/
	}
	else
	{
		if (iIndex < 3)
		{
			/*_float4 vPos = m_arrSkillUI[iIndex][Icon]->Get_Pos();
			m_pSkillCoolTextArr[iIndex]->Set_Pos(vPos);
			ENABLE_GAMEOBJECT(m_pSkillCoolTextArr[iIndex]);*/
		}
	}

	// 키 버튼 비활성화
	if (iKeyIdx == 99)
	{
		//DISABLE_GAMEOBJECT(m_arrSkillUI[iIndex][Key]);
	}
	else
	{
		if (iKeyIdx == 46)
		{
			//m_arrSkillUI[iIndex][Key]->Set_Scale(31.f, 15.f);
		}
		else
		{
			//m_arrSkillUI[iIndex][Key]->Set_Scale(15.f);
		}

		//GET_COMPONENT_FROM(m_arrSkillUI[iIndex][Key], CTexture)->Set_CurTextureIndex(iKeyIdx);
	}

	//GET_COMPONENT_FROM(m_arrSkillUI[iIndex][Icon], CTexture)->Set_CurTextureIndex(iIconIdx);
}

void CUI_Skill::Enable_Outline()
{
	if (m_bAbleOutline)
	{
		m_fAccTime += fDT(0);

		_float fDuration = 0.5f;

		for (_uint i = 0; i < m_iBtnCount; ++i)
		{
			if (i != m_iBtnCount - 1)
			{
				//m_arrSkillUI[i][Outline1]->Set_Scale(95.f);
				//m_arrSkillUI[i][Outline1]->DoScale(-45.f, fDuration);

				//m_arrSkillUI[i][Outline1]->Set_Scale(150.f);
				//m_arrSkillUI[i][Outline2]->DoScale(-100.f, fDuration);

				if (m_iLerpCount > 1)
				{
					//ENABLE_GAMEOBJECT(m_arrSkillUI[i][Outline1]);
					//ENABLE_GAMEOBJECT(m_arrSkillUI[i][Outline2]);
				}
			}
		}

		m_bAbleOutline = false;
	}
}

void CUI_Skill::Ready_SkillHUD()
{
	/*for (int i = 0; i < 3; ++i)
	{
		m_Prototypes[i] = m_pUIMap[TEXT("Skill_Outline")];
	}

	m_Prototypes[BG] = m_pUIMap[TEXT("Skill_BG")];
	m_Prototypes[BG]->Set_Sort(0.3f);

	m_Prototypes[Icon] = m_pUIMap[TEXT("Skill_Icon")];
	m_Prototypes[Icon]->Set_Sort(0.29f);

	GET_COMPONENT_FROM(m_Prototypes[Icon], CTexture)->Remove_Texture(0);
	Read_Texture(m_Prototypes[Icon], "/HUD/Skill", "_");
	Read_Texture(m_Prototypes[Icon], "/HUD/Relic", "Relic");
	m_Prototypes[Icon]->SetTexture(TEXT("../Bin/Resources/Textures/UI/HUD/Relic/T_RelicIconMask.dds"));

	m_Prototypes[Key] = m_pUIMap[TEXT("Skill_Key")];
	m_Prototypes[Key]->Set_Sort(0.3f);
	Read_Texture(m_Prototypes[Key], "/KeyIcon/Keyboard", "Key");

	m_Prototypes[HeroKey] = m_pUIMap[TEXT("Skill_HeroKeySkill")];
	m_Prototypes[HeroKey]->Set_Sort(0.2f);

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < Type_End; ++j)
		{
			m_arrSkillUI[i][j] = m_Prototypes[j]->Clone();
		}
	}

	for (_uint i = 0; i < Type_End; ++i)
	{
		CREATE_GAMEOBJECT(m_Prototypes[i], GROUP_UI);
		DELETE_GAMEOBJECT(m_Prototypes[i]);
		m_Prototypes[i] = nullptr;
	}

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < Type_End; ++j)
		{
			CREATE_GAMEOBJECT(m_arrSkillUI[i][j], GROUP_UI);
			DISABLE_GAMEOBJECT(m_arrSkillUI[i][j]);
		}
	}*/
}

void CUI_Skill::Create_SkillCoolText()
{
	m_pSkillCoolText = CUI_Object::Create();

	GET_COMPONENT_FROM(m_pSkillCoolText, CTexture)->Remove_Texture(0);

	m_pSkillCoolText->Set_Sort(0.15f);

	m_pSkillCoolText->Set_FontRender(true);
	m_pSkillCoolText->Set_FontStyle(true);

	m_pSkillCoolText->Set_FontScale(0.3f);
	m_pSkillCoolText->Set_FontOffset(-18.f, -15.f);

	m_pSkillCoolText->Set_FontText(TEXT("5.5"));

	CREATE_GAMEOBJECT(m_pSkillCoolText, GROUP_UI);
	DELETE_GAMEOBJECT(m_pSkillCoolText);

	/*for (int i = 0; i < SkillEnd; ++i)
	{
		m_pSkillCoolTextArr[i] = m_pSkillCoolText->Clone();

		float fPosX = 480.f - (55.f * i);

		m_pSkillCoolTextArr[i]->Set_Pos(fPosX, -315.f);

		CREATE_GAMEOBJECT(m_pSkillCoolTextArr[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pSkillCoolTextArr[i]);
	}*/
}

void CUI_Skill::Create_SkillCoolBG()
{
	m_pSkillCoolBG = CUI_Object::Create();

	m_pSkillCoolBG->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Circle/T_256Circle.dds"));

	m_pSkillCoolBG->Set_Color(_float4(0.5f, 0.f, 0.f, 0.588f));
	m_pSkillCoolBG->Set_Sort(0.2f);
	m_pSkillCoolBG->Set_Scale(44.f);

	CREATE_GAMEOBJECT(m_pSkillCoolBG, GROUP_UI);
	DELETE_GAMEOBJECT(m_pSkillCoolBG);

	/*for (int i = 0; i < SkillEnd; ++i)
	{
		m_pSkillCoolBGArr[i] = m_pSkillCoolBG->Clone();

		float fPosX = 480.f - (55.f * i);
		m_pSkillCoolBGArr[i]->Set_Pos(fPosX, -315.f);

		CREATE_GAMEOBJECT(m_pSkillCoolBGArr[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pSkillCoolBGArr[i]);
	}*/
}

void CUI_Skill::Set_Pass()
{
	for (int i = 0; i < 4; ++i)
	{
		//GET_COMPONENT_FROM(m_arrSkillUI[i][HeroKey], CUI_Renderer)->Set_Pass(VTXTEX_PASS_UI_Color);
		//GET_COMPONENT_FROM(m_arrSkillUI[i][Outline0], CUI_Renderer)->Set_Pass(VTXTEX_PASS_UI_VerticalGauge);
	}
}

void CUI_Skill::Bind_Shader()
{
	for (int i = 0; i < 4; ++i)
	{
		//GET_COMPONENT_FROM(m_arrSkillUI[i][HeroKey], CShader)->CallBack_SetRawValues += bind(&CUI_Skill::Set_ShaderResources_HeroKeySkill, this, placeholders::_1, "g_vColor");
	}

	//GET_COMPONENT_FROM(m_arrSkillUI[Skill1][Outline0], CShader)->CallBack_SetRawValues += bind(&CUI_Skill::Set_Shader_SkillGauge1, this, placeholders::_1, "g_fValue");
	//GET_COMPONENT_FROM(m_arrSkillUI[Skill2][Outline0], CShader)->CallBack_SetRawValues += bind(&CUI_Skill::Set_Shader_SkillGauge2, this, placeholders::_1, "g_fValue");
	//GET_COMPONENT_FROM(m_arrSkillUI[Skill3][Outline0], CShader)->CallBack_SetRawValues += bind(&CUI_Skill::Set_Shader_SkillGauge3, this, placeholders::_1, "g_fValue");
}
