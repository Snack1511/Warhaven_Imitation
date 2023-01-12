#pragma once
#include "Client_Defines.h"
BEGIN(Engine)
class CGameObject;
END;

BEGIN(Client)

class CTable_Conditions;
class CTrigger;
class CPlayer;
class CAIController;

class CBehavior
{
	friend class CAIPersonality;
	friend class CTable_Conditions;

	DECLARE_PROTOTYPE(CBehavior)
public:
	enum class eConditionType
	{
		eWhen,
		eWhat,
		eTick,
		eCount,
	};
protected:
	CBehavior(eBehaviorType BehaviorType, CTable_Conditions* pTable);
	CBehavior(const CBehavior& rhs);
	virtual ~CBehavior();

public:
	static CBehavior* Create(eBehaviorType BehaviorType, CTable_Conditions* pTable);
	void Release();
public:
	static CBehavior* Load(wstring strSaveName, CTable_Conditions* pTable);
	void Save(wstring strPersonalityName);
public:
	void Save_Functions(ofstream& rhsWriteFile, vector<wstring>& rhsDatas);
	void Load_Functions(ifstream& rhsReadFile, const function<void(wstring)>& Func);
public:
	HRESULT Initialize_Prototype();
	HRESULT Initialize();
	HRESULT Start();
public:
	void Add_WhatCondition(wstring strWhatConditionName);
	void Add_OtherCondition(wstring strOtherConditionName);
	void Add_CallBack_Tick(wstring strCallbackTickName);
	void Add_Condition(wstring strConditionName, eConditionType eType);
	void Add_BehaviorTick(wstring strBehaviorTickName);
	void Set_Priority(_int iPriorityScore);
	_uint Get_Priority();

	void Delete_WhatCondition(wstring strWhatConditionName);
	void Delete_OtherCondition(wstring strOtherConditionName);
	void Delete_CallBack_Tick(wstring strBehaviorTickName);
	void Delete_Condition(wstring strConditionName, eConditionType eType);

	void Clear_Condition(eConditionType eType);

	BEHAVIOR_DESC* Check_Condition(_bool& bOut, CPlayer* pPlayer, CAIController* pAIController);

	eBehaviorType Get_BehaviorType() { return m_eBehaviorType; }
	BEHAVIOR_DESC* Get_BehaviorDesc() { return m_pBehaviorDesc; }
	wstring Get_BehaviorName() { return m_strDebugBehaviorName; }
	void Set_BehaviorName(wstring strDebugNames) { m_strDebugBehaviorName = strDebugNames; }
	vector<wstring>& Get_ConditionNames(eConditionType eType);
	vector<wstring>& Get_BehaviorTickNames();
public:
	//타겟과 관련된 조건검사
	CDelegate <_bool&, BEHAVIOR_DESC* & , CPlayer*, CAIController*>Callback_WhatCondition;
	//타겟 이외의 모든 것에 대한 조건검사
	CDelegate<_bool&, CPlayer*, CAIController*>  Callback_OtherCondition;
	/*비해비어 틱*/
	CDelegate<CPlayer*, CAIController*> Callback_BehaviorTick;

private:
	vector<wstring> m_strConditionName[_uint(eConditionType::eCount)];
	vector<wstring> m_strBehaviorTickName;
	BEHAVIOR_DESC* m_pBehaviorDesc = nullptr;
	eBehaviorType m_eBehaviorType = eBehaviorType::ePatrol;
	_uint m_iStateType = 0;
	CTable_Conditions* m_pConditionTable = nullptr;
	wstring m_strDebugBehaviorName;
};
END
