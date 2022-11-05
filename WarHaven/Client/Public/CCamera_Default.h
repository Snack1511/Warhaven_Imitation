#pragma once
#include "Camera.h"
#include "Client_Defines.h"

BEGIN(Client)

class CCamera_Default final
	: public CCamera
{
	DECLARE_PROTOTYPE(CCamera_Default);
	DECLARE_GAMEOBJECT(CCamera_Default);

private:
	CCamera_Default();
	virtual ~CCamera_Default();


public:
	// CCamera��(��) ���� ��ӵ�
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;

private:

};

END