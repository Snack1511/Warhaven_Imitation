#include "CTable_Conditions.h"
#include "Functor.h"
#include "CAIController.h"
#include "CPlayer.h"
#include "CUnit.h"
#include "GameInstance.h"
#include "Transform.h"
#include "CAIPersonality.h"
#include "CTrigger.h"
#include "CTeamConnector.h"
#include "CSquad.h"
#include "CBehavior.h"
#include "CUser.h"
#include "CPath.h"
#include "CPlayer.h"
#include "CNavigation.h"
#include "CGameSystem.h"
#include "CTrigger_Stage.h"
#include "CTrigger_Glider.h"
#include "CCannon.h"



#define    MAX_FOLLOW_LENGTH 3.f

#define CHECKFALSEOUTCONDITION(OutCondition)\
if (OutCondition == false)\
{\
    return;\
}

CTable_Conditions::CTable_Conditions()
{
}

CTable_Conditions::~CTable_Conditions()
{
	Release();
}

CTable_Conditions* CTable_Conditions::Create()
{
	CTable_Conditions* pInstance = new CTable_Conditions();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CTable_Conditions");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CTable_Conditions::Release()
{
	for (auto& elem : m_mapAllBehaviors)
		SAFE_DELETE(elem.second);

	m_mapAllBehaviors.clear();
}

#define Add_WhyCondition(strFunctionName, Function)\
m_OtherConditions.emplace(\
Convert_ToHash(strFunctionName),\
bind(&CTable_Conditions::Function,\
    this, placeholders::_1, placeholders::_2, placeholders::_3));\
m_vecStrConditionName[_uint(eBehaviorConditionType::eWhen)].push_back(strFunctionName)

#define Add_WhatCondition(strFunctionName, Function)\
m_WhatConditions.emplace(\
Convert_ToHash(strFunctionName),\
bind(&CTable_Conditions::Function,\
    this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4));\
m_vecStrConditionName[_uint(eBehaviorConditionType::eWhat)].push_back(strFunctionName)

#define Add_BehaviorTick(strBehaviorTickName, BehaviorTick)\
    m_BehaviorTick.emplace(\
    Convert_ToHash(strBehaviorTickName),\
    bind(&CTable_Conditions::BehaviorTick,\
        this, placeholders::_1, placeholders::_2));\
m_vecStrConditionName[_uint(eBehaviorConditionType::eTick)].push_back(strBehaviorTickName);

HRESULT CTable_Conditions::Initialize()
{
	if (FAILED(SetUp_Conditions()))
		return E_FAIL;

	if (FAILED(SetUp_Behaviors()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTable_Conditions::SetUp_Conditions()
{
	Add_WhyCondition(wstring(L"EmptyOtherCondition"), EmptyOtherCondition);

#pragma region 플레이어 체크
	Add_WhyCondition(wstring(L"Check_FarAwayLeader"), Check_FarAwayLeader);
	Add_WhyCondition(wstring(L"Check_LookEnemy"), Check_LookEnemy);
	Add_WhyCondition(wstring(L"Check_DeadAllies"), Check_DeadAllies);
	Add_WhyCondition(wstring(L"Check_ValidPath"), Check_ValidPath);
	Add_WhyCondition(wstring(L"Check_ReviveTeam"), Check_ReviveTeam);
#pragma endregion 플레이어 체크

#pragma region 맵 체크
	Add_WhyCondition(wstring(L"Check_Paden"), Check_Paden);
	Add_WhyCondition(wstring(L"Check_Hwara"), Check_Hwara);
	Add_WhyCondition(wstring(L"Check_Need_Conquer"), Check_Need_Conquer);
	Add_WhyCondition(wstring(L"Check_Conquer_MainPoint"), Check_Conquer_MainPoint);
	Add_WhyCondition(wstring(L"Check_Conquer_Respawn"), Check_Conquer_Respawn);
	Add_WhyCondition(wstring(L"Check_Conquer_PadenCannon"), Check_Conquer_PadenCannon);
	Add_WhyCondition(wstring(L"Check_Conquer_HwaraFinal"), Check_Conquer_HwaraFinal);
	Add_WhyCondition(wstring(L"Check_UsableCannon"), Check_UsableCannon);
	Add_WhyCondition(wstring(L"Check_InCannonConquerTrigger"), Check_InCannonConquerTrigger);
	Add_WhyCondition(wstring(L"Check_GriderTrigger"), Check_GriderTrigger);
#pragma endregion 맵 체크

#pragma region 비해비어 체크
	Add_WhyCondition(wstring(L"Check_PatrolBehavior"), Check_PatrolBehavior);
	Add_WhyCondition(wstring(L"Check_GotoTriggerBehavior"), Check_GotoTriggerBehavior);
	Add_WhyCondition(wstring(L"Check_PadenCannonInteractBehavior"), Check_PadenCannonInteractBehavior);
	Add_WhyCondition(wstring(L"Check_ReviveBehavior"), Check_ReviveBehavior);
	Add_WhyCondition(wstring(L"Check_CombatBehavior"), Check_CombatBehavior);
	Add_WhyCondition(wstring(L"Check_ChangeBehavior"), Check_ChangeBehavior);
#pragma endregion 비해비어 체크

#pragma region 플레이어 상태 체크
	Add_WhyCondition(wstring(L"Check_AdjCannon"), Check_AdjCannon);
	Add_WhyCondition(wstring(L"Check_AbleHero"), Check_AbleHero);
	Add_WhyCondition(wstring(L"Check_EmptyRoute"), Check_EmptyRoute);
#pragma endregion 플레이어 상태 체크

#pragma region 패스 체크
	Add_WhyCondition(wstring(L"Check_GlidePath"), Check_GlidePath);
#pragma endregion 패스 체크


	Add_WhatCondition(wstring(L"EmptyWhatCondition"), EmptyWhatCondition);
#pragma region 플레이어 선택
	Add_WhatCondition(wstring(L"Select_Leader"), Select_Leader);
	Add_WhatCondition(wstring(L"Select_NearEnemy"), Select_NearEnemy);
	Add_WhatCondition(wstring(L"Select_NearAllies"), Select_NearAllies);
	Add_WhatCondition(wstring(L"Select_MainPlayer"), Select_MainPlayer);
	Add_WhatCondition(wstring(L"Select_Teammate"), Select_Teammate);
#pragma endregion 플레이어 선택

#pragma region 트리거 선택
	Add_WhatCondition(wstring(L"Select_ConquerTrigger"), Select_ConquerTrigger);
	Add_WhatCondition(wstring(L"Select_RandomConquerTrigger"), Select_RandomConquerTrigger);

	Add_WhatCondition(wstring(L"Select_PadenCannonTrigger"), Select_PadenCannonTrigger);
	Add_WhatCondition(wstring(L"Select_RespawnTrigger"), Select_RespawnTrigger);
	Add_WhatCondition(wstring(L"Select_MainTrigger"), Select_MainTrigger);
	Add_WhatCondition(wstring(L"Select_HwaraFinalTrigger"), Select_HwaraFinalTrigger);
#pragma endregion 트리거 선택


	Add_BehaviorTick(wstring(L"EmptyBehaviorTick"), EmptyBehaviorTick);
	Add_BehaviorTick(wstring(L"Callback_Tick_UpdatePatrol"), Callback_Tick_UpdatePatrol);
	return S_OK;
}


#define Add_Behavior(pBehaviorObject, strBehaviorName, BehaviorType)\
    pBehaviorObject = CBehavior::Create(BehaviorType, this);\
    pBehaviorObject->Set_BehaviorName(strBehaviorName);\
    m_mapAllBehaviors.emplace(Convert_ToHash(strBehaviorName), pBehavior);\
    m_vecBehaviorName.push_back(strBehaviorName);

HRESULT CTable_Conditions::SetUp_Behaviors()
{
	CBehavior* pBehavior = nullptr;
	Add_Behavior(pBehavior, wstring(L"Patrol"), eBehaviorType::ePatrol);
	Add_Behavior(pBehavior, wstring(L"PathFinding"), eBehaviorType::ePathFinding);
	Add_Behavior(pBehavior, wstring(L"Gliding"), eBehaviorType::eGliding);
	Add_Behavior(pBehavior, wstring(L"Revive"), eBehaviorType::eRevive);
	Add_Behavior(pBehavior, wstring(L"PadenCannonInteract"), eBehaviorType::ePadenCannonInteract);
	Add_Behavior(pBehavior, wstring(L"Combat"), eBehaviorType::eCombat);
	Add_Behavior(pBehavior, wstring(L"CatchCannon"), eBehaviorType::eCatchCannon);
	Add_Behavior(pBehavior, wstring(L"Change"), eBehaviorType::eChange);

	return S_OK;
}

function<void(_bool&, CPlayer*, CAIController*)> CTable_Conditions::Find_OtherCondition(wstring strConditionName)
{
	try {
		_hashcode hsConditionName = Convert_ToHash(strConditionName);

		auto ConditionIter = m_OtherConditions.find(hsConditionName);

		if (m_OtherConditions.end() == ConditionIter)
			throw strConditionName;
		return ConditionIter->second;
	}
	catch (wstring ConditionName)
	{
		string strMsg = "Not Found : ";
		strMsg += CFunctor::To_String(ConditionName);
		strMsg += " - Find_OtherCondition()";
		Make_Dump(typeid(CTable_Conditions).name(), strMsg);
		return bind(&CTable_Conditions::EmptyOtherCondition, this, placeholders::_1, placeholders::_2, placeholders::_3);
	}
}

function<void(_bool&, BEHAVIOR_DESC*&, CPlayer*, CAIController*)> CTable_Conditions::Find_WhatCondition(wstring strConditionName)
{
	try {
		_hashcode hsConditionName = Convert_ToHash(strConditionName);

		auto ConditionIter = m_WhatConditions.find(hsConditionName);

		if (m_WhatConditions.end() == ConditionIter)
			throw strConditionName;
		return ConditionIter->second;
	}
	catch (wstring ConditionName)
	{
		string strMsg = "Not Found : ";
		strMsg += CFunctor::To_String(ConditionName);
		strMsg += " - Find_WhatCondition()";
		Make_Dump(typeid(CTable_Conditions).name(), strMsg);
		return bind(&CTable_Conditions::EmptyWhatCondition, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);
	}
}

function<void(CPlayer*, CAIController*)> CTable_Conditions::Find_BehaviorTick(wstring strConditionName)
{
	try {
		_hashcode hsConditionName = Convert_ToHash(strConditionName);

		auto BehaviorIter = m_BehaviorTick.find(hsConditionName);

		if (m_BehaviorTick.end() == BehaviorIter)
			throw strConditionName;
		return BehaviorIter->second;
	}
	catch (wstring ConditionName)
	{
		string strMsg = "Not Found : ";
		strMsg += CFunctor::To_String(ConditionName);
		strMsg += " - Find_BehaviorTick()";
		Make_Dump(string("CTable_Condition"), strMsg);
		return nullptr;
	}
}

CBehavior* CTable_Conditions::Find_Behavior(wstring strBehavior)
{
	try {
		_hashcode hsConditionName = Convert_ToHash(strBehavior);

		auto BehaviorIter = m_mapAllBehaviors.find(hsConditionName);

		if (m_mapAllBehaviors.end() == BehaviorIter)
			throw strBehavior;
		return BehaviorIter->second;
	}
	catch (wstring ConditionName)
	{
		string strMsg = "Not Found : ";
		strMsg += CFunctor::To_String(ConditionName);
		strMsg += " - Find_Behavior()";
		Make_Dump(string("CTable_Condition"), strMsg);
		return nullptr;
	}
}
vector<wstring>& CTable_Conditions::Get_ConditionNames(_uint iConditionType)
{
	return m_vecStrConditionName[iConditionType];
}
vector<wstring>& CTable_Conditions::Get_BehaviorTickNames()
{
	return m_vecBehaviorTickName;
}
vector<wstring>& CTable_Conditions::Get_BehaviorNames()
{
	return m_vecBehaviorName;
}
#define CHECK_EMPTY(listname) if (listname.empty()) {OutCondition = false; return;}

void CTable_Conditions::Check_FarAwayLeader(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
	CHECKFALSEOUTCONDITION(OutCondition);

	CPlayer* pLeader = pPlayer->Get_Squad()->Get_LeaderPlayer();
	if (pLeader == pPlayer)
		OutCondition = false;

	_float4 vLeaderPos = pLeader->Get_WorldPos();
	_float4 vMyPos = pPlayer->Get_WorldPos();

	_float Length = (vLeaderPos - vMyPos).Length();

	if (Length > /*pAIController->Get_Personality()->Get_LeaderLengthLimit()*/5.f)
		OutCondition = true;
	else
		OutCondition = false;
}


void CTable_Conditions::Check_LookEnemy(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
	CHECKFALSEOUTCONDITION(OutCondition);


	//플레이어 Look방향 반원
	_float4 MyPositoin = pPlayer->Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS);

	list<CPlayer*>& Enemies = pAIController->Get_NearEnemy();
	CHECK_EMPTY(Enemies);

	for (auto iter = Enemies.begin(); iter != Enemies.end();)
	{
		//적이 죽은지 확인 --> 죽었으면 삭제
		_bool bEnemyDead = (*iter)->Is_Died();
		if (RemovePlayer(bEnemyDead, Enemies, iter))
		{
			continue;
		}
		else
		{
			_float4 vTargetPosition = (*iter)->Get_WorldPos();
			_float4 vDist = (vTargetPosition - MyPositoin);
			if (pAIController->Get_Personality()->Get_LimitRouteDistance() < vDist.Length())
			{
				_float4 vDir = vDist.Normalize();
				_float4 vMyLook = pPlayer->Get_LookDir();

				_float DotDir = vMyLook.Dot(vDir);

				//1보다 떨어진 적이 정면에 있는지 확인 --> 없으면 삭제 --> 못본거
				if (RemovePlayer((DotDir < 0.f), Enemies, iter))
					continue;
			}
		}
		iter++;

	}

	OutCondition = true;

}


void CTable_Conditions::Check_DeadAllies(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
	CHECKFALSEOUTCONDITION(OutCondition);


	//플레이어 Look방향 반원
	_float4 MyPositoin = pPlayer->Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS);

	list<CPlayer*>& Enemies = pAIController->Get_NearAllies();
	CHECK_EMPTY(Enemies);

	for (auto iter = Enemies.begin(); iter != Enemies.end();)
	{
		_bool bAlliesDead = ((*iter)->IsMainPlayer());
		_bool bRevival = !((*iter)->Is_AbleRevival());
		//돌이 안됬거나 부활 가능한 아군이 아니면 삭제
		if (RemovePlayer((!bAlliesDead || !bRevival), Enemies, iter))
		{
			continue;
		}
		//else
		//{
		//	_float4 vTargetPosition = (*iter)->Get_WorldPos();

		//	_float4 vDist = (vTargetPosition - MyPositoin);
		//	if (pAIController->Get_Personality()->Get_LimitRouteDistance() < vDist.Length())
		//	{
		//		_float4 vDir = vDist.Normalize();
		//		_float4 vMyLook = pPlayer->Get_LookDir();

		//		_float DotDir = vMyLook.Dot(vDir);

		//		//1보다 떨어진 돌이 된 아군이 정면에 있는지 확인 --> 없으면 삭제 --> 못본거
		//		if (RemovePlayer((DotDir < 0.f), Enemies, iter))
		//			continue;
		//	}
		//}
		iter++;

	}

	OutCondition = true;
}

void CTable_Conditions::Check_ValidPath(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
	CHECKFALSEOUTCONDITION(OutCondition);

	if (!pPlayer->Get_CurMainPath() || !pPlayer->Get_CurPath())
	{
		OutCondition = false;
		return;
	}



	//일단 MainPath는 다 타야해
	if (pPlayer->Get_CurPath()->Is_Arrived())
	{
		OutCondition = false;
		return;
	}




	OutCondition = true;

}

void CTable_Conditions::Check_CombatDelay(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
}

void CTable_Conditions::Check_Paden(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
	CHECKFALSEOUTCONDITION(OutCondition);
	if (Check_Level(LEVEL_PADEN))
		OutCondition = true;
	else OutCondition = false;
}

void CTable_Conditions::Check_Hwara(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
	CHECKFALSEOUTCONDITION(OutCondition);
	if (Check_Level(LEVEL_HWARA))
		OutCondition = true;
	else OutCondition = false;
}

void CTable_Conditions::Check_Need_Conquer(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
	CHECKFALSEOUTCONDITION(OutCondition);
	CTeamConnector* pMyTeamConnect = pPlayer->Get_Team();
	if (nullptr == pMyTeamConnect)
		OutCondition = false;
	if (Check_Level(LEVEL_PADEN))
	{
		if (!pMyTeamConnect->Has_MainTrigger()
			|| !pMyTeamConnect->Has_RespawnTrigger()
			|| !pMyTeamConnect->Has_CannonTrigger())
		{
			OutCondition = true;
		}
		else OutCondition = false;

	}
	else if (Check_Level(LEVEL_HWARA))
	{
		if (!pMyTeamConnect->Has_CenterTrigger()
			|| !pMyTeamConnect->Has_HwaraRespawnTrigger()
			|| !pMyTeamConnect->Has_HwaraFinalTrigger())
		{
			OutCondition = true;
		}
		else OutCondition = false;
	}
}

void CTable_Conditions::Check_Conquer_PadenCannon(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
	CHECKFALSEOUTCONDITION(OutCondition);
	CTeamConnector* pMyTeamConnect = pPlayer->Get_Team();
	if (nullptr == pMyTeamConnect)
		OutCondition = false;
	if (pMyTeamConnect->Has_CannonTrigger())
		OutCondition = true;
	else OutCondition = false;
}

void CTable_Conditions::Check_Conquer_Respawn(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
	CHECKFALSEOUTCONDITION(OutCondition);
	CTeamConnector* pMyTeamConnect = pPlayer->Get_Team();
	if (nullptr == pMyTeamConnect)
		OutCondition = false;

	if (Check_Level(LEVEL_PADEN))
	{
		if (pMyTeamConnect->Has_RespawnTrigger())
			OutCondition = true;
		else OutCondition = false;
	}
	else if (Check_Level(LEVEL_HWARA))
	{
		if (pMyTeamConnect->Has_HwaraRespawnTrigger())
			OutCondition = true;
		else OutCondition = false;
	}
	else
	{
		OutCondition = false;
	}

}

void CTable_Conditions::Check_Conquer_MainPoint(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
	CHECKFALSEOUTCONDITION(OutCondition);
	OutCondition = false;
	CTeamConnector* pMyTeamConnect = pPlayer->Get_Team();
	if (nullptr == pMyTeamConnect)
		OutCondition = false;

	if (Check_Level(LEVEL_PADEN))
	{
		if (pMyTeamConnect->Has_MainTrigger())
			OutCondition = true;
		else
			OutCondition = false;
	}
	else if (Check_Level(LEVEL_HWARA))
	{
		if (pMyTeamConnect->Has_CenterTrigger())
			OutCondition = true;
		else
			OutCondition = false;
	}
	else
	{
		OutCondition = false;
	}

}

void CTable_Conditions::Check_Conquer_HwaraFinal(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
	CHECKFALSEOUTCONDITION(OutCondition);
	CTeamConnector* pMyTeamConnect = pPlayer->Get_Team();
	if (nullptr == pMyTeamConnect)
		OutCondition = false;
	if (pMyTeamConnect->Has_HwaraFinalTrigger())
		OutCondition = true;
	else OutCondition = false;
}

void CTable_Conditions::Check_PatrolBehavior(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
	CHECKFALSEOUTCONDITION(OutCondition);

	OutCondition = false;
	CBehavior* pBehavior = pAIController->Get_CurBehavior();
	if (Check_Behavior(pBehavior, eBehaviorType::ePatrol))
		OutCondition = true;
}

void CTable_Conditions::Check_GotoTriggerBehavior(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
	CHECKFALSEOUTCONDITION(OutCondition);

	OutCondition = false;
	CBehavior* pBehavior = pAIController->Get_CurBehavior();
	if (Check_Behavior(pBehavior, eBehaviorType::ePathFinding))
		OutCondition = true;
}

void CTable_Conditions::Check_PadenCannonInteractBehavior(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
	CHECKFALSEOUTCONDITION(OutCondition);

	OutCondition = false;
	CBehavior* pBehavior = pAIController->Get_CurBehavior();
	if (Check_Behavior(pBehavior, eBehaviorType::ePadenCannonInteract))
		OutCondition = true;
}

void CTable_Conditions::Check_ReviveBehavior(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
	CHECKFALSEOUTCONDITION(OutCondition);

	OutCondition = false;
	CBehavior* pBehavior = pAIController->Get_CurBehavior();
	if (Check_Behavior(pBehavior, eBehaviorType::eRevive))
		OutCondition = true;
}

void CTable_Conditions::Check_CombatBehavior(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
	CHECKFALSEOUTCONDITION(OutCondition);

	OutCondition = false;
	CBehavior* pBehavior = pAIController->Get_CurBehavior();
	if (Check_Behavior(pBehavior, eBehaviorType::eCombat))
		OutCondition = true;
}




void CTable_Conditions::Check_ChangeBehavior(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
	CHECKFALSEOUTCONDITION(OutCondition);

	OutCondition = false;
	CBehavior* pBehavior = pAIController->Get_CurBehavior();
	if (Check_Behavior(pBehavior, eBehaviorType::eChange))
		OutCondition = true;
}


void CTable_Conditions::Check_UsableCannon(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
	CHECKFALSEOUTCONDITION(OutCondition);
	OutCondition = CGameSystem::Get_Instance()->Get_Cannon()->Can_ControlCannon(pPlayer);
}

void CTable_Conditions::Check_InCannonConquerTrigger(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
	CHECKFALSEOUTCONDITION(OutCondition);
	
	CTrigger* pCannonTrigger = CGameSystem::Get_Instance()->Find_Trigger("Paden_Trigger_C");

	if (nullptr == pCannonTrigger)
		OutCondition = false;

	list<CTrigger*> TriggerList = pAIController->Get_NearTrigger();
	for (auto Trigger : TriggerList)
	{
		if (Trigger == pCannonTrigger)
		{
			OutCondition = true;
			return;
		}
	}

	OutCondition = false;
}

void CTable_Conditions::Check_GriderTrigger(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
	CHECKFALSEOUTCONDITION(OutCondition);


	list<CTrigger*> TriggerList = pAIController->Get_NearTrigger();
	CHECK_EMPTY(TriggerList);

	for (auto Trigger : TriggerList)
	{
		//Glider트리거가 존재할 때 조건 타게
		if (Trigger->Get_TypeHash() == typeid(CTrigger_Glider).hash_code())
		{
			OutCondition = true;
			return;
		}
	}

	OutCondition = false;
}

void CTable_Conditions::Check_AdjCannon(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
	CHECKFALSEOUTCONDITION(OutCondition);
	CUnit* pUnit = pPlayer->Get_CurrentUnit();
	if(!pUnit)
		OutCondition = false;

	if (!pUnit->Is_Valid())
		OutCondition = false;

	if(nullptr == pUnit->Get_AdjCannon())
		OutCondition = false;
	else 
		OutCondition = true;
}

void CTable_Conditions::Check_AbleHero(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
	CHECKFALSEOUTCONDITION(OutCondition);

	if (pPlayer->AbleHero())
		OutCondition = true;
	else
		OutCondition = false;;

}

void CTable_Conditions::Check_EmptyRoute(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
	CHECKFALSEOUTCONDITION(OutCondition);

	OutCondition = !pPlayer->Is_FindRoute();

	/*if (pPlayer->Is_FindRoute())
	{
		OutCondition = false;
		return;
	}*/

	//OutCondition = !pPlayer->Get_CurRoute().empty();
}

void CTable_Conditions::Check_GlidePath(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
	CHECKFALSEOUTCONDITION(OutCondition);
	CPath* pPath = pPlayer->Get_CurPath();
	if (nullptr == pPath) 
	{
		OutCondition = false;
		return;
	}
	OutCondition = (0 <= _int(pPath->Get_PathName().find("_Glide")));
}

void CTable_Conditions::Check_ReviveTeam(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
	CHECKFALSEOUTCONDITION(OutCondition);
	OutCondition = false;

	//부활 가능한 아군에 닿았고 그 아군이 able revival일 때 
	if (pPlayer->Get_CurrentUnit()->Get_RevivalPlayer())
	{
		if (pPlayer->Get_CurrentUnit()->Get_RevivalPlayer()->Is_AbleRevival())
		{
			OutCondition = true;
		}
	}

}

void CTable_Conditions::Select_Leader(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController)
{
	//CHECKFALSEOUTCONDITION(OutCondition);


	CSquad* pSquad = pPlayer->Get_Squad();
	if (nullptr == pSquad)
	{
		OutCondition = false;
		return;
	}
	CPlayer* pLeaderPlayer = pSquad->Get_LeaderPlayer();
	if (nullptr == pLeaderPlayer)
	{
		OutCondition = false;
		return;
	}
	if (pLeaderPlayer == pPlayer)
	{
		OutCondition = false;
		return;
	}


	pPlayer->Set_TargetPos(pLeaderPlayer->Get_WorldPos());
	OutDesc->pAlliesPlayer = pLeaderPlayer;
	OutCondition = true;
}


int	Func_Ray(CPlayer* pPlayer)
{
	pPlayer->Target_Lock();
	

	list<CPlayer*>* pSortedEnemies = pPlayer->Get_SortedEnemiesP();

	_float4 vRayStartPos = pPlayer->Get_WorldPos();
	vRayStartPos.y += 0.5f;

	CPlayer* pTargetPlayer = nullptr;

	for (auto& elem : (*pSortedEnemies))
	{
		if (!elem->Is_Valid())
			continue;

		_float4 vDir = elem->Get_WorldPos() - pPlayer->Get_WorldPos();
		_float fLength = vDir.Length();

		if (!GAMEINSTANCE->Shoot_RaytoStaticActors(nullptr, nullptr, vRayStartPos, vDir, fLength))
		{
			pTargetPlayer = elem;
			break;
		}

	}

	pPlayer->ReserveTargetPlayer(pTargetPlayer);

	return 0;
}

int	Func_Ray_Revive(CPlayer* pPlayer)
{
	pPlayer->Ally_Lock();


	list<CPlayer*>* pSortedEnemies = pPlayer->Get_SortedAlliesP();

	_float4 vRayStartPos = pPlayer->Get_WorldPos();
	vRayStartPos.y += 0.5f;

	CPlayer* pTargetPlayer = nullptr;

	for (auto& elem : (*pSortedEnemies))
	{
		if (!elem->Is_Valid())
			continue;

		_float4 vDir = elem->Get_WorldPos() - pPlayer->Get_WorldPos();
		_float fLength = vDir.Length();

		if (!GAMEINSTANCE->Shoot_RaytoStaticActors(nullptr, nullptr, vRayStartPos, vDir, fLength))
		{
			pTargetPlayer = elem;
			break;
		}

	}

	pPlayer->ReserveTargetAlly(pTargetPlayer);

	return 0;
}


void CTable_Conditions::Select_NearEnemy(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController)
{
	//CHECKFALSEOUTCONDITION(OutCondition);

	//소팅하고 정리해놓기
	if (!pPlayer->Get_CurrentUnit()->Is_Valid())
	{
		OutCondition = false;
		return;
	}

	_float4 MyPositoin = pPlayer->Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS);

	list<CPlayer*> Enemies = pAIController->Get_NearEnemy();

	CHECK_EMPTY(Enemies);

	Enemies.sort([&MyPositoin](auto& Sour, auto& Dest)
		{
			_float4 SourPosition = Sour->Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS);
	_float4 DestPosition = Dest->Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS);
	if ((SourPosition - MyPositoin).Length() > (DestPosition - MyPositoin).Length())
		return true;
	else return false;
		});

	//정리해놓고, 만약 그동안 쓰레드가 내놓은 타겟 플레이어가 있으면 갱신 시키기. (동기화?)
	pPlayer->Set_SortedEnemies(Enemies);


	if (pPlayer->Get_ReserveTargetPlayer())
	{
			OutDesc->pEnemyPlayer = pPlayer->Get_ReserveTargetPlayer();
			OutCondition = true;
	}
	else
		OutCondition = false;

	if (!pPlayer->Is_TargetLocked())
		if (!Enemies.empty())
			std::future<int>	newThread = std::async(std::launch::async, bind(Func_Ray, pPlayer));


	if (OutCondition)
		pPlayer->Set_TargetPos(OutDesc->pEnemyPlayer->Get_WorldPos());
}

