//=============================================================================
//
// オブジェクト表示処理 [said.cpp]
// Author : Shun Yokomichi
//
//=============================================================================
#include "display.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODE_MOVE			(10.0f)			// 移動量
#define MODE_DECELERATION	(0.2f)			// 速度減速
#define Y_MOVE				(1.0f)			// Y軸の上げ下げの幅
#define ROT_VECTER_NUM		(4)
#define PRESS_COUNT			(10)			// 押している時間の条件
#define SQUARE_MOVE			(5.0f)			// 升目移動量
#define ADD_MOVE			(5.0f)			// 通常移動量
#define SLOWING_DOWN		(0.2f)			// 減速量
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
D3DXVECTOR3				CDisplay::m_pos = {};						// 位置
D3DXVECTOR3				CDisplay::m_rot = {};						// 向き
bool					CDisplay::m_bUse = false;
//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CDisplay::CDisplay()
{
	m_move		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
	m_rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き情報(現在)
	m_rotDest	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き情報(目的)
	m_nNumRot	= 0;
	m_nCntPress = 0;
	m_fMove		= 0.0f;
	m_State		= STATE_NONE;
	m_bPress	= false;
	//m_SizeType	= TYPE_SIZE_BIG;
}
//*****************************************************************************
// デストラクタ
//*****************************************************************************
CDisplay::~CDisplay()
{
}
//*****************************************************************************
// 基本操作
//*****************************************************************************
void CDisplay::BasicOperation(void)
{
	float fRot = 0.0f;		// 回転量の保管

	CCamera *pCamera = CManager::GetCamera();	// カメラの取得
	D3DXVECTOR3 CameraRot = pCamera->GetRot();		// カメラの向き取得

	if (CGame::GetTabCount() == 1)
	{
		MoveChange(CameraRot);		// 移動方法の変化
	}

	if (m_State == STATE_TRIGGER) { m_bPress = true; }		// 判定をTRIGGERに
	else { m_bPress = false; }

	CDebugProc::Print("**********************************************************\n");
	CDebugProc::Print(" 左SHIFT 長押し						: マス目移動\n");								// マス目移動
	CDebugProc::Print(" W 前移動							: ( Z     / %0.1f )\n", CDisplay::m_pos.z);		// 前移動
	CDebugProc::Print(" S 後移動							: ( Z     / %0.1f )\n", CDisplay::m_pos.z);		// 後移動
	CDebugProc::Print(" A 左移動							: ( X     / %0.1f )\n", CDisplay::m_pos.x);		// 左移動
	CDebugProc::Print(" D 右移動							: ( X     / %0.1f )\n", CDisplay::m_pos.x);		// 右移動
	CDebugProc::Print(" R 上昇								: ( Y     / %0.1f )\n", CDisplay::m_pos.y);		// 上昇
	CDebugProc::Print(" F 下降								: ( Y     / %0.1f )\n", CDisplay::m_pos.y);		// 下降
	CDebugProc::Print("**********************************************************\n");
	CDebugProc::Print(" 回転設定\n");																		// 回転設定
	CDebugProc::Print(" Q 右回転							: ( rot.Y / %0.1f )\n", CDisplay::m_rot.y);		// 右回転
	CDebugProc::Print(" E 左回転							: ( rot.Y / %0.1f )\n", CDisplay::m_rot.y);		// 左回転
	CDebugProc::Print("**********************************************************\n");
	CDebugProc::Print(" リセット方法\n");																	// リセット方法
	CDebugProc::Print(" Z 位置と向きをリセット\n");															// 位置と向きをリセット
	CDebugProc::Print(" X 位置をリセット\n");																// 位置をリセット
	CDebugProc::Print(" C 向きをリセット\n");																// 向きをリセット
}

//*****************************************************************************
// 基本操作(向き設定)
//*****************************************************************************
void CDisplay::BasicSetRot(void)
{
	if (CGame::GetTabCount() == 1)
	{
		float fRotY = 0.0f;
		if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_LSHIFT))
		{
			fRotY = 0.5f;
			// 回転操作
			if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_Q))
			{	// 左回転
				m_nNumRot = (m_nNumRot + 1) % ROT_VECTER_NUM;		// 選択モデルを変える
			}
			else if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_E))
			{	// 右回転
				m_nNumRot = (m_nNumRot + (ROT_VECTER_NUM - 1)) % ROT_VECTER_NUM;		// 選択モデルを変える
			}

			if (m_nNumRot >= ROT_VECTER_NUM - 1)
			{
				fRotY = fRotY - (0.5f * (ROT_VECTER_NUM - m_nNumRot));
			}
			m_rot.y = D3DX_PI * (0.5f * m_nNumRot);
		}
		else
		{
			if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_Q))
			{	// 左回転
				m_rot.y += 0.05f;		// 選択モデルを変える
			}
			else if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_E))
			{	// 右回転
				m_rot.y -= 0.05f;		// 選択モデルを変える
			}
		}

		if (m_rot.y > D3DX_PI) { m_rot.y -= D3DX_PI * 2.0f; }
		if (m_rot.y < -D3DX_PI) { m_rot.y += D3DX_PI * 2.0f; }
	}
}

//*****************************************************************************
// 基本操作(移動方法選択)
//*****************************************************************************
void CDisplay::MoveChange(D3DXVECTOR3 fCameraRot)
{
	if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_LSHIFT)) { m_bFlag = true; }
	else { m_bFlag = false; }

	if (m_bFlag == true) { SquareMove(fCameraRot); }			// マス目移動
	else if (m_bFlag == false) { NomalMove(fCameraRot); }		// 通常移動
}

