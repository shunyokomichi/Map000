//=============================================================================
//
// オブジェクト処理 [object.h]
// Author : Shun Yokomichi
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "sceneX.h" 

#define MAX_MODEL		(15)		// モデルの最大数
//*****************************************************************************
//	クラス定義(オブジェクト)
//*****************************************************************************
class CObject : public CSceneX
{
public:
	CObject();
	~CObject();

	static HRESULT Load(int MaxModel);				// ロード
	static void Unload(void);						// アンロード
	static CObject *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int objType);		// 生成

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int objType);			// 初期化
	void Uninit(void);														// 終了
	void Update(void);														// 更新
	void Draw(void);														// 描画

	// 取得や設定の関数
	static int GetNum() { return m_nNumAll; }			// モデルの総合数の取得
	int GetType() { return m_nTypeNum; }				// モデルのタイプの取得
	int GetModelNum() { return m_nNum; }				// モデルの配置番号の取得
	void SetNum(int nNum) { m_nNum = nNum; }			// モデルの番号設定

private:
	static LPDIRECT3DTEXTURE9	*m_pTexture[MAX_MODEL];	// テクスチャ情報へのポインタ
	static LPD3DXMESH			m_pMesh[MAX_MODEL];		// メッシュ情報へのポインタ
	static LPD3DXBUFFER			m_pBuffMat[MAX_MODEL];	// マテリアル情報へのポインタ
	static DWORD				m_nNumMat[MAX_MODEL];	// マテリアル情報の数

	static int m_nNumAll;			// 総合配置数
	int m_nTypeNum;					// タイプ番号
	int m_nNum;						// 配置数
};

#endif // !_OBJECT_H_

