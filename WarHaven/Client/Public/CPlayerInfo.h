#pragma once
#include "Client_Defines.h"

#define	DECLARE_PLAYERINFO(classname) public:\
static classname* Create() \
{ \
classname* pInstance = new classname(); \
if (FAILED(pInstance->Initialize()))\
{\
	string text = "Failed to Initialize : ";\
	text += typeid(classname).name();\
	wstring wText;\
	wText.assign(text.begin(), text.end());\
	Call_MsgBox(wText.c_str());\
	delete pInstance;\
	pInstance = nullptr;\
}\
return pInstance;\
}\


//���� mainPlayer ���� ����ִ� Ŭ����
//�κ��丮, �ʱ�ȭ ���� ���
//�� ������ ���� ��Ī�ǰ� �÷��̾� Ŭ���� �����������

//GameSystem�� 16���� Player Info�� ����ֵ�.
//�������� ���� �� ��� ��Ī���ش�.

// Warrior, Engineer �� ���ְ� �޿��� ����ϱ� ������ ���� Define �� �ּ��Ǵٸ� PlayerInfo_Leader ���� #else �� #ifndef �� ó�����ּ���

#define WARRIOR_TH
#define ARCHER_TH
//#define FIONA_TH
//#define PALADIN_TH
#define ENGINEER_TH
//#define PRIEST_TH
//#define QANDA_TH
//#define LANCER_TH

BEGIN(Client)

class CTeamConnector;
class CSquad;
class CPlayer;  
class CAIPersonality;
class CPlayerInfo abstract
{

public:
	struct PLAYER_INFO
	{
		wstring wstrName;
		wstring wstrCamName;
		_uint	iLevel = 1;

	};

	struct PLAYER_SETUP_DATA
	{
		/* Custom ���� */
		wstring	wstrHelmetMeshPath[CLASS_END];
		wstring	wstrBodyMeshPath[CLASS_END];
		wstring	wstrWeaponMeshPath[CLASS_END];
	};

	friend class CGameSystem;
	friend class CPlayer;

protected:
	CPlayerInfo();
	virtual ~CPlayerInfo();

public:
	/* ���� �ε��� �� �Լ��� ���ؼ� Player�� ���� */
	CPlayer* Make_Player();

public:
	CPlayer* Get_Player() { return m_pMyPlayer; }
	eTEAM_TYPE	Get_TeamType() { return m_eTeamType; }
	void	Set_TeamType(eTEAM_TYPE eEnum) { m_eTeamType = eEnum; }
	void	Set_Squad(CSquad* pSquad) { m_pMySquad = pSquad; }
	void	Set_TeamConnector(CTeamConnector* pTeam) { m_pMyTeam = pTeam; }
	void	Set_ChosenClass(CLASS_TYPE eEnum) { m_eCurChosenClass = eEnum; }

	CLASS_TYPE	Get_ChonsenClass() { return m_eCurChosenClass; }
	CLASS_TYPE	Get_ChangeHeroClass() { return m_eChangeHeroClass; }

	_uint	Get_UnitType() { return m_iUnitType; }

	wstring Get_PlayerName() { return m_tPlayerInfo.wstrName; }
	wstring Get_LookAtCamName() { return m_tPlayerInfo.wstrCamName; }

	PLAYER_SETUP_DATA	Get_SetUpData() { return m_tPlayerSetUpData; }

public:
	/* Ŀ���� �߰��� ������ ���� enum �߰��ؼ� �־���� */
	enum class eCUSTOM_HEAD { eDEFAULT, eHEAD1, eHEAD2, eRABBIT };
	void	Set_CustomHead(CLASS_TYPE eClassType, eCUSTOM_HEAD eHeadEnum);

	enum class eCUSTOM_BODY { eDEFAULT, eBODY1, eBODY2 };
	void	Set_CustomBody(CLASS_TYPE eClassType, eCUSTOM_BODY eBodyEnum);

