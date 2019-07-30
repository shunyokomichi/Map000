//=============================================================================
//
// ���C������ [input.h]
// Author : �r�J�R�N
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define	NUM_KEY_MAX			(256)				// �L�[�̍ő吔
#define	NUM_MOUSE_MAX		(8)					// �}�E�X�̍ő吔
#define MAX_JOYPAD			(3)					// �ڑ��ł���p�b�h�̐�
#define REPEAT_FRAME		(20)				// ���s�[�g�����N�����n�߂�t���[�����̍��v
#define REPEAT_TRIGGER		(6)					// ���s�[�g�����N������t���[���̊Ԋu
#define	NUM_BUTTON_MAX		(32)				// �L�[�̍ő吔

//=====================
//	   ��{�N���X
//=====================
class CInput
{
public:// �N�ł�������
	CInput();
	virtual ~CInput();
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void	Uninit(void);
	virtual void	Update(void) = 0;
protected://�l�ł̂ݎg��
	static LPDIRECTINPUT8	m_pInput;		// DirerctInput�I�u�W�F�N�g�ւ̃|�C���^ //�S��������������Ă���΂�������static
	LPDIRECTINPUTDEVICE8	m_pDevice;		// ���̓f�o�C�X�i�L�[�{�[�h�j�ւ̃|�C���^
};

//=====================
//	   �L�[�{�[�h�̔h���N���X
//=====================
class CInputKeyboard : public CInput
{
public:// �N�ł�������
	CInputKeyboard();
	~CInputKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void	Uninit(void);
	void	Update(void);
	bool	GetKeyboardPress(int nKey);		// �L�[�{�[�h�v���X
	bool	GetKeyboardTrigger(int nKey);		// �L�[�{�[�h�g���K�[
	bool	GetKeyboardAny(int nNumber);	// �L�[�{�[�h�g���K�[
	static CInputKeyboard *Create(HINSTANCE hInstance, HWND hWnd);
private:// �l�ł̂ݎg��
	BYTE m_aKeyState[NUM_KEY_MAX];		// �L�[�{�[�h�̓��͏��i�v���X���j
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];		// �L�[�{�[�h�̓��͏�� (�g���K�[���)
};

//=====================
//	   �}�E�X�̔h���N���X
//=====================
class CInputMouse : public CInput
{
public:// �N�ł�������
	CInputMouse();
	~CInputMouse();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void	Uninit(void);
	void	Update(void);
	bool	GetMousePress(int Key);			// �}�E�X�v���X
	bool	GetMouseTrigger(int Key);		// �}�E�X�g���K�[
	bool	GetMouseRelease(int Key);		// �}�E�X�����[�X
	static CInputMouse *Create(HINSTANCE hInstance, HWND hWnd);
	static D3DXVECTOR3 *GetMousePos(void) { return &m_MousePos; }
	void	MouseLoop(void);

private://�l�ł̂ݎg��
	RECT					m_moveRect;				// ��ʏ�œ�����͈�
	int						m_x;					// X���W
	int						m_y;					// Y���W
	bool					m_lButton;				// ���{�^��
	bool					m_rButton;				// �E�{�^��
	bool					m_cButton;				// �^�񒆃{�^��
	int						m_moveAdd;				// �ړ���
	RECT					m_imgRect;				// �}�E�X�p�摜��`
	int						m_imgWidth;				// �}�E�X�摜��
	int						m_imgHeight;			// �}�E�X�摜����
	POINT					m_Pos, m_WPos;			// �}�E�X�̍��W
	static D3DXVECTOR3		m_MousePos;				// �}�E�X�̍��W
	DIMOUSESTATE2			m_MouseState;			// �}�E�X�̏��
	DIMOUSESTATE2			m_MouseStatePress;		// �}�E�X�̓��͏��i�v���X���j
	DIMOUSESTATE2			m_MouseStateTrigger;	// �}�E�X�̓��͏��i�g���K�[���j
	DIMOUSESTATE2			m_MouseStateRelease;	// �}�E�X�̓��͏��i�����[�X���)
};

//=====================
//	   �R���g���[���[�̔h���N���X
//=====================
class CInputJoypad : public CInput
{
public:
	typedef enum
	{
		BUTTON_A = 0,
		BUTTON_B,
		BUTTON_X,
		BUTTON_Y,
		BUTTON_LB,
		BUTTON_RB,
		BUTTON_BACK,
		BUTTON_START,
		BUTTON_LS,
		BUTTON_RS,
		BUTTON_10,
		BUTTON_11,
		BUTTON_12,
		BUTTON_13,
		BUTTON_14,
		BUTTON_15,
		POV_UP_RIGHT,	// �\���L�[�@�E��
		POV_UP_LEFT,	// �\���L�[�@����
		POV_DOWN_RIGHT,	// �\���L�[�@�E��
		POV_DOWN_LEFT,	// �\���L�[�@����
		POV_UP,			// �\���L�[�@��
		POV_DOWN,		// �\���L�[�@��
		POV_RIGHT,		// �\���L�[�@�E
		POV_LEFT,		// �\���L�[�@��
		STICK_L_UP,		// ���X�e�B�b�N�@��
		STICK_L_DOWN,	// ���X�e�B�b�N�@��
		STICK_L_RIGHT,	// ���X�e�B�b�N�@�E
		STICK_L_LEFT,	// ���X�e�B�b�N�@��
		STICK_R_UP,		// �E�X�e�B�b�N�@��
		STICK_R_DOWN,	// �E�X�e�B�b�N�@��
		STICK_R_RIGHT,	// �E�X�e�B�b�N�@�E
		STICK_R_LEFT,	// �E�X�e�B�b�N�@��
		MAX_BUTTON,
	}DIJS_BUTTON;
	CInputJoypad();
	~CInputJoypad();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void	Uninit(void);
	void	Update(void);
	bool	GetPress(int nKey);
	bool	GetTrigger(int nKey);
	bool	GetRelease(int nKey);
	bool	GetJoyPadAny(int nKey);

	float GetLeftAxiz(void);
	float GetRightAxiz(void);
	float GetRadian(void);

	static CInputJoypad *Create(HINSTANCE hInstance, HWND hWnd);

protected:
	BYTE	m_aKeyState[NUM_BUTTON_MAX];			// �R���g���[���[�̓��͏��(�v���X)
	BYTE	m_aKeyStateTrigger[NUM_BUTTON_MAX];		// �R���g���[���[�̓��͏��(�g���K�[)
	BYTE	m_aKeyStateRelease[NUM_BUTTON_MAX];		// �R���g���[���[�̓��͏��(�����[�X)
	float	m_LeftAxizX;								// ���X�e�B�b�N��X��
	float	m_LeftAxizY;								// ���X�e�B�b�N��Y��
	float	m_RightAxizX;								// �E�X�e�B�b�N��X��
	float	m_RightAxizY;								// �E�X�e�B�b�N��Y��
	float	m_Radian;									// �p�x
};
#endif