#pragma once
#include "CEffect.h"

BEGIN(Engine)

END

BEGIN(Client)

class CSword_Effect final
	: public CEffect
{
	DECLARE_PROTOTYPE(CSword_Effect);

private:
	CSword_Effect();
	virtual ~CSword_Effect();

public:
	static CSword_Effect* Create();
	//static CSword_Effect* Create(ifstream* pReadFile);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;
	//private:
	//	HRESULT	SetUp_DefaultEffect(ifstream* pReadFile);

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	virtual	void	OnEnable() override;
	virtual	void	OnDisable() override;
};
END