//**************************************************************************
//
//				デバック表示処理[debugproc.cpp]
//				Auther : Shun Yokomichi
//
//**************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#include "debugproc.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include <stdarg.h >
#include <string.h>

//*********************************************************************
//	静的メンバ変数宣言
//*********************************************************************
LPD3DXFONT		CDebugProc::m_pFont = NULL;
char			CDebugProc::m_aStr[MAX_WORD] = {};
bool CDebugProc::m_bUse = false;

//*********************************************************************
//	コンストラクタ
//*********************************************************************
CDebugProc::CDebugProc()
{
	// デフォルト値にする
//	m_aStr[MAX_WORD] = {};
}

//*********************************************************************
//	デストラクタ
//*********************************************************************
CDebugProc::~CDebugProc() {}

//*********************************************************************
//	初期化処理
//*********************************************************************
HRESULT CDebugProc::Init()
{
	//m_aStr[MAX_WORD] = {};

	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// デバッグ情報表示用フォントの生成
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "メイリオ", &m_pFont);
	return S_OK;
}

//*********************************************************************
//	終了処理
//*********************************************************************
void CDebugProc::Uninit()
{
	// デバッグ情報表示用フォントの破棄
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
}

//*********************************************************************
//	描画処理
//*********************************************************************
void CDebugProc::Draw()
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	m_aStr[MAX_WORD];

	if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_F1))
	{
		m_bUse = m_bUse ? false : true;		// 使用切り替え
	}

	// テキスト描画
	if (m_bUse == false)
	{
		m_pFont->DrawText(NULL, &m_aStr[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	}

	m_aStr[0] = '\0';		// クリア
}

//*********************************************************************
//	デバック表示
//*********************************************************************
void CDebugProc::Print(char *fmt, ...)
{
	va_list v1;
	char cString[MAX_WORD];	// 保管用に
	cString[0] = '\0';		// 文字のクリア
	va_start(v1, fmt);

	vsprintf(cString, fmt, v1);

	strcat(m_aStr, cString);	// 保管していた文字をおおもとへ

	va_end(v1);		// リストの終了
}