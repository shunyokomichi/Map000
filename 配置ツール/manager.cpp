//=======================================================//
//
// �}�l�[�W���[���� [Manager.cpp]
// Author : Shun Yokomichi
//
//=======================================================//

//*******************************************************//
// �w�b�_�t�@�C���̃C���N���[�h
//*******************************************************//
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[
#include "input.h"			// ����
#include "scene.h"			// Scene
#include "camera.h"			// �J����
#include "light.h"			// ���C�g
#include "fade.h"			// �t�F�[�h
#include "game.h"			// �Q�[��
#include "sound.h"			// �T�E���h
#include "debugproc.h"		// �f�o�b�N
#include "texture.h"
//*******************************************************//
// �}�N����`
//*******************************************************//
#define MODE_0	(MODE_TITLE)	// �^�C�g��
#define MODE_1	(MODE_TUTORIAL)	// �`���[�g���A��
#define MODE_2	(MODE_GAME)		// �Q�[��
#define MODE_3	(MODE_RESULT)	// ���U���g
#define MODE_4	(MODE_RANKING)	// �����L���O(���ݖ���)
#define MODE_OPTION	(MODE_2)	// ���[�h�ݒ�

//*******************************************************//
// �ÓI�����o�ϐ�
//*******************************************************//
CManager::MODE	CManager	::m_mode			= CManager::MODE_NONE;	// ���[�h
CRenderer		*CManager	::m_pRenderer		= NULL;	// �����_���[
CInputKeyboard	*CManager	::m_pInputKeyboard	= NULL;	// �L�[�{�[�h
CInputJoypad	*CManager	::m_pInputJoypad	= NULL;	// �R���g���[���[
//CInputDInputPad	*CManager	::m_pInputDIPad		= NULL;	// DInput�p�b�h
//CInputXPad		*CManager	::m_pInputXPad		= NULL;	// X�p�b�h
CInputMouse		*CManager	::m_pInputMouse		= NULL;	// �}�E�X
CCamera			*CManager	::m_pCamara			= NULL;	// �J����
CLight			*CManager	::m_pLight			= NULL;	// ���C�g
CGame			*CManager	::m_pGame			= NULL;	// �Q�[��
CPause			*CManager	::m_pPause			= NULL;	// �|�[�Y
CSound			*CManager	::m_pSound			= NULL;	// �T�E���h
int				CManager	::m_nPlayNum		= 0;	// �v���C�l��
CDebugProc		*CManager	::m_pDebug			= NULL;	// �f�o�b�N

//=======================================================//
// ����������(CManager)
//=======================================================//
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	//********************************************//
	//
	//	* �e�t���[�����[�N�̃������m�ہE����������
	//	[NULL�̏ꍇ�A�������m��]
	//	[�m�ۊ�����A����������]
	//	* ERROR�������p�̃��b�Z�[�WBOX����
	//
	//********************************************//

	// �T�E���h
	if (m_pSound == NULL)			{ m_pSound = new CSound; }
	else							{ MessageBox(0, "(manager�uInit] : Sound��NULL�ł͂Ȃ�", "ERROR", MB_OK); }
	if (m_pSound != NULL)			{ if (FAILED(m_pSound->Init(hWnd))) { return -1; } }
	else							{ MessageBox(0, "(manager�uInit] : Sound��NULL", "ERROR", MB_OK); }

	//�L�[�{�[�h
	if (m_pInputKeyboard == NULL)	{ m_pInputKeyboard = CInputKeyboard::Create(hInstance, hWnd); }
	else							{ MessageBox(0, "(manager�uInit] : InputKeyboard��NULL�ł͂Ȃ�", "ERROR", MB_OK); }

	//	�R���g���[���[
	if (m_pInputJoypad == NULL)		{ m_pInputJoypad = CInputJoypad::Create(hInstance, hWnd); }
	else							{ MessageBox(0, "(manager�uInit] : InputJoypad��NULL�ł͂Ȃ�", "ERROR", MB_OK); }
	//	�}�E�X
	if (m_pInputMouse == NULL)
	{
		m_pInputMouse = CInputMouse::Create(hInstance, hWnd);
	}
	else { MessageBox(0, "(manager�uInit] : InputMouse��NULL�ł͂Ȃ�", "ERROR", MB_OK); }

	//// �L�[�{�[�h
	//if (m_pInputKeyboard == NULL)	{ m_pInputKeyboard = new CInputKeyboard; }
	//else							{ MessageBox(0, "(manager�uInit] : InputKeyboard��NULL�ł͂Ȃ�", "ERROR", MB_OK); }
	//if (m_pInputKeyboard != NULL)	{ if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd))) { return -1; } }
	//else							{ MessageBox(0, "(manager�uInit] : InputKeyboard��NULL", "ERROR", MB_OK); }

	//if (m_pInputDIPad == NULL)		{ m_pInputDIPad = new CInputDInputPad; }
	//else							{ MessageBox(0, "(manager�uInit] : InputDIPad��NULL�ł͂Ȃ�", "ERROR", MB_OK); }
	//if (m_pInputDIPad != NULL)		{ if (FAILED(m_pInputDIPad->Init(hInstance, hWnd))) { return -1; } }
	//else							{ MessageBox(0, "(manager�uInit] : InputDIPad��NULL", "ERROR", MB_OK); }

	//// X�p�b�h
	//if (m_pInputXPad == NULL)		{ m_pInputXPad = new CInputXPad; }
	//else							{ MessageBox(0, "(manager�uInit] : InputXPad��NULL�ł͂Ȃ�", "�x��", MB_OK); }
	//if (m_pInputXPad != NULL)		{ if (FAILED(m_pInputXPad->Init(hInstance, hWnd))) { return -1; } }
	//else							{ MessageBox(0, "(manager�uInit] : InputXPad��NULL", "ERROR", MB_OK); }

	// �}�E�X
	//if (m_pInputMouse == NULL) { m_pInputMouse = new CInputMouse; }
	//else { MessageBox(0, "(manager�uInit] : InputMouse��NULL�ł͂Ȃ�", "�x��", MB_OK); }
	//if (m_pInputMouse != NULL) { if (FAILED(m_pInputMouse->Init(hInstance, hWnd))) { return -1; } }
	//else { MessageBox(0, "(manager�uInit] : InputMouse��NULL", "ERROR", MB_OK); }

	if (m_pInputMouse == NULL) { m_pInputMouse = m_pInputMouse->Create(hInstance, hWnd); }


	// �����_���[
	if (m_pRenderer == NULL)		{ m_pRenderer = new CRenderer; }
	else							{ MessageBox(0, "(manager�uInit] : Renderer��NULL�ł͂Ȃ�", "ERROR", MB_OK); }
	if (m_pRenderer != NULL)		{ if (FAILED(m_pRenderer->Init(hWnd, bWindow))) { return -1; } }
	else							{ MessageBox(0, "(manager�uInit] : Renderer��NULL", "ERROR", MB_OK); }

	// �J����
	if (m_pCamara == NULL)			{ m_pCamara = new CCamera; }
	else							{ MessageBox(0, "(manager�uInit] : Camera��NULL�ł͂Ȃ�", "ERROR", MB_OK); }
	if (m_pCamara != NULL)			{ if (FAILED(m_pCamara->Init())) { return -1; } }
	else							{ MessageBox(0, "(manager�uInit] : Camera��NULL", "ERROR", MB_OK); }

	// ���C�g
	if (m_pLight == NULL)			{ m_pLight = new CLight; }
	else							{ MessageBox(0, "(manager�uInit] : Light��NULL�ł͂Ȃ�", "ERROR", MB_OK); }
	if (m_pLight != NULL)			{ if (FAILED(m_pLight->Init())) { return -1; } }
	else							{ MessageBox(0, "(manager�uInit] : Light��NULL", "�x��", MB_OK); }

	// �f�o�b�N
	if (m_pDebug == NULL)			{ m_pDebug = new CDebugProc; }
	else							{ MessageBox(0, "(manager�uInit] : Debug��NULL�ł͂Ȃ�", "ERROR", MB_OK); }
	if (m_pDebug != NULL)			{ m_pDebug->Init(); }
	else							{ MessageBox(0, "(manager�uInit] : Debug��NULL", "�x��", MB_OK); }

	// �e�N�X�`���ǂݍ���
	CTexture::Load();

	SetMode(MODE_OPTION);	// ���[�h�ݒ�
	m_bPause = false;		// �|�[�Y�g�p���
	return S_OK;
}

