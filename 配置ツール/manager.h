//=======================================================//
//
// �}�l�[�W���[���� [Manager.h]
// Author : Shun Yokomichi
//
//=======================================================//
#ifndef _MANAGER_H_
#define _MANAGER_H_

//*******************************************************//
// �w�b�_�t�@�C���̃C���N���[�h
//*******************************************************//
#include "main.h"	// ���C��

//*******************************************************//
// �}�N����`
//*******************************************************//
#define MAX_PLAYER	(2)	// �v���C���[�ő吔

//*******************************************************//
// �O���錾
//*******************************************************//
class CRenderer;		// �����_���[
class CInputKeyboard;	// �L�[�{�[�h
class CInputJoypad;		// �R���g���[���[
//class CInputDInputPad;// DInout�p�b�h
//class CInputXPad;		// X�p�b�h
class CInputMouse;		// �}�E�X
class CCamera;			// �J����
class CLight;			// ���C�g
class CGame;			// �Q�[��
class CPause;			// �|�[�Y
class CSound;			// �T�E���h
class CDebugProc;		// �f�o�b�N

//*******************************************************//
// �N���X��`
//*******************************************************//
class CManager
{// CManager
public:
	typedef enum
	{	// ���[�h���
		MODE_NONE = 0,	// �������Ă��Ȃ�
		MODE_GAME,		// �Q�[��
		MODE_MAX		// �ő吔
	}MODE;

	CManager() {};											// �R���X�g���N�^(���g����)
	virtual ~CManager() {};								// �f�X�g���N�^(���g����)

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);		// ����������
	virtual void Uninit(void);							// �I������
	virtual void Update(void);							// �X�V����
	virtual void Draw(void);							// �`�揈��

	static CRenderer		*GetRenderer(void)		{ return m_pRenderer; }			// �����_���[�擾
	static CInputKeyboard	*GetInputKeyboard(void)	{ return m_pInputKeyboard; }	// �L�[�{�[�h�擾
	static CInputJoypad		*GetInputJoypad(void)	{ return m_pInputJoypad; }		// �R���g���[���[�擾
	//static CInputDInputPad	*GetInputDIPad(void)	{ return m_pInputDIPad; }		// DI�p�b�h�擾
	//static CInputXPad		*GetInputXPad(void)		{ return m_pInputXPad; }		// X�p�b�h�擾
	static CInputMouse		*GetInputMouse(void)	{ return m_pInputMouse; }		// �}�E�X�擾
	static CCamera			*GetCamera(void)		{ return m_pCamara; }			// �J�����擾
	static CLight			*GetLight(void)			{ return m_pLight; }			// ���C�g�擾
	static CSound			*GetSound(void)			{ return m_pSound; }			// �T�E���h�擾

	static void SetMode(MODE mode);					// ���[�h�ݒ�
	static MODE GetMode(void) { return m_mode; }	// ���[�h�擾

	// �v���C�l��
	static void SetPlayNum(int nPlayerNum)	{ if (nPlayerNum <= MAX_PLAYER)m_nPlayNum = nPlayerNum; else m_nPlayNum = MAX_PLAYER; }
	static int GetPlayNum(void)				{ return m_nPlayNum; }

private:
	static MODE m_mode;							// ���[�h

	static CRenderer		*m_pRenderer;		// �����_���[
	static CInputKeyboard	*m_pInputKeyboard;	// �L�[�{�[�h
	static CInputJoypad		*m_pInputJoypad;	// �R���g���[���[
	//static CInputDInputPad	*m_pInputDIPad;		// DI�p�b�h
	//static CInputXPad		*m_pInputXPad;		// X�p�b�h
	static CInputMouse		*m_pInputMouse;		// �}�E�X
	static CCamera			*m_pCamara;			// �J����
	static CLight			*m_pLight;			// ���C�g
	static CSound			*m_pSound;			// �T�E���h
	static CGame			*m_pGame;			// �Q�[��

	static int m_nPlayNum;						// �v���C�l��
	static CPause *m_pPause;					// �|�[�Y
	static CDebugProc		*m_pDebug;			// �f�o�b�N
	bool m_bPause;								// �|�[�Y���
};
#endif