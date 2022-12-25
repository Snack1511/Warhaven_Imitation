#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUI_MiniMap : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_MiniMap);
	DECLARE_GAMEOBJECT(CUI_MiniMap);

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

public:
	void SetActive_MiniMap(_bool value);

	void Set_ConquestTime(_uint iPointIdx, _float fConquestTime, _float fMaxConquestTime);
	void Set_GaugeColor(_bool IsMainTeam, _uint iPointIdx);
	void Set_PointColor(_bool IsMainTeam, _uint iPoinIdx);

public:
	virtual void Set_Shader_Guage_PointA(CShader* pShader, const char* pConstName);
	virtual void Set_Shader_Guage_PointR(CShader* pShader, const char* pConstName);
	virtual void Set_Shader_Guage_PointC(CShader* pShader, const char* pConstName);

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

	void Bind_Shader();

private:
	LEVEL_TYPE_CLIENT m_eLoadLevel = LEVEL_END;

	CUI_Object* m_pMiniMap = nullptr;

	enum Point { Point_A, Point_R, Point_C, Point_End };
	enum MiniMapPoint { MP_Outline, MP_Gauge, MP_Text, MP_End };

	CUI_Object* m_pMiniMapPoint[MP_End];
	CUI_Object* m_pArrMiniMapPoint[Point_End][MP_End];

	CUI_Object* m_pPlayerIcon[8];

	_float m_fConquestTime[Point_End];
	_float m_fMaxConquestTime[Point_End];
	_float m_fConquestRatio[Point_End];

	_float m_pPointOutlineScale = 20.f;
	_float m_pPointGaugeScale = 15.f;
	_float m_pPointTextScale = 25.f;

	_float4 m_vColorOutline = _float4(0.7f, 0.7f, 0.7f, 1.f);
	_float4 m_vColorGauge = _float4(0.5f, 0.5f, 0.5f, 0.5f);

	_float4 m_vColorBlue = _float4(0.1f, 0.6f, 1.f, 0.9f);
	_float4 m_vColorRed = _float4(1.f, 0.2f, 0.1f, 0.9f);

private:
	void Create_MiniMap();
	void Create_MiniMapPoint();
	void Create_PlayerIcon();

	void Init_MiniMap();
	void Init_MiniMapPoint();
	void Init_PlayerIcon();
};

END