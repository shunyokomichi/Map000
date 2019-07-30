//=============================================================================
//
// カメラ処理 [camera.h]
// Author : Shun Yokomichi
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*********************************************************************
//	オブジェクトクラス(カメラ)
//*********************************************************************
class CCamera
{
public:
	typedef enum
	{	// カメラ状態
		CAMTYPE_NONE = 0,	// 何もしていない
		CAMTYPE_V,			// 視点
		CAMTYPE_R,			// 注視点
		CAMTYPE_VR,			// 視点＆注視点
		CAMTYPE_MAX			// 最大数
	} CAMTYPE;

	CCamera();
	~CCamera();

	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void SetCamera(void);	// 描画処理

	void BasicMove(void);
	void Move_R(void);	// 注視点の移動量
	void Move_VR(void);	// 視点と注視点の移動量
	void SetUpCamera(void);

	// カメラの視点 & 注視点
	void SetPos(D3DXVECTOR3 posV, D3DXVECTOR3 posR) { m_posV = posV; m_posR = posR; }
	D3DXVECTOR3 GetRot() { return m_rot; }			// 向きの取得
	float GetAngle() { return m_fAngle; }			// 向きの取得(Y軸の角度)
	void SetType(CAMTYPE type) { m_Type = type; }	// カメラのタイプ取得

private:
	D3DXVECTOR3		m_posV;				//視点(現在)
	D3DXVECTOR3		m_posR;				//注視点(現在)
	D3DXVECTOR3		m_posVDest;			//視点(目的)
	D3DXVECTOR3		m_posRDest;			//注視点(目的)
	D3DXVECTOR3		m_rot;				// 向き(現在)
	D3DXVECTOR3		m_rotDest;			// 向き(目的)
	D3DXVECTOR3		m_vecU;				//上方向のベクトル
	D3DXVECTOR3		m_move;				//移動
	D3DXVECTOR3		m_DiffAngle;		// 向きの記録
	D3DXMATRIX		m_mtxProjection;	//プロジェクションマトリックス
	D3DXMATRIX		m_mtxView;			//ビューマトリックス
	float			m_fAngle;
	float			m_nCounterAngle;
	D3DXVECTOR3		m_Length;			// posVとposRの差分
	bool			m_bSwitching;		// 切替
	static CAMTYPE	m_Type;				// カメラのタイプ
	D3DXVECTOR3		m_MousePos;			// マウスの座標(現在)
	D3DXVECTOR3		m_MousePosOld;		// マウスの座標(過去)
	D3DXVECTOR3		m_MousePosAngle;	// マウスの差分
	float			m_fMouseAngle;		// マウスの角度
	float			m_fMousLength;		// マウスの距離
};
#endif