#include "CUI_Skill.h"
#include "GameInstance.h"
#include "CUI_Object.h"
#include "Transform.h"
#include "Texture.h"
#include "Functor.h"
#include "CUnit.h"
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
	Read_UI("Skill");

	Ready_SkillHUD();

	return S_OK;
}

HRESULT CUI_Skill::Initialize()
{
	return S_OK;
}

HRESULT CUI_Skill::Start()
{
	// Bind_Shader();

	__super::Start();

	return S_OK;
}

void CUI_Skill::My_Tick()
{
	__super::My_Tick();

	/*if (!m_bIsHero)
	{
		if (KEY(T, TAP))
		{
			static int iIndex = 0;
			iIndex++;
			if (iIndex >= 6)
				iIndex = 0;

			Set_SkillHUD(iIndex);
		}

		if (KEY(NUM1, TAP))
		{
			m_bIsHero = true;

			Set_SkillHUD(6);
		}
		else if (KEY(NUM2, TAP))
		{
			m_bIsHero = true;

			Set_SkillHUD(7);
		}
		else if (KEY(NUM3, TAP))
		{
			m_bIsHero = true;

			Set_SkillHUD(8);
		}
		else if (KEY(NUM4, TAP))
		{
			m_bIsHero = true;

			Set_SkillHUD(9);
		}
	}
	else
	{
		if (KEY(NUM1, TAP))
		{
			m_bIsHero = false;

			Set_SkillHUD(m_iPrvSkill);
		}
	}*/

	m_fRelicValue += fDT(0);

	Enable_Outline();
}

void CUI_Skill::Set_ShaderResources_Relic(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_fValue", &m_fRelicValue, sizeof(_float));
}

void CUI_Skill::Set_SkillHUD(_uint iIndex)
{
	m_iPrvSkill = m_iCurSkill;
	m_iCurSkill = iIndex;

	m_bAbleOutline = true;

	switch (iIndex)
	{
	case CUnit::WARRIOR:

		Active_SkillHUD(3);

		Set_SkillBtn(2);
		Set_SkillBtn(1, 44, 25, false);
		Set_SkillBtn(0, 9, 24, false);

		break;

	case CUnit::SPEAR:

		Active_SkillHUD(4);

		Set_SkillBtn(3);
		Set_SkillBtn(2, 44, 23, false);
		Set_SkillBtn(1, 9, 22, false);
		Set_SkillBtn(0, 99, 21, false);

		break;

	case CUnit::ARCHER:

		Active_SkillHUD(4);

		Set_SkillBtn(3);
		Set_SkillBtn(2, 44, 2, false);
		Set_SkillBtn(1, 9, 1, false);
		Set_SkillBtn(0, 74, 0, false);

		break;

	case CUnit::PALADIN:

		Active_SkillHUD(4);

		Set_SkillBtn(3);
		Set_SkillBtn(2, 44, 15, false);
		Set_SkillBtn(1, 9, 14, false);
		Set_SkillBtn(0, 76, 13, false);

		break;

	case CUnit::PRIEST:

		Active_SkillHUD(4);

		Set_SkillBtn(3);
		Set_SkillBtn(2, 44, 18, false);
		Set_SkillBtn(1, 9, 17, false);
		Set_SkillBtn(0, 46, 16, false);

		break;

	case CUnit::ENGINEER:

		Active_SkillHUD(4);

		Set_SkillBtn(3);
		Set_SkillBtn(2, 44, 5, false);
		Set_SkillBtn(1, 9, 4, false);
		Set_SkillBtn(0, 45, 3, false);

		break;

	case CUnit::FIONA:

		Active_SkillHUD(4);

		Set_SkillBtn(3);
		Set_SkillBtn(2, 44, 8, false);
		Set_SkillBtn(1, 9, 7, false);
		Set_SkillBtn(0, 99, 6, false);

		break;

	case CUnit::QANDA:

		Active_SkillHUD(3);

		Set_SkillBtn(2);
		Set_SkillBtn(1, 44, 20, false);
		Set_SkillBtn(0, 9, 19, false);

		break;

	case CUnit::HOEDT:

		Active_SkillHUD(4);

		Set_SkillBtn(3);
		Set_SkillBtn(2, 44, 11, false);
		Set_SkillBtn(1, 68, 10, false);
		Set_SkillBtn(0, 9, 9, false);

		break;

	case CUnit::LANCER:

		Active_SkillHUD(2);

		Set_SkillBtn(1);
		Set_SkillBtn(0, 68, 12, false);

		break;
	}
}

void CUI_Skill::Active_SkillHUD(_uint iIndex)
{
	m_iBtnCount = iIndex;

	for (_uint i = 0; i < 4; ++i)
	{
		for (_uint j = 0; j < Type_End; ++j)
		{
			DISABLE_GAMEOBJECT(m_arrSkillUI[i][j]);
		}
	}

	_float m_fLerpSpeed = 0.3f;

	for (_uint i = 0; i < iIndex; ++i)
	{
		float fPosX = 480.f - (55.f * i);

		m_arrSkillUI[i][BG]->Set_Sort(0.1f);

		for (_uint j = 0; j < Type_End; ++j)
		{
			m_arrSkillUI[i][j]->Set_PosX(fPosX);

			if (m_iLerpCount > 0)
			{
				if (i < iIndex - 1)
				{
					m_arrSkillUI[i][j]->Lerp_Scale(125.f, 50.f, m_fLerpSpeed);

					if (j == Outline0)
					{
						m_arrSkillUI[i][j]->Lerp_Scale(125.f, 49.f, m_fLerpSpeed);
					}

					if (j == Icon)
					{
						m_arrSkillUI[i][j]->Lerp_Scale(125.f, 40.f, m_fLerpSpeed);
					}
				}
			}

			ENABLE_GAMEOBJECT(m_arrSkillUI[i][j]);

			if (m_iLerpCount < 1)
			{
				DISABLE_GAMEOBJECT(m_arrSkillUI[i][Outline1]);
				DISABLE_GAMEOBJECT(m_arrSkillUI[i][Outline2]);
			}
		}
	}

	m_iLerpCount++;
}

