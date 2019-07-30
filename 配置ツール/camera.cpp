//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : Yokomichi Shun
//
//=============================================================================
#include "camera.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"	// レンダラー
#include "debugproc.h"
#include "game.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_FIRST (2.5f)		// カメラの旋回量

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CCamera::CAMTYPE	CCamera::m_Type		= CAMTYPE_NONE;		// マウスでのカメラ移動のタイプ

//*********************************************************************
//	コンストラクタ
//*********************************************************************
CCamera::CCamera()
{
	// デフォルト値にする

}

//*********************************************************************
//	デストラクタ
//*********************************************************************
CCamera::~CCamera() {}

//*********************************************************************
// 初期化処理
//*********************************************************************
HRESULT CCamera::Init(void)
{
	m_posV = D3DXVECTOR3(0.0f, 300.0f, -400.0f);	// 視点の初期化
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 注視点の初期化
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 向き(現在)の初期化
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き(目的)の初期化
	m_DiffAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fAngle = 0.0f;
	m_Length = m_posV - m_posR;				// 差分の初期化
	m_nCounterAngle = 0;							// 切替のカウンターの初期化
	m_bSwitching = false;

	m_MousePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_MousePosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_MousePosAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fMouseAngle = 0.0f;
	m_fMousLength = 0.0f;

	return S_OK;
}

//*********************************************************************
// 終了処理
//*********************************************************************
void CCamera::Uninit(void)
{
}

//*********************************************************************
// 更新処理
//*********************************************************************
void CCamera::Update(void)
{
	if (CGame::GetTabCount() == 0)
	{
		CInputMouse *pMouse = CManager::GetInputMouse();	// 入力情報の取得(マウス)
		m_MousePos = *pMouse->GetMousePos();				// マウスの位置の取得

		if (pMouse->GetMousePress(1) || pMouse->GetMousePress(2))
		{// 右クリックかホイールを押している時にポインターをループさせる
			pMouse->MouseLoop();

			if (pMouse->GetMousePress(1))
			{// 注視点の操作
				Move_R();
			}
			else if (pMouse->GetMousePress(2))
			{// 視点と注視点の操作(X,Y)
				Move_VR();
			}
		}
		m_MousePosOld = m_MousePos;		// 過去位置の更新

		BasicMove();					// 基本的な移動

		// カメラの拡大縮小
		if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_Q) == true)
		{// Qで拡大
			m_Length.z += 4.0f;
		}
		if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_E) == true)
		{// Eで縮小
			m_Length.z += -4.0f;
		}

		SetUpCamera();	// カメラの最終更新

		CDebugProc::Print("***********************************************************\n");
		CDebugProc::Print(" カメラ操作\n", m_fAngle);
		CDebugProc::Print(" Q : カメラ左旋回 (向き / %0.1f )\n", m_fAngle);
		CDebugProc::Print(" E : カメラ右旋回 (向き / %0.1f )\n", m_fAngle);
		CDebugProc::Print(" W : 前移動       (X / %.1f , Z / %.1f)\n", m_posR.x, m_posR.z);
		CDebugProc::Print(" S : 後移動       (X / %.1f , Z / %.1f)\n", m_posR.x, m_posR.z);
		CDebugProc::Print(" A : 左移動       (X / %.1f , Z / %.1f)\n", m_posR.x, m_posR.z);
		CDebugProc::Print(" D : 右移動       (X / %.1f , Z / %.1f)\n", m_posR.x, m_posR.z);
		CDebugProc::Print(" H : 覗き込み\n");
		CDebugProc::Print(" Y : 俯瞰反転\n");
		CDebugProc::Print("**********************************************************\n");
	}

}

//*********************************************************************
// 設定処理
//*********************************************************************
void CCamera::SetCamera(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH /
		(float)SCREEN_HEIGHT,
		10.0f,
		20000.0f);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}
