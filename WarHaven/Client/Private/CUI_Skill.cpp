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

	tSkillHud.m_pUIInstance[SkillHud::OUTLINE] = m_pUIMap[TEXT("Skill_Outline")];
	tSkillHud.m_pUIInstance[SkillHud::BG] = m_pUIMap[TEXT("Skill_BG")];
	tSkillHud.m_pUIInstance[SkillHud::ICON] = m_pUIMap[TEXT("Skill_Icon")];
	tSkillHud.m_pUIInstance[SkillHud::KEY] = m_pUIMap[TEXT("Skill_Key")];

	GET_COMPONENT_FROM(tSkillHud.m_pUIInstance[SkillHud::ICON], CTexture)->Remove_Texture(0);

	Read_Texture(tSkillHud.m_pUIInstance[SkillHud::ICON], "/HUD/Skill", "_");
	Read_Texture(tSkillHud.m_pUIInstance[SkillHud::ICON], "/HUD/Relic", "Relic");

	tSkillHud.m_pUIInstance[SkillHud::ICON]->SetTexture(TEXT("../Bin/Resources/Textures/UI/T_RelicIconMask.dds"));

	Read_Texture(tSkillHud.m_pUIInstance[SkillHud::KEY], "/KeyIcon/Keyboard", "Key");

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < SkillHud::NAME_END; ++j)
		{
			m_arrSkillHud[i].m_pUIInstance[j] = tSkillHud.m_pUIInstance[j]->Clone();
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
	for (_uint i = 0; i < SkillHud::NAME_END; ++i)
	{
		CREATE_GAMEOBJECT(tSkillHud.m_pUIInstance[i], GROUP_UI);
		DISABLE_GAMEOBJECT(tSkillHud.m_pUIInstance[i]);
	}

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < SkillHud::NAME_END; ++j)
		{
			CREATE_GAMEOBJECT(m_arrSkillHud[i].m_pUIInstance[j], GROUP_UI);
			DISABLE_GAMEOBJECT(m_arrSkillHud[i].m_pUIInstance[j], GROUP_UI);
		}
	}

	__super::Start();

	for (int i = 0; i < 4; ++i)
	{
		GET_COMPONENT_FROM(m_arrSkillHud[i].m_pUIInstance[SkillHud::ICON], CShader)->CallBack_SetRawValues += bind(&CUI_Skill::Set_ShaderResources_Relic, this, placeholders::_1, "g_fValue");
	}

	return S_OK;
}

void CUI_Skill::Set_ShaderResources(CShader* pShader, const char* pConstName)
{
}

void CUI_Skill::Set_ShaderResources_Relic(CShader* pShader, const char* pConstName)
{
	for (int i = 0; i < 4; ++i)
	{
		CTexture* pTexture = GET_COMPONENT_FROM(m_arrSkillHud[i].m_pUIInstance[SkillHud::ICON], CTexture);
		m_iRelicIndex = GET_COMPONENT_FROM(m_arrSkillHud[i].m_pUIInstance[SkillHud::ICON], CTexture)->Get_CurTextureIndex();

		if (m_iRelicIndex == 29)
		{
			CShader* pShader = GET_COMPONENT_FROM(m_arrSkillHud[i].m_pUIInstance[SkillHud::ICON], CShader);
			pShader->SetUp_ShaderResources(pTexture, "g_NoiseTexture");
		}
	}

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
		for (_uint j = 0; j < SkillHud::NAME_END; ++j)
		{
			DISABLE_GAMEOBJECT(m_arrSkillHud[i].m_pUIInstance[j], GROUP_UI);
		}
	}

	for (_uint i = 0; i < iIndex; ++i)
	{
		float fPosX = 480.f - 55.f * i;

		for (_uint j = 0; j < SkillHud::NAME_END; ++j)
		{
			CRenderer* pRenderer = GET_COMPONENT_FROM(m_arrSkillHud[i].m_pUIInstance[SkillHud::ICON], CRenderer);
			pRenderer->Set_Pass(VTXTEX_PASS_UI_RELIC);

			_float4 vPos = m_arrSkillHud[i].m_pUIInstance[j]->Get_Transform()->Get_World(WORLD_POS);
			m_arrSkillHud[i].m_pUIInstance[j]->Set_Pos(fPosX, vPos.y);

			if (j == 1)
				m_arrSkillHud[i].m_pUIInstance[j]->Set_Sort(0.1f);

			ENABLE_GAMEOBJECT(m_arrSkillHud[i].m_pUIInstance[j], GROUP_UI);
		}
	}
}

void CUI_Skill::Set_SkillIcon(_uint iIndex, _uint iKeyIdx, _uint iIconIdx, bool bRelic)
{
	if (bRelic == true)
	{
		DISABLE_GAMEOBJECT(m_arrSkillHud[iIndex].m_pUIInstance[SkillHud::BG]);
		DISABLE_GAMEOBJECT(m_arrSkillHud[iIndex].m_pUIInstance[SkillHud::OUTLINE]);
	}
	else
	{
		GET_COMPONENT_FROM(m_arrSkillHud[iIndex].m_pUIInstance[SkillHud::BG], CTexture)->Set_CurTextureIndex(iIconIdx);
		GET_COMPONENT_FROM(m_arrSkillHud[iIndex].m_pUIInstance[SkillHud::OUTLINE], CTexture)->Set_CurTextureIndex(iIconIdx);
	}

	if (iKeyIdx == 99)
	{
		DISABLE_GAMEOBJECT(m_arrSkillHud[iIndex].m_pUIInstance[SkillHud::KEY]);
	}
	else if (iKeyIdx == 46)
	{
		_float4 vScale = m_arrSkillHud[iIndex].m_pUIInstance[SkillHud::KEY]->Get_Transform()->Get_Scale();
		m_arrSkillHud[iIndex].m_pUIInstance[SkillHud::KEY]->Set_Scale(31.5f, vScale.y);

		GET_COMPONENT_FROM(m_arrSkillHud[iIndex].m_pUIInstance[SkillHud::KEY], CTexture)->Set_CurTextureIndex(iKeyIdx);
	}
	else
	{
		_float4 vScale = m_arrSkillHud[iIndex].m_pUIInstance[SkillHud::KEY]->Get_Transform()->Get_Scale();
		m_arrSkillHud[iIndex].m_pUIInstance[SkillHud::KEY]->Set_Scale(21.f, vScale.y);

		GET_COMPONENT_FROM(m_arrSkillHud[iIndex].m_pUIInstance[SkillHud::KEY], CTexture)->Set_CurTextureIndex(iKeyIdx);
	}

	GET_COMPONENT_FROM(m_arrSkillHud[iIndex].m_pUIInstance[SkillHud::ICON], CTexture)->Set_CurTextureIndex(iIconIdx);
}

void CUI_Skill::My_Tick()
{
	__super::My_Tick();

	static int iIndex = 0;

	if (KEY(E, TAP))
	{
		iIndex++;
		if (iIndex >= 10)
			iIndex = 0;
	}

	if (KEY(Q, TAP))
	{
		iIndex--;
		if (iIndex < 0)
			iIndex = 9;
	}

	Set_SkillUI(iIndex);

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
