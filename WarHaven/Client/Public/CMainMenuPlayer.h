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
	// �̰� �� �ٲٴ� �Ű�
	void		Set_CurClassType(CLASS_TYPE eClassType);
	// �̰� ��Ų �ٲٴ°ǵ�
	void		Change_ModelParts(CLASS_TYPE eClassType, MODEL_PART_TYPE eModelPartType);

	// ���� �����̾��������� ������ ������ߵȴ�

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