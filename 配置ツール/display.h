//=============================================================================
//
// �I�u�W�F�N�g�\������ [display.h]
// Author : Shun Yokomichi
//
//=============================================================================
#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "sceneX.h"		//	X���f��
#include "object.h"		// �I�u�W�F�N�g
#include "meshfield.h"	// ���b�V���t�B�[���h
//*****************************************************************************
//	�N���X��`(�\���ɕK�v�ȕ����܂Ƃ߂�)
//*****************************************************************************
class CDisplay
{
public:
	typedef enum
	{
		STATE_NONE = 0,	// �����Ȃ����
		STATE_PRESS,	// ���������
		STATE_TRIGGER,	// 1�񉟂��ꂽ���
	}STATE;

	typedef enum
	{
		TYPE_SIZE_MIN = 0,	// �ŏ��T�C�Y
		TYPE_SIZE_BIG,		// �ő�T�C�Y
		TYPE_SIZE_MAX
	}TYPE_SIZE;

	CDisplay();
	~CDisplay();
	
	void BasicOperation(void);								// ��{����(�ړ�)
	void BasicSetRot(void);									// ��{����(�����ݒ�)
	void ResetButton(void);									// ���Z�b�g�{�^��
	static D3DXVECTOR3 GetPos() { return m_pos; }			// �ʒu�̎擾
	static D3DXVECTOR3 GetRot() { return m_rot; }			// �����̎擾
	static void ChangeUninit(void) { m_bUse = true; }		// �ؑ֎��̏I��
	TYPE_SIZE GetSizeType(void) { return m_SizeType; }
	void SetChange(D3DXVECTOR2 nNum) { m_ChangeNum.x = nNum.x; m_ChangeNum.y = nNum.y; }
	int Redo(int nCntRedo);

protected:
	void StateCounter(bool bPress);			// ���
	int SelectNum(int nSelectNum, int nMaxOperation, int nButtan000, int nButtan001);	// �����I������
	void SetSize(D3DXVECTOR3 min, D3DXVECTOR3 max) { m_sizeMin = min + min; m_sizeMax = max + max;}
	void MoveChange(D3DXVECTOR3 fCameraRot);								// �ړ��ݒ�
	void SquareMove(D3DXVECTOR3 fCameraRot);								// ���ڈړ�
	void NomalMove(D3DXVECTOR3 fCameraRot);									// �ʏ�ړ�

	static	D3DXVECTOR3		m_pos;			// �ʒu���
	D3DXVECTOR3				m_move;			// �ړ���
	static	D3DXVECTOR3		m_rot;			// �������(����)
	D3DXVECTOR3				m_rotDest;		// �������(�ړI)
	D3DXVECTOR3				m_size;			// �T�C�Y���
	D3DXVECTOR3				m_sizeMax;		// �T�C�Y��
	D3DXVECTOR3				m_sizeMin;		// �T�C�Y��
	STATE					m_State;		// ��Ԃ̎��
	static	bool			m_bUse;			// �ؑւ̏ꍇ
	bool 					m_bPress;		// �����ꂽ���ǂ���
	bool					m_bFlag;		// �t���O
	int						m_nNumRot;		// �����̔ԍ�
	int						m_nCntPress;	// �������̃J�E���g
	float					m_fMove;		// �ړ������l
	TYPE_SIZE				m_SizeType;
	D3DXVECTOR2				m_ChangeNum;

};

#endif