//=======================================================//
// �I������(CManager)
//=======================================================//
void CManager::Uninit(void)
{
	//********************************************//
	//
	//	* �e�t���[�����[�N�̃��������ݎ��E�I������
	//	[NULL�ȊO�̎��A�I������]
	//	[����������A�������j��]
	//	[�S�s���I����A�l��NULL�ɍĐݒ�]
	//
	//********************************************//

	CTexture::Unload();
	if (m_pSound			!= NULL) { m_pSound->Uninit();			delete m_pSound;			m_pSound			= NULL; }	// �T�E���h
	if (m_pInputKeyboard	!= NULL) { m_pInputKeyboard->Uninit();	delete m_pInputKeyboard;	m_pInputKeyboard	= NULL; }	// �L�[�{�[�h
	if (m_pInputJoypad		!= NULL) { m_pInputJoypad->Uninit();	delete m_pInputJoypad;		m_pInputJoypad		= NULL; }	// �L�[�{�[�h
	//if (m_pInputDIPad		!= NULL) { m_pInputDIPad->Uninit();		delete m_pInputDIPad;		m_pInputDIPad		= NULL; }	// DInput�p�b�h
	//if (m_pInputXPad		!= NULL) { m_pInputXPad->Uninit();		delete m_pInputXPad;		m_pInputXPad		= NULL; }	// X�p�b�h
	if (m_pInputMouse		!= NULL) { m_pInputMouse->Uninit();		delete m_pInputMouse;		m_pInputMouse = NULL; }	// �}�E�X


	switch (m_mode)
	{	// ���[�h
	case CManager::MODE_GAME:		// �Q�[��
		if (m_pGame		!= NULL) { m_pGame->Uninit();		delete m_pGame;		m_pGame		= NULL; }
		break;
	}

	if (m_pRenderer != NULL) { m_pRenderer->Uninit();	delete m_pRenderer;	m_pRenderer = NULL; }	// �����_���[
	if (m_pCamara	!= NULL) { m_pCamara->Uninit();		delete m_pCamara;	m_pCamara	= NULL; }	// �J����
	if (m_pLight	!= NULL) { m_pLight->Uninit();		delete m_pLight;	m_pLight	= NULL; }	// ���C�g
	if (m_pDebug != NULL)	 { m_pDebug->Uninit();		delete m_pDebug;	m_pDebug = NULL; }		// �f�o�b�N

}