void CTable_Conditions::Select_NearAllies(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController)
{
	//CHECKFALSEOUTCONDITION(OutCondition);

	if (pPlayer->Get_CurClass() >= FIONA)
	{
		OutCondition = false;
		return;
	}

	//소팅하고 정리해놓기
	if (!pPlayer->Get_CurrentUnit()->Is_Valid())
	{
		OutCondition = false;
		return;
	}

	_float4 MyPositoin = pPlayer->Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS);

	list<CPlayer*> Allies = pAIController->Get_NearAllies();

	CHECK_EMPTY(Allies);

	for (auto& elem : Allies)
	{
		if (elem->IsMainPlayer())
		{
			if (elem->Is_AbleRevival() && elem->Is_Died())
			{
				OutCondition = true;
				OutDesc->pAlliesPlayer = elem;
				return;
			}
		}
	}

	OutCondition = false;
}

void CTable_Conditions::Select_MainPlayer(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController)
{
	//CHECKFALSEOUTCONDITION(OutCondition);

	OutCondition = true;

	CPlayer* pTargetPlayer = CUser::Get_Instance()->Get_MainPlayerInfo()->Get_Player();
	pPlayer->Set_TargetPos(pTargetPlayer->Get_WorldPos());
	OutDesc->pAlliesPlayer = pTargetPlayer;
}

