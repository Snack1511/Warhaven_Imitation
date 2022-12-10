#pragma once
#include "Client_Defines.h"
/*
	* CAIPersonality 
	* AI의 성향'만'을 기술하는 클래스
	* 각각의 가중치들을 설정하고 이에 따라 상태 전환 여부를 반환
	* 상태전환을 직접적으로 수행하지는 않음..
	* 현재 플레이어가 지닌 Personality에 따라 어떤 유닛을 픽할지, 어떤 행동을 중점으로 수행할지, 얼마나 튀는 행동을 할지 등등을 서술
	* 
	* Personality에서 체크 가능한 목적들을 설정할 수 있게 --> 델리게이트로 넘겨주자
*/



BEGIN(Client)

class CPlayer;
class CAIController;
class CBehavior;
class CTable_Conditions;

class CAIPersonality abstract
{
	friend class CPlayerInfo;

public:
	//전체 행동패턴에 영향
	enum eMainPersonality
	{
		eMain_Default,	//모든 확률이 반반 --> 기본 딜레이
		eMain_Elite,	//딜레이 n분에 1
		eMain_Dummy,	//딜레이 n배
		eMain_Crazy,	//모든 확률이 랜덤 --> 기본 딜레이
		eMain_END
	};

	//유닛 선택 및 전투 성향
	enum eFightPersonality 
	{
		eFight_Default,		//랜덤선택
		eFight_Offensive,	//공격위주
		eFight_Deffensive,	//방어위주
	};

	//팀업 협조 성향
	enum eCoopPersonality 
	{
		eCoop_Default,		//기본
		eCoop_Altruistic,	//이타적인
		eCoop_Selfish,		//이기적인
	};

	enum eBehaviorType 
	{
		eBehavior_Move,
		eBehavior_Attack,
		eBehavior_Trigger,
	};

public:
	typedef struct tPersonalityDescription
	{
		//해시 저장용
		wstring strPersonalityName;
		
		//상태전환시 딜레이용
		_float fChangeDelayTime = 0.01f;

		//상태 전환시 딜레이 가중치용 --> 합연산
		_float fDelayWeight = 0.f;

		//목표 인식 범위
		_float fSIghtRadius = 30.f;

		//인식범위 가중치용 --> 합연산
		_float fSightRadiusWeight = 0.f;

		//확인할 HP
		_float fCheckedHP = 30.f;

		//확인 할 HP의 가중치용 --> 합연산
		_float fHPWeight = 0.f;

		//MainPersonality레벨 --> 곱연산으로 가중치 붙음
		_int iMainPersonalityLevel = 1;

		//확률 계산시 전체적인 선택에 영향을 줄 메인 성향
		eMainPersonality eMainPerosnality = eMain_END;
		
		//유닛 선택및 전투시 영향
		eFightPersonality eFightPersonality = eFight_Default;

		//팀업 시 영향
		eCoopPersonality eCoopPersonality = eCoop_Default;

	}PersonalDesc;

public:

public:
	friend class CGameSystem;
	friend class CPlayer;
	friend class CAIController;
public:
	typedef vector<CDelegate<_bool&, void*>> ConditionContainer;
	typedef vector<map<_hashcode, _uint>> ConditionsIndices;
	typedef CDelegate<_bool&, void*> Condition;

protected:
	CAIPersonality(CTable_Conditions* pConditionTable);
	virtual ~CAIPersonality();

public:
	virtual HRESULT Initailize();
	virtual void Release();

public:
	_float Get_Delay();
	_float Get_Range();
	_float Get_CheckedHP();
	list<CBehavior*> Get_BehaviorList() { return m_BehaviorList; }

public:
	//목적 행동시 거쳐야 할 조건들..
	//CDelegate<_bool&> CallBack_CheckBehavior;

protected:
	//Personality클래스에서 셋팅해줘야 할 정보들..
	PersonalDesc m_tPersonalDesc;

	CTable_Conditions* m_pConditionTable = nullptr;

	/* Clone으로 받아온 복제품들 */
	list<CBehavior*> m_BehaviorList;

};

END