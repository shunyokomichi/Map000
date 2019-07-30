//=======================================================//
//
// SceneX���� [SceneX.h]
// Author : Shun Yokomichi
//
//=======================================================//
#ifndef _SCENEX_H_
#define _SCENEX_H_

//*******************************************************//
// �w�b�_�t�@�C���̃C���N���[�h
//*******************************************************//
#include "main.h"	// ���C��
#include "scene.h"	// Scene

//*******************************************************//
// �N���X��`
//*******************************************************//
class CSceneX : public CScene
{// CSceneX�i�e�FCScene�j
public:
	typedef enum 
	{
		TYPE_NONE = 0,		// �����Ȃ����
		TYPE_NORMAL,		// �ʏ���
		TYPE_SAID,			// �\�����
		TYPE_SELECT,		// �I�����
		TYPE_MAX			// ����
	}XTYPE;

	CSceneX(int nPriority, CScene::OBJTYPE objType);	// �R���X�g���N�^
	~CSceneX() {};										// �f�X�g���N�^(���g����)

	HRESULT Init(void);							// ����������
	void Uninit(void) { CScene::SetDeath(); };	// �I������
	void Update(void) {};						// �X�V����(���g����)
	void Draw(void);							// �`�揈��

	void SetInitAll(LPDIRECT3DTEXTURE9 *pTexture, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
	{	// �����l�ݒ�
		m_pTexture	= pTexture;	// �e�N�X�`��
		m_pMesh		= pMesh;	// ���b�V��
		m_pBuffMat	= pBuffMat;	// �}�e���A���o�b�t�@
		m_nNumMat	= nNumMat;	// �}�e���A����
		m_pos		= pos;		// �ʒu
		m_rot		= rot;		// ����
	}

	void SetType(XTYPE xType) { m_Type = xType; }

	void Setpos(D3DXVECTOR3 pos)	{ m_pos = pos; }	// �ʒu�ݒ�
	D3DXVECTOR3 Getpos(void)		{ return m_pos; }	// �ʒu�擾

	void Setrot(D3DXVECTOR3 rot) { m_rot = rot; }	// �����ݒ�
	D3DXVECTOR3 Getrot(void)		{ return m_rot; }	// �����擾

	D3DXMATRIX GetmtxWorld(void) { return m_mtxWorld; }	// ���[���h�}�g���b�N�X�擾

	D3DXVECTOR3 Getsize(int nNum) { if (nNum == 0) return m_vtxMax; else return m_vtxMin;  }	// �T�C�Y�擾
	void Setcol(D3DXCOLOR col)		{ m_col = col; m_bcolChange = true; }						// �J���[�ݒ�
	D3DXCOLOR GetCol(void) { return m_col; }

	void SetSize(LPD3DXMESH pMesh);

private:
	LPDIRECT3DTEXTURE9	*m_pTexture = NULL;	// �e�N�X�`�����ւ̃|�C���^
	LPD3DXMESH			m_pMesh		= NULL;	// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER		m_pBuffMat	= NULL;	// �}�e���A�����ւ̃|�C���^
	DWORD				m_nNumMat	= NULL;	// �}�e���A�����̐�
	D3DXMATRIX			m_mtxWorld;			// ���[���h�}�g���b�N�X

	D3DXVECTOR3			m_vtxMin, m_vtxMax;	// ���_�ŏ��l,���_�ő�l
	D3DXVECTOR3			m_pos,m_rot;		// �ʒu,����
	D3DXCOLOR			m_col;				// �J���[
	XTYPE				m_Type;				// ���f���̏��
	bool				m_bcolChange;		// �J���[�ύX
};
#endif