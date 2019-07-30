//=============================================================================
//
// エネミー表示処理 [disenemy.h]
// Author : Shun Yokomichi
//
//=============================================================================
#ifndef _DISENEMY_H_
#define _DISENEMY_H_

#include "sceneX.h"		//	Xモデル
#include "meshfield.h"	// メッシュフィールド
#include "display.h"
//*****************************************************************************
//	クラス定義(モデル用)
//*****************************************************************************
class CDsiEnemy : public CDisplay, CSceneX
{
public:
	CDsiEnemy();
	~CDsiEnemy() {};

	static HRESULT Load(int MaxModel);				// ロード
	static void Unload(void);						// アンロード
	static CDsiEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int objType);	// 生成

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int objType);			// 初期化
	void Uninit(void);														// 終了
	void Update(void);														// 更新
	void Draw(void);														// 描画

	void SelectEnemy(void);									// モデル選択処理
	void EnemyCreate(void);

	// 取得 & 設定の関数
	D3DXVECTOR3 SetModelSize(int nSize);
	static int GetNum() { return m_nNum; }					// モデル番号の取得


private:
	static LPDIRECT3DTEXTURE9	*m_pTexture[MAX_MODEL];		// テクスチャ情報へのポインタ
	static LPD3DXMESH			m_pMesh[MAX_MODEL];			// メッシュ情報へのポインタ
	static LPD3DXBUFFER			m_pBuffMat[MAX_MODEL];		// マテリアル情報へのポインタ
	static DWORD				m_nNumMat[MAX_MODEL];		// マテリアル情報の数
	static int m_nNum;			// モデル番号
};

#endif

