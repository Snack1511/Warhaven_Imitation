#pragma once
#include "Client_Defines.h"
#include "Component.h"

BEGIN(Engine)
class CMeshContainer;
class CGameObject;
class CModel;
class CAnimator;
END

BEGIN(Client)

class CUnit;

class CColorController 
	: public CComponent
{
public:
	enum FADE_STYLE{TIME, KEYFRAME, FADESTYLE_END};

	struct COLORDESC
	{
		enum FADETYPE
		{
			FADEINREADY,
			FADEIN,
			FADEOUTREADY,
			FADEOUT,
			FADE_END,
		};
		FADETYPE	eCurFadeType = FADEINREADY;
		FADE_STYLE	eFadeStyle = FADESTYLE_END;

		_float		fFadeTimeAcc = 0.f;
		_float		fFadeInStartTime = 0.f;
		_float		fFadeInTime = 0.f;
		_float		fFadeOutStartTime = 9999.f;
		_float		fFadeOutTime = 0.f;

		

		_float4 vCurColor = {};
		_float4 vTargetColor = {};


		_uint iMeshPartType;
		_uint iStartKeyFrame = 0;
		_uint iEndKeyFrame = 0;

		_uint	iOriginState = 0;

		

	};

public:
	DECLARE_PROTOTYPE(CColorController);

private:
	CColorController(_uint iGroupIdx);
	~CColorController();

public:
	static CColorController* Create(_uint iGroupIdx);

public: //starttime > 몇초부터 변할지 endtime > starttime으로부터 걸리는 시간
	HRESULT Add_ColorControll(const COLORDESC& tColorDesc);

public:
	// CComponent을(를) 통해 상속
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void Start() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual void Release() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnDead()	override;

private:
	CModel* m_pTargetModel = nullptr;
	CAnimator* m_pTargetAnimator = nullptr;
	CUnit* m_pOwnerUnit = nullptr;

private:
	_float4 m_vOriginColor[MODEL_PART_END] = {};
	list<COLORDESC> m_ColorDesclist;

private:
	_bool LerpColor(COLORDESC& tColorDesc);
	_bool Fade_Time(COLORDESC& tColorDesc);
	_bool Fade_KeyFrame(COLORDESC& tColorDesc);
};
END
