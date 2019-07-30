//=============================================================================
//
// �G�l�~�[�\������ [disenemy.h]
// Author : Shun Yokomichi
//
//=============================================================================
#ifndef _DISENEMY_H_
#define _DISENEMY_H_

#include "sceneX.h"		//	X���f��
#include "meshfield.h"	// ���b�V���t�B�[���h
#include "display.h"
//*****************************************************************************
//	�N���X��`(���f���p)
//*****************************************************************************
class CDsiEnemy : public CDisplay, CSceneX
{
public:
	CDsiEnemy();
	~CDsiEnemy() {};

	static HRESULT Load(int MaxModel);				// ���[�h
	static void Unload(void);						// �A�����[�h
	static CDsiEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int objType);	// ����

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int objType);			// ������
	void Uninit(void);														// �I��
	void Update(void);														// �X�V
	void Draw(void);														// �`��

	void SelectEnemy(void);									// ���f���I������
	void EnemyCreate(void);

	// �擾 & �ݒ�̊֐�
	D3DXVECTOR3 SetModelSize(int nSize);
	static int GetNum() { return m_nNum; }					// ���f���ԍ��̎擾


private:
	static LPDIRECT3DTEXTURE9	*m_pTexture[MAX_MODEL];		// �e�N�X�`�����ւ̃|�C���^
	static LPD3DXMESH			m_pMesh[MAX_MODEL];			// ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER			m_pBuffMat[MAX_MODEL];		// �}�e���A�����ւ̃|�C���^
	static DWORD				m_nNumMat[MAX_MODEL];		// �}�e���A�����̐�
	static int m_nNum;			// ���f���ԍ�
};

#endif

