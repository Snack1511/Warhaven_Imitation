#include "CUI_Skill.h"
#include "GameInstance.h"
#include "CUI_Object.h"
#include "Transform.h"
#include "Texture.h"
#include "Functor.h"
#include "CShader.h"
#include "CUI_Renderer.h"
#include "Easing_Utillity.h"
#include "CFader.h"
#include "CShader.h"	

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
	Create_SkillCoolUI();

	return S_OK;
}

HRESULT CUI_Skill::Initialize()
{
	return S_OK;
}

HRESULT CUI_Skill::Start()
{
	__super::Start();

	Bind_Shader();

	Set_SkillUI(WARRIOR);

	return S_OK;
}

void CUI_Skill::Set_Shader_SkillCoolGauge0(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_fValue", &m_fSkillCoolTimeRatio[0], sizeof(_float));
}

void CUI_Skill::Set_Shader_SkillCoolGauge1(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_fValue", &m_fSkillCoolTimeRatio[1], sizeof(_float));
}

void CUI_Skill::Set_Shader_SkillCoolGauge2(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_fValue", &m_fSkillCoolTimeRatio[2], sizeof(_float));
}

void CUI_Skill::Set_SkillUI(_uint iClass)
{
	m_iPrvClass = m_iCurClass;
	m_iCurClass = iClass;

	/*if (m_iPrvClass == iClass)
		return;*/

	_float fPosX = m_pArrSkillUI[SU_BG][1]->Get_PosX();

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
		m_pHeroKeySkillIcon->Set_PosX(fPosX - 10.f);
		break;
	}
}

void CUI_Skill::Set_SkillCoolTime(_uint iSkillIdx, _float fSkillCoolTime, _float fSkillMaxCoolTime)
{
	m_fSkillCoolTime[iSkillIdx] = fSkillCoolTime;
	m_fSkillMaxCoolTime[iSkillIdx] = fSkillMaxCoolTime;

	if (m_fSkillCoolTime[iSkillIdx] > 0.f)
	{
		m_fSkillCoolTimeRatio[iSkillIdx] = m_fSkillCoolTime[iSkillIdx] / m_fSkillMaxCoolTime[iSkillIdx];

		cout << iSkillIdx << " : " << m_fSkillCoolTimeRatio[iSkillIdx] << endl;

		for (int i = 0; i < SC_End; ++i)
		{
			m_pArrSkillCoolUI[i][iSkillIdx]->SetActive(true);
		}
	}
	else
	{
		for (int i = 0; i < SC_End; ++i)
		{
			m_pArrSkillCoolUI[i][iSkillIdx]->SetActive(false);
		}
	}
}

