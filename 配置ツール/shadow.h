//=======================================================//
//
// 影処理 [Shadow.h]
// Author : Shun Yokomichi
//
//=======================================================//
#ifndef _SHADOW_H_
#define _SHADOW_H_

//*******************************************************//
// ヘッダファイルのインクルード
//*******************************************************//
#include "main.h"

//*******************************************************//
// クラス定義
//*******************************************************//
class CShadow
{// CShadow
public:
	CShadow() {};							// コンストラクタ(中身無し)
	~CShadow() {};							// デストラクタ(中身無し)

	HRESULT Init(void)	{ return S_OK; };	// 初期化処理
	void Uninit(void)	{};					// 終了処理(中身無し)
	void Update(void)	{};					// 更新処理(中身無し)
	void Draw(void)		{};					// 描画処理(中身無し)

	static void SetShadow(D3DXMATRIX &mtxWorld, LPD3DXMESH &pMesh, DWORD &nNumMat, D3DXVECTOR3 pos);
};
#endif