void CTable_Conditions::Select_Teammate(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController)
{
	//CHECKFALSEOUTCONDITION(OutCondition);
	CTeamConnector* pTeamConnector = pPlayer->Get_Team();
	CPlayer* pTargetPlayer = nullptr;
	_float4 vMyPos = pPlayer->Get_WorldPos();


	CPlayer* pMyLeaderPlayer = pPlayer->Get_Squad()->Get_LeaderPlayer();
	pTargetPlayer = pMyLeaderPlayer;

	if (pTargetPlayer->Get_CurrentUnit()->Is_Valid() && pTargetPlayer != pPlayer)
	{
		_float4 vPlayerPos = pTargetPlayer->Get_WorldPos();
		_float fLength = (vPlayerPos - vMyPos).Length();

		if (fLength < 3.f)
		{
			OutCondition = false;
			return;
		}


		OutCondition = true;
		OutDesc->pAlliesPlayer = pTargetPlayer;

		return;
	}

	pTargetPlayer = pTeamConnector->Get_SquadList().back()->Get_LeaderPlayer();
	if (pTargetPlayer == pMyLeaderPlayer)
		pTargetPlayer = pTeamConnector->Get_SquadList().front()->Get_LeaderPlayer();

	if (pTargetPlayer->Get_CurrentUnit()->Is_Valid() && pTargetPlayer != pPlayer)
	{
		_float4 vPlayerPos = pTargetPlayer->Get_WorldPos();
		_float fLength = (vPlayerPos - vMyPos).Length();

		if (fLength < MAX_FOLLOW_LENGTH)
		{
			OutCondition = false;
			return;
		}


		OutCondition = true;
		OutDesc->pAlliesPlayer = pTargetPlayer;
		return;
	}

	pTargetPlayer = nullptr;
	_float fMinDist = 9999.f;

	for (auto& elem : (pTeamConnector->Get_AllPlayers()))
	{
		if (elem == pPlayer)
			continue;

		if (elem->IsLeaderPlayer())
			continue;

		if (!elem->Get_CurrentUnit()->Is_Valid())
			continue;

		_float4 vPlayerPos = elem->Get_WorldPos();
		_float fLength = (vPlayerPos - vMyPos).Length();

		if (fLength < MAX_FOLLOW_LENGTH)
		{
			OutCondition = false;
			return;
		}

		if (fLength < fMinDist)
		{
			fMinDist = fLength;
			pTargetPlayer = elem;
		}
	}

	if (!pTargetPlayer)
	{
		OutCondition = false;
		return;
	}


	OutDesc->pAlliesPlayer = pTargetPlayer;
	OutCondition = true;


}

