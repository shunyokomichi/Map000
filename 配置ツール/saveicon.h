//=======================================================//
//
// セーブアイコン処理 [saveicon.h]
// Author : Shun Yokomichi
//
//=======================================================//
#ifndef _SAVEICON_H_
#define _SAVEICON_H_

//*******************************************************//
// ヘッダファイルのインクルード
//*******************************************************//
#include "scene2d.h"	// 2Dポリゴン(親)

//*******************************************************//
// クラス定義
//*******************************************************//
class CSaveIcon : public CScene2D
{// CScene2D（親：CScene）
public:
	typedef enum
	{
		TYPE_NONE = 0,		// 何もない状態
		TYPE_TRANSPARENCE,	// 不透明から透明(自動)
		TYPE_OPAQUE,		// 透明から不透明(自動)
		TYPE_MAX			// 総数
	}TYPE;

	CSaveIcon();											// コンストラクタ
	~CSaveIcon() {};										// デストラクタ(中身無し)
	static CSaveIcon *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col);		// 生成


	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col);	// 初期化処理
	void Uninit(void);												// 終了処理
	void Update(void);												// 更新処理
	void Draw(void);												// 描画処理

private:
	TYPE	m_Type;								// 透明度変化の種類
	int m_nCounter;								// 透明度のカウンタ
};
#endif