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

class CAIPersonality abstract
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

	enum eBehaviorType 
	{
		eBehavior_Move,
		eBehavior_Attack,
		eBehavior_Trigger,
	};

public:
	typedef struct tPersonalityDescription
	{
		//�ؽ� �����
		wstring strPersonalityName;
		
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
	//���� �ൿ�� ���ľ� �� ���ǵ�..
	//CDelegate<_bool&> CallBack_CheckBehavior;

protected:
	//PersonalityŬ�������� ��������� �� ������..
	PersonalDesc m_tPersonalDesc;

	CTable_Conditions* m_pConditionTable = nullptr;

	/* Clone���� �޾ƿ� ����ǰ�� */
	list<CBehavior*> m_BehaviorList;

};

END