//=======================================================//
// �X�V����(CManager)
//=======================================================//
void CManager::Update(void)
{
	//********************************************//
	//
	//	* �e�t���[�����[�N�̍X�V����
	//	[NULL�ȊO�̎��A�X�V����]
	//
	//	MODE_GAME��	[�|�[�Y����]
	//
	//********************************************//

	if (m_pInputKeyboard	!= NULL) { m_pInputKeyboard->Update(); }	// �L�[�{�[�h
	if (m_pInputJoypad		!= NULL) { m_pInputJoypad->Update(); }		// �R���g���[���[
	//if (m_pInputDIPad		!= NULL) { m_pInputDIPad->Update(); }		// DInput�p�b�h
	//if (m_pInputXPad		!= NULL) { m_pInputXPad->Update(); }		// X�p�b�h
	if (m_pInputMouse		!= NULL) { m_pInputMouse->Update(); }		// �}�E�X


	switch (m_mode)
	{	// ���[�h
	case CManager::MODE_GAME:		// �Q�[��
		if (m_pGame != NULL)
		{// NULL�ȊO�̏ꍇ
			if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_P) && CFade::GetFade() == CFade::FADE_NONE || CManager::GetInputJoypad()->GetTrigger(XINPUT_GAMEPAD_START) == true && CFade::GetFade() == CFade::FADE_NONE)
			{
				// �|�[�Y����(�M��֥�L)
			}

			m_pGame->Update();
		}
		break;
	}

	if (m_pRenderer != NULL) { m_pRenderer->Update(); }	// �����_���[
	if (m_pCamara	!= NULL) { m_pCamara->Update(); }	// �J����
	if (m_pLight	!= NULL) { m_pLight->Update(); }	// ���C�g
}

