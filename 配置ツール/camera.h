//=============================================================================
//
// �J�������� [camera.h]
// Author : Shun Yokomichi
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*********************************************************************
//	�I�u�W�F�N�g�N���X(�J����)
//*********************************************************************
class CCamera
{
public:
	typedef enum
	{	// �J�������
		CAMTYPE_NONE = 0,	// �������Ă��Ȃ�
		CAMTYPE_V,			// ���_
		CAMTYPE_R,			// �����_
		CAMTYPE_VR,			// ���_�������_
		CAMTYPE_MAX			// �ő吔
	} CAMTYPE;

	CCamera();
	~CCamera();

	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void SetCamera(void);	// �`�揈��

	void BasicMove(void);
	void Move_R(void);	// �����_�̈ړ���
	void Move_VR(void);	// ���_�ƒ����_�̈ړ���
	void SetUpCamera(void);

	// �J�����̎��_ & �����_
	void SetPos(D3DXVECTOR3 posV, D3DXVECTOR3 posR) { m_posV = posV; m_posR = posR; }
	D3DXVECTOR3 GetRot() { return m_rot; }			// �����̎擾
	float GetAngle() { return m_fAngle; }			// �����̎擾(Y���̊p�x)
	void SetType(CAMTYPE type) { m_Type = type; }	// �J�����̃^�C�v�擾

private:
	D3DXVECTOR3		m_posV;				//���_(����)
	D3DXVECTOR3		m_posR;				//�����_(����)
	D3DXVECTOR3		m_posVDest;			//���_(�ړI)
	D3DXVECTOR3		m_posRDest;			//�����_(�ړI)
	D3DXVECTOR3		m_rot;				// ����(����)
	D3DXVECTOR3		m_rotDest;			// ����(�ړI)
	D3DXVECTOR3		m_vecU;				//������̃x�N�g��
	D3DXVECTOR3		m_move;				//�ړ�
	D3DXVECTOR3		m_DiffAngle;		// �����̋L�^
	D3DXMATRIX		m_mtxProjection;	//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX		m_mtxView;			//�r���[�}�g���b�N�X
	float			m_fAngle;
	float			m_nCounterAngle;
	D3DXVECTOR3		m_Length;			// posV��posR�̍���
	bool			m_bSwitching;		// �ؑ�
	static CAMTYPE	m_Type;				// �J�����̃^�C�v
	D3DXVECTOR3		m_MousePos;			// �}�E�X�̍��W(����)
	D3DXVECTOR3		m_MousePosOld;		// �}�E�X�̍��W(�ߋ�)
	D3DXVECTOR3		m_MousePosAngle;	// �}�E�X�̍���
	float			m_fMouseAngle;		// �}�E�X�̊p�x
	float			m_fMousLength;		// �}�E�X�̋���
};
#endif