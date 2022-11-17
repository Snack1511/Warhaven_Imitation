#pragma once
#include "Client_Defines.h"
BEGIN(Engine)
class CGameObject;
class CTransform;
END
BEGIN(Client)
class CWindow_Map;
class CFunc_ObjectControl
{
public:
	enum CONTROLTYPE { CONTROL_SCALING, CONTROL_ROTATE, CONTROL_MOVE };
	typedef struct tagMapToolObjectData
	{
		wstring strMeshName;
		//wstring strGroupName;
		wstring strMeshPath;
		_float4x4 ObjectStateMatrix;
		_float4 vScale;
		_byte byteLightFlag;

	public:
		void Initialize();
	}MTO_DATA;
	typedef vector<CGameObject*> OBJVECTOR;
	typedef vector<MTO_DATA> DATAVECTOR;
	typedef map<size_t, vector<CGameObject*>> OBJGROUPING;
	typedef map<size_t, vector<MTO_DATA>> DATAGROUPING;
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
	void Func_GroupControl();
	void Func_ObjectControl();
	void Func_Picking();
	void Tick_Function();
	_bool Is_CurSelectObject();
	void Confirm_Group();
	void Confirm_Data();
public:
	void Func_DeleteOBject();
	void Func_AddObject();
public:
	void Add_MeshGroup(char* pMeshGroupName);
	void Delete_MeshGroup(char* pMeshGroupName);
	void Add_Object(string MeshGroup, string Meshpath, string MeshName);
	void Add_Object(string MeshGroup, MTO_DATA& tData);
	void Delete_Object(string MeshName, vector<CGameObject*>& ObjList, vector<MTO_DATA>& DataList);

	void Clear_MeshGroup(char* pMeshGroupName);

	void Routine_MeshSelect(void* tTreeNode);
private:
	void SetUp_CurSelectObject();
	void Show_GroupMatrix();
	void Show_ObjectData();
	void Set_ObjectSpeed();

	void Control_Group();

	void Select_DataControlFlag();
	void Control_Object();

	void Scaling_Group();
	void Rotate_Group();
	void Position_Group();
	void Place_Group();

	void Scaling_Object();
	void Rotate_Object();
	void Position_Object();
	void Place_Object();
	void Change_Object_UpDir();

	void Update_Data();
	void Update_Group();
	//void Update_
public:
	void Save_ObjectGroup(string BasePath, string SaveName);
	void Load_ObjectGroup(string FilePath);
	void Clear_TupleData(vector<tuple<char*, bool>>& ArrData);
private:
	CWindow_Map* m_pMapTool = nullptr;
private:
	vector<tuple<char*, bool>> m_arrMeshGroupName;
	map<size_t, vector<MTO_DATA>> m_ObjectDataGroupMap;
	map<size_t, vector<CGameObject*>> m_ObjectGroupMap;
	map<size_t, _int> m_ObjNameCallStack;

	vector<MTO_DATA*>* m_pDataGroup = nullptr;
	vector<CGameObject*>* m_pObjGroup = nullptr;
private:
	CGameObject* m_pCurSelectGameObject = nullptr;
	CTransform* m_pObjTransform = nullptr;
	MTO_DATA* m_pCurSelectData = nullptr;
	_float4x4 m_matGroup;
	CONTROLTYPE m_eControlType = CONTROL_MOVE;
	
	string m_strCurSelectObjectName = "Test....";
	
	_float m_fTickPerScalingSpeed = 1.f;
	_float m_fTickPerRotSpeed = 1.f;
	_float m_fTickPerMoveSpeed = 1.f;


	_float m_fTickPerGroupScalingSpeed = 1.f;
	_float m_fTickPerGroupRotSpeed = 1.f;
	_float m_fTickPerGroupMoveSpeed = 1.f;
	
	_int m_iCurSelectObjectIndex = 0;
	_int m_SelectMeshGroupIndex = 0;


	_bool m_ObjectLightFlagOpt[4] = { false };

	string				m_CurSelectedMeshFilePath = string("");
	string				m_CurSelectedMeshName = string("");
	vector<string>		m_vecSelectedMeshFilePath;
	vector<string>		m_vecSelectedMeshName;

	_bool m_bGroupControl = false;

	void* m_pMeshRoot = nullptr;
};

END
