#pragma once
#include "Client_Defines.h"
BEGIN(Engine)
class CGameObject;
END;

BEGIN(Client)

class CTable_Conditions;
class CTrigger;
class CPlayer;

class CBehavior
{
	friend class CAIPersonality;
	friend class CTable_Conditions;

	DECLARE_PROTOTYPE(CBehavior)

protected:
	CBehavior(eBehaviorType BehaviorType, CTable_Conditions* pTable);
	CBehavior(const CBehavior& rhs);
	virtual ~CBehavior();

public:
	static CBehavior* Create(eBehaviorType BehaviorType, CTable_Conditions* pTable);
	void Release();

public:
	HRESULT Initialize_Prototype();
	HRESULT Initialize();
	HRESULT Start();

public:
	void Add_WhatCondition(wstring strWhatConditionName);
	void Add_OtherCondition(wstring strOtherConditionName);

	BEHAVIOR_DESC* Check_Condition(_bool& bOut, CPlayer* pPlayer, CAIController* pAIController);

	eBehaviorType Get_BehaviorType() { return m_eBehaviorType; }
	BEHAVIOR_DESC* Get_BehaviorDesc() { return m_pBehaviorDesc; }

public:
	//타겟과 관련된 조건검사
	CDelegate <_bool&, BEHAVIOR_DESC* & , CPlayer*, CAIController*>Callback_WhatCondition;
	//타겟 이외의 모든 것에 대한 조건검사
	CDelegate<_bool&, CPlayer*, CAIController*>  Callback_OtherCondition;

private:
	BEHAVIOR_DESC* m_pBehaviorDesc = nullptr;
	eBehaviorType m_eBehaviorType = eBehaviorType::ePatrol;
	_uint m_iStateType = 0;
	CTable_Conditions* m_pConditionTable = nullptr;

};
END
