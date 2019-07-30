//=======================================================//
//
// シーンメッシュ処理 [SceneMesh.h]
// Author : Shun Yokomichi
//
//=======================================================//
#ifndef _SCENEMESH_H_
#define _SCENEMESH_H_

//*******************************************************//
// ヘッダファイルのインクルード
//*******************************************************//
#include "main.h"	// メイン
#include "scene.h"	// Scene

//*******************************************************//
// クラス定義
//*******************************************************//
class CSceneMesh : public CScene
{// CSceneMesh（親：CScene）
public:
	CSceneMesh(int nPriority, CScene::OBJTYPE objType);	// コンストラクタ
	~CSceneMesh() {};									// デストラクタ(中身無し)

	typedef enum
	{	// メッシュタイプ
		MESHTYPE_FIELD = 0,	// フィールド
		MESHTYPE_WALL,		// ウォール
		MESHTYPE_CYLINDER,	// シリンダー
		MESHTYPE_MAX		// 最大数
	}MESHTYPE;

	typedef enum
	{	// テクスチャタイプ
		MESHTEX_ALL = 0,	// 全面
		MESHTEX_NUMSIZE,	// 分割
		MESHTEX_MAX			// 最大数
	}MESHTEX;

	typedef enum
	{	// サインタイプ(線状判定基軸)
		SINTYPE_CIRCLE = 0,	// 円状
		SINTYPE_RIGHT,		// 右
		SINTYPE_LEFT,		// 左
		SINTYPE_UP,			// 上
		SINTYPE_DOWN,		// 下
		SINTYPE_MAX			// 最大数
	}SINTYPE;

	typedef enum
	{	// サインタイプ(範囲判定基軸)
		SINSTART_POS = 0,	// 位置
		SINSTART_CENTER,	// 中央基軸
		SINSTART_LU,		// 左上基軸
		SINSTART_RU,		// 右上基軸
		SINSTART_LD,		// 左下基軸
		SINSTART_RD,		// 右下基軸
		SINSTART_MAX		// 最大数
	}SINSTART;

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }	// テクスチャ反映

	HRESULT Init(void) { return S_OK; };	// 初期化処理
	void Uninit(void);						// 終了処理
	void Update(void) {};					// 更新処理(中身無し)
	void Draw(void);						// 描画処理

	// 初期値設定
	void SetInitAll(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHTEX meshTex);

	void Setpos(D3DXVECTOR3 pos) { m_pos = pos; }	// 位置設定
	D3DXVECTOR3 Getpos(void) { return m_pos; }		// 位置取得

	void Setrot(D3DXVECTOR3 rot) { m_rot = rot; }	// 方向設定
	D3DXVECTOR3 Getrot(void) { return m_rot; }		// 方向取得

	D3DXVECTOR3 Getsize(void) { return m_size; }	// サイズ取得

	void SetTex(D3DXVECTOR2 TexMoveUV);				// アニメーション情報設定

	void SetCol(D3DXCOLOR col) { m_col = col; }		// 色の設定
	void SetSize(D3DXVECTOR3 size, D3DXVECTOR3 nNum);					// サイズの設定

	D3DXVECTOR3 GetVtx(int nNum);	// 頂点取得
	D3DXVECTOR3 GetNor(int nNum);	// 法線取得

	// 判定使用有無の確認処理
	bool ColInMesh(D3DXVECTOR3 pos);

	void SinUpdate(SINTYPE sinType, SINSTART sinStart, D3DXVECTOR3 pos, float frot, float fUp, float fLine);				// サイン更新処理(線状)
	void SinUpdate(SINTYPE sinType, SINSTART sinStart, D3DXVECTOR3 pos, float frot, float fUp, float fLine, float fRange);	// サイン更新処理(範囲)

	void SetHeight(D3DXVECTOR3 pos, float fValue, float fRange);	// 高度設定
	float GetHeight(D3DXVECTOR3 pos);								// 高度取得
	float GetUpDown(D3DXVECTOR3 pos, D3DXVECTOR3 rot);				// 高度取得(傾斜)
	float GetBlockNor(D3DXVECTOR3 pos);								// 法線取得(ブロック)

private:
	void SetVecNor(void);	// 法線計算

	LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャ情報へのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファ情報へのポインタ
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;		// インデックス情報へのポインタ
	D3DXMATRIX				m_mtxWorld;		// ワールドマトリックス
	int						m_nMaxVtx;		// 頂点最大数
	int						m_nMaxIdx;		// インデックス最大数
	int						m_nMaxPolygon;	// ポリゴン最大数

	MESHTYPE				m_meshType;		// メッシュタイプ
	MESHTEX					m_meshTex;		// テクスチャタイプ
	D3DXVECTOR3				m_num;			// 枚数
	D3DXVECTOR3				m_pos;			// 位置
	D3DXVECTOR3				m_rot;			// 方向
	D3DXVECTOR3				m_size;			// サイズ
	D3DXVECTOR2				m_TexUV;		// UV
	D3DXCOLOR				m_col;			// 色
};
#endif