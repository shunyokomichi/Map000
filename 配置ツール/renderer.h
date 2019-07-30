//=======================================================//
//
// レンダラー処理 [Renderer.h]
// Author : Shun Yokomichi
//
//=======================================================//
#ifndef _RENDERER_H_
#define _RENDERER_H_

//*******************************************************//
// ヘッダファイルのインクルード
//*******************************************************//
#include "main.h"	// メイン

//*******************************************************//
// 前方宣言
//*******************************************************//

//*******************************************************//
// クラス定義
//*******************************************************//
class CRenderer
{// CRenderer
public:
	CRenderer();		// コンストラクタ
	~CRenderer() {};	// デストラクタ(中身無し)

	HRESULT Init(HWND hWnd, bool bWindow);	// 初期化処理
	void Uninit(void);						// 終了処理
	void Update(void);						// 更新処理
	void Draw(void);						// 描画処理

	// デバイス取得
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }

private:
	LPDIRECT3D9			m_pD3D;			// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9	m_pD3DDevice;	// Deviceオブジェクト(描画に必要)
	bool				m_bUse;			// デバックの表示
};
#endif