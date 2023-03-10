#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CGameObject;
class CComponent;
class CLevel;

class CEvent_Manager
{
	DECLARE_SINGLETON(CEvent_Manager);

private:
	CEvent_Manager();
	virtual ~CEvent_Manager();

private:
	enum EVENT_TYPE { EVENT_DELETE_OBJECT, EVENT_CREATE_OBJECT, EVENT_DELETE_COMPONENT, EVENT_CREATE_COMPONENT,
		EVENT_CREATE_STATIC, EVENT_CHANGE_LEVEL, EVENT_DISABLE_OBJECT, EVENT_ENABLE_OBJECT, EVENT_DISABLE_COMPONENT, EVENT_ENABLE_COMPONENT,
		EVENT_END};

	typedef struct tag_Event_Info
	{
		EVENT_TYPE		eEven;
		DWORD_PTR		lParam;
		CGameObject* pGameObject = nullptr;
		CComponent* pComponent = nullptr;
		CLevel* pLevel = nullptr;

		tag_Event_Info(EVENT_TYPE _eEven, DWORD_PTR _lParam, CGameObject* _pGameObject, CComponent* _pComponent, CLevel* _pLevel)
			: eEven(_eEven)
			, lParam(_lParam)
			, pGameObject(_pGameObject)
			, pComponent(_pComponent)
			, pLevel(_pLevel)
		{}
	}EVENT;

public:
	void	Tick();

public:
	void	Delete_GameObject(CGameObject*	pGameObject);
	void	Create_GameObject(CGameObject*	pGameObject, const _uint&	iGroupIdx);

	void	Delete_Component(CComponent* pComponent, CGameObject* pGameObject);
	void	Create_Component(CComponent* pComponent, CGameObject* pGameObject);

	void	Enable_GameObject(CGameObject*	pGameObject);
	void	Disable_GameObject(CGameObject*	pGameObject);

	void	Disable_Component(CComponent* pComponent);
	void	Enable_Component(CComponent* pComponent);

	void	Create_StaticObject(CGameObject* pGameObject, _hashcode hcClassName);
	void	Change_Level(CLevel* pLevel);

	void	Clear_All_Event();
	void	Clear_Enable_Events();

private:
	void	Add_Event(const EVENT_TYPE& eEven, CGameObject* _pGameObject, DWORD_PTR lParam = 0, CComponent* _pComponent = nullptr, CLevel* _pLevel = nullptr);
	void	Execute(const EVENT& tEvent);

private:
	vector<EVENT>			m_vecEvent;
	vector<CGameObject*>	m_vecDeadObjects;
	vector<CComponent*>		m_vecDeadComponents;
};

END