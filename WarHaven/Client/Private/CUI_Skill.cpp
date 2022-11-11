#include "CUI_Skill.h"
#include "GameInstance.h"
#include "CUI_Object.h"
#include "Transform.h"
#include "Texture.h"
#include "Functor.h"
#include "CUnit.h"
#include "CShader.h"
#include "Renderer.h"

CUI_Skill::CUI_Skill()
{
}

CUI_Skill::CUI_Skill(const CUI_Skill& Prototype)
	: CUI_Wrapper(Prototype)
{
}

CUI_Skill::~CUI_Skill()
{
}

HRESULT CUI_Skill::Initialize_Prototype()
{
	Read_UI("Skill");

	m_Prototypes[UI_TYPE::Outline] = m_pUIMap[TEXT("Skill_Outline")];
	m_Prototypes[UI_TYPE::BG] = m_pUIMap[TEXT("Skill_BG")];
	m_Prototypes[UI_TYPE::Icon] = m_pUIMap[TEXT("Skill_Icon")];
	m_Prototypes[UI_TYPE::Key] = m_pUIMap[TEXT("Skill_Key")];

	GET_COMPONENT_FROM(m_Prototypes[UI_TYPE::Icon], CTexture)->Remove_Texture(0);

	Read_Texture(m_Prototypes[UI_TYPE::Icon], "/HUD/Skill", "_");
	Read_Texture(m_Prototypes[UI_TYPE::Icon], "/HUD/Relic", "Relic");

	m_Prototypes[UI_TYPE::Icon]->SetTexture(TEXT("../Bin/Resources/Textures/UI/HUD/Relic/T_RelicIconMask.dds"));

	Read_Texture(m_Prototypes[UI_TYPE::Key], "/KeyIcon/Keyboard", "Key");

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < UI_TYPE::Type_End; ++j)
		{
			m_arrSkillUI[i][j] = m_Prototypes[j]->Clone();
		}
	}

	return S_OK;
}

HRESULT CUI_Skill::Initialize()
{
	return S_OK;
}

HRESULT CUI_Skill::Start()
{
	for (_uint i = 0; i < UI_TYPE::Type_End; ++i)
	{
		CREATE_GAMEOBJECT(m_Prototypes[i], GROUP_UI);
		DELETE_GAMEOBJECT(m_Prototypes[i]);
	}

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < UI_TYPE::Type_End; ++j)
		{
			CREATE_GAMEOBJECT(m_arrSkillUI[i][j], GROUP_UI);
			DISABLE_GAMEOBJECT(m_arrSkillUI[i][j]);
		}
	}

	GET_COMPONENT_FROM(m_arrSkillUI[0][UI_TYPE::Icon], CShader)
		->CallBack_SetRawValues += bind(&CUI_Skill::Set_ShaderResources_Relic, this, placeholders::_1, "g_fValue");

	__super::Start();

	return S_OK;
}

void CUI_Skill::Set_ShaderResources_Relic(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_fValue", &m_fRelicValue, sizeof(_float));
}

void CUI_Skill::Set_SkillUI(_uint iIndex)
{
	switch (iIndex)
	{
	case CUnit::CLASS_WARRIOR:

		ActiveSkillBtn(3);

		Set_SkillIcon(2);
		Set_SkillIcon(1, 44, 25, false);
		Set_SkillIcon(0, 9, 24, false);

		break;

	case CUnit::CLASS_SPEAR:

		ActiveSkillBtn(4);

		Set_SkillIcon(3);
		Set_SkillIcon(2, 44, 23, false);
		Set_SkillIcon(1, 9, 22, false);
		Set_SkillIcon(0, 99, 21, false);

		break;

	case CUnit::CLASS_ARCHER:

		ActiveSkillBtn(4);

		Set_SkillIcon(3);
		Set_SkillIcon(2, 44, 2, false);
		Set_SkillIcon(1, 9, 1, false);
		Set_SkillIcon(0, 74, 0, false);

		break;

	case CUnit::CLASS_PALADIN:

		ActiveSkillBtn(4);

		Set_SkillIcon(3);
		Set_SkillIcon(2, 44, 15, false);
		Set_SkillIcon(1, 9, 14, false);
		Set_SkillIcon(0, 76, 13, false);

		break;

	case CUnit::CLASS_PRIEST:

		ActiveSkillBtn(4);

		Set_SkillIcon(3);
		Set_SkillIcon(2, 44, 18, false);
		Set_SkillIcon(1, 9, 17, false);
		Set_SkillIcon(0, 46, 16, false);

		break;

	case CUnit::CLASS_ENGINEER:

		ActiveSkillBtn(4);

		Set_SkillIcon(3);
		Set_SkillIcon(2, 44, 5, false);
		Set_SkillIcon(1, 9, 4, false);
		Set_SkillIcon(0, 45, 3, false);

		break;

	case CUnit::CLASS_FIONA:

		ActiveSkillBtn(4);

		Set_SkillIcon(3);
		Set_SkillIcon(2, 44, 8, false);
		Set_SkillIcon(1, 9, 7, false);
		Set_SkillIcon(0, 99, 6, false);

		break;

	case CUnit::CLASS_QANDA:

		ActiveSkillBtn(3);

		Set_SkillIcon(2);
		Set_SkillIcon(1, 44, 20, false);
		Set_SkillIcon(0, 9, 19, false);

		break;

	case CUnit::CLASS_HOEDT:

		ActiveSkillBtn(4);

		Set_SkillIcon(3);
		Set_SkillIcon(2, 44, 11, false);
		Set_SkillIcon(1, 68, 10, false);
		Set_SkillIcon(0, 9, 9, false);

		break;

	case CUnit::CLASS_LANCER:

		ActiveSkillBtn(2);

		Set_SkillIcon(1);
		Set_SkillIcon(0, 68, 12, false);

		break;
	}
}

