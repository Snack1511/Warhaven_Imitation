#pragma once
#include "Client_Defines.h"

#define KOR u8

BEGIN(Engine)
class CGameObject;
class CTransform;
END
BEGIN(Client)
class CStructure;
class CWindow_Map;
class CFunc_ObjectControl
{
public:
	enum CONTROLTYPE { CONTROL_SCALING, CONTROL_ROTATE, CONTROL_MOVE };
	enum CLONEDIR { CLONE_RIGHT, CLONE_UP, CLONE_LOOK, CLONE_NONE, CLONE_END };
	const char* CloneTypeDesc[CLONE_END] =
	{
		"CLONE_RIGHT",
		"CLONE_UP",
		"CLONE_LOOK",
		"CLONE_NONE"
	};
	typedef struct tagMapToolObjectMergeData
	{
		_float4x4 ObjectStateMatrix;
		_float4 vScale;
		_int ObejectIndex;
		_byte byteLightFlag;
	public:
		void Initialize();
	}MERGE_MTO_DATA;
	
	typedef struct tagMapToolObjectData
	{
		//wstring strGroupName;
		_bool bIgnoreFlag;
		wstring strObejctName;
		wstring strMeshPath;
		_float4x4 ObjectStateMatrix;
		_float4 vScale;
		_int ObejectIndex;
		_byte byteLightFlag;

	public:
		void Initialize();
	}MTO_DATA;
	typedef struct tagMapToolCollisionData
	{
		wstring strObejctName;
		_uint iCollisionType;
		_uint iLODType;
		_uint iBoxNums;
		_float4* pBoxScaleInfo;
		_float4* pBoxRotInfo;
		_float4* pBoxTranslateInfo;

	public:
		void Initialize();

	}MTC_DATA;

	typedef struct tagMapTempToolObjectData
	{
		wstring strObejctName;
		//wstring strGroupName;
		wstring strMeshPath;
		_float4x4 ObjectStateMatrix;
		_float4 vScale;
		_int ObejectIndex;
		_byte byteLightFlag;

	public:
		void Initialize();
	}TEMP_MTO_DATA;

	/*typedef vector<CGameObject*> OBJVECTOR;
	typedef vector<MTO_DATA> DATAVECTOR;
	typedef map<size_t, vector<CGameObject*>> OBJGROUPING;
	typedef map<size_t, vector<MTO_DATA>> DATAGROUPING;*/
public:
	enum GroupTuple {Tuple_GroupName, Tuple_SelectFlag};
	typedef vector<tuple<string, bool>> GroupingArr;
public:
	enum eNameTuple { Tuple_ObjectName, Tuple_IndexList };
	typedef tuple<string, list<_int>> ObjectNameTuple;
	typedef vector<tuple<string, list<_int>>> ObjectNameTupleArr;
	typedef map<size_t, vector<tuple<string, list<_int>>>> OBJECTGROUPINGMAP;
public:
	typedef map<size_t, vector<CGameObject*>> ObjectMap;
	typedef map<size_t, vector<MTO_DATA>> DataMap;

	typedef vector<CGameObject*> ObjectArr;
	typedef vector<MTO_DATA> DataArr;
private:
	CFunc_ObjectControl();
	virtual ~CFunc_ObjectControl();
public:
	static CFunc_ObjectControl* Create(CWindow_Map* pMaptool);
	void Destroy();
public:
	void Func_Grouping();
	void Func_FBXList();
	void Func_ObjectList();
	void Func_SelectedObject_NameBase();
	void Func_ObjectControl();

	void Func_PickStart();
	void Func_PickEnd();
	void Func_Picking();

	void Func_SetUpCollider();

	void Tick_Function();
	_bool Is_CurSelectObject();
	void Confirm_Group();
	void Confirm_Data();
public:
	void Func_AddObject();
	void Func_DeleteOBject();
public:
	void Add_ObjectGroup(char* pMeshGroupName);
	void Delete_ObjectGroup(char* pMeshGroupName);
	void CleanUp_ObjectGroup(ObjectMap::iterator& ObjectMapIter, DataMap::iterator& DataMapIter);

