#pragma once
#include "CTrigger.h"
BEGIN(Client)
class CTrigger_Glider :
    public CTrigger
{
    DECLARE_PROTOTYPE(CTrigger_Glider);
protected:
    CTrigger_Glider();
    virtual ~CTrigger_Glider();
public:
    static CTrigger_Glider* Create(string strPositionKey, _float fRadius);
public:
    virtual void	Trigger_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos);
    virtual void	Trigger_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType);
    virtual void	Trigger_CollisionExit(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType);
private:
    string				m_strTriggerName;
};
END
