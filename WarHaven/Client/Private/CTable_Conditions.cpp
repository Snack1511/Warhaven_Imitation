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
m_vecStrConditionName[_uint(CBehavior::eConditionType::eWhen)].push_back(strFunctionName)

#define Add_WhatCondition(strFunctionName, Function)\
m_WhatConditions.emplace(\
Convert_ToHash(strFunctionName),\
bind(&CTable_Conditions::Function,\
    this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4));\
m_vecStrConditionName[_uint(CBehavior::eConditionType::eWhat)].push_back(strFunctionName)

HRESULT CTable_Conditions::Initialize()
{
    if (FAILED(SetUp_Conditions()))
        return E_FAIL;    
    
    if (FAILED(SetUp_BehaviorTick()))
        return E_FAIL;

    if (FAILED(SetUp_Behaviors()))
        return E_FAIL;

    return S_OK;
}

HRESULT CTable_Conditions::SetUp_Conditions()
{
    Add_WhyCondition(wstring(L"EmptyOtherCondition"), EmptyOtherCondition);
    Add_WhyCondition(wstring(L"Check_FarAwayLeader"), Check_FarAwayLeader);
    Add_WhyCondition(wstring(L"Check_PathArrived"), Check_PathArrived);
    Add_WhyCondition(wstring(L"Check_FarAwayRoute"), Check_FarAwayRoute);
    Add_WhyCondition(wstring(L"Check_NearFromRoute"), Check_NearFromRoute);
    Add_WhyCondition(wstring(L"Check_LookEnemy"), Check_LookEnemy);
    Add_WhyCondition(wstring(L"Check_DeadAllies"), Check_DeadAllies);
    Add_WhyCondition(wstring(L"Check_CombatBehavior"), Check_CombatBehavior);
    Add_WhyCondition(wstring(L"Check_FollowBehavior"), Check_FollowBehavior);
    Add_WhyCondition(wstring(L"Check_ChangeBehavior"), Check_ChangeBehavior);
    Add_WhyCondition(wstring(L"Check_ResurrectBehavior"), Check_ResurrectBehavior);
    Add_WhyCondition(wstring(L"Check_AbleHero"), Check_AbleHero);
    Add_WhyCondition(wstring(L"Check_InRayTarget"), Check_InRayTarget);

    Add_WhatCondition(wstring(L"EmptyWhatCondition"), EmptyWhatCondition);
    Add_WhatCondition(wstring(L"Select_Leader"), Select_Leader);
    Add_WhatCondition(wstring(L"Select_NearPath"), Select_NearPath);
    Add_WhatCondition(wstring(L"Select_NearEnemy"), Select_NearEnemy);
    Add_WhatCondition(wstring(L"Select_NearAllies"), Select_NearAllies);
    Add_WhatCondition(wstring(L"Select_NearTrigger"), Select_NearTrigger);
    Add_WhatCondition(wstring(L"Select_MainPlayer"), Select_MainPlayer);
    //Add_WhatCondition(wstring(L"Select_NearRouteEnemy"), Select_NearEnemy);
    return S_OK;
}

#define Add_BehaviorTick(strBehaviorTickName, BehaviorTick)\
    m_BehaviorTick.emplace(\
    Convert_ToHash(strBehaviorTickName),\
    bind(&CTable_Conditions::BehaviorTick,\
        this, placeholders::_1, placeholders::_2));\