void CUI_Skill::ActiveSkillBtn(_uint iIndex)
{
	for (_uint i = 0; i < 4; ++i)
	{
		for (_uint j = 0; j < UI_TYPE::Type_End; ++j)
		{
			DISABLE_GAMEOBJECT(m_arrSkillUI[i][j]);
		}
	}

	for (_uint i = 0; i < iIndex; ++i)
	{
		float fPosX = 480.f - (55.f * i);

		CRenderer* pRenderer = GET_COMPONENT_FROM(m_arrSkillUI[i][UI_TYPE::Icon], CRenderer);
		pRenderer->Set_Pass(VTXTEX_PASS_UI_RELIC);

		m_arrSkillUI[i][UI_TYPE::BG]->Set_Sort(0.1f);

		for (_uint j = 0; j < UI_TYPE::Type_End; ++j)
		{
			_float4 vPos = m_arrSkillUI[i][j]->Get_Transform()->Get_World(WORLD_POS);
			m_arrSkillUI[i][j]->Set_Pos(fPosX, vPos.y);

			ENABLE_GAMEOBJECT(m_arrSkillUI[i][j]);
		}
	}
}

void CUI_Skill::Set_SkillIcon(_uint iIndex, _uint iKeyIdx, _uint iIconIdx, bool bRelic)
{
	if (bRelic == true)
	{
		DISABLE_GAMEOBJECT(m_arrSkillUI[iIndex][UI_TYPE::BG]);
		DISABLE_GAMEOBJECT(m_arrSkillUI[iIndex][UI_TYPE::Outline]);
	}
	else
	{
		GET_COMPONENT_FROM(m_arrSkillUI[iIndex][UI_TYPE::BG], CTexture)->Set_CurTextureIndex(iIconIdx);
		GET_COMPONENT_FROM(m_arrSkillUI[iIndex][UI_TYPE::Outline], CTexture)->Set_CurTextureIndex(iIconIdx);
	}

	if (iKeyIdx == 99)
	{
		DISABLE_GAMEOBJECT(m_arrSkillUI[iIndex][UI_TYPE::Key]);
	}
	else if (iKeyIdx == 46)
	{
		_float4 vScale = m_arrSkillUI[iIndex][UI_TYPE::Key]->Get_Transform()->Get_Scale();
		m_arrSkillUI[iIndex][UI_TYPE::Key]->Set_Scale(31.5f, vScale.y);

		GET_COMPONENT_FROM(m_arrSkillUI[iIndex][UI_TYPE::Key], CTexture)->Set_CurTextureIndex(iKeyIdx);
	}
	else
	{
		_float4 vScale = m_arrSkillUI[iIndex][UI_TYPE::Key]->Get_Transform()->Get_Scale();
		m_arrSkillUI[iIndex][UI_TYPE::Key]->Set_Scale(20.f);

		GET_COMPONENT_FROM(m_arrSkillUI[iIndex][UI_TYPE::Key], CTexture)->Set_CurTextureIndex(iKeyIdx);
	}

	GET_COMPONENT_FROM(m_arrSkillUI[iIndex][UI_TYPE::Icon], CTexture)->Set_CurTextureIndex(iIconIdx);
}

void CUI_Skill::My_Tick()
{
	__super::My_Tick();

	if (KEY(T, TAP))
	{
		static int iIndex = 0;
		iIndex++;
		if (iIndex >= 10)
			iIndex = 0;

		Set_SkillUI(iIndex);
	}

	m_fRelicValue += fDT(0);
}

void CUI_Skill::My_LateTick()
{
	__super::My_LateTick();
}

void CUI_Skill::OnEnable()
{
	__super::OnEnable();
}

void CUI_Skill::OnDisable()
{
	__super::OnDisable();
}