	CStructure* Add_ObjectNamingMap(string GroupName, string Meshpath, string ObjectName);
	CStructure* Add_ObjectNamingMap(string GroupName, MTO_DATA& tData);
	CStructure* Add_ObjectNamingMap(MTO_DATA& tData);

	CStructure* Add_Object(string ObjectName, vector<CGameObject*>& rhsObjectArr, string MeshPath);
	MTO_DATA Add_Data(string ObjectName, vector<MTO_DATA>& rhsDataArr, string MeshPath);

	CStructure* Add_Object(vector<CGameObject*>& rhsObjectArr, MTO_DATA& tData);
	MTO_DATA Add_Data(vector<MTO_DATA>& rhsDataArr, MTO_DATA& tData);

	CStructure* Merge_Object(string ObjectName);
	void Split_Object(string ObjectName);

	void Merge_All();
	void Split_All();

	void Delete_ObjectNamingMap(string strSearchObejctName, list<_int>& IndexList);
	void Delete_Object(map<size_t, vector<CGameObject*>>::iterator& ObjectIter, list<_int> IndexList);
	void Delete_Data(map<size_t, vector<MTO_DATA>>::iterator& DataIter, list<_int> IndexList);

	void Delete_Object(vector<CGameObject*>& rhsObjectGroup, _int TargetIndex);
	void Delete_Data(vector<MTO_DATA>& rhsDataGroup, _int TargetIndex);

	void Clear_SameNameObject(string ObjectName, _int NameIndexInGroup);
	//void Clear_ObjectGroup(string ObjectGroupName);
	void Clear_SameNameInGroup(string ObjectGroupName, _int NameIndexInGroup);
	void Clear_AllDatas();

	void Routine_MeshSelect(void* tTreeNode);

public:
	void Add_HLOD();

private:
	void SetUp_CurSelectObject();
	void Show_GroupMatrix();
	void Show_ObjectData();
	void Set_ControlSpeed(_float* fMoveSpeed, _float* fRotateSpeed, _float* fScaleSpeed);
	void Show_ControlInfo(string ScaleInfo, string RotateInfo, string MoveInfo);
	//void Control_Group();

	void Select_DataControlFlag();
	void Control_Object();

	//void Scaling_Group();
	//void Rotate_Group();
	//void Position_Group();
	//void Place_Group();

	void Scaling_Object();
	void Rotate_Object();
	void Position_Object();
	void Position_Object_Mouse();
	void Place_Object();
	void Change_Object_UpDir();

	void Update_Data();
	void Update_Group();

	ObjectNameTupleArr& Get_TupleArr(string GroupName);
	void Clone_Group();
	void Pick_Anchor();
	void SetUp_Matrix(CGameObject* pGameObject, _float4x4 Matrix);
	void Update_GroupMatrixForAnchor();
	void Update_GroupObject();
private:
	void Pick_inOjbect();
	//void Update_
public:
	void Save_Data(string BasePath, string SaveName);
	void Load_Data(string FilePath);
private:
	void Save_ObjectData(string BasePath, string SaveName);
	void Save_ObjectGroup(string BasePath, string SaveName);
	void Save_ObjectSplit(string BasePath, string SaveName);
	void Save_ObjectMerge(string BasePath, string SaveName);

