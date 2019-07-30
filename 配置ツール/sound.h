//=======================================================//
//
// サウンド処理 [Sound.h]
// Author : Shun Yokomichi
//
//=======================================================//
#ifndef _SOUND_H_
#define _SOUND_H_

//*******************************************************//
// ヘッダファイルのインクルード
//*******************************************************//
#include "main.h"	// メイン

//*******************************************************//
// クラス定義
//*******************************************************//
class CSound
{// CSound
public:
	//*************************************
	// サウンドファイル
	//*************************************
	typedef enum
	{// サウンド名
		// BGM
		SOUND_LABEL_BGM000 = 0,	// BGM(タイトル)
		SOUND_LABEL_BGM001,		// BGM(チュートリアル)
		SOUND_LABEL_BGM002,		// BGM(ゲーム)
		SOUND_LABEL_BGM003,		// BGM(ゲーム2)
		SOUND_LABEL_BGM004,		// BGM(リザルト)
		SOUND_LABEL_BGM005,		// BGM(リザルト2)

		// SE(システム)
		SOUND_LABEL_SE000,		// SE(ポーズメニュー表示)
		SOUND_LABEL_SE001,		// SE(決定)
		SOUND_LABEL_SE002,		// SE(セレクト)
		SOUND_LABEL_SE003,		// SE(キャンセル)

		// SE(プレイヤー周辺)
		SOUND_LABEL_SE004,		// SE(歩行(平地))
		SOUND_LABEL_SE005,		// SE(ダッシュ(平地))
		SOUND_LABEL_SE006,		// SE(ジャンプ)
		SOUND_LABEL_SE007,		// SE(着地)
		SOUND_LABEL_SE008,		// SE(死亡)

		// SE(環境音)
		SOUND_LABEL_SE009,		// SE(巨人の足音)
		SOUND_LABEL_MAX,		// 最大数
	} SOUND_LABEL;

	CSound() {};	// コンストラクタ(中身無し)
	~CSound() {};	// デストラクタ(中身無し)

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT PlaySound(SOUND_LABEL label);
	void StopSound(SOUND_LABEL label);
	void StopSound(void);
	void ChangeVolume(SOUND_LABEL label, float fVolume);
	void ChagePitch(SOUND_LABEL label, float fPitch);

private:
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント(-1でループ再生)
	} SOUNDPARAM;

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2									= NULL;	// XAudio2オブジェクトへのポインタ
	IXAudio2MasteringVoice *m_pMasteringVoice				= NULL;	// マスターボイスへのポインタ
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX]	= {};	// ソースボイスへのポインタ
	BYTE *m_apDataAudio[SOUND_LABEL_MAX]					= {};	// オーディオデータへのポインタ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX]						= {};	// オーディオデータサイズ

	// 各音素材のパラメータ
	SOUNDPARAM m_aSoundParam[SOUND_LABEL_MAX] =
	{// データパス(サウンドリスト)
		// BGM
		{ "Data/SOUND/BGM/Title002.wav", -1 },				// BGM0	// タイトル
		{ "Data/SOUND/BGM/Tutorial_Loop.wav", -1 },			// BGM1 // チュートリアル
		{ "Data/SOUND/BGM/Game000.wav", -1 },				// BGM2	// ゲーム
		{ "Data/SOUND/BGM/Game003.wav", -1 },				// BGM3	// ゲーム2
		{ "Data/SOUND/BGM/GameClear.wav", -1 },				// BGM4	// リザルト
		{ "Data/SOUND/BGM/Result000.wav", -1 },				// BGM5 // リザルト2

		// システムSE
		{ "Data/SOUND/SE/SYSTEM/Pause_Open.wav", 0 },		// SE0	// ポーズメニュー表示
		{ "Data/SOUND/SE/SYSTEM/Pause_Decision.wav", 0 },	// SE1	// 決定
		{ "Data/SOUND/SE/SYSTEM/Pause_Select.wav", 0 },		// SE2	// セレクト
		{ "Data/SOUND/SE/SYSTEM/Pause_Cancel.wav", 0 },		// SE3	// キャンセル

		// キャラクター周辺SE
		{ "Data/SOUND/SE/PLAYER/Walk_Normal.wav", 0 },		// SE4	// 歩行(平地)
		{ "Data/SOUND/SE/PLAYER/Dash_Normal.wav", 0 },		// SE5	// ダッシュ(平地)
		{ "Data/SOUND/SE/PLAYER/Action_Jump.wav", 0 },		// SE6	// ジャンプ
		{ "Data/SOUND/SE/PLAYER/Action_Landing.wav", 0 },	// SE7	// 着地
		{ "Data/SOUND/SE/PLAYER/Action_Death.wav", 0 },		// SE8	// 死亡

		// 環境SE
		{ "Data/SOUND/SE/GAME/Walk_Giant.wav", 0 },			// SE9	// 巨人の足音
	};
};
#endif