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
	virtual void My_Tick() override;
	virtual void My_LateTick() override;

public:
	void SetActive_MiniMap(_bool value);

private:
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	LEVEL_TYPE_CLIENT m_eLoadLevel = LEVEL_END;

	CUI_Object* m_pMiniMap = nullptr;

	enum MiniMapPoint { MP_Outline, MP_Gauge, MP_Text, MP_End };
	enum Point { Point_A, Point_R, Point_C, Point_End };
	CUI_Object* m_pMiniMapPoint[MP_End];
	CUI_Object* m_pArrMiniMapPoint[Point_End][MP_End];

private:
	void Create_MiniMap();
	void Create_MiniMapPoint();

	void Init_MiniMap();
};

END