m_vecBehaviorTickName.push_back(strBehaviorTickName);
HRESULT CTable_Conditions::SetUp_BehaviorTick()
{
    Add_BehaviorTick(wstring(L"EmptyBehaviorTick"), EmptyBehaviorTick);
    Add_BehaviorTick(wstring(L"Callback_Tick_UpdatePatrol"), Callback_Tick_UpdatePatrol);
    Add_BehaviorTick(wstring(L"Callback_Tick_Check_NaviTime"), Callback_Tick_Check_NaviTime);
    Add_BehaviorTick(wstring(L"Callback_Tick_PatiFind"), Callback_Tick_PatiFind);
    Add_BehaviorTick(wstring(L"Callback_Tick_FollowTarget"), Callback_Tick_FollowTarget);

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

    Add_Behavior(pBehavior, wstring(L"Follow"), eBehaviorType::eFollow);
    Add_Behavior(pBehavior, wstring(L"Attack"), eBehaviorType::eAttack);
    Add_Behavior(pBehavior, wstring(L"Resurrect"), eBehaviorType::eResurrect);
    Add_Behavior(pBehavior, wstring(L"Change"), eBehaviorType::eChange);

    //Add_Behavior(pBehavior, wstring(L"Patrol"), eBehaviorType::ePatrol);
    //Add_Behavior(pBehavior, wstring(L"Attack"), eBehaviorType::eCombat);
    Add_Behavior(pBehavior, wstring(L"PathNavigation"), eBehaviorType::ePathNavigation);

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

void CTable_Conditions::Check_InRayTarget(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
    CHECKFALSEOUTCONDITION(OutCondition);
    /* 타겟플레이어가 계산된 이후에 쓰셈 */
    _float4 vOutPos;
    _float fOutDist;


    _float4 vTargetPos = pPlayer->Get_TargetPos();
    _float4 vMyPos = pPlayer->Get_WorldPos();

    _float4 vDir = vTargetPos - vMyPos;
    _float fLength = vDir.Length();
    vDir.Normalize();

    OutCondition = GAMEINSTANCE->Shoot_RaytoStaticActors(&vOutPos, &fOutDist, vMyPos, vDir, fLength);
}

void CTable_Conditions::Check_FarAwayLeader(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
    CHECKFALSEOUTCONDITION(OutCondition);

    OutCondition = true;
}

void CTable_Conditions::Check_PathArrived(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
    CHECKFALSEOUTCONDITION(OutCondition);

    if (!pPlayer->Get_CurPath())
    {
        OutCondition = false;
        return;
    }
    OutCondition = !pPlayer->Get_CurPath()->Is_Arrived();
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

            _float YDiff = vTargetPosition.y - MyPositoin.y;
            _bool IsDifferentY = (1.f < (YDiff * YDiff));
            if (RemovePlayer(IsDifferentY, Enemies, iter))
                continue;

            MyPositoin.y = 0.f;
            vTargetPosition.y = 0.f;

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

void CTable_Conditions::Check_FarAwayRoute(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
    CHECKFALSEOUTCONDITION(OutCondition);

    OutCondition = false;
    if (!pPlayer->Get_CurPath())
        return;
    _float4 vNearestPosition = pPlayer->Get_CurPath()->Get_LatestPosition();
    _float4 vMyPosition = pPlayer->Get_WorldPos();

    if ((vNearestPosition - vMyPosition).Length() > pAIController->Get_Personality()->Get_LimitRouteDistance())
    {
        OutCondition = true;
    }
}

void CTable_Conditions::Check_NearFromRoute(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
    CHECKFALSEOUTCONDITION(OutCondition);

    OutCondition = false;

    if (!pPlayer->Get_CurPath())
        return;

    if (pPlayer->Get_CurPath()->Is_Arrived())
    {
        OutCondition = true;
        return;
    }

    _float4 vLatestPosition = pPlayer->Get_CurPath()->Get_LatestPosition();
    _float4 vMyPosition = pPlayer->Get_WorldPos();

    vLatestPosition.y = 0;
    vMyPosition.y = 0;

    if ((vLatestPosition - vMyPosition).Length() <= pAIController->Get_Personality()->Get_LimitRouteDistance())
    {
        OutCondition = true;
    }
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
        _bool bAlliesDead = ((*iter)->Is_Died());
        _bool bRevival = !((*iter)->Is_EndRevivalTime());
        //돌이 안됬거나 부활 가능한 아군이 아니면 삭제
        if (RemovePlayer(!bAlliesDead || !bRevival, Enemies, iter))
        {
            continue;
        }
        else
        {
            _float4 vTargetPosition = (*iter)->Get_WorldPos();

            _float YDiff = vTargetPosition.y - MyPositoin.y;
            _bool IsDifferentY = (1.f < (YDiff * YDiff));
            if (RemovePlayer(IsDifferentY, Enemies, iter))
                continue;

            MyPositoin.y = 0.f;
            vTargetPosition.y = 0.f;

            _float4 vDist = (vTargetPosition - MyPositoin);
            if (pAIController->Get_Personality()->Get_LimitRouteDistance() < vDist.Length())
            {
                _float4 vDir = vDist.Normalize();
                _float4 vMyLook = pPlayer->Get_LookDir();

                _float DotDir = vMyLook.Dot(vDir);

                //1보다 떨어진 돌이 된 아군이 정면에 있는지 확인 --> 없으면 삭제 --> 못본거
                if (RemovePlayer((DotDir < 0.f), Enemies, iter))
                    continue;
            }
        }
        iter++;

    }

    OutCondition = true;
}

void CTable_Conditions::Check_CombatBehavior(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
    CHECKFALSEOUTCONDITION(OutCondition);

    OutCondition = false;
    CBehavior* pBehavior = pAIController->Get_CurBehavior();
    if (Check_Behavior(pBehavior, eBehaviorType::eAttack))
        OutCondition = true;
}

void CTable_Conditions::Check_FollowBehavior(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
    CHECKFALSEOUTCONDITION(OutCondition);

    OutCondition = false;
    CBehavior* pBehavior = pAIController->Get_CurBehavior();
    if (Check_Behavior(pBehavior, eBehaviorType::eFollow))
        OutCondition = true;
}

void CTable_Conditions::Check_ResurrectBehavior(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
    CHECKFALSEOUTCONDITION(OutCondition);

    OutCondition = false;
    CBehavior* pBehavior = pAIController->Get_CurBehavior();
    if (Check_Behavior(pBehavior, eBehaviorType::eResurrect))
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



void CTable_Conditions::Check_AbleHero(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
    CHECKFALSEOUTCONDITION(OutCondition);

    if (pPlayer->AbleHero())
        OutCondition = true;
    else
        OutCondition = false;;

}


void CTable_Conditions::Check_CurCellBlocked(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
    CHECKFALSEOUTCONDITION(OutCondition);

    if (pPlayer->Is_OpenCell())
        OutCondition = false;
    else
        OutCondition = true;
}


void CTable_Conditions::Select_Leader(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController)
{


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

void CTable_Conditions::Select_NearPath(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController)
{
    OutCondition = true;
    pPlayer->Change_NearPath();
    CPath* pTargetPath = pPlayer->Get_CurPath();
    pPlayer->Set_TargetPos(pTargetPath->Get_FirstPos());
}



void CTable_Conditions::Select_NearEnemy(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController)
{
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
    CPlayer* pTargetPlayer = Enemies.front();
    pPlayer->Set_TargetPos(pTargetPlayer->Get_WorldPos());
    OutDesc->pEnemyPlayer = pTargetPlayer;

    OutCondition = true;

}

void CTable_Conditions::Select_NearAllies(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController)
{
    _float4 MyPositoin = pPlayer->Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS);

    list<CPlayer*> Allies = pAIController->Get_NearAllies();
    CHECK_EMPTY(Allies);

    Allies.sort([&MyPositoin](auto& Sour, auto& Dest)
        {
            _float4 SourPosition = Sour->Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS);
            _float4 DestPosition = Dest->Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS);
            if ((SourPosition - MyPositoin).Length() > (DestPosition - MyPositoin).Length())
                return true;
            else return false;
        });
    CPlayer* pTargetPlayer = Allies.front();
    pPlayer->Set_TargetPos(pTargetPlayer->Get_WorldPos());
    OutDesc->pAlliesPlayer = pTargetPlayer;
    OutCondition = true;

}

