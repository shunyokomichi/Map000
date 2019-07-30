//=======================================================//
//
// 選択表示処理 [selecticon.h]
// Author : Shun Yokomichi
//
//=======================================================//
#ifndef _SELECTICON_H_
#define _SELECTICON_H_

//*******************************************************//
// ヘッダファイルのインクルード
//*******************************************************//
#include "scene3d.h"		// 3Dポリゴン

//*******************************************************//
// クラス定義
//*******************************************************//
class CSelectIcon : public CScene3D
{
public:
	typedef enum
	{
		TYPE_NONE = 0,		// 何もない状態
		TYPE_UP,			// Y軸を上げている状態
		TYPE_DWON,			// Y軸を下げている状態
		TYPE_MAX			// 総数
	}TYPE;

	CSelectIcon();											// コンストラクタ
	~CSelectIcon() {};										// デストラクタ(中身無し)
	static CSelectIcon *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);		// 生成

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 siz);	// 初期化処理
	void Uninit(void);								// 終了処理
	void Update(void);								// 更新処理
	void Draw(void);								// 描画処理

	// 取得 & 設定の関数
	static void Setpos(D3DXVECTOR3 pos)	{ m_pos = pos; }	// 位置設定
	D3DXVECTOR3 Getpos(void)			{ return m_pos; }		// 位置取得

private:
	static D3DXVECTOR3 m_pos;				// 位置
	TYPE m_Type;							// Y軸の変化のタイプ
	int m_nCounter;							// カウンター
	float	m_fPosY;
};
#endif