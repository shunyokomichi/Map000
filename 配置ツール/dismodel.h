//=============================================================================
//
// オブジェクト処理 [dismodel.h]
// Author : Shun Yokomichi
//
//=============================================================================
#ifndef _DISMODEL_H_
#define _DISMODEL_H_

#include "sceneX.h"		//	Xモデル
#include "object.h"		// オブジェクト
#include "meshfield.h"	// メッシュフィールド
#include "display.h"

//*****************************************************************************
//	マクロ定義
//*****************************************************************************
#define MAX_REDO		(10)		// 保存できる回数

//*****************************************************************************
//	クラス定義(モデル用)
//*****************************************************************************
class CDisModel : public CDisplay, CSceneX
{
public:
	typedef struct
	{
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 rot;	// 向き
		int			nNum;	// 番号
	}REDO;

	CDisModel();
	~CDisModel() {};

	static HRESULT Load(int MaxModel);				// ロード
	static void Unload(void);						// アンロード
	static CDisModel *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int objType);	// 生成

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int objType);			// 初期化
	void Uninit(void);														// 終了
	void Update(void);														// 更新
	void Draw(void);														// 描画

	void SelectModel(void);									// モデル選択処理
	void ModelCreate(void);

	// 取得 & 設定の関数
	D3DXVECTOR3 SetModelSize(int nSize);
	static int GetNum() { return m_nNum; }					// モデル番号の取得

private:
	void Select();

	static LPDIRECT3DTEXTURE9	*m_pTexture[MAX_MODEL];		// テクスチャ情報へのポインタ
	static LPD3DXMESH			m_pMesh[MAX_MODEL];			// メッシュ情報へのポインタ
	static LPD3DXBUFFER			m_pBuffMat[MAX_MODEL];		// マテリアル情報へのポインタ
	static DWORD				m_nNumMat[MAX_MODEL];		// マテリアル情報の数
	static int m_nNum;			// モデル番号
	int m_nSelectNum;	
	REDO m_nRedo[MAX_REDO];
	int m_nCntRedo;
};

#endif

