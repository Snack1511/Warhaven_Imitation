#include "CUI_MiniMap.h"
#include "CUI_Object.h"
#include "GameInstance.h"
#include "Loading_Manager.h"
#include "Texture.h"
#include "CUI_Renderer.h"

HRESULT CUI_MiniMap::Initialize_Prototype()
{
	Create_MiniMap();
	Create_MiniMapPoint();

	return S_OK;
}

HRESULT CUI_MiniMap::Initialize()
{
	return S_OK;
}

HRESULT CUI_MiniMap::Start()
{
	__super::Start();

	m_eLoadLevel = CLoading_Manager::Get_Instance()->Get_LoadLevel();

	Init_MiniMap();
	Init_MiniMapPoint();

	return S_OK;
}

void CUI_MiniMap::My_Tick()
{
	__super::My_Tick();
}

void CUI_MiniMap::My_LateTick()
{
	__super::My_LateTick();
}

void CUI_MiniMap::SetActive_MiniMap(_bool value)
{
	m_pMiniMap->SetActive(value);

	for (int i = 0; i < Point_End; ++i)
	{
		for (int j = 0; j < MP_End; ++j)
		{
			m_pArrMiniMapPoint[i][j]->SetActive(value);
		}
	}
}

void CUI_MiniMap::OnEnable()
{
	__super::OnEnable();

	SetActive_MiniMap(true);
}

void CUI_MiniMap::OnDisable()
{
	__super::OnDisable();

	SetActive_MiniMap(false);
}

void CUI_MiniMap::Create_MiniMap()
{
	m_pMiniMap = CUI_Object::Create();

	m_pMiniMap->Set_Color(_float4(1.f, 1.f, 1.f, 0.5f));

	m_pMiniMap->Set_Sort(0.51f);

	CREATE_GAMEOBJECT(m_pMiniMap, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pMiniMap);
}

void CUI_MiniMap::Create_MiniMapPoint()
{
	for (int i = 0; i < MP_End; ++i)
	{
		m_pMiniMapPoint[i] = CUI_Object::Create();

		switch (i)
		{
		case MP_Outline:

			GET_COMPONENT_FROM(m_pMiniMapPoint[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pMiniMapPoint[i], "/Paden", "Point");

			m_pMiniMapPoint[i]->Set_Color(m_vColorOutline);

			m_pMiniMapPoint[i]->Set_Scale(m_pPointOutlineScale);
			m_pMiniMapPoint[i]->Set_Sort(0.5f);

			break;

		case MP_Gauge:

			GET_COMPONENT_FROM(m_pMiniMapPoint[i], CUI_Renderer)->Set_Pass(VTXTEX_PASS_UI_VerticalGauge);

			GET_COMPONENT_FROM(m_pMiniMapPoint[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pMiniMapPoint[i], "/Paden", "Gauge");

			m_pMiniMapPoint[i]->Set_Color(m_vColorGauge);

			m_pMiniMapPoint[i]->Set_Scale(m_pPointGaugeScale);
			m_pMiniMapPoint[i]->Set_Sort(0.49f);

			break;

		case MP_Text:

			GET_COMPONENT_FROM(m_pMiniMapPoint[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pMiniMapPoint[i], "/Paden/Text", "Text");

			m_pMiniMapPoint[i]->Set_Scale(m_pPointTextScale);
			m_pMiniMapPoint[i]->Set_Sort(0.48f);

			break;
		}

		CREATE_GAMEOBJECT(m_pMiniMapPoint[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pMiniMapPoint[i]);

		for (int j = 0; j < Point_End; ++j)
		{
			m_pArrMiniMapPoint[j][i] = m_pMiniMapPoint[i]->Clone();

			CREATE_GAMEOBJECT(m_pArrMiniMapPoint[j][i], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pArrMiniMapPoint[j][i]);
		}
	}
}

void CUI_MiniMap::Init_MiniMap()
{
	switch (m_eLoadLevel)
	{
	case Client::LEVEL_PADEN:

		m_pMiniMap->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Map/MiniMap/T_MinimapPadenBlack.dds"));
		m_pMiniMap->Set_Pos(-500.f, 250.f);
		m_pMiniMap->Set_Scale(250.f);

		break;
	case Client::LEVEL_HWARA:
		break;
	}
}

void CUI_MiniMap::Init_MiniMapPoint()
{
	switch (m_eLoadLevel)
	{
	case Client::LEVEL_PADEN:

		for (int i = 0; i < MP_End; ++i)
		{
			for (int j = 0; j < Point_End; ++j)
			{
				m_pArrMiniMapPoint[j][i]->Set_PosX(-500.f);

				if (j > Point_A)
				{
					GET_COMPONENT_FROM(m_pArrMiniMapPoint[j][MP_Outline], CTexture)->Set_CurTextureIndex(j);
					GET_COMPONENT_FROM(m_pArrMiniMapPoint[j][MP_Gauge], CTexture)->Set_CurTextureIndex(1);
					GET_COMPONENT_FROM(m_pArrMiniMapPoint[j][MP_Text], CTexture)->Set_CurTextureIndex(j);
				}
			}

			if (i == MP_Text)
			{
				m_pArrMiniMapPoint[Point_A][i]->Set_PosY(237.f);
				m_pArrMiniMapPoint[Point_R][i]->Set_PosY(189.f);
				m_pArrMiniMapPoint[Point_C][i]->Set_PosY(304.f);

				continue;
			}

			m_pArrMiniMapPoint[Point_A][i]->Set_PosY(238.f);
			m_pArrMiniMapPoint[Point_R][i]->Set_PosY(190.f);
			m_pArrMiniMapPoint[Point_C][i]->Set_PosY(305.f);
		}

		break;

	case Client::LEVEL_HWARA:
		break;
	}
}
