#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
END

BEGIN(Client)

class CPlayerInfo;
class CMainMenuUnit;

class CMainMenuPlayer : public CGameObject
{
	DECLARE_PROTOTYPE(CMainMenuPlayer);

private:
	CMainMenuPlayer();
	virtual ~CMainMenuPlayer();

public:
	static CMainMenuPlayer* Create(CPlayerInfo* pMainPlayerInfo);

public:
	// 이게 모델 바꾸는 거고
	void		Set_CurClassType(CLASS_TYPE eClassType);
	// 이게 스킨 바꾸는건데
	void		Change_ModelParts(CLASS_TYPE eClassType, MODEL_PART_TYPE eModelPartType);

	// 메인 프렐이어이인포를 갱신을 시켜줘야된대

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

private:
	CLASS_TYPE		m_eCurClassType = WARRIOR;
	CPlayerInfo* m_pMainPlayerInfo = nullptr;
	CMainMenuUnit* m_pMainMenuUnit[CLASS_END] = {};

private:
	virtual void My_Tick() override;

};

END