	void Load_ObjectData(string FilePath, string& GroupFilePath, string& SplitFilePath, string& MergeFilePath);
	void Load_ObjectGroup(string FilePath);
	void Load_ObjectGroup_Temp(string FilePath);
	void Load_ObjectSplit(string FilePath);
	void Clear_TupleData(vector<tuple<char*, bool>>& ArrData);

private:
	void SetUp_ColliderType();
	void SetUp_LODLevel();
private:
	_bool Find_ObjectGroupInfo(string strGroupName, vector<tuple<string, bool>>::iterator& OutGroupingIter, size_t& OutHashNum);
	_bool Find_ObjectGroupingName(size_t GroupNameHashNum, map<size_t, vector<tuple<string, list<_int>>>>::iterator& OutGroupingName);
	_bool Find_ObjectDatas(string strObjectName, map<size_t, vector<CGameObject*>>::iterator& OutObjectIter, map<size_t, vector<MTO_DATA>>::iterator& OutDataIter);
private:
	void Place_Clone();
	void Clone_SamePosition();
	CStructure* Clone_Object(_float4 vObjectPosition, _float4 vObjectCompDir, MTO_DATA tData);
private:
	CWindow_Map* m_pMapTool = nullptr;
private:
	//GroupName, SelectFlag--> ?????? ???????? IMGUI???? ??????
	vector<tuple<string, bool>> m_GroupingInfo;

	//HashNum[GroupName], vector<tuple<ObjectName, Index>> --> ?????? ???????? ???????? ?????? ?????????? ????
	map<size_t, vector<tuple<string, list<_int>>>> m_ObjectNameGroupingMap;
	
	//HashNum[ObjectName], CGameObject* --> ???? ???????? ???????? ?????? ???????? ??????
	map<size_t, vector<CGameObject*>> m_ObjectNamingGroupMap;

	//HashNum[ObjectName], Datas --> ???? ???????? ???????? ?????? ???????? ??????
	map<size_t, vector<MTO_DATA>> m_DataNamingGroupMap;

private:
	//?????? ?????? ??????
	vector<tuple<string, list<_int>>>* m_pCurSelectGroupingNameArr = nullptr;
	//???? ???????? ???? ??????????
	vector<CGameObject*>* m_pCurSelectObjectGroup = nullptr;
	//???? ???????? ???? ????????
	vector<MTO_DATA>* m_pCurSelectDataGroup = nullptr;


private:
	CGameObject* m_pCurSelectGameObject = nullptr;
	CTransform* m_pObjTransform = nullptr;
	MTO_DATA* m_pCurSelectData = nullptr;

	CONTROLTYPE m_eControlType = CONTROL_MOVE;
	
	string m_strCurSelectObjectName = "Test....";
	
	_float m_fTickPerScalingSpeed = 1.f;
	_float m_fTickPerRotSpeed = 1.f;
	_float m_fTickPerMoveSpeed = 1.f;


	_float m_fTickPerGroupScalingSpeed = 1.f;
	_float m_fTickPerGroupRotSpeed = 1.f;
	_float m_fTickPerGroupMoveSpeed = 1.f;
	
	_int m_iCurSelectObjecNametIndex = 0;
	_int m_iCurSelectObjectIndex = 0;
	_int m_SelectObjectGroupIndex = 0;


	_bool m_ObjectLightFlagOpt[4] = { false };

	string				m_CurSelectedMeshFilePath = string("");
	string				m_CurSelectedMeshName = string("");
	vector<string>		m_vecSelectedMeshFilePath;
	vector<string>		m_vecSelectedMeshName;

	_bool m_bGroupControl = false;

	void* m_pMeshRoot = nullptr;

	vector<tuple<string, _uint>> m_listColliderType;
	vector<tuple<string, _uint>> m_listLODLevel;

	_uint m_ColliderType = 0;
	_uint m_LODLevel = 3;
	_uint m_iCurSelectBoxIndex = 0;

	_bool bTest = false;

	map<size_t, CStructure*> m_MergeMap;

	_float4 m_vClonePos;
	_float4 m_vCompDir;
	CLONEDIR m_eCloneDir = CLONE_NONE;
	//???????? ????.
	list<CStructure*> m_pHLODList;
	_bool m_bHLOD_HIDE = false;

	string strFromeGroup = "";
	string strToGroup = "";
	_float4x4 m_matPickedAnchor;
	_uint m_iSelectedControlGroup = 0;
	list<_float4x4> m_ObjectOriginMatrixlist;
	_float4 m_AnchorPos;
	_float4 m_AnchorRot;
};

END