void CTable_Conditions::Select_ConquerTrigger(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController)
{
	//CHECKFALSEOUTCONDITION(OutCondition);
	OutCondition = false;
	CTrigger* pTargetTrigger = nullptr;
	OutDesc->pTriggerPtr = nullptr;
	CTeamConnector* pTeamConnector = pPlayer->Get_Team();
	if (Check_Level(LEVEL_PADEN))
	{
		if (!pTeamConnector->Has_MainTrigger())
		{
			pTargetTrigger = CGameSystem::Get_Instance()->Find_Trigger("Paden_Trigger_A");
		}
		else if (!pTeamConnector->Has_RespawnTrigger())
		{
			pTargetTrigger = CGameSystem::Get_Instance()->Find_Trigger("Paden_Trigger_R");
		}
		else if (!pTeamConnector->Has_CannonTrigger())
		{
			pTargetTrigger = CGameSystem::Get_Instance()->Find_Trigger("Paden_Trigger_C");

		}

	}
	else if (Check_Level(LEVEL_HWARA))
	{
		if (!pTeamConnector->Has_HwaraRespawnTrigger())
		{
			pTargetTrigger = CGameSystem::Get_Instance()->Find_Trigger("Hwara_Respawn");
		}
		else if (!pTeamConnector->Has_CenterTrigger())
		{
			pTargetTrigger = CGameSystem::Get_Instance()->Find_Trigger("Hwara_Center");
		}
		else if (!pTeamConnector->Has_HwaraFinalTrigger())
		{
			if (Check_Team(pTeamConnector, eTEAM_TYPE::eRED))
			{
				pTargetTrigger = CGameSystem::Get_Instance()->Find_Trigger("Hwara_Final_Red");
			}
			else if (Check_Team(pTeamConnector, eTEAM_TYPE::eBLUE))
			{
				pTargetTrigger = CGameSystem::Get_Instance()->Find_Trigger("Hwara_Final_Blue");
			}

		}
	}

	if (nullptr == pTargetTrigger)
		OutCondition = false;
	else
	{
		/* Trigger와 거리 재기 */
		_float4 vTriggerPos = pTargetTrigger->Get_Transform()->Get_World(WORLD_POS);
		_float4 vPlayerPos = pPlayer->Get_WorldPos();

		_float fLength = (vTriggerPos - vPlayerPos).Length();

		//2보다 멀면 true로
		if (fLength > 2.f)
			OutCondition = true;
	}

	//false면 이제 patrol 갈 듯



	OutDesc->pTriggerPtr = pTargetTrigger;
}

