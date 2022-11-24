#pragma once

//#define HASHING(Type, Value) hash<Type>()(Value)

#include "CImGui_Window.h"

BEGIN(Engine)
class CGameObject;
class CTransform;
class CCamera;
END

BEGIN(Client)

class CTerrain;
class CDrawable_Terrain;
class CFunc_ObjectControl;
class CWindow_Map final
	: public CImGui_Window
{
public:
	enum TUPLEDATA { Tuple_CharPtr, Tuple_Bool, Tuple_Index };
//	enum CONTROLTYPE { CONTROL_SCALING, CONTROL_ROTATE, CONTROL_MOVE };
	enum PICKINGTYPE {PICK_GROUP, PICK_OBJECT, PICK_CLONE, PICK_TERRAINVERT, PICK_TERRAINTEX, PICK_INSTANCEOBJECT, PICK_NONE};
	enum CAMERATYPE {CAM_RIGHT, CAM_UP, CAM_LOOK, CAM_FREE};
	enum PICKOUTTYPE {PICK_OUTPOS, PICK_OUTLOCALPOS, PICK_OUTNORM};
	struct MAPDATA
	{
		wstring TerrainDataPath;
		wstring ObjectDataPath;
		wstring InstanceDataPath;
		wstring NavDataPath;
		wstring LightDataPath;

	public:
		void Initialize();
		void Make_Path(string BasePath, string DataName);
		HRESULT SaveData(ofstream& rhsWriteFile, string BasePath, string DataName);
		HRESULT LoadData(ifstream& rhsReadFile, string FilePath);
		HRESULT SavePath(ofstream& rhsWriteFile, wstring strPath);
		HRESULT LoadPath(ifstream& rhsReadFile, wstring& strPath);

	};



	struct TREE_DATA
	{
		string strFullPath;
		string strFileName;
		string strFolderPath;
		vector<TREE_DATA>	vecChildren;
	};

	typedef struct tagMapToolTerrainData
	{
		wstring strTileTexturePath;
		_uint iNumVerticesX;
		_uint iNumVerticesZ;
		_float3* pCurTerrainVertPos;
		_float4* pCurTerrainColor;
	public:
		typedef tuple<wstring, _uint, _uint, _float3*, _float4*> Terrain_TUPLE;
		enum TupleType
		{
			Tuple_TileTexture,
			Tuple_VerticesX,
			Tuple_VerticesZ,
			Tuple_TerrainPosPtr,
			Tuple_TerrainColorPtr,
		};
	public:
		void Initialize();
		void Make_Data(Terrain_TUPLE& tTerrainData);
	}MTT_DATA;

	typedef struct tagMapToolInstanceObjData
	{
		wstring strInstanceGorupName;
		wstring strMeshPath;
		_uint iInstanceNums;
		_float4 InstancePosition;
		VTXINSTANCE* ArrInstanceVTX;
	public:
		typedef tuple<wstring, wstring, _uint, _float4, VTXINSTANCE*> InstancingTuple;
		enum TupleType
		{
			Tuple_GroupName,
			Tuple_MeshPath,
			Tuple_InstanceNums,
			Tuple_Positon,
			Tuple_VTXArrayPtr,
		};
	public:
		void Initialize();
		void Make_Data(InstancingTuple& tTerrainData);
		void Save(ofstream& wirteFile);
		void Load(ifstream& readFile);
	}MTINSTANCE_DATA;

	typedef tuple<_float4, _float4, _float4> PICKDATA;
	typedef vector<tuple<char*, bool>> DataComboArr;

	//typedef struct tagMapToolObjectData
	//{
	//	wstring strMeshName;
	//	//wstring strGroupName;
	//	wstring strMeshPath;
	//	_float4x4 ObjectStateMatrix;
	//	_float4 vScale;
	//	_byte byteLightFlag;

	//public:
	//	void Initialize();
	//}MTO_DATA;

	const char* ArrObjectGroup[GROUP_END] =
	{
		"Static",
		"Default",
		"Enemy",
		"Decoration",
		"Prop",
		"UI",
		"Player",
		"Loading",
		"Effect",
	};
private:
	CWindow_Map();
	virtual ~CWindow_Map();

public:
	static CWindow_Map* Create();

public:
	// CImGui_Window��(��) ���� ��ӵ�
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual HRESULT Render() override;
public:
	void Change_CurPickMode(PICKINGTYPE eType);
	_bool Is_CurPickMode(PICKINGTYPE eType);
	PICKDATA Get_PickData();
protected:

protected:

private:
	typedef struct tagMapToolObjectData MTO_DATA;

	typedef tuple<char*, bool> SelectComboData;
	//typedef vector<CGameObject*> OBJVECTOR;
	//typedef vector<MTO_DATA> DATAVECTOR;
	//typedef map<size_t, vector<CGameObject*>> OBJGROUPING;
	//typedef map<size_t, vector<MTO_DATA>> DATAGROUPING;
private:
	HRESULT Functions_Maptool();
#pragma region Private ������Ʈ��
private:
	void Func_FileControl();
	void Ready_FileArray();
	void Ready_ObjectGroupID();

	void Update_FileArray();
	void SetUp_FilePath(string& strFilePath, char* szData, string strExt = "");
	void SetUp_CurPickingGroup();

#pragma endregion


#pragma region Private ����Ʈ��Ʈ��
private:
	void Func_LightControl();

	void Ready_LightGroup();
	void Ready_LightType();

	void Add_LightGroupList(char* pLightGroupName);
	void Delete_LightGroupList(char* pLightGroupName);

	void Add_Light(char* pLightName);
	void Delete_Light(char* pLightName);
#pragma endregion

#pragma region Priavate ���� ����� �Լ�
private:
	void Save_MapData(string BasePath, string SaveName);
	void Load_MapData(string FilePath);

	void Save_TerrainData(string BasePath, string SaveName);
	void Load_TerrainData(string FilePath);

	void Save_InstanceData(string BasePath, string SaveName);
	void Save_SpliteData(string BasePath, string SaveName);
	void Save_MergeData(string BasePath, string SaveName);
	void Load_InstanceData(string FilePath);


	void Save_NavGroup(string BasePath, string SaveName);
	void Load_NavGroup(string FilePath);

	void Save_LightGroup(string BasePath, string SaveName);
	void Load_LightGroup(string FilePath);
#pragma endregion

#pragma region Private �ͷ��� ��Ʈ��
private:
	void Ready_TerrainBrushType();
	HRESULT Disable_DefaultTerrain();
	void Func_TerrainControl();
	void Generate_Terrain();

	void Change_TileTexture();
	void Increase_Height();

	void Edit_TerrainVert();
	void Edit_TerrainTex();
	void Edit_TerrainData();
	void Add_Brush(const char* BrushName);

	list<_uint> Select_Vertices();
	_bool Check_InBrush(_float3* CompVert, _float4 vCenterPos);
	_float3 Easing_Vertices(_float3* pArrVertPos);

#pragma endregion

private:
	void Func_InstanceObjectControl();
	void Make_InstanceObject();
	void Delete_InstanceObject();
	void Clear_InstanceGroup();
	void Merge_Instance();
	void Split_Instance();
	//_bool Search_NearInstanceObject();
private:
	_bool Calculate_Pick();


#pragma region Private ��Ÿ �Լ�
	void		Create_SubWindow(const char* szWindowName, const ImVec2& Pos, const ImVec2& Size, function<void(CWindow_Map&)> func);
	_bool		Make_Combo(const char* szLabel, vector<tuple<char*, bool>>& szDataArr, int* pCurIndex, function<void()> SelectFunction);
	void		Clear_TupleData(vector<tuple<char*, bool>>& ArrData);
public:
	void		DebugData(const char* szTitleName, string& strData, ImVec4 Color = ImVec4(1.f, 1.f, 1.f, 1.f));
	string		CutOut_Ext(string& Origin, string& Ext);
	void		Show_TreeData(void* tTree, function<void(void*)> SelectFunction);
	void		Read_Folder_ForTree(const char* pFolderPath, TREE_DATA& tRootTree);
private:
	list<string>Read_Folder_ToStringList(const char* pFolderPath);
	void		Routine_InstanceMeshSelect(void* tTreeNode);

	HRESULT		SetUp_Cameras();
	void		Select_Camera();
	void		Change_Camera(_int Index);

	void		Set_BrushInform();
	_bool		Picked_VertList(list<_uint>& VertsList, _float4 vPosition, _float4& OutPos, _float4& OutNormal);
	void		EmptyFunction() {}
#pragma endregion

#pragma region Value ������Ʈ��
private:
	vector<tuple<char*, bool>> m_arrSaveFilesCombo;
	string m_strPath = string("");

	_int m_SelectObjectGroupIDIndex = 0;
	vector<tuple<char*, bool>> m_arrObjectGroupId;
#pragma endregion

#pragma region Value �ͷ�����Ʈ��
private:
	CTerrain* m_pDefaultTerrain = nullptr;
	CDrawable_Terrain* m_pCurTerrain = nullptr;
	MTT_DATA m_CurTerrainData;
	vector<tuple<char*, bool>> m_arrBrushType;
	vector<tuple<char*, bool>> m_arrTileTextureName;
	_int m_iCurSelectTerrainBrush = 0;
	_uint3 m_i3PickedIndex = _uint3(0, 0, 0);
	PICKDATA m_OutDatas;
	_float m_fBrushSize = 1.f;
	_float m_fBrushWeight = 1.f;

	//Diffuse, Normal
	_float4 m_vTileTypeFlag = _float4(0.f, 0.f, 1.f, 1.f);

#pragma endregion

private:
	typedef map<size_t, vector<tuple<MTINSTANCE_DATA, CGameObject*>>> INSTANCEGROUPING;
	typedef vector<tuple<MTINSTANCE_DATA, CGameObject*>> INSTANCEVECTOR;
	typedef tuple<MTINSTANCE_DATA, CGameObject*> INSTANCETUPLE;
	_int m_iDrawInstanceObjectNums = 0;
	_float m_fRandomRatio = 0.f;
	string m_strCurSelectInstanceMeshPath = "";
	string m_strCurSelectInstanceMeshName = "";
	TREE_DATA m_tInstanceMeshDataRoot;
	CGameObject* m_pCurSelectInstanceObject = nullptr;
	vector<string> m_strArrInstanceMeshName;
	map<size_t, vector<tuple<MTINSTANCE_DATA, CGameObject*>>> m_InstanceMap;
	map<size_t, CGameObject*> m_MergeObjects;
	_int m_iCurSelectInstanceNameIndex = 0;
	_int m_iCurSelectInstanceObjectIndex = 0;
#pragma region Value ������Ʈ ��Ʈ��
//private:
//	CGameObject* m_pCurSelectGameObject = nullptr;
//	CTransform* m_pObjTransform = nullptr;
//	MTO_DATA* m_pCurSelectData = nullptr;
//	CONTROLTYPE m_eControlType = CONTROL_MOVE;
//
//	_float m_fTickPerScalingSpeed = 1.f;
//	_float m_fTickPerRotSpeed = 1.f;
//	_float m_fTickPerMoveSpeed = 1.f;
#pragma endregion

#pragma region Value ����Ʈ��Ʈ��
private:
	vector<tuple<char*, bool>> m_arrLightGroupCombo;
	vector<tuple<char*, bool>> m_arrLightTypeCombo;
#pragma endregion
	public:
		TREE_DATA* Get_MeshRoot() {
			return &m_MeshRootNode;
		}
#pragma region ��Ÿ �ʿ亯��
	TREE_DATA			m_MeshRootNode;

	list<CGameObject*>* m_pCurObjectList = nullptr;

	_bool m_bHoverWindow = false;
	_bool m_TerrainWireFrame = false;

	PICKINGTYPE m_ePickingType = PICK_NONE;

	string m_curSelectBGTextureName;
	string m_curSelectSourTextureName;
	string m_curSelectDestTextureName;
	_int m_iSourIndex = 0;
	_int m_iDestIndex = 0;
	_int m_iBGIndex = 0;

	vector<tuple<wstring, CCamera*, _float4>> m_ArrCams;
	_float4x4 m_matTerrain;
private:
	CFunc_ObjectControl* m_pObjectController = nullptr;
	_int m_iTileIndexFlag = 0;
#pragma endregion


};
END