	enum class eCUSTOM_WEAPON { eDEFAULT, eWEAPON1 };
	void	Set_CustomWeapon(CLASS_TYPE eClassType, eCUSTOM_WEAPON eWeaponEnum);

public:
	/* m_vecPrefClassType �ȿ��� �������� ��� ��� �Լ� */
	CLASS_TYPE	Choose_Character();

public:
	virtual HRESULT	Initialize() PURE;
	virtual void Release();

public:
	/* 
		�ܺο��� AI������ �����ϴ� �뵵	
		AI������ ����ϱ� ���� ���� 
	*/
	void Set_Personality(CAIPersonality* pPersonality);

protected:
	/* GameObject�� ������ */
	CPlayer* m_pMyPlayer = nullptr;
	CSquad* m_pMySquad = nullptr;
	CTeamConnector* m_pMyTeam = nullptr;

protected:
	/*AI�� ���� ����*/
	CAIPersonality* m_pPersonality = nullptr;


protected:
	eTEAM_TYPE	m_eTeamType = eTEAM_TYPE::eBLUE;
	CLASS_TYPE	m_eCurChosenClass = WARRIOR;

	CLASS_TYPE	m_eChangeHeroClass = FIONA;

protected:
	PLAYER_INFO	m_tPlayerInfo;
	_bool	m_bIsMainPlayer = false;

	_uint	m_iUnitType = 0;

protected:
	/* �� �ȿ� �ִ� Ŭ������ �ϳ��� �������� �����ؼ� �÷��� �Ѵ�. */
	vector<CLASS_TYPE>		m_vecPrefClassType;

protected:
	/* �� ����ü�� ��� ��η� Player Ŭ������ �� ���� Unit Ŭ�������� ������� */
	PLAYER_SETUP_DATA	m_tPlayerSetUpData;

protected:
	/* Player Ŭ���� ����� ���� ���õ� �������� ��ȿ���� Ȯ�� */
	_bool	Can_Make_Player(const PLAYER_INFO& tInfo);

protected:
	virtual HRESULT	SetUp_AIPersonality();

private:
	void	Set_CustomHead_Warrior(eCUSTOM_HEAD eHeadEnum);
	void	Set_CustomBody_Warrior(eCUSTOM_BODY eBodyEnum);
	void	Set_CustomWeapon_Warrior(eCUSTOM_WEAPON eWeaponEnum);

	void	Set_CustomHead_Paladin(eCUSTOM_HEAD eHeadEnum);
	void	Set_CustomBody_Paladin(eCUSTOM_BODY eBodyEnum);
	void	Set_CustomWeapon_Paladin(eCUSTOM_WEAPON eWeaponEnum);

	void	Set_CustomHead_Arhcer(eCUSTOM_HEAD eHeadEnum);
	void	Set_CustomBody_Arhcer(eCUSTOM_BODY eBodyEnum);
	void	Set_CustomWeapon_Arhcer(eCUSTOM_WEAPON eWeaponEnum);

	void	Set_CustomHead_Engineer(eCUSTOM_HEAD eHeadEnum);
	void	Set_CustomBody_Engineer(eCUSTOM_BODY eBodyEnum);
	void	Set_CustomWeapon_Engineer(eCUSTOM_WEAPON eWeaponEnum);

	void	Set_CustomHead_Fiona(eCUSTOM_HEAD eHeadEnum);
	void	Set_CustomBody_Fiona(eCUSTOM_BODY eBodyEnum);
	void	Set_CustomWeapon_Fiona(eCUSTOM_WEAPON eWeaponEnum);

	void	Set_CustomHead_Qanda(eCUSTOM_HEAD eHeadEnum);
	void	Set_CustomBody_Qanda(eCUSTOM_BODY eBodyEnum);
	void	Set_CustomWeapon_Qanda(eCUSTOM_WEAPON eWeaponEnum);

	void	Set_CustomHead_Priest(eCUSTOM_HEAD eHeadEnum);
	void	Set_CustomBody_Priest(eCUSTOM_BODY eBodyEnum);
	void	Set_CustomWeapon_Priest(eCUSTOM_WEAPON eWeaponEnum);

	void	Set_CustomHead_Lancer(eCUSTOM_HEAD eHeadEnum);
	void	Set_CustomBody_Lancer(eCUSTOM_BODY eBodyEnum);
	void	Set_CustomWeapon_Lancer(eCUSTOM_WEAPON eWeaponEnum);

};

END