void CTable_Conditions::Select_RandomConquerTrigger(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController)
{
	//CHECKFALSEOUTCONDITION(OutCondition);
	OutCondition = false;
	CTrigger* pTargetTrigger = nullptr;
	_float4 vMyPos = pPlayer->Get_WorldPos();

	if (pPlayer->Get_BehaviorDesc() && pPlayer->Get_BehaviorDesc()->pTriggerPtr)
	{
		if (!pPlayer->Get_Team()->Has_Trigger((_uint)static_cast<CTrigger_Stage*> (pPlayer->Get_BehaviorDesc()->pTriggerPtr)->Get_TriggerType() - 1))
		{
			OutCondition = true;

			if ((pPlayer->Get_BehaviorDesc()->pTriggerPtr->Get_Transform()->Get_World(WORLD_POS) - vMyPos).Length() < 2.f)
				OutCondition = false;


			return;
		}
	}

	OutDesc->pTriggerPtr = nullptr;
	CTeamConnector* pTeamConnector = pPlayer->Get_Team();

	vector<CTrigger*> vecTargetTrigger;

	if (Check_Level(LEVEL_PADEN))
	{
		if (!pTeamConnector->Has_MainTrigger())
		{
			vecTargetTrigger.push_back(CGameSystem::Get_Instance()->Find_Trigger("Paden_Trigger_A"));
		}
		else if (!pTeamConnector->Has_RespawnTrigger())
		{
			vecTargetTrigger.push_back(CGameSystem::Get_Instance()->Find_Trigger("Paden_Trigger_R"));
		}
		else if (!pTeamConnector->Has_CannonTrigger())
		{
			vecTargetTrigger.push_back(CGameSystem::Get_Instance()->Find_Trigger("Paden_Trigger_C"));

		}

	}
	else if (Check_Level(LEVEL_HWARA))
	{
		if (!pTeamConnector->Has_HwaraRespawnTrigger())
		{
			vecTargetTrigger.push_back(CGameSystem::Get_Instance()->Find_Trigger("Hwara_Respawn"));
		}
		else if (!pTeamConnector->Has_CenterTrigger())
		{
			vecTargetTrigger.push_back(CGameSystem::Get_Instance()->Find_Trigger("Hwara_Center"));
		}
		else if (!pTeamConnector->Has_HwaraFinalTrigger())
		{
			if (Check_Team(pTeamConnector, eTEAM_TYPE::eRED))
			{
				vecTargetTrigger.push_back(CGameSystem::Get_Instance()->Find_Trigger("Hwara_Final_Red"));
			}
			else if (Check_Team(pTeamConnector, eTEAM_TYPE::eBLUE))
			{
				vecTargetTrigger.push_back(CGameSystem::Get_Instance()->Find_Trigger("Hwara_Final_Blue"));
			}

		}
	}


	if (vecTargetTrigger.empty())
		OutCondition = false;
	else
	{
		_int iMaxRand = vecTargetTrigger.size() - 1;

		_int iRand = random(0, iMaxRand);
		pTargetTrigger = vecTargetTrigger[iRand];
		OutCondition = true;

	}

	//false면 이제 patrol 갈 듯



	OutDesc->pTriggerPtr = pTargetTrigger;
}

