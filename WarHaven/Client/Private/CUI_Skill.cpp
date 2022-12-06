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
	Create_HeroKeySkillIcon();

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
	SetActive_SkillUI(false);
	SetActive_Outline(false);

	for (int i = 0; i < SU_End; ++i)
	{
		for (int j = 0; j < m_iIndex; ++j)
		{
			GET_COMPONENT_FROM(m_pArrSkillUI[SU_Icon][j], CTexture)->Set_CurTextureIndex(m_iSkillNum + j);

			m_pArrSkillUI[i][j]->SetActive(true);

			if (i == SU_BG)
			{
				m_pArrSkillUI[i][j]->Lerp_Scale(125.f, 50.f, 0.3f);
			}
			else if (i == SU_Icon)
			{
				m_pArrSkillUI[i][j]->Lerp_Scale(115.f, 28.f, 0.3f);
			}
		}
	}

	for (int i = 0; i < Outline_End; ++i)
	{
		for (int j = 0; j < m_iIndex; ++j)
		{
			Enable_Fade(m_pArrOutline[i][j], 0.f);

			if (i == Outline0)
			{
				m_pArrOutline[i][j]->Lerp_Scale(125.f, 40.f, 0.3f);
			}
			else if (i == Outline1)
			{
				m_pArrOutline[i][j]->Lerp_Scale(155.f, 40.f, 0.4f);
			}
			else if (i == Outline2)
			{
				m_pArrOutline[i][j]->Lerp_Scale(205.f, 40.f, 0.5f);
			}
		}
	}

	Active_HeroKeySkillIcon(m_iCurClass);
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

		_float fDuration = 0.3f + (i * 0.1f);
		m_pOutline[i]->Set_FadeDesc(fDuration);

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

void CUI_Skill::Create_HeroKeySkillIcon()
{
	m_pHeroKeySkillIcon = CUI_Object::Create();

	// m_pHeroKeySkillIcon->Set_UIShaderFlag(SH_UI_HARDBLOOM);

	m_pHeroKeySkillIcon->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/Skill/HeroKeyIcon.dds"));
	m_pHeroKeySkillIcon->Set_Color(_float4(0.9f, 0.8f, 0.5f, 1.f));

	m_pHeroKeySkillIcon->Set_PosY(-300.f);
	m_pHeroKeySkillIcon->Set_Scale(28.f);
	m_pHeroKeySkillIcon->Set_Sort(0.48f);

	CREATE_GAMEOBJECT(m_pHeroKeySkillIcon, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pHeroKeySkillIcon);
}

void CUI_Skill::Active_HeroKeySkillIcon(_uint eHeroClass)
{
	if (eHeroClass < FIONA)
	{
		m_pHeroKeySkillIcon->SetActive(false);

		return;
	}

	_float fPosX;
	switch (eHeroClass)
	{
	case Client::FIONA:
		fPosX = m_pArrSkillUI[SU_BG][1]->Get_PosX();
		m_pHeroKeySkillIcon->Set_PosX(fPosX - 10.f);
		break;
	case Client::QANDA:
		break;
	case Client::HOEDT:
		break;
	case Client::LANCER:
		break;
	}

	m_pHeroKeySkillIcon->SetActive(true);
}

void CUI_Skill::Create_SkillCoolUI()
{
	for (int i = 0; i < SC_End; ++i)
	{
		m_pSkillCoolUI[i] = CUI_Object::Create();

		if (i == SC_BG)
		{
			m_pSkillCoolUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Circle/T_256Circle.dds"));
			m_pSkillCoolUI[i]->Set_Color(_float4(1.f, 0.f, 0.f, 0.5f));

			m_pSkillCoolUI[i]->Set_Scale(32.f);
			m_pSkillCoolUI[i]->Set_Sort(0.49f);
		}
		else if (i == SC_Text)
		{
			m_pSkillCoolUI[i]->Set_Sort(0.48f);
			m_pSkillCoolUI[i]->Set_FontRender(true);
			m_pSkillCoolUI[i]->Set_FontStyle(true);
			m_pSkillCoolUI[i]->Set_FontCenter(true);
			m_pSkillCoolUI[i]->Set_FontScale(0.3f);
		}

		CREATE_GAMEOBJECT(m_pSkillCoolUI[i], GROUP_UI);
		DELETE_GAMEOBJECT(m_pSkillCoolUI[i]);

		for (int j = 0; j < 3; ++j)
		{
			m_pArrSkillCoolUI[i][j] = m_pSkillCoolUI[i]->Clone();

			CREATE_GAMEOBJECT(m_pArrSkillCoolUI[i][j], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pArrSkillCoolUI[i][j]);
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
