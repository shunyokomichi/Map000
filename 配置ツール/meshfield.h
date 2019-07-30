//=======================================================//
//
// メッシュフィールド処理 [Meshfield.h]
// Author : Shun Yokomichi
//
//=======================================================//
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

//*******************************************************//
// ヘッダファイルのインクルード
//*******************************************************//
#include "main.h"		// メイン
#include "Scenemesh.h"	// シーンメッシュ

#define MAX_TEX_FIELD	(5)

//*******************************************************//
// クラス定義
//*******************************************************//
class CMeshField : public CSceneMesh
{// CMeshField（親：CSceneMesh）
public:
	CMeshField() : CSceneMesh(CScene::PRIORITY_1, CScene::OBJTYPE_MESHFIELD) {};	// コンストラクタ(中身無し)
	~CMeshField() {};																// デストラクタ(中身無し)

	// 生成処理
	static CMeshField *Create(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHTEX meshTex, int nTex);
	static HRESULT Load(int nMaxTex);	// テクスチャ読み込み処理
	static void Unload(void);	// テクスチャ開放処理

	// 初期化処理
	HRESULT Init(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHTEX meshTex);
	void Uninit(void)	{ m_nNumAll--; CSceneMesh::Uninit(); };	// 終了処理
	void Update(void){};							// 更新処理(中身無し)
	void Draw(void)		{ CSceneMesh::Draw(); };	// 描画処理

	// 取得 or 設定の関数
	static int GetNum() { return m_nNumAll; }			// モデルの総合数の取得
	int GetModelNum() { return m_nNum; }				// モデルの配置番号の取得
	void SetNum(int nNum) { m_nNum = nNum; }			// モデルの番号設定
	int GetTex() { return m_nTex; }


private:
	static int m_nNumAll;			// 総合配置数
	static LPDIRECT3DTEXTURE9	m_pTexture[MAX_TEX_FIELD];	// テクスチャ情報へのポインタ
	int							m_nNum;		// 配置数
	int							m_nTex;

};
#endif