void CTable_Conditions::Select_PadenCannonTrigger(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController)
{
	//CHECKFALSEOUTCONDITION(OutCondition);
	CTrigger* pTargetTrigger = CGameSystem::Get_Instance()->Find_Trigger("Paden_Trigger_C");
	if (nullptr == pTargetTrigger)
	{
		OutCondition = false;
		return;
	}

	OutDesc->pTriggerPtr = pTargetTrigger;
	OutCondition = CGameSystem::Get_Instance()->Get_Cannon()->Can_ControlCannon(pPlayer);


}

void CTable_Conditions::Select_RespawnTrigger(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController)
{
	//CHECKFALSEOUTCONDITION(OutCondition);

	CTrigger* pTargetTrigger = nullptr;
	if (Check_Level(LEVEL_PADEN))
	{
		pTargetTrigger = CGameSystem::Get_Instance()->Find_Trigger("Paden_Trigger_R");
	}
	else if (Check_Level(LEVEL_HWARA))
	{

		pTargetTrigger = CGameSystem::Get_Instance()->Find_Trigger("Hwara_Respawn");
	}
	else
	{
		OutCondition = false;
	}

	if (nullptr == pTargetTrigger)
		OutCondition = false;
	else
	{
		OutCondition = true;
	}
	OutDesc->pTriggerPtr = pTargetTrigger;

}