//=======================================================//
// �`�揈��(CManager)
//=======================================================//
void CManager::Draw(void)
{
	//********************************************//
	//
	//	* �e�t���[�����[�N�̕`�揈��
	//	�����_���[	[NULL�ȊO�̎��A�`�揈��]
	//	�J����		[NULL�ȊO�̎��A�ݒ菈��]
	//
	//********************************************//

	if (m_pRenderer != NULL) { m_pRenderer->Draw(); }		// �����_���[
	if (m_pCamara	!= NULL) { m_pCamara->SetCamera(); }	// �J����
}

//=======================================================//
// �ݒ菈��(GameMode)(CManager)
//=======================================================//
void CManager::SetMode(CManager::MODE mode)
{
	//********************************************//
	//
	//	* �e�Q�[�����[�h�̐ݒ菈��
	//	[�Q�[���̏�Ԃ��擾]
	//	[NULL�ȊO�̎��A���[�h�ݒ�ƎQ��]
	//	[���[�h��������ꍇ�A�J��������]
	//	[���̌�A�I������ �� �������j�� �� NULL���]
	//
	//********************************************//

	// �Q�[����Ԏ擾
	CGame::MODEGAME pGame = m_pGame->GetGame();

	switch (m_mode)
	{	// ���[�h�i���݁j
	case CManager::MODE_GAME:		// �Q�[��
		if (m_pGame != NULL)
		{
			// ���[�h���������
			if (m_mode != mode) { m_pGame->Unload(); }

			// ����ȏꍇ
			m_pGame->Uninit(); delete m_pGame; m_pGame = NULL;
		}
		m_pSound->StopSound(CSound::SOUND_LABEL_BGM002);
		break;
	}

	// ������m��
	char cName[32];

	switch (mode)
	{	// ���[�h�i����j
	case CManager::MODE_GAME:		wsprintf(cName, "Game");		break;	// �Q�[��
	}



	//********************************************//
	//
	//	* �e�Q�[�����[�h�̐ݒ菈��
	//	[NULL�̎��A�������m��]
	//	[NULL�ȊO�̎��A���[�h�Q��]
	//	[����ȏꍇ�A���������� �� �^�C�g���̂݃v���C���[��������]
	//	[���[�h��������ꍇ�A�ǂݍ��ݏ�����]
	//
	//********************************************//

	switch (mode)
	{	// ���[�h�i����j
	case CManager::MODE_GAME:		// �Q�[��
		if (m_pGame == NULL)
		{	// �������m��
			m_pGame = new CGame;

			if (m_pGame != NULL)
			{
				// ���[�h���������
				if (m_mode != mode) { m_pGame->Load(); }

				// ����ȏꍇ
				m_pGame->Init();
			}
		}
		m_pSound->PlaySoundA(CSound::SOUND_LABEL_BGM001);
		break;
	}

	m_mode = mode;	// ���[�h�ύX
}
