//*****************************************************************************
//
// アイテム表示処理 [disitem.h]
// Author : Shun Yokomichi
//
//*****************************************************************************
#ifndef _DISITEM3D_H_
#define _DISITEM3D_H_

#include "scene.h"
#include "model.h"
#include "item3D.h"
#include "display.h"

class CModel;

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
//	クラス定義(3Dモデル)
//*****************************************************************************
class CDisItem3D : public CDisplay, CScene
{
public:
	//**********************
	// 構造体定義
	//**********************
	typedef struct
	{
		D3DXVECTOR3 VtxMinModel, VtxMaxModel;	//モデルの最小値、最大値
	} VTX;

	typedef enum
	{
		TYPE_NONE = 0,	// 何もない
		TYPE_KEY,		// 鍵取得
		TYPE_HP,		// 回復
		TYPE_EXPLOSION,	// 爆発
		TYPE_MAX		// 総数
	}TYPE;

	typedef struct
	{	// モーションに必要な変数
		int nParts;		// 総合パーツ数
		int nIndex;		// インデックス
		int nParent;		// ペアレント

	}MOTION_ITEM;


	CDisItem3D(int nPriority = 3, OBJTYPE objType = CScene::OBJTYPE_SAID);
	~CDisItem3D() ;
	
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void TxtLoad();
	char *ReadLine(FILE *pFile, char*pDst, int nPatten);
	char *SearchLine(char*pDst);

	static CDisItem3D * Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type);
	static HRESULT Load(void);	//	読み込む関数(テクスチャ)
	static void Unload(void);	// 開放する関数(テクスチャ)

	void SetCol(int nNum);
	void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void SetType(TYPE type) { m_type = type; }

	void SelectType(void);									// モデル選択処理
	void ModelCreate(void);

	// 取得 & 設定の関数
	static int GetNum() { return m_nNum; }					// モデル番号の取得
	D3DXVECTOR3 SetModelSize(int nSize);


private:
	D3DXVECTOR3 m_pos;						//位置
	D3DXVECTOR3 m_rot;						//向き
	D3DXCOLOR	m_col;
	D3DXMATRIX	m_mtxWorld;					//ワールドマトリックス
	D3DXVECTOR3 m_VtxMinModel, m_VtxMaxModel;	//モデルの最小値、最大値
	D3DXMATERIAL *m_pMat;
	VTX m_aModelVtx;
	static LPD3DXMESH	m_pMesh[MAX_ITEM_PARTS];
	static LPD3DXBUFFER m_pBuffMat[MAX_ITEM_PARTS];
	static DWORD		m_nNumMat[MAX_ITEM_PARTS];
	static LPDIRECT3DTEXTURE9	*m_pTexture[MAX_ITEM_PARTS];	// テクスチャ情報へのポインタ

	CModel *m_pModel[MAX_ITEM_PARTS];	// MODELのポインタ
	static int m_nNum;			// モデル番号

	FILE *pFile;
	char *pStrCur;
	char aLine[256];
	char aStr[256];
	int m_aIndex[MAX_ITEM_PARTS];
	int m_aParts[MAX_ITEM_PARTS];
	MOTION_ITEM m_motionItem;
	TYPE m_type;
	D3DXVECTOR3 m_pos1;						//位置
	D3DXVECTOR3 m_rot1;						//位置


};

#endif