void CTable_Conditions::Select_MainTrigger(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController)
{
	//CHECKFALSEOUTCONDITION(OutCondition);
	CTrigger* pTargetTrigger = nullptr;
	if (Check_Level(LEVEL_PADEN))
	{
		pTargetTrigger = CGameSystem::Get_Instance()->Find_Trigger("Paden_Trigger_A");
	}
	else if (Check_Level(LEVEL_HWARA))
	{
		pTargetTrigger = CGameSystem::Get_Instance()->Find_Trigger("Hwara_Center");
	}
	else
	{
		OutCondition = false;
	}
	if (nullptr == pTargetTrigger)
		OutCondition = false;
	else
	{
		OutCondition = true;
	}
	OutDesc->pTriggerPtr = pTargetTrigger;

}

void CTable_Conditions::Select_HwaraFinalTrigger(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController)
{
	//CHECKFALSEOUTCONDITION(OutCondition);
	CTeamConnector* pMyTeam = pPlayer->Get_Team();
	if (nullptr == pMyTeam)
		OutCondition = false;
	CTrigger* pTargetTrigger = nullptr;
	if (Check_Team(pMyTeam, eTEAM_TYPE::eRED))
	{
		pTargetTrigger = CGameSystem::Get_Instance()->Find_Trigger("Hwara_Final_Red");
	}
	else if (Check_Team(pMyTeam, eTEAM_TYPE::eBLUE))
	{
		pTargetTrigger = CGameSystem::Get_Instance()->Find_Trigger("Hwara_Final_Blue");
	}

	if (nullptr == pTargetTrigger)
		OutCondition = false;
	else
	{
		OutCondition = true;
	}
	OutDesc->pTriggerPtr = pTargetTrigger;
}

