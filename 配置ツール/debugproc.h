//**************************************************************************
//
//				�f�o�b�N�\������[debugproc.h]
//				Auther : Shun Yokomichi
//
//**************************************************************************
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_

#include "main.h"

//**************************************************************************
//				�}�N����`
//**************************************************************************
#define			MAX_WORD		(16250)

//**************************************************************************
//				�N���X��`�i�f�o�b�N�\���j
//**************************************************************************
class CDebugProc
{
public:
	CDebugProc();
	~CDebugProc();

	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������

	// �ÓI�����o�֐�
	static void Print(char *fmt, ...);		// �����\��
	static void Draw(void);					// �`�揈��

private:
	static LPD3DXFONT m_pFont;
	static char m_aStr[MAX_WORD];
	static bool m_bUse;
};

#endif // !_DEBUGPROC_H_