//*********************************************************************
// 基本的な移動
//*********************************************************************
void CCamera::BasicMove(void)
{
	// それぞれのキーで移動
	if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_D) == true)
	{	// Dで右移動(X軸)
		m_posR.x += sinf((D3DX_PI * 0.5f) + m_rot.y) * 20.0f;
		m_posR.z += cosf((D3DX_PI * 0.5f) + m_rot.y) * 20.0f;
	}
	if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_A) == true)
	{	// Aで左移動(X軸)
		m_posR.x -= sinf((D3DX_PI * 0.5f) + m_rot.y) * 20.0f;
		m_posR.z -= cosf((D3DX_PI * 0.5f) + m_rot.y) * 20.0f;
	}
	if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_W) == true)
	{	// Wで上移動(Z軸)
		m_posR.x -= sinf((D3DX_PI)+m_rot.y) * 20.0f;
		m_posR.z -= cosf((D3DX_PI)+m_rot.y) * 20.0f;
	}
	if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_S) == true)
	{	// Sで下移動(Z軸)
		m_posR.x += sinf((D3DX_PI)+m_rot.y) * 20.0f;
		m_posR.z += cosf((D3DX_PI)+m_rot.y) * 20.0f;
	}
}
//*********************************************************************
// 注視点の移動
//*********************************************************************
void CCamera::Move_R(void)
{
	CInputMouse	*pMouse = CManager::GetInputMouse();		// 入力情報の取得(マウス)

	if (pMouse != NULL)
	{
		//=============================
		//   マウスでカメラ操作
		//=============================
		// 注視点位置変更
		if (m_MousePosOld != m_MousePos)
		{
			m_MousePosAngle = m_MousePos - m_MousePosOld;		// 差分の計算

			if (m_MousePosAngle.y < 0)
			{// Y座標(上移動)
				if (m_rot.x > -1.5f)
				{
					if (m_MousePosAngle.y <= -15.0f)
					{
						m_MousePosAngle.y = -15.0f;
					}
					m_rot.x += (m_MousePosAngle.y * 0.005f);
				}
			}
			if (m_MousePosAngle.y > 0)
			{// Y座標(下移動)
				if (m_rot.x < 1.5f)
				{
					if (m_MousePosAngle.y >= 15.0f)
					{
						m_MousePosAngle.y = 15.0f;
					}
					m_rot.x += (m_MousePosAngle.y * 0.005f);
				}
			}
			if (m_MousePosAngle.x < 0)
			{// X座標(左移動)
				m_rot.y += (m_MousePosAngle.x * 0.005f);
			}
			if (m_MousePosAngle.x > 0)
			{// X座標(右移動)
				m_rot.y += (m_MousePosAngle.x * 0.005f);
			}
		}
	}
	CDebugProc::Print(" 差分 : ( %f ,%f , %f )\n", m_MousePosAngle.x, m_MousePosAngle.y, m_MousePosAngle.z);
}
//*********************************************************************
// 視点＆注視点の移動
//*********************************************************************
void CCamera::Move_VR(void)
{
	CInputMouse	*pMouse = CManager::GetInputMouse();		// 入力情報の取得(マウス)

	if (pMouse != NULL)
	{
		// 注視点位置変更
		if (m_MousePosOld != m_MousePos)
		{
			m_MousePosAngle = m_MousePos - m_MousePosOld;		// 差分の計算

			if (m_MousePosAngle.y < 0)
			{// Y座標(下移動)
				if (m_MousePosAngle.y <= -15.0f)
				{
					m_MousePosAngle.y = -15.0f;
				}
				m_posR.y += m_MousePosAngle.y;
			}
			if (m_MousePosAngle.y > 0)
			{// Y座標(上移動)
				if (m_MousePosAngle.y >= 15.0f)
				{
					m_MousePosAngle.y = 15.0f;
				}
				m_posR.y += m_MousePosAngle.y;
			}
			if (m_MousePosAngle.x > 0)
			{// X座標(左移動)
				if (m_MousePosAngle.x >= 15.0f)
				{
					m_MousePosAngle.x = 15.0f;
				}
				m_posR.x += sinf((D3DX_PI * 0.5f) + m_rot.y)* (m_MousePosAngle.x * 2.0f);
				m_posR.z += cosf((D3DX_PI * 0.5f) + m_rot.y)* (m_MousePosAngle.x * 2.0f);
			}
			if (m_MousePosAngle.x < 0)
			{// X座標(右移動)
				if (m_MousePosAngle.x <= -15.0f)
				{
					m_MousePosAngle.x = -15.0f;
				}
				m_posR.x += sinf((D3DX_PI * 0.5f) + m_rot.y)* (m_MousePosAngle.x * 2.0f);
				m_posR.z += cosf((D3DX_PI * 0.5f) + m_rot.y)* (m_MousePosAngle.x * 2.0f);
			}
		}
	}
	CDebugProc::Print(" 差分 : ( %f ,%f , %f )\n", m_MousePosAngle.x, m_MousePosAngle.y, m_MousePosAngle.z);
}
//*********************************************************************
// カメラの最終更新
//*********************************************************************
void CCamera::SetUpCamera(void)
{
	D3DXMATRIX mtxRot, mtxTrans, mtxWorld;	// 計算用マトリックス

	// カメラの追尾
	D3DXMatrixIdentity(&mtxWorld);		// ワールドマトリックスの初期化
	// 回転
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, (m_rot.x * -1.0f), m_rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
	// 位置
	D3DXMatrixTranslation(&mtxTrans, m_posR.x, m_posR.y, m_posR.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);
	// 位置
	D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, m_Length.z);
	D3DXMatrixMultiply(&mtxTrans, &mtxTrans, &mtxWorld);		// mtxWorldの角度を利用して生成

	m_posV.x = mtxTrans._41;
	m_posV.y = mtxTrans._42;
	m_posV.z = mtxTrans._43;
}