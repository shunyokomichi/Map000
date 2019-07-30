//*****************************************************************************
//
// アイテム処理 [item.h]
// Author : Shun Yokomichi
//
//*****************************************************************************
#ifndef _ITEM3D_H_
#define _ITEM3D_H_

#include "scene.h"
#include "model.h"

class CModel;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ITEM_TYPE		(1)			// モデル種類の最大数
#define MAX_ITEM_PARTS		(2)			// パーツ数

//*****************************************************************************
//	クラス定義(3Dモデル)
//*****************************************************************************
class CItem3D : public CScene
{
public:
	//**********************
	// 構造体定義
	//**********************
	typedef struct
	{
		D3DXVECTOR3 VtxMinModel, VtxMaxModel;	//モデルの最小値、最大値
	} VTX;

	typedef struct
	{
		float fPosX;
		float fPosY;
		float fPosZ;
		float fRotX;
		float fRotY;
		float fRotZ;
	}KEY;

	typedef struct
	{
		int nFrame;
		KEY aKey[MAX_ITEM_PARTS];
	}KEY_INFO;

	typedef struct
	{	// モーションに必要な変数
		int nParts;		// 総合パーツ数
		int nIndex;		// インデックス
		int nParent;		// ペアレント
	}MOTION_ITEM;

	typedef enum
	{
		TYPE_NONE = 0,	// 何もない
		TYPE_KEY,		// 鍵取得
		TYPE_HP,		// 回復
		TYPE_EXPLOSION,	// 爆発
		TYPE_MAX		// 総数
	}TYPE;

	CItem3D(int nPriority = 3, OBJTYPE objType = CScene::OBJTYPE_ITEM3D);
	~CItem3D();
	
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void TxtLoad();
	char *ReadLine(FILE *pFile, char*pDst, int nPatten);
	char *SearchLine(char*pDst);

	static CItem3D * Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type);
	static HRESULT Load(void);	//	読み込む関数(テクスチャ)
	static void Unload(void);	// 開放する関数(テクスチャ)

	void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void SetType(TYPE type) { m_type = type; 	m_nTypeNum = type;}
	void SetNum(int nNum) { m_nNum = nNum; }			// モデルの番号設定
	void SetCol(D3DXCOLOR col) { m_col = col; }			// 色の設定
	D3DXVECTOR3 GetPos() { return m_pos; }
	D3DXVECTOR3 GetRot() {return m_rot;}
	static int GetNum() { return m_nNumAll; }			// モデルの総合数の取得
	int GetType() { return m_nTypeNum; }				// モデルのタイプの取得
	int GetModelNum() { return m_nNum; }				// モデルの配置番号の取得




private:
	D3DXVECTOR3 m_pos;						//位置
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_rot;						//向き
	D3DXCOLOR	m_col;						// 色
	D3DXMATRIX	m_mtxWorld;					//ワールドマトリックス
	D3DXVECTOR3 m_VtxMinModel, m_VtxMaxModel;	//モデルの最小値、最大値
	D3DXMATERIAL *m_pMat;
	VTX m_aModelVtx;
	static LPD3DXMESH	m_pMesh[MAX_ITEM_PARTS];
	static LPD3DXBUFFER m_pBuffMat[MAX_ITEM_PARTS];
	static DWORD		m_nNumMat[MAX_ITEM_PARTS];
	static LPDIRECT3DTEXTURE9	*m_pTexture[MAX_ITEM_PARTS];	// テクスチャ情報へのポインタ

	CModel *m_pModel[MAX_ITEM_PARTS];	// MODELのポインタ

	FILE *pFile;
	char *pStrCur;
	char aLine[256];
	char aStr[256];
	int m_aIndex[MAX_ITEM_PARTS];
	int m_aParts[MAX_ITEM_PARTS];
	D3DXVECTOR3 m_pos1;						//位置
	D3DXVECTOR3 m_rot1;						//位置

	MOTION_ITEM m_motionItem;
	TYPE m_type;

	static int m_nNumAll;			// 総合配置数
	int m_nTypeNum;					// タイプ番号
	int m_nNum;						// 配置数

};

#endif