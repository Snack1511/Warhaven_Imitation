#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CPlayer;

class CUI_MiniMap : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_MiniMap);
	DECLARE_GAMEOBJECT(CUI_MiniMap);

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual HRESULT	Start();

public:
	virtual void Set_Shader_Guage_PointA(CShader* pShader, const char* pConstName);
	virtual void Set_Shader_Guage_PointR(CShader* pShader, const char* pConstName);
	virtual void Set_Shader_Guage_PointC(CShader* pShader, const char* pConstName);
	virtual void Set_Shader_Guage_PointE(CShader* pShader, const char* pConstName);

public:
	void SetActive_MiniMap(_bool value);

	void Set_ConquestTime(string strPadenPointKey, _float fConquestTime, _float fMaxConquestTime);
	void Set_ConquestTime(_uint iPointIdx, _float fConquestTime, _float fMaxConquestTime);
	void Set_GaugeColor(_bool IsMainTeam, _uint iPointIdx);
	void Set_PointColor(_bool IsMainTeam, _uint iPoinIdx);

	void Set_Player(CPlayer* pPlayer);

	void Set_BattleIcon(_bool IsBattle);

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

	void Bind_Shader();

private:
	LEVEL_TYPE_CLIENT m_eLoadLevel = LEVEL_END;

	CUI_Object* m_pMiniMap = nullptr;

	enum Point { Point_A, Point_R, Point_C, Point_E, Point_End };
	enum MiniMapPoint { MP_Outline, MP_Gauge, MP_Text, MP_End };

	CUI_Object* m_pMiniMapPoint[MP_End];
	CUI_Object* m_pArrMiniMapPoint[Point_End][MP_End];

private:
	eTEAM_TYPE m_eTeamType = eTEAM_TYPE::eCOUNT;

	void Set_TeamType();

private:
	CUI_Object* m_pPlayerIcon[8];

	CPlayer* m_pPlayers[8];
	CTransform* m_pPlayerTransform[8];

	_uint m_iMainSquadIdx = 1;
	_uint m_iMainSquadMaxIdx = 3;

	_uint m_iMainTeamIdx = 4;
	_uint m_iMainTeamMaxIdx = 8;

	_float m_fIconOffsetX = -240.f;
	_float m_fIconOffsetY = -500.f;

	_float m_fHwaraOffSetX = 500.f;
	_float m_fHwaraOffSetY = 230.f;

	_bool m_bIsBattle = false;
	_uint m_iStartCount = 0;

private:
	_float m_fConquestTime[Point_End];
	_float m_fMaxConquestTime[Point_End];
	_float m_fConquestRatio[Point_End];

	_float m_pPointOutlineScale = 20.f;
	_float m_pPointGaugeScale = 15.f;
	_float m_pPointTextScale = 25.f;

	_float4 m_vColorOutline = _float4(0.7f, 0.7f, 0.7f, 1.f);
	_float4 m_vColorGauge = _float4(0.5f, 0.5f, 0.5f, 0.5f);

	_float4 m_vColorRed = _float4(1.f, 0.2f, 0.1f, 0.9f);
	_float4 m_vColorBlue = _float4(0.1f, 0.6f, 1.f, 0.9f);
	_float4 m_vColorLightGreen = _float4(0.6f, 0.85f, 0.3f, 1.f);

private:
	void Create_MiniMap();
	void Create_MiniMapPoint();
	void Create_PlayerIcon();

	void Init_MiniMap();
	void Init_MiniMapPoint();
};

END