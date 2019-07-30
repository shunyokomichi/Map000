//=======================================================//
//
// �V�[�����b�V������ [SceneMesh.h]
// Author : Shun Yokomichi
//
//=======================================================//
#ifndef _SCENEMESH_H_
#define _SCENEMESH_H_

//*******************************************************//
// �w�b�_�t�@�C���̃C���N���[�h
//*******************************************************//
#include "main.h"	// ���C��
#include "scene.h"	// Scene

//*******************************************************//
// �N���X��`
//*******************************************************//
class CSceneMesh : public CScene
{// CSceneMesh�i�e�FCScene�j
public:
	CSceneMesh(int nPriority, CScene::OBJTYPE objType);	// �R���X�g���N�^
	~CSceneMesh() {};									// �f�X�g���N�^(���g����)

	typedef enum
	{	// ���b�V���^�C�v
		MESHTYPE_FIELD = 0,	// �t�B�[���h
		MESHTYPE_WALL,		// �E�H�[��
		MESHTYPE_CYLINDER,	// �V�����_�[
		MESHTYPE_MAX		// �ő吔
	}MESHTYPE;

	typedef enum
	{	// �e�N�X�`���^�C�v
		MESHTEX_ALL = 0,	// �S��
		MESHTEX_NUMSIZE,	// ����
		MESHTEX_MAX			// �ő吔
	}MESHTEX;

	typedef enum
	{	// �T�C���^�C�v(���󔻒�)
		SINTYPE_CIRCLE = 0,	// �~��
		SINTYPE_RIGHT,		// �E
		SINTYPE_LEFT,		// ��
		SINTYPE_UP,			// ��
		SINTYPE_DOWN,		// ��
		SINTYPE_MAX			// �ő吔
	}SINTYPE;

	typedef enum
	{	// �T�C���^�C�v(�͈͔���)
		SINSTART_POS = 0,	// �ʒu
		SINSTART_CENTER,	// �����
		SINSTART_LU,		// ����
		SINSTART_RU,		// �E��
		SINSTART_LD,		// �����
		SINSTART_RD,		// �E���
		SINSTART_MAX		// �ő吔
	}SINSTART;

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }	// �e�N�X�`�����f

	HRESULT Init(void) { return S_OK; };	// ����������
	void Uninit(void);						// �I������
	void Update(void) {};					// �X�V����(���g����)
	void Draw(void);						// �`�揈��

	// �����l�ݒ�
	void SetInitAll(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHTEX meshTex);

	void Setpos(D3DXVECTOR3 pos) { m_pos = pos; }	// �ʒu�ݒ�
	D3DXVECTOR3 Getpos(void) { return m_pos; }		// �ʒu�擾

	void Setrot(D3DXVECTOR3 rot) { m_rot = rot; }	// �����ݒ�
	D3DXVECTOR3 Getrot(void) { return m_rot; }		// �����擾

	D3DXVECTOR3 Getsize(void) { return m_size; }	// �T�C�Y�擾

	void SetTex(D3DXVECTOR2 TexMoveUV);				// �A�j���[�V�������ݒ�

	void SetCol(D3DXCOLOR col) { m_col = col; }		// �F�̐ݒ�
	void SetSize(D3DXVECTOR3 size, D3DXVECTOR3 nNum);					// �T�C�Y�̐ݒ�

	D3DXVECTOR3 GetVtx(int nNum);	// ���_�擾
	D3DXVECTOR3 GetNor(int nNum);	// �@���擾

	// ����g�p�L���̊m�F����
	bool ColInMesh(D3DXVECTOR3 pos);

	void SinUpdate(SINTYPE sinType, SINSTART sinStart, D3DXVECTOR3 pos, float frot, float fUp, float fLine);				// �T�C���X�V����(����)
	void SinUpdate(SINTYPE sinType, SINSTART sinStart, D3DXVECTOR3 pos, float frot, float fUp, float fLine, float fRange);	// �T�C���X�V����(�͈�)

	void SetHeight(D3DXVECTOR3 pos, float fValue, float fRange);	// ���x�ݒ�
	float GetHeight(D3DXVECTOR3 pos);								// ���x�擾
	float GetUpDown(D3DXVECTOR3 pos, D3DXVECTOR3 rot);				// ���x�擾(�X��)
	float GetBlockNor(D3DXVECTOR3 pos);								// �@���擾(�u���b�N)

private:
	void SetVecNor(void);	// �@���v�Z

	LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`�����ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@���ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;		// �C���f�b�N�X���ւ̃|�C���^
	D3DXMATRIX				m_mtxWorld;		// ���[���h�}�g���b�N�X
	int						m_nMaxVtx;		// ���_�ő吔
	int						m_nMaxIdx;		// �C���f�b�N�X�ő吔
	int						m_nMaxPolygon;	// �|���S���ő吔

	MESHTYPE				m_meshType;		// ���b�V���^�C�v
	MESHTEX					m_meshTex;		// �e�N�X�`���^�C�v
	D3DXVECTOR3				m_num;			// ����
	D3DXVECTOR3				m_pos;			// �ʒu
	D3DXVECTOR3				m_rot;			// ����
	D3DXVECTOR3				m_size;			// �T�C�Y
	D3DXVECTOR2				m_TexUV;		// UV
	D3DXCOLOR				m_col;			// �F
};
#endif