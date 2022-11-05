#pragma once
#include "Component.h"

BEGIN(Engine)

typedef struct tag_TimerEventInfo
{
	_uint iEventNum;
	_float fOriginTime;
	_float fCurTime;
	_bool	bLoop;

}TIMER_EVENT;

class ENGINE_DLL CTimer final
	: public CComponent
{
	DECLARE_PROTOTYPE(CTimer)

private:
	CTimer(_uint iGroupID);
	virtual ~CTimer();

public:
	static	CTimer* Create(_uint iGroupID);

public:
	void	Add_Timer(const _float& fTime, const _uint& iEventNum, _bool bLoop = false);

public:
	// CComponent��(��) ���� ��ӵ�
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void	Tick() override;
	virtual void	Late_Tick() override;

	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	list<TIMER_EVENT>		m_TimerEvents;

private:
	virtual void Release() override;

};

END