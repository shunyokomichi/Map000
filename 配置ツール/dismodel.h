//=============================================================================
//
// �I�u�W�F�N�g���� [dismodel.h]
// Author : Shun Yokomichi
//
//=============================================================================
#ifndef _DISMODEL_H_
#define _DISMODEL_H_

#include "sceneX.h"		//	X���f��
#include "object.h"		// �I�u�W�F�N�g
#include "meshfield.h"	// ���b�V���t�B�[���h
#include "display.h"

//*****************************************************************************
//	�}�N����`
//*****************************************************************************
#define MAX_REDO		(10)		// �ۑ��ł����

//*****************************************************************************
//	�N���X��`(���f���p)
//*****************************************************************************
class CDisModel : public CDisplay, CSceneX
{
public:
	typedef struct
	{
		D3DXVECTOR3 pos;	// �ʒu
		D3DXVECTOR3 rot;	// ����
		int			nNum;	// �ԍ�
	}REDO;

	CDisModel();
	~CDisModel() {};

	static HRESULT Load(int MaxModel);				// ���[�h
	static void Unload(void);						// �A�����[�h
	static CDisModel *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int objType);	// ����

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int objType);			// ������
	void Uninit(void);														// �I��
	void Update(void);														// �X�V
	void Draw(void);														// �`��

	void SelectModel(void);									// ���f���I������
	void ModelCreate(void);

	// �擾 & �ݒ�̊֐�
	D3DXVECTOR3 SetModelSize(int nSize);
	static int GetNum() { return m_nNum; }					// ���f���ԍ��̎擾

private:
	void Select();

	static LPDIRECT3DTEXTURE9	*m_pTexture[MAX_MODEL];		// �e�N�X�`�����ւ̃|�C���^
	static LPD3DXMESH			m_pMesh[MAX_MODEL];			// ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER			m_pBuffMat[MAX_MODEL];		// �}�e���A�����ւ̃|�C���^
	static DWORD				m_nNumMat[MAX_MODEL];		// �}�e���A�����̐�
	static int m_nNum;			// ���f���ԍ�
	int m_nSelectNum;	
	REDO m_nRedo[MAX_REDO];
	int m_nCntRedo;
};

#endif

