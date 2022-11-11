#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUI_Portrait final : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_Portrait);
	DECLARE_GAMEOBJECT(CUI_Portrait);

	typedef struct tagPortraitHud
	{
		enum SKILLHUDNAME { BG, Port, NAME_END };
		CUI_Object* m_pUIInstance[NAME_END] = {};
	}Portrait;

private:
	CUI_Portrait();
	CUI_Portrait(const CUI_Portrait& Prototype);
	virtual ~CUI_Portrait();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

public:
	void Set_Portrait(_uint iIndex);

protected:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;

private:
	Portrait m_tPort;
	Portrait m_arrtPort[5];

	// 스몰이랑 빅 배경
	// 스몰 오끝 480 -230
	// 빅 -580 -280

	// 포트는 알지?

	// 히어로 포트는 돌면서 들어감, 나타남

	// 변신할 때 돌면서 자기와 변신할 사진 페이드하며 늘줄늘줄
};

END