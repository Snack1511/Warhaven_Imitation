#pragma once
#include "CState.h"

BEGIN(Engine)
class CHierarchyNode;
END

class CState_Hit abstract
	: public CState
{

public:
	enum HIT_STATE { HIT_STATE_E, HIT_STATE_N, HIT_STATE_S, HIT_STATE_W, HIT_STATE_END };
	enum GUARD_STATE { GUARD_STATE_F, GUARD_STATE_L, GUARD_STATE_R, GUARD_STATE_TOP, GUARD_STATE_END };
	enum GROGGY_STATE { GROGGY_STATE_E, GROGGY_STATE_N, GROGGY_STATE_S, GROGGY_STATE_W, GROGGY_STATE_PLACE, GROGGY_STATE_END };


protected:
	CState_Hit();
	virtual ~CState_Hit();

public:
	virtual HRESULT		Initialize();
	virtual void		Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevStateType, void* pData = nullptr);
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void		Exit(CUnit* pOwner, CAnimator* pAnimator) override;



protected:
	_uint			m_iHitIndex[HIT_STATE_END]; // ��Ʈ ����
	_uint			m_iGuardIndex[GUARD_STATE_END]; // ���� ����
	_uint			m_iGroggyIndex[GROGGY_STATE_END]; // �׷α� ����(������ ���� ���µ� ����)

	_uint			m_iHitStingIndex[HIT_STATE_END]; // ��� ��� �ִϸ��̼�

	_uint			m_iFallHitIndex = 0;
	_uint			m_iFlyHitIndex = 0;

	_bool			m_bGuardBreak = false; // GuardBreak �� hit ���¿� ���� ������ ���� �������.
	_bool			m_bHitSting = false;
	_bool			m_bStingDead = false; // �ι� �񸮴� �� ���� ó��


	CHierarchyNode* m_pStingBone = nullptr;



	/*For. Enter*/

/*For. Tick*/
private:
	

/*For. Enter*/
protected:
	void	Face_Check(CUnit* pOwner);

	void Fly_State();

	void	Hit_State(CUnit* pOwner);
	void	Guard_State(CUnit* pOwner);
	void	Groggy_State(CUnit* pOwner);

	void	Sting_State(CUnit* pOwner);

	



};