void CUI_Skill::SetActive_SkillUI(_bool value)
{
	for (int i = 0; i < SU_End; ++i)
	{
		for (int j = 0; j < m_iIndex; ++j)
		{
			GET_COMPONENT_FROM(m_pArrSkillUI[SU_Icon][j], CTexture)->Set_CurTextureIndex(m_iSkillNum + j);

			m_pArrSkillUI[i][j]->SetActive(value);

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
}

void CUI_Skill::SetActive_Outline(_bool value)
{
	for (int i = 0; i < Outline_End; ++i)
	{
		for (int j = 0; j < m_iIndex; ++j)
		{
			if (value == true)
			{
				Enable_Fade(m_pArrOutline[i][j], 0.f);

				if (i == Outline0)
				{
					m_pArrOutline[i][j]->Lerp_Scale(125.f, 40.f, 0.3f);
				}
				else if (i == Outline1)
				{
					m_pArrOutline[i][j]->Lerp_Scale(155.f, 40.f, m_fOutline1LerpTime);
				}
				else if (i == Outline2)
				{
					m_pArrOutline[i][j]->Lerp_Scale(205.f, 40.f, m_fOutline2LerpTime);
				}
			}
			else
			{
				m_pArrOutline[i][j]->SetActive(false);
			}
		}
	}
}

void CUI_Skill::SetActive_SkillCool(_bool value)
{
	for (int i = 0; i < SC_End; ++i)
	{
		for (int j = 0; j < m_iIndex; ++j)
		{
			m_pArrSkillCoolUI[i][j]->SetActive(value);
		}
	}
}

void CUI_Skill::SetActvie_HeroKeyIcon(_bool value)
{
	m_pHeroKeySkillIcon->SetActive(value);
}

void CUI_Skill::Transform_SkillUI(_uint iClass)
{
	SetActive_SkillUI(false);
	SetActive_Outline(false);
	SetActive_SkillCool(false);
	SetActvie_HeroKeyIcon(false);

	Set_SkillUI(iClass);

	SetActive_SkillUI(true);
	SetActive_Outline(true);
	SetActive_SkillCool(true);

	if (m_iCurClass >= FIONA)
	{
		SetActvie_HeroKeyIcon(true);
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

		if (i == Outline0)
		{
			GET_COMPONENT_FROM(m_pOutline[i], CUI_Renderer)->Set_Pass(VTXTEX_PASS_UI_VerticalGauge);
		}

		m_pOutline[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/Skill/Outline.png"));

		m_pOutline[i]->Set_PosY(-315.f);
		m_pOutline[i]->Set_Scale(40.f);
		m_pOutline[i]->Set_Sort(0.48f);

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

void CUI_Skill::Disable_Outline()
{
	for (int i = 0; i < m_iIndex; ++i)
	{
		if (m_pArrOutline[Outline1][i]->Is_Valid())
		{
			m_fOutline1AccTime[i] += fDT(0);
			if (m_fOutline1AccTime[i] > m_fOutline1LerpTime)
			{
				m_fOutline1AccTime[i] = 0.f;
				m_pArrOutline[Outline1][i]->SetActive(false);
			}
		}

		if (m_pArrOutline[Outline2][i]->Is_Valid())
		{
			m_fOutline2AccTime[i] += fDT(0);
			if (m_fOutline2AccTime[i] > m_fOutline2LerpTime)
			{
				m_fOutline2AccTime[i] = 0.f;
				m_pArrOutline[Outline2][i]->SetActive(false);
			}
		}
	}
}

void CUI_Skill::Create_HeroKeySkillIcon()
{
	m_pHeroKeySkillIcon = CUI_Object::Create();

	m_pHeroKeySkillIcon->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/Skill/HeroKeyIcon.dds"));
	m_pHeroKeySkillIcon->Set_Color(_float4(0.9f, 0.8f, 0.5f, 1.f));

	m_pHeroKeySkillIcon->Set_PosY(-300.f);
	m_pHeroKeySkillIcon->Set_Scale(28.f);
	m_pHeroKeySkillIcon->Set_Sort(0.47f);

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

	m_pHeroKeySkillIcon->SetActive(true);
}

void CUI_Skill::Create_SkillCoolUI()
{
	for (int i = 0; i < SC_End; ++i)
	{
		m_pSkillCoolUI[i] = CUI_Object::Create();

		m_pSkillCoolUI[i]->Set_PosY(-315.f);

		if (i == SC_BG)
		{
			m_pSkillCoolUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Circle/T_256Circle.dds"));
			m_pSkillCoolUI[i]->Set_Color(_float4(1.f, 0.f, 0.f, 0.5f));

			m_pSkillCoolUI[i]->Set_Scale(35.f);
			m_pSkillCoolUI[i]->Set_Sort(0.49f);
		}
		else if (i == SC_Text)
		{
			GET_COMPONENT_FROM(m_pSkillCoolUI[i], CTexture)->Remove_Texture(0);

			m_pSkillCoolUI[i]->Set_Sort(0.48f);
			m_pSkillCoolUI[i]->Set_FontRender(true);
			m_pSkillCoolUI[i]->Set_FontStyle(true);
			m_pSkillCoolUI[i]->Set_FontCenter(true);

			m_pSkillCoolUI[i]->Set_FontOffset(6.f, 4.f);
			m_pSkillCoolUI[i]->Set_FontScale(0.2f);
		}

		CREATE_GAMEOBJECT(m_pSkillCoolUI[i], GROUP_UI);
		DELETE_GAMEOBJECT(m_pSkillCoolUI[i]);

		for (int j = 0; j < 3; ++j)
		{
			m_pArrSkillCoolUI[i][j] = m_pSkillCoolUI[i]->Clone();

			_float fPosX = 480.f - (55.f * j);
			m_pArrSkillCoolUI[i][j]->Set_PosX(fPosX);

			CREATE_GAMEOBJECT(m_pArrSkillCoolUI[i][j], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pArrSkillCoolUI[i][j]);
		}
	}
}

void CUI_Skill::Update_SkillCoolTime()
{
	for (int i = 0; i < m_iIndex; ++i)
	{
		_tchar  szSkill[MAX_STR] = {};
		swprintf_s(szSkill, TEXT("%.1f"), m_fSkillCoolTime[i]);
		m_pArrSkillCoolUI[SC_Text][i]->Set_FontText(szSkill);
	}
}

void CUI_Skill::Reset_SkillCoolTime()
{
	for (int i = 0; i < 3; ++i)
	{
		m_fSkillCoolTimeRatio[i] = 0.f;
	}
}

void CUI_Skill::My_Tick()
{
	__super::My_Tick();

	Disable_Outline();

	Update_SkillCoolTime();
}

void CUI_Skill::OnEnable()
{
	__super::OnEnable();

	SetActive_SkillUI(true);
	SetActive_Outline(true);
	SetActive_SkillCool(true);
	SetActvie_HeroKeyIcon(true);
}

void CUI_Skill::OnDisable()
{
	__super::OnDisable();

	SetActive_SkillUI(false);
	SetActive_Outline(false);
	SetActive_SkillCool(false); 
	SetActvie_HeroKeyIcon(false);

	Reset_SkillCoolTime();
}

void CUI_Skill::Bind_Shader()
{
	GET_COMPONENT_FROM(m_pArrOutline[Outline0][0], CShader)->CallBack_SetRawValues += bind(&CUI_Skill::Set_Shader_SkillCoolGauge0, this, placeholders::_1, "g_fValue");
	GET_COMPONENT_FROM(m_pArrOutline[Outline0][1], CShader)->CallBack_SetRawValues += bind(&CUI_Skill::Set_Shader_SkillCoolGauge1, this, placeholders::_1, "g_fValue");
	GET_COMPONENT_FROM(m_pArrOutline[Outline0][2], CShader)->CallBack_SetRawValues += bind(&CUI_Skill::Set_Shader_SkillCoolGauge2, this, placeholders::_1, "g_fValue");
}
