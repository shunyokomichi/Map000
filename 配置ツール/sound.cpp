//=======================================================//
//
// サウンド処理 [Sound.cpp]
// Author : Shun Yokomichi
//
//=======================================================//

//*******************************************************//
// ヘッダファイルのインクルード
//*******************************************************//
#include "sound.h"	//サウンド

//=======================================================//
// 初期化処理(CSound)
//=======================================================//
HRESULT CSound::Init(HWND hWnd)
{
	HRESULT hr;

	// COMライブラリ初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクト作成
	hr = XAudio2Create(&m_pXAudio2, 0);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリ終了処理
		CoUninitialize();
		return E_FAIL;
	}

	// マスターボイス生成
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		// XAudio2オブジェクト開放
		if (m_pXAudio2) { m_pXAudio2->Release(); m_pXAudio2 = NULL; }

		// COMライブラリ終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// サウンドデータ初期化
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize		= 0;
		DWORD dwChunkPosition	= 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// バッファクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイル生成
		hFile = CreateFile(m_aSoundParam[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "ERROR", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// ファイルポインタを先頭に移動
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "ERROR", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVEファイルチェック
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))				{ MessageBox(hWnd, "WAVEチェック失敗(1)", "ERROR", MB_ICONWARNING);				return S_FALSE; }

		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))				{ MessageBox(hWnd, "WAVEチェック失敗(2)", "ERROR", MB_ICONWARNING);				return S_FALSE; }

		if (dwFiletype != 'EVAW')	{ MessageBox(hWnd, "WAVEチェック失敗(3)", "ERROR", MB_ICONWARNING);				return S_FALSE; }

		// フォーマットチェック
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))				{ MessageBox(hWnd, "フォーマットチェック失敗(1)", "ERROR", MB_ICONWARNING);		return S_FALSE; }

		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if (FAILED(hr))				{ MessageBox(hWnd, "フォーマットチェック失敗(2)", "ERROR", MB_ICONWARNING);		return S_FALSE; }

		// オーディオデータ読み込み
		hr = CheckChunk(hFile, 'atad', &m_aSizeAudio[nCntSound], &dwChunkPosition);
		if (FAILED(hr))				{ MessageBox(hWnd, "オーディオデータ読み込み失敗(1)", "ERROR", MB_ICONWARNING); return S_FALSE; }
		m_apDataAudio[nCntSound] = (BYTE*)malloc(m_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, m_apDataAudio[nCntSound], m_aSizeAudio[nCntSound], dwChunkPosition);
		if (FAILED(hr))				{ MessageBox(hWnd, "オーディオデータ読み込み失敗(2)", "ERROR", MB_ICONWARNING); return S_FALSE; }

		// ソースボイス生成
		hr = m_pXAudio2->CreateSourceVoice(&m_apSourceVoice[nCntSound], &(wfx.Format));
		if (FAILED(hr))				{ MessageBox(hWnd, "ソースボイス生成失敗", "ERROR", MB_ICONWARNING);			return S_FALSE; }

		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes	= m_aSizeAudio[nCntSound];
		buffer.pAudioData	= m_apDataAudio[nCntSound];
		buffer.Flags		= XAUDIO2_END_OF_STREAM;
		buffer.LoopCount	= m_aSoundParam[nCntSound].nCntLoop;

		// オーディオバッファ登録
		m_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);
	}
	return S_OK;
}

//=======================================================//
// 終了処理(CSound)
//=======================================================//
void CSound::Uninit(void)
{
	// 一時停止
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if (m_apSourceVoice[nCntSound])
		{
			// 一時停止
			m_apSourceVoice[nCntSound]->Stop(0);

			// ソースボイス破棄
			m_apSourceVoice[nCntSound]->DestroyVoice();
			m_apSourceVoice[nCntSound] = NULL;

			// オーディオデータ開放
			free(m_apDataAudio[nCntSound]);
			m_apDataAudio[nCntSound] = NULL;
		}
	}

	// マスターボイス破棄
	m_pMasteringVoice->DestroyVoice();
	m_pMasteringVoice = NULL;

	// XAudio2オブジェクト開放
	if (m_pXAudio2) { m_pXAudio2->Release(); m_pXAudio2 = NULL; }

	// COMライブラリ終了処理
	CoUninitialize();
}

//=======================================================//
// セグメント再生(停止)(CSound)
//=======================================================//
HRESULT CSound::PlaySound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes	= m_aSizeAudio[label];
	buffer.pAudioData	= m_apDataAudio[label];
	buffer.Flags		= XAUDIO2_END_OF_STREAM;
	buffer.LoopCount	= m_aSoundParam[label].nCntLoop;

	// 状態取得
	m_apSourceVoice[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		m_apSourceVoice[label]->Stop(0);

		// オーディオバッファ削除
		m_apSourceVoice[label]->FlushSourceBuffers();
	}

	// オーディオバッファ登録
	m_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

	// 再生
	m_apSourceVoice[label]->Start(0);

	return S_OK;
}

//=======================================================//
// セグメント停止(CSound)
//=======================================================//
void CSound::StopSound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	m_apSourceVoice[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		m_apSourceVoice[label]->Stop(0);

		// オーディオバッファ削除
		m_apSourceVoice[label]->FlushSourceBuffers();
	}
}

//=======================================================//
// セグメント停止2(CSound)
//=======================================================//
void CSound::StopSound(void)
{
	// 一時停止
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if (m_apSourceVoice[nCntSound])
		{
			// 一時停止
			m_apSourceVoice[nCntSound]->Stop(0);

			// オーディオバッファ削除
			m_apSourceVoice[nCntSound]->FlushSourceBuffers();
		}
	}
}

//=======================================================//
// チャンクチェック(CSound)
//=======================================================//
HRESULT CSound::CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;

	// ファイルポインタを先頭に移動
	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER) { return HRESULT_FROM_WIN32(GetLastError()); }

	while (hr == S_OK)
	{
		// チャンク読み込み
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0) { hr = HRESULT_FROM_WIN32(GetLastError()); }

		// チャンクデータ読み込み
		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0) { hr = HRESULT_FROM_WIN32(GetLastError()); }

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize	= dwChunkDataSize;
			dwChunkDataSize = 4;

			// ファイルタイプ読み込み
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0) { hr = HRESULT_FROM_WIN32(GetLastError()); }
			break;

		default:
			// ファイルポインタをチャンクデータ分移動
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER) { return HRESULT_FROM_WIN32(GetLastError()); }
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format) { *pChunkSize = dwChunkDataSize; *pChunkDataPosition = dwOffset; return S_OK; }

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize) { return S_FALSE; }
	}
	return S_OK;
}

//=======================================================//
// チャンクデータ読み込み(CSound)
//=======================================================//
HRESULT CSound::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	// ファイルポインタを指定位置まで移動
	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER) { return HRESULT_FROM_WIN32(GetLastError()); }

	// データ読み込み
	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0) { return HRESULT_FROM_WIN32(GetLastError()); }
	return S_OK;
}

//=======================================================//
// 音量変更(CSound)
//=======================================================//
void CSound::ChangeVolume(SOUND_LABEL label, float fVolume) { m_apSourceVoice[label]->SetVolume(fVolume); }

//=======================================================//
// ピッチ変更(CSound)
//=======================================================//
void CSound::ChagePitch(SOUND_LABEL label, float fPitch) { m_apSourceVoice[label]->SetFrequencyRatio(fPitch); }