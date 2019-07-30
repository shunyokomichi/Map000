//=============================================================================
//
// オブジェクト表示処理 [display.h]
// Author : Shun Yokomichi
//
//=============================================================================
#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "sceneX.h"		//	Xモデル
#include "object.h"		// オブジェクト
#include "meshfield.h"	// メッシュフィールド
//*****************************************************************************
//	クラス定義(表示に必要な物をまとめた)
//*****************************************************************************
class CDisplay
{
public:
	typedef enum
	{
		STATE_NONE = 0,	// 何もない状態
		STATE_PRESS,	// 長押し状態
		STATE_TRIGGER,	// 1回押された状態
	}STATE;

	typedef enum
	{
		TYPE_SIZE_MIN = 0,	// 最少サイズ
		TYPE_SIZE_BIG,		// 最大サイズ
		TYPE_SIZE_MAX
	}TYPE_SIZE;

	CDisplay();
	~CDisplay();
	
	void BasicOperation(void);								// 基本操作(移動)
	void BasicSetRot(void);									// 基本操作(向き設定)
	void ResetButton(void);									// リセットボタン
	static D3DXVECTOR3 GetPos() { return m_pos; }			// 位置の取得
	static D3DXVECTOR3 GetRot() { return m_rot; }			// 向きの取得
	static void ChangeUninit(void) { m_bUse = true; }		// 切替時の終了
	TYPE_SIZE GetSizeType(void) { return m_SizeType; }
	void SetChange(D3DXVECTOR2 nNum) { m_ChangeNum.x = nNum.x; m_ChangeNum.y = nNum.y; }
	int Redo(int nCntRedo);

protected:
	void StateCounter(bool bPress);			// 状態
	int SelectNum(int nSelectNum, int nMaxOperation, int nButtan000, int nButtan001);	// 処理選択処理
	void SetSize(D3DXVECTOR3 min, D3DXVECTOR3 max) { m_sizeMin = min + min; m_sizeMax = max + max;}
	void MoveChange(D3DXVECTOR3 fCameraRot);								// 移動設定
	void SquareMove(D3DXVECTOR3 fCameraRot);								// 升目移動
	void NomalMove(D3DXVECTOR3 fCameraRot);									// 通常移動

	static	D3DXVECTOR3		m_pos;			// 位置情報
	D3DXVECTOR3				m_move;			// 移動量
	static	D3DXVECTOR3		m_rot;			// 向き情報(現在)
	D3DXVECTOR3				m_rotDest;		// 向き情報(目的)
	D3DXVECTOR3				m_size;			// サイズ情報
	D3DXVECTOR3				m_sizeMax;		// サイズ大
	D3DXVECTOR3				m_sizeMin;		// サイズ小
	STATE					m_State;		// 状態の種類
	static	bool			m_bUse;			// 切替の場合
	bool 					m_bPress;		// 押されたかどうか
	bool					m_bFlag;		// フラグ
	int						m_nNumRot;		// 向きの番号
	int						m_nCntPress;	// 長押しのカウント
	float					m_fMove;		// 移動距離値
	TYPE_SIZE				m_SizeType;
	D3DXVECTOR2				m_ChangeNum;

};

#endif