void CTable_Conditions::Select_NearGliderTrigger(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController)
{

	_float4 MyPositoin = pPlayer->Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS);

	list<CTrigger*> TriggerList = pAIController->Get_NearTrigger();
	CHECK_EMPTY(TriggerList);

	TriggerList.sort([&MyPositoin](auto& Sour, auto& Dest)
		{
			_float4 SourPosition = Sour->Get_Position();
			_float4 DestPosition = Dest->Get_Position();
			if ((SourPosition - MyPositoin).Length() > (DestPosition - MyPositoin).Length())
				return true;
			else return false;
		});


	for (auto Trigger : TriggerList)
	{
		//Glider트리거가 존재할 때 조건 타게
		if (Trigger->Get_TypeHash() == typeid(CTrigger_Glider).hash_code())
		{
			pPlayer->Set_TargetPos(Trigger->Get_Position());
			OutDesc->pTriggerPtr = Trigger;
			OutCondition = true;
			return;
		}
	}

}


_bool CTable_Conditions::RemovePlayer(_bool bFlag, list<CPlayer*>& PlayerList, list<CPlayer*>::iterator& rhsIter)
{
	if (bFlag)
		rhsIter = PlayerList.erase(rhsIter);

	return bFlag;

}

_bool CTable_Conditions::Check_Behavior(CBehavior* pBehavior, eBehaviorType eType)
{
	if (nullptr != pBehavior)
	{
		if (pBehavior->Get_BehaviorType() == eType)
			return true;
	}

	return false;
}

_bool CTable_Conditions::Check_Level(LEVEL_TYPE_CLIENT eLevelType)
{
	CUser* pUserInstance = CUser::Get_Instance();
	try
	{
		if (nullptr == pUserInstance)
			throw 1;
		if (pUserInstance->Get_CurLevel() == eLevelType)
			return true;
		else return false;
	}
	catch (int ErrCode)
	{
		if (ErrCode == 1)
			Make_Dump("CUser_ErrLog", "CUser Is Null");
		return false;
	}


}

_bool CTable_Conditions::Check_Team(CTeamConnector* pTeamConnector, eTEAM_TYPE eTeam)
{
	if (nullptr == pTeamConnector)
		return false;
	if (pTeamConnector->Get_TeamType() == eTeam)
		return true;
	else return false;
}

int	Find_Path(CPlayer* pPathOwner)
{
	//Path 사용중이면 대기 시키기
	
		CPath* pCurPath = CGameSystem::Get_Instance()->Clone_RandomNearestPath(pPathOwner->Get_WorldPos(), pPathOwner);
		if (pCurPath)
		{
			while (1)
			{
				if (pPathOwner->Get_CurPath()->IsLocked())
					continue;

				pPathOwner->Set_NewPath(pCurPath);
				break;
			}

			return 0;
		}

		pCurPath = CGameSystem::Get_Instance()->Clone_RandomReleasePath(pPathOwner->Get_WorldPos());

		if (pCurPath)
		{
			while (1)
			{
				if (pPathOwner->Get_CurPath()->IsLocked())
					continue;

				pPathOwner->Set_NewPath(pCurPath);
				break;
			}

			return 0;
		}

	

	return 0;

}


void CTable_Conditions::Callback_Tick_UpdatePatrol(CPlayer* pPlayer, CAIController* pAIController)
{
	if (!pPlayer->Get_CurrentUnit()->Is_Valid())
		return;

	CAIPersonality* pPersonality = pAIController->Get_Personality();
	pPersonality->Update_RemainTime(eBehaviorType::ePatrol);

	/* Patrol에 오래 머물렀으믄 새 Path 주자 */
	if (pPersonality->Is_LongTimeRemain(eBehaviorType::ePatrol))
	{
		pPersonality->Init_RemainTime(eBehaviorType::ePatrol);
		std::future<int> result = std::async(bind(Find_Path, pPlayer));

		return;
	}
}

