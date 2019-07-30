//=============================================================================
//
// 床表示処理 [disfloor.h]
// Author : Shun Yokomichi
//
//=============================================================================
#ifndef _DISFLOOR_H_
#define _DISFLOOR_H_

#include "sceneX.h"		//	Xモデル
#include "object.h"		// オブジェクト
#include "meshfield.h"	// メッシュフィールド
#include "display.h"

class CDisplay;
//*****************************************************************************
//	クラス定義(メッシュフィールド用)
//*****************************************************************************
class CDisFloor : public CDisplay, CSceneMesh
{
public:
	CDisFloor();
	~CDisFloor() {};

	static HRESULT Load(int MaxModel);				// ロード
	static void Unload(void);						// アンロード
	static CDisFloor *Create(D3DXVECTOR3 Num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHTEX meshTex, int nTex);	// 生成

	HRESULT Init(D3DXVECTOR3 Num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHTEX meshTex, int nTex);			// 初期化
	void Uninit(void);														// 終了
	void Update(void);														// 更新
	void Draw(void);														// 描画
	void SelectOperation();													// 処理を決める
	void SelectNum(int nNum);												// 選択番号
	void MeshSize();														// メッシュフィールドの面数
	void FloorCreate();

	// 取得 & 設定の関数
	void SetSizeChange();						// サイズの設定
	void SelectTex(void);									// Texture選択処理
	static int GetNum() { return m_nNum; }					// Texture番号の取得


private:
	static LPDIRECT3DTEXTURE9	m_pTexture[MAX_MODEL];		// テクスチャ情報へのポインタ
	static int m_nNum;										// Texture番号
	D3DXVECTOR3 num;										// 床の枚数決め(XとZ)
	int							m_SelectNum;				// 選択している番号(番号処理を決める)
	D3DXVECTOR3					m_MeshSize;					// メッシュフィールドの面数
	
};

#endif

