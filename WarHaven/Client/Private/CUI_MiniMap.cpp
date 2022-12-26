#include "CUI_MiniMap.h"
#include "CUI_Object.h"
#include "GameInstance.h"
#include "Loading_Manager.h"
#include "Texture.h"
#include "CUI_Renderer.h"
#include "CShader.h"
#include "CUser.h"
#include "CPlayer.h"
#include "CPlayerInfo.h"
#include "CUtility_Transform.h"
#include "CUnit.h"
#include "Transform.h"
#include "CTeamConnector.h"
#include "Functor.h"

HRESULT CUI_MiniMap::Initialize_Prototype()
{
	Create_MiniMap();
	Create_MiniMapPoint();
	Create_PlayerIcon();

	return S_OK;
}

HRESULT CUI_MiniMap::Start()
{
	__super::Start();

	m_eLoadLevel = CLoading_Manager::Get_Instance()->Get_LoadLevel();

	Init_MiniMap();
	Init_MiniMapPoint();
	Init_PlayerIcon();

	Bind_Shader();

	return S_OK;
}

void CUI_MiniMap::Set_Shader_Guage_PointA(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_fValue", &m_fConquestRatio[Point_A], sizeof(_float));
}

void CUI_MiniMap::Set_Shader_Guage_PointR(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_fValue", &m_fConquestRatio[Point_R], sizeof(_float));
}

void CUI_MiniMap::Set_Shader_Guage_PointC(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_fValue", &m_fConquestRatio[Point_C], sizeof(_float));
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

	for (int i = 0; i < 8; ++i)
	{
		m_pPlayerIcon[i]->SetActive(value);
	}
}

void CUI_MiniMap::Set_ConquestTime(_uint iPointIdx, _float fConquestTime, _float fMaxConquestTime)
{
	_float fConquestRatio = 1.f - (fConquestTime / fMaxConquestTime);

	switch (iPointIdx)
	{
	case 0:
		m_fConquestRatio[Point_A] = fConquestRatio;
		break;
	case 1:
		m_fConquestRatio[Point_R] = fConquestRatio;
		break;
	case 2:
		m_fConquestRatio[Point_C] = fConquestRatio;
		break;
	}
}

void CUI_MiniMap::Set_GaugeColor(_bool IsMainTeam, _uint iPointIdx)
{
	if (IsMainTeam)
	{
		m_pArrMiniMapPoint[iPointIdx][MP_Gauge]->Set_Color(m_vColorBlue);
	}
	else
	{
		m_pArrMiniMapPoint[iPointIdx][MP_Gauge]->Set_Color(m_vColorRed);
	}
}

void CUI_MiniMap::Set_PointColor(_bool IsMainTeam, _uint iPoinIdx)
{
	_float4 vColor;
	vColor = IsMainTeam ? m_vColorBlue : m_vColorRed;

	for (int i = 0; i < MP_Text; ++i)
	{
		m_pArrMiniMapPoint[iPoinIdx][i]->Set_Color(vColor);
	}
}

void CUI_MiniMap::Set_Player(CPlayer* pPlayer)
{
	_bool isMainTeam = pPlayer->Get_Team()->IsMainPlayerTeam();
	if (!isMainTeam)
		return;

	_bool isMainPlayer = pPlayer->IsMainPlayer();
	if (isMainPlayer)
	{
		m_pPlayerTransform[0] = pPlayer->Get_Transform();
	}
	else
	{
		if (pPlayer->Get_OutlineType() == CPlayer::eSQUADMEMBER)
		{
			if (m_iMainSquadIdx > m_iMainSquadMaxIdx)
				return;

			m_pPlayerTransform[m_iMainSquadIdx++] = pPlayer->Get_Transform();
		}
		else
		{
			if (m_iMainTeamIdx > m_iMainTeamMaxIdx)
				return;

			m_pPlayerTransform[m_iMainTeamIdx++] = pPlayer->Get_Transform();
		}
	}

	cout << CFunctor::To_String(pPlayer->Get_PlayerName()) << endl;
}

void CUI_MiniMap::My_Tick()
{
	__super::My_Tick();
}