void CTable_Conditions::Select_NearTrigger(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController)
{
    _float4 MyPositoin = pPlayer->Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS);

    list<CTrigger*> Triggers = pAIController->Get_NearTrigger();
    CHECK_EMPTY(Triggers);

    Triggers.sort([&MyPositoin](auto& Sour, auto& Dest)
        {
            _float4 SourPosition = Sour->Get_Transform()->Get_World(WORLD_POS);
            _float4 DestPosition = Dest->Get_Transform()->Get_World(WORLD_POS);
            if ((SourPosition - MyPositoin).Length() > (DestPosition - MyPositoin).Length())
                return true;
            else return false;
        });

    CTrigger* pTargetTrigger = Triggers.front();
    _float4 vPosition = pTargetTrigger->Get_Position();
    if (vPosition != ZERO_VECTOR) 
    {
        vPosition.w = 1.f;
        pPlayer->Set_TargetPos(vPosition);
    }
    OutDesc->pTriggerPtr = Triggers.front();

    OutCondition = true;

}

//void CTable_Conditions::Select_NearRouteEnemy(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController)
//{
//    OutCondition = false;
//    if (!pPlayer->Get_CurPath())
//        return;
//    _float4 vNearestPosition = pPlayer->Get_CurPath()->Get_LatestPosition();
//    _float4 vMyPosition = pPlayer->Get_WorldPos();
//    list<CPlayer*> Enemies = pAIController->Get_NearEnemy();
//
//    CHECK_EMPTY(Enemies);
//
//    Enemies.sort([&vMyPosition](auto& Sour, auto& Dest)
//        {
//            _float4 SourPosition = Sour->Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS);
//            _float4 DestPosition = Dest->Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS);
//            if ((SourPosition - vMyPosition).Length() > (DestPosition - vMyPosition).Length())
//                return true;
//            else return false;
//        });
//
//    _float4 vEnemyPosition = Enemies.front()->Get_WorldPos();
//
//    if ((vNearestPosition - vEnemyPosition).Length() <= pAIController->Get_Personality()->Get_LimitRouteDistance())
//    {
//        OutCondition = true;
//        OutDesc->pEnemyPlayer = Enemies.front();
//    }
//    else
//    {
//        OutCondition = false;
//        OutDesc->pEnemyPlayer = nullptr;
//    }
//}

void CTable_Conditions::Select_MainPlayer(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController)
{

    OutCondition = true;

    CPlayer* pTargetPlayer = CUser::Get_Instance()->Get_MainPlayerInfo()->Get_Player();
    pPlayer->Set_TargetPos(pTargetPlayer->Get_WorldPos());
    OutDesc->pAlliesPlayer = pTargetPlayer;
}

//패트롤 틱..
void CTable_Conditions::Callback_Tick_UpdatePatrol(CPlayer* pPlayer, CAIController* pAIController)
{
    CAIPersonality* pPersonality = pAIController->Get_Personality();

    //패트롤에 머무른 시간 확인 --> 초기 설정치보다 넘은 경우
    CPath* pPath = pPlayer->Get_CurPath();
    //pPath가 nullptr --> 패스 할당 못받음 --> 맨처음 시작단계..
    if (nullptr == pPath) 
        return;   
    if (nullptr == pPersonality)
        return;

    if (pPersonality->Is_LongTimeRemain(eBehaviorType::ePatrol))
    {
        pPath->Set_Arrived(); // 강제로 마지막 인덱스로..
        if (pPath->Is_Arrived())//도착 여부 확인..
        {
            //경로 업데이트 구문 호출..
            pAIController->Change_NearPath();
            //patrolRemainTime초기화..
            pPersonality->Init_RemainTime(eBehaviorType::ePatrol);
        }
    }
    else
    {
        //아닐경우 PatrolRemainTime 계산..
        pPersonality->Update_RemainTime(eBehaviorType::ePatrol);
    }
}

//네비의 틱..
void CTable_Conditions::Callback_Tick_Check_NaviTime(CPlayer* pPlayer, CAIController* pAIController)
{
    CAIPersonality* pPersonality = pAIController->Get_Personality();
    CPath* pPath = pPlayer->Get_CurPath();

    if (nullptr == pPath)
        return;

    if (nullptr == pPersonality)
        return;

    pPersonality->Update_RemainTime(eBehaviorType::ePathNavigation);

    if (pPersonality->Is_LongTimeRemain(eBehaviorType::ePathNavigation))
    {
        //누적량 체크
        if (pPersonality->Check_LessMoveAcc(eBehaviorType::ePathNavigation, pPath->Get_MoveAcc()))
        {
            _float fLength = fabsf(pPlayer->Get_WorldPos().y - pPath->Get_CurY());

            if (fLength > 1.5f)
                pPath->Set_Arrived();


        }

        pPath->Init_MoveAcc();
        pPersonality->Init_RemainTime(eBehaviorType::ePathNavigation);
    }

}

//길찾기
void CTable_Conditions::Callback_Tick_PatiFind(CPlayer* pPlayer, CAIController* pAIController)
{
//    if (!pPlayer->Get_CurRoute().empty())
//        return;

    //eBehaviorType eType = pAIController->Get_CurBehavior()->Get_BehaviorType();
    pPlayer->Set_IsFindRoute(false);
    CPath* pPath = pPlayer->Get_CurPath();
    if (nullptr == pPath)
        return;

    _float4 vPosition;
    vPosition = pPath->Get_LastPos();

    pPlayer->Make_BestRoute(vPosition);
}

void CTable_Conditions::Callback_Tick_InRayTarget(CPlayer* pPlayer, CAIController* pAIController)
{
    CAIPersonality* pPersonality = pAIController->Get_Personality();
    
   if (nullptr == pPersonality)
       return;
    pPersonality->Update_RemainTime(eBehaviorType::eAttack);
    
    if (pPersonality->Is_LongTimeRemain(eBehaviorType::eAttack))
    {
        //누적량 체크

        _float4 vOutPos;
        _float fOutDist;


        _float4 vTargetPos = pPlayer->Get_TargetPos();
        _float4 vMyPos = pPlayer->Get_WorldPos();

        _float4 vDir = vTargetPos - vMyPos;
        _float fLength = vDir.Length();
        vDir.Normalize();

        //pPlayer->Set_InRayTarget(GAMEINSTANCE->Shoot_RaytoStaticActors(&vOutPos, &fOutDist, vMyPos, vDir, fLength));
    
        pPersonality->Init_RemainTime(eBehaviorType::eAttack);
    }

}

void CTable_Conditions::Callback_Tick_FollowTarget(CPlayer* pPlayer, CAIController* pAIController)
{
    pPlayer->Set_IsFindRoute(false);
    eBehaviorType eType = pAIController->Get_CurBehavior()->Get_BehaviorType();
    CPlayer::eTargetPlayerType ePlayerType;
    switch (eType)
    {
    case eBehaviorType::eAttack:
        ePlayerType = CPlayer::eTargetPlayerType::eEnemy;
        break;
    case eBehaviorType::eResurrect:
        ePlayerType = CPlayer::eTargetPlayerType::eAllies;
        break;
    default:
        ePlayerType = CPlayer::eTargetPlayerType::eEnemy;
        break;

    }

    CPlayer* pTargetPlayer = nullptr;
    pTargetPlayer = pPlayer->Get_TargetPlayer(ePlayerType);
    if (nullptr == pTargetPlayer)
        return;
    
    if (!pTargetPlayer->Is_OpenCell())
        return;

    pPlayer->Make_BestRoute(pTargetPlayer->Get_WorldPos());    
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
