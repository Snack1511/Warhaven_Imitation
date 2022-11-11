#pragma once
#include "Client_Defines.h"

BEGIN(Client)

class CState;

class CState_Manager
{
	DECLARE_SINGLETON(CState_Manager);
	

private:
	CState_Manager();
	~CState_Manager();

public:
	CState* Get_State(STATE_TYPE eType)
	{
		return m_arrStates[eType];
	}

public:
	HRESULT	Initialize();

private:
	CState* m_arrStates[STATE_END] = {};

private:
	/*For Warrior*/
	//void Warrior_Base_R();
	//void Warrior_Base_L();

	//void Warrior_Base_Attack_R();
	//void Warrior_Base_Attack_L();
	//void Warrior_Base_Attack_Default();

	//void Warrior_Base_Hit();
	//void Warrior_Base_ETC();

	//void Warrior_Others();

};
END