//*****************************************************************************
// 移動方法(升目移動)
//*****************************************************************************
void CDisplay::SquareMove(D3DXVECTOR3 fCameraRot)
{
	// 移動
	if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_L))
	{	// Lキー押下の場合
		if (m_State == STATE_NONE)
		{
			m_SizeType = TYPE_SIZE_BIG;
			m_pos.x -= sinf((D3DX_PI * 0.5f) + fCameraRot.y) * (m_sizeMax.x * m_ChangeNum.x);
			m_pos.z -= cosf((D3DX_PI * 0.5f) + fCameraRot.y) * (m_sizeMax.x * m_ChangeNum.x);
			m_State = STATE_TRIGGER;
		}
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_J))
	{	// Jキー押下の場合
		if (m_State == STATE_NONE)
		{
			m_SizeType = TYPE_SIZE_MIN;
			m_pos.x -= sinf((D3DX_PI * 0.5f) + fCameraRot.y) * (m_sizeMin.x * m_ChangeNum.y);
			m_pos.z -= cosf((D3DX_PI * 0.5f) + fCameraRot.y) * (m_sizeMin.x * m_ChangeNum.y);
			m_State = STATE_TRIGGER;
		}
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_K))
	{	// Kキー押下
		if (m_State == STATE_NONE)
		{
			m_SizeType = TYPE_SIZE_MIN;
			m_pos.x += sinf(D3DX_PI + fCameraRot.y) * (m_sizeMin.x * m_ChangeNum.y);
			m_pos.z += cosf(D3DX_PI + fCameraRot.y) * (m_sizeMin.x * m_ChangeNum.y);
			m_State = STATE_TRIGGER;
		}
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_I))
	{	// Iキー押下
		if (m_State == STATE_NONE)
		{
			m_SizeType = TYPE_SIZE_BIG;
			m_pos.x -= sinf(0 + fCameraRot.y) * (m_sizeMax.x * m_ChangeNum.x);
			m_pos.z -= cosf(0 + fCameraRot.y) * (m_sizeMax.x * m_ChangeNum.x);
			m_State = STATE_TRIGGER;
		}
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_UP))
	{
		if (m_State == STATE_NONE)
		{
			m_SizeType = TYPE_SIZE_MIN;
			m_pos.y += m_sizeMin.y;
			m_State = STATE_TRIGGER;
		}
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_DOWN))
	{
		if (m_State == STATE_NONE)
		{
			m_SizeType = TYPE_SIZE_BIG;
			m_pos.y += m_sizeMax.y;
			m_State = STATE_TRIGGER;
		}
	}

	StateCounter(m_bPress);		// 升目移動用のフラグ
}

//*****************************************************************************
// 移動方法(通常移動)
//*****************************************************************************
void CDisplay::NomalMove(D3DXVECTOR3 fCameraRot)
{
	// 移動
	if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_D))
	{	// Lキー押下の場合
		m_pos.x += sinf((D3DX_PI * 0.5f) + fCameraRot.y) * ADD_MOVE;
		m_pos.z += cosf((D3DX_PI * 0.5f) + fCameraRot.y) * ADD_MOVE;
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_A))
	{	// Jキー押下の場合
		m_pos.x += sinf((-D3DX_PI * 0.5f) + fCameraRot.y) * ADD_MOVE;
		m_pos.z += cosf((-D3DX_PI * 0.5f) + fCameraRot.y) * ADD_MOVE;
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_S))
	{	// Kキー押下
		m_pos.x += sinf(D3DX_PI + fCameraRot.y) * ADD_MOVE;
		m_pos.z += cosf(D3DX_PI + fCameraRot.y) * ADD_MOVE;
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_W))
	{	// Iキー押下
		m_pos.x += sinf(0 + fCameraRot.y) * ADD_MOVE;
		m_pos.z += cosf(0 + fCameraRot.y) * ADD_MOVE;
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_R))
	{
		m_pos.y += ADD_MOVE;
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_F))
	{
		m_pos.y -= ADD_MOVE;
	}
}

//*****************************************************************************
// 基本操作(処理選択)
//*****************************************************************************
int CDisplay::SelectNum(int nSelectNum, int nMaxOperation, int nButtan000, int nButtan001)
{
	if (CManager::GetInputKeyboard()->GetKeyboardTrigger(nButtan000) == true)
	{	// Zキー押下の場合
		nSelectNum = (nSelectNum + (nMaxOperation - 1)) % nMaxOperation;		// 選択番号を変える
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardTrigger(nButtan001) == true)
	{	// Zキー押下の場合
		nSelectNum = (nSelectNum + 1) % nMaxOperation;		// 選択番号を変える
	}

	return nSelectNum;
}

//*****************************************************************************
// リセットボタン
//*****************************************************************************
void CDisplay::ResetButton(void)
{
	if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_Z))
	{	// Rキーを押下した場合
		m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置情報リセット
		m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き情報リセット
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_X))
	{	// Bキーを押下した場合
		m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向きのリセット
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_C))
	{	// Fキーを押下した場合
		m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置のリセット
	}
}

//*****************************************************************************
// 状態の管理
//*****************************************************************************
void CDisplay::StateCounter(bool bPress)
{
	if (m_bPress == true)
	{	// 判定がtrueなら
		m_nCntPress++;	//加算
		if ((m_nCntPress % PRESS_COUNT) == 0)
		{
			m_State = STATE_NONE;
			m_nCntPress = 0;
		}
	}
}