void CUI_MiniMap::My_LateTick()
{
	__super::My_LateTick();

	for (int i = 0; i < 8; ++i)
	{
		_float4 vPos = m_pPlayerTransform[i]->Get_World(WORLD_POS);
		m_pPlayerIcon[i]->Set_Pos(vPos.z, -vPos.x);
	}

	//_float4 vPos = m_pPlayerTransform[0]->Get_World(WORLD_POS);

	//_float fIconPosY = (vPos.x * 0.5f);

	// cout << "변환 위치 : X : " << -vPos.z << ", Z : " << fIconPosY << endl;

	//m_pPlayerIcon->Set_Pos(-vPos.z, -fIconPosY);
	//m_pPlayerIcon[0]->Set_Pos(vPos.z, vPos.x);
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

void CUI_MiniMap::Bind_Shader()
{
	GET_COMPONENT_FROM(m_pArrMiniMapPoint[Point_A][MP_Gauge], CShader)->CallBack_SetRawValues += bind(&CUI_MiniMap::Set_Shader_Guage_PointA, this, placeholders::_1, "g_fValue");
	GET_COMPONENT_FROM(m_pArrMiniMapPoint[Point_R][MP_Gauge], CShader)->CallBack_SetRawValues += bind(&CUI_MiniMap::Set_Shader_Guage_PointR, this, placeholders::_1, "g_fValue");
	GET_COMPONENT_FROM(m_pArrMiniMapPoint[Point_C][MP_Gauge], CShader)->CallBack_SetRawValues += bind(&CUI_MiniMap::Set_Shader_Guage_PointC, this, placeholders::_1, "g_fValue");
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
			m_pMiniMapPoint[i]->Set_Sort(0.51f);

			break;

		case MP_Text:

			GET_COMPONENT_FROM(m_pMiniMapPoint[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pMiniMapPoint[i], "/Paden/Text", "Text");

			m_pMiniMapPoint[i]->Set_Scale(m_pPointTextScale);
			m_pMiniMapPoint[i]->Set_Sort(0.49f);

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

void CUI_MiniMap::Create_PlayerIcon()
{
	for (int i = 0; i < 8; ++i)
	{
		m_pPlayerIcon[i] = CUI_Object::Create();

		m_pPlayerIcon[i]->Set_Sort(0.48f);

		m_pPlayerIcon[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/MiniMap/T_MinimapPlayerIcon.dds"));

		if (i == 0)
		{
			m_pPlayerIcon[i]->Set_Scale(20.f);
		}
		else
		{
			m_pPlayerIcon[i]->Set_Scale(15.f);

			if (i < 4)
			{
				m_pPlayerIcon[i]->Set_Color(m_vColorLightGreen);
			}
			else
			{
				m_pPlayerIcon[i]->Set_Color(m_vColorBlue);
			}
		}

		CREATE_GAMEOBJECT(m_pPlayerIcon[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pPlayerIcon[i]);
	}
}

void CUI_MiniMap::Init_MiniMap()
{
	switch (m_eLoadLevel)
	{
	case Client::LEVEL_PADEN:

		m_pMiniMap->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Map/MiniMap/T_MinimapPadenBlack.dds"));
		//m_pMiniMap->Set_Pos(-500.f, 250.f);
		//m_pMiniMap->Set_RotationZ(180.f);
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

void CUI_MiniMap::Init_PlayerIcon()
{
	switch (m_eLoadLevel)
	{
	case Client::LEVEL_PADEN:
	{
		// 플레이어가 생성된 위치에 따라 아이콘 생성 위치 지정
		// 플레이어는 기본적으로 c 거점을 등지고 서있으니 룩벡터를 다르게 지정해서 똑같이 이동시키자

		_uint iTeamType = (_uint)CUser::Get_Instance()->Get_MainPlayerInfo()->Get_TeamType();
		switch (iTeamType)
		{
		case 0:	// Red

			//m_pPlayerIcon->Set_Pos(-550.f, 280.f);
			m_pPlayerIcon[0]->Set_Color(m_vColorRed);

			break;

		case 1:	// Blue

			//m_pPlayerIcon->Set_Pos(-450.f, 280.f);
			m_pPlayerIcon[0]->Set_Color(m_vColorBlue);

			break;
		}
	}
	break;

	case Client::LEVEL_HWARA:
		break;
	}
}