void CUI_Skill::Set_SkillBtn(_uint iIndex, _uint iKeyIdx, _uint iIconIdx, bool bRelic)
{
	if (bRelic == true)
	{
		for (int i = 0; i < 4; ++i)
		{
			DISABLE_GAMEOBJECT(m_arrSkillUI[iIndex][i]);
		}
	}

	// 키 버튼 비활성화
	if (iKeyIdx == 99)
	{
		DISABLE_GAMEOBJECT(m_arrSkillUI[iIndex][Key]);
	}
	else
	{
		_float4 vScale = m_arrSkillUI[iIndex][Key]->Get_Transform()->Get_Scale();

		if (iKeyIdx == 46)
		{
			m_arrSkillUI[iIndex][Key]->Set_Scale(31.f, 18.f);
		}
		else
		{
			m_arrSkillUI[iIndex][Key]->Set_Scale(18.f);
		}

		GET_COMPONENT_FROM(m_arrSkillUI[iIndex][Key], CTexture)->Set_CurTextureIndex(iKeyIdx);
	}

	GET_COMPONENT_FROM(m_arrSkillUI[iIndex][Icon], CTexture)->Set_CurTextureIndex(iIconIdx);
}

void CUI_Skill::Enable_Outline()
{
	if (m_bAbleOutline)
	{
		m_fAccTime += fDT(0);

		_float4 vScale = m_arrSkillUI[1][BG]->Get_Transform()->Get_Scale();
		_float m_fLeprSpeed = 0.5f;

		for (int i = 0; i < m_iBtnCount; ++i)
		{
			if (i != m_iBtnCount - 1)
			{
				m_arrSkillUI[i][Outline1]->Lerp_Scale(95.f, 50.f, m_fLeprSpeed);
				m_arrSkillUI[i][Outline2]->Lerp_Scale(150.f, 50.f, m_fLeprSpeed);

				if (m_iLerpCount > 1)
				{
					ENABLE_GAMEOBJECT(m_arrSkillUI[i][Outline1]);
					ENABLE_GAMEOBJECT(m_arrSkillUI[i][Outline2]);
				}
			}
		}

		m_bAbleOutline = false;
	}
}

void CUI_Skill::Ready_SkillHUD()
{
	for (int i = 0; i < 3; ++i)
	{
		m_Prototypes[i] = m_pUIMap[TEXT("Skill_Outline")];
	}

	m_Prototypes[BG] = m_pUIMap[TEXT("Skill_BG")];
	m_Prototypes[Icon] = m_pUIMap[TEXT("Skill_Icon")];
	m_Prototypes[Key] = m_pUIMap[TEXT("Skill_Key")];

	GET_COMPONENT_FROM(m_Prototypes[Icon], CTexture)->Remove_Texture(0);

	Read_Texture(m_Prototypes[Icon], "/HUD/Skill", "_");
	Read_Texture(m_Prototypes[Icon], "/HUD/Relic", "Relic");

	m_Prototypes[Icon]->SetTexture(TEXT("../Bin/Resources/Textures/UI/HUD/Relic/T_RelicIconMask.dds"));

	Read_Texture(m_Prototypes[Key], "/KeyIcon/Keyboard", "Key");

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < Type_End; ++j)
		{
			m_arrSkillUI[i][j] = m_Prototypes[j]->Clone();
		}
	}

	// 원본 객체 생성 후 삭제
	for (_uint i = 0; i < Type_End; ++i)
	{
		CREATE_GAMEOBJECT(m_Prototypes[i], GROUP_UI);
		DELETE_GAMEOBJECT(m_Prototypes[i]);
	}

	// 사용할 클론 생성 후 비활성화
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < Type_End; ++j)
		{
			CREATE_GAMEOBJECT(m_arrSkillUI[i][j], GROUP_UI);
			DISABLE_GAMEOBJECT(m_arrSkillUI[i][j]);
		}
	}
}

void CUI_Skill::Set_Pass()
{
	for (int i = 0; i < 4; ++i)
	{
		// 버튼들을 돌면서 현재 버튼의 텍스처가 29번이면 패스 활성화
		_uint m_iRelic = GET_COMPONENT_FROM(m_arrSkillUI[i][Icon], CTexture)->Get_CurTextureIndex();

		if (m_iRelic)
		{
			CRenderer* pRenderer = GET_COMPONENT_FROM(m_arrSkillUI[i][Icon], CRenderer);
			pRenderer->Set_Pass(VTXTEX_PASS_UI_RELIC);
		}
	}
}

void CUI_Skill::Bind_Shader()
{
	GET_COMPONENT_FROM(m_arrSkillUI[0][Icon], CShader)
		->CallBack_SetRawValues += bind(&CUI_Skill::Set_ShaderResources_Relic, this, placeholders::_1, "g_fValue");
}
