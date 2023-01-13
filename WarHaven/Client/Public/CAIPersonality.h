#pragma once
#include "Client_Defines.h"
/*
	* CAIPersonality 
	* AI�� ����'��'�� ����ϴ� Ŭ����
	* ������ ����ġ���� �����ϰ� �̿� ���� ���� ��ȯ ���θ� ��ȯ
	* ������ȯ�� ���������� ���������� ����..
	* ���� �÷��̾ ���� Personality�� ���� � ������ ������, � �ൿ�� �������� ��������, �󸶳� Ƣ�� �ൿ�� ���� ����� ����
	* 
	* Personality���� üũ ������ �������� ������ �� �ְ� --> ��������Ʈ�� �Ѱ�����
*/



BEGIN(Client)

class CPlayer;
class CAIController;
class CBehavior;
class CTable_Conditions;

class CAIPersonality
{
	friend class CPlayerInfo;

public:
	//��ü �ൿ���Ͽ� ����
	enum eMainPersonality
	{
		eMain_Default,	//��� Ȯ���� �ݹ� --> �⺻ ������
		eMain_Elite,	//������ n�п� 1
		eMain_Dummy,	//������ n��
		eMain_Crazy,	//��� Ȯ���� ���� --> �⺻ ������
		eMain_END
	};

	//���� ���� �� ���� ����
	enum eFightPersonality 
	{
		eFight_Default,		//��������
		eFight_Offensive,	//��������
		eFight_Deffensive,	//�������
	};

	//���� ���� ����
	enum eCoopPersonality 
	{
		eCoop_Default,		//�⺻
		eCoop_Altruistic,	//��Ÿ����
		eCoop_Selfish,		//�̱�����
	};

	enum ePurposeType 
	{
		eBehavior_Move,
		eBehavior_Attack,
		eBehavior_Trigger,
	};

public:


	typedef struct tPersonalityData
	{
		//������ȯ�� �����̿�
		_float fChangeDelayTime = 0.01f;

		//���� ��ȯ�� ������ ����ġ�� --> �տ���
		_float fDelayWeight = 0.f;

		//��ǥ �ν� ����
		_float fSIghtRadius = 30.f;

		//�νĹ��� ����ġ�� --> �տ���
		_float fSightRadiusWeight = 0.f;

		//Ȯ���� HP
		_float fCheckedHP = 30.f;

		//Ȯ�� �� HP�� ����ġ�� --> �տ���
		_float fHPWeight = 0.f;

		//MainPersonality���� --> ���������� ����ġ ����
		_int iMainPersonalityLevel = 1;

		//Ȯ�� ���� ��ü���� ���ÿ� ������ �� ���� ����
		eMainPersonality eMainPerosnality = eMain_END;

		//���� ���ù� ������ ����
		eFightPersonality eFightPersonality = eFight_Default;

		//���� �� ����
		eCoopPersonality eCoopPersonality = eCoop_Default;

		//�ൿ�� �ִ�� �ӹ��� �ð�
		_float fRemainMaxTime[_uint(eBehaviorType::eCNT)] = { 0.f };

		// �񺳴��� üũ �뵵
		_float fMinMoveAcc[_uint(eBehaviorType::eCNT)] = { 0.f };



	}PersonalData;
	typedef struct tPersonalityDescription
	{
		//�ؽ� �����
		wstring strPersonalityName;
		PersonalData tPersonalityData;



	}PersonalDesc;
public:
	friend class CGameSystem;
	friend class CPlayer;
	friend class CAIController;
public:
	typedef vector<CDelegate<_bool&, void*>> ConditionContainer;
	typedef vector<map<_hashcode, _uint>> ConditionsIndices;
	typedef CDelegate<_bool&, void*> Condition;

public:
	static CAIPersonality* Create(CTable_Conditions* pConditionTable);
	static CAIPersonality* Create(wstring strPersonalityName, CTable_Conditions* pConditionTable);
public:
	void Load(wstring strPersonalityName);
	void Save();

protected:
	CAIPersonality(CTable_Conditions* pConditionTable);
	virtual ~CAIPersonality();

public:
	virtual HRESULT Initailize();
	virtual void Release();
public:
	HRESULT SetUp_PersonalityName(wstring strPersonalityName);
	HRESULT SetUp_DefaultBehavior();
public:
	_float Get_Delay();
	_float Get_Range();
	_float Get_CheckedHP();
	list<CBehavior*>& Get_BehaviorList() { return m_BehaviorList; }
	CBehavior* Get_DefaultBehavior() { return m_pDefaultBehavior; }
	void Set_DefaultBehavior(CBehavior* pBehavior) { m_pDefaultBehavior = pBehavior; }
	_bool Is_LongTimeRemain(eBehaviorType eBhavior);
	void Update_RemainTime(eBehaviorType eBhavior);
	void Init_RemainTime(eBehaviorType eBhavior);
public:
	CBehavior* Add_Behavior(wstring strBehaviorName);
	void Delete_Behavior(CBehavior* pBehavior);
	void Clear_Behavior();
	CBehavior* Get_LastBehavior();
	void Sort_BehaviorWithPriority();
	void Complete_Update();
	_bool Is_UpdateList() { return m_bBehaviorListUpdateFlag; }
public:
	void Set_PersonalityName(wstring strPersonalityName) { m_tPersonalDesc.strPersonalityName = strPersonalityName; }
	wstring Get_PersonalityName() { return m_tPersonalDesc.strPersonalityName; }

public:
	/* CurMove�� �� ������ True */
	_bool	Check_LessMoveAcc(eBehaviorType eBhavior, _float fCurMoveAcc);

public:
	_float Get_LimitRouteDistance();
	//���� �ൿ�� ���ľ� �� ���ǵ�..
	//CDelegate<_bool&> CallBack_CheckBehavior;

protected:
	//PersonalityŬ�������� ��������� �� ������..
	PersonalDesc m_tPersonalDesc;

	CTable_Conditions* m_pConditionTable = nullptr;

	/* Clone���� �޾ƿ� ����ǰ�� */
	list<CBehavior*> m_BehaviorList;
	CBehavior* m_pDefaultBehavior = nullptr;
	_float m_fRemainAccTime[_uint(eBehaviorType::eCNT)] = { 0.f };
	_float m_fRemainPathIndexAccTime = 0.f;
	_float m_fMaxRemainPathIndexTime = 5.f;
	_bool m_bBehaviorListUpdateFlag = false;

};

END