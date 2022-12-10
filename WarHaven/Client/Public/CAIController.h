#pragma once
#include "Component.h"
#include "Client_Defines.h"
#include "CTable_Conditions.h"
/*
    * AI는 컴포넌트 형태로 존재
    * 상태변경에 직접적으로 관여
    * 외부에서 AIPersonality라는 클래스를 던져줘서 AI의 성향을 셋팅
    * AIPersonality는 AI들의 성향을 기술하는 클래스 --> 툴로 빼서 작업해봐도 좋지 않을 까...
    * 현재는 AIController로 이름이 설정되어 있지만, 정확히는 상태선택지시자가 맟지 않을 까...
    * 해당 컴포넌트는 Transform이전에 돌아야 함
    * 
    * AIController는 일정한 단계를 돌려야 하는 컴포넌트
        0. 목적 설정 단계 CPhase_SetBehaviorCondition
            --> 어떤 행동을 할지 선택하는 상태
            --> Personality에 따라 현재 목적과 목표 플래그가 갱신
            --> 갱신된 플래그들에 따라 적절행동들, 반대행동들, 행동조건, 목표 재선택 조건, 목적 재선택 조건 가져옴
            -->  적절행동들과, 반대행동들을 가져오지 못할경우 진행 불가
        
         1. 행동선택단계 단계 CPhase_SelectBehavior
            --> 앞서 가져온 행동들 중 하나씩을 선택하는 단계
            --> 행동 조건에 따라 행동들을 선택
            --> 행동 종료 조건또한 여기서 결정
            --> 행동 종료가 되면 여기서 새로운 행동들을 할당
        
        2. 탐색 단계 
            --> 타겟의 위치를 탐색하는 단계 
            --> 선택된 목표에 따라 TileLayer를 참조해 길찾기 
            --> 목표가 근거리이고 같은 TileLayer일 경우 매 프레임 탐색, 아닐 경우 일정 조건(본인 레이어 변경 or 특정 시간초)마다 탐색

        3. 행동 단계
            --> 선택된 타겟에 도달해 목적을 달성하기 위해 Unit의 상태를 변경하는 단계 
            --> 행동 종료 후 목적달성 단계로 넘어갈 수 있음

        4. 목적 달성 단계 
            --> 초기 설정한 목적을 달성 했다고 '판단'하는 단계 
            --> 달성 없이도 Personality에 따라 달성했다 치고 넘어갈 수 있음
    * 
    * 적용루틴
    --> GameSystem에서 CPlayer객체를 생성할 때, mainPlayer가 아닌 것들은 필수적으로 AIPersonality를 생성해서 넣어주도록..
    --> AIPersonality는 상속을 통해 확장성을 추구해야함

    * 목적과 목표
        --> 목적과 목표를 조합해 해당 상황에 맞는 행동을 하도록 디자인하자!
        ex1) Fight + Allies = 아군 지원
        ex2) Fight + Enemy = 적과 교전
        ex3) Interact + Enemy = 티배깅
        ex4) Interact + Allies = 티배깅 or 부활 or 팀챗등등.. 성향따라서 바뀌도록..

    * 조건 관리
    * 목적 + 목표 + Personality에 따라 행동조건을 가져옴 --> 또한 목표 재선택 조건과 목적 재선택 조건또한 동시에 가져옴 --> 

*/
BEGIN(Engine)

class CGameObject;
class CTile;
class CTileLayer;

END

BEGIN(Client)

class CUnit;
class CPlayer;
class CAIPersonality;
class CBehavior;
class CTrigger;
class CPath;

class CAIController :
    public CComponent
{

    DECLARE_PROTOTYPE(CAIController)
public:
    friend class CPlayer;
private:
    CAIController(_uint iGroupID);
    CAIController(const CAIController& rhs);
    virtual ~CAIController();

public:
    static CAIController* Create(CAIPersonality* pPersonality);

public:
    // CComponent을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize() override;
    virtual void Start() override;
    virtual void Tick() override;
    virtual void Late_Tick() override;
    virtual void Release() override;

    virtual void OnEnable() override;
    virtual void OnDisable() override;

public:
    list<CPlayer*> Get_NearAllies() { return m_NearAlliesList; }
    list<CPlayer*> Get_NearEnemy() { return m_NearEnemyList; }
    list<CTrigger*> Get_NearTrigger() { return m_NearTriggerList; }
public:
    void Ready_Controller();

private:
    CPlayer* m_pOwnerPlayer = nullptr;
    CAIPersonality* m_pPersonality = nullptr;
    list<CGameObject*> m_NearObjectList;
   
    CBehavior* m_pCurrentBehavior = nullptr;
    list<CBehavior*> m_BehaviorList;

private:
    /* AI 판단에 필요한 정보들 */
    list<CPlayer*> m_NearAlliesList;
    list<CPlayer*> m_NearEnemyList;
    list<CTrigger*> m_NearTriggerList;
    /* 현재 타고 있는 경로*/
    CPath* m_pCurPath = nullptr;
};

END