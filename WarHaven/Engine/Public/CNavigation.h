#pragma once
#include "Component.h"

BEGIN(Engine)

class CCell;
class CPhysics;
class CNode;
class CCellLayer;

/*
	가시성 검사를 활용한 네비게이션
	이동 --> A*를 돌린 이후의 CellList를 하나씩 받아서 이동
	네비게이션 매시는? 따로 클래스 만들자..
		--> Cell_Layer 
		--> GameSystem이 가지고 있는 것으로..
		--> 관리방식 : Map<레벨, vector<CellLayer>>
	
	Cell_Layer는 생성되는 동시에 가시성 검사와 해당 검사를 통한 기본 탐색선 설정과 이웃셀 설정을 한다
	Cell_Layer에서 Get_BestCells라는 함수를 통해 최적경로의 CellList를 가져온다
		--> 탐색시 기본 탐색용 선들을 가져와서, 임시변수에 할당.
		--> 두 점을 통과하는 셀들을 가져와서 BestCell에 저장
		--> BestCell을 통해 이동한다.
		--> 작성 완료

	Set_StartCell과 Set_EndSell함수를 통해 길찾기를 시작하고 끝낼 Cell을 만들고, 가시성검사 또한 진행한다.
	동적으로 만들어지는 Cell은 조건검사를 통해 가시선을 생성한다.
		--> 조건 1 : 시작 점과 도착 점 사이의 구간내에 있는 Blocked Cell을 모은다
		--> 조건 2 : 시작점과 도착점을 기준으로 Blocked Cell들의 중점과 이은 직선이 다른 BlockedCell을 교차하는 지 검사
		--> 조건 3 : 교차되지 않는 Blocked Cell의 정점들로 다시한번 교차점 검사
		--> 만들어진 가시선의 두 점들을 통해 탐색선을 만들어 지정
		--> 작성 완료
	
	네비게이션 컴포넌트를 통해 받은 셀을 이동시킴


*/

class ENGINE_DLL CNavigation
	: public CComponent
{
public:
	enum CELL_TYPE {DEFAULT, WALL, BLOCKED, CELL_END};
	DECLARE_PROTOTYPE(CNavigation)

protected:
	CNavigation(_uint iGroupIdx);
	virtual ~CNavigation();

public:
	static CNavigation* Create(_uint iGroupIdx, CCell* pStartCell, CPhysics* pPhysicsCom);

public:
	void		Set_StartCell(CCell* pStartCell) { m_pCurCell = pStartCell; }
	CCell* Get_CurCell() { return m_pCurCell; }

	void		Set_Epsilon(_float fEpsilon) { m_fEpsilon = fEpsilon; }
	void		Set_ExamArena(_bool bEnable, _float4 vCenter) {
		m_bExamArena = bEnable;
		m_vExamArenaCenter	= vCenter;
	}

	void		Reset_OnWall() { m_bOnWall = false; 	m_bBlocked = false;
	}
	_bool		Is_OnWall() { return m_bOnWall; }
	_bool		Is_InWall();
	_float4x4	Get_CurWallMat();
	_float		Get_TargetY() { return m_fTargetY; }
	_float4		Get_CurWallNormal();
	_bool		Is_Blocked() { return m_bBlocked; }
	void		Set_StartPosition(_float4 vPosition);
	void		Set_EndPosition(_float4 vPosition);
	//시작 점과 끝 점을 기반으로 각 레이어에서의 도착점 생성
	list<pair<_float4, CCellLayer*>> Get_Goals(map<_float, CCellLayer*>& Layers, _float4 vStart, _float4 vEnd);
	list<_float4> Get_BestRoute(map<_float, CCellLayer*>& Layers, _float4 vStart, _float4 vEnd);
public:
	//CELL_TYPE	isMove(_vector vPosition, _float4* pOutPos);
	_float4		Enter_Wall();
	void		Exit_Wall();

public:
	// CComponent을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void Start() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual void Release() override;

protected:
	CNode* m_pStartNode = nullptr;
	CNode* m_pEndNode = nullptr;
	CCell*		m_pCurCell = nullptr;
	CCell*		m_pCurWallCell = nullptr;
	CPhysics*	m_pPhysicsCom = nullptr;
	_float		m_fEpsilon = 0.f;

	_bool		m_bOnWall = false;
	_bool		m_bExamArena = false;

	_bool		m_bOrigin = false;
	_float4		m_vExamArenaCenter;
	_float		m_fExamDistance = 59.f;

	_float		m_fTargetY = 0.f;

	_bool		m_bInWall = false;
	_bool		m_bBlocked = false;


protected:
	//_float4		Correct_Move(_float4 vPos);

	_float4		Correct_Move_Default(_float4 vPos);

	//현재 위치랑, 셀 던져주면
	_float4		Correct_Move_BlockedAgain(CCell* pCell, _uint eOutLine, _float4 vPos, _float4 _vDir, _float _fSpeed, _int iDepth);

	_float4		Get_NewPosFromCell(CCell* pCell, _vector vPos);
	_float4		Get_NewDirFromCellLine(CCell* pCell, _uint eOutLine, _vector vPos, _float4 _vDir, _float _fSpeed);
	_float4		Get_NewPosFromWall(CCell* pCell, _float4 vRayPos, _float4 vRayDir);

};


END