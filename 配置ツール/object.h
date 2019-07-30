//=============================================================================
//
// �I�u�W�F�N�g���� [object.h]
// Author : Shun Yokomichi
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "sceneX.h" 

#define MAX_MODEL		(15)		// ���f���̍ő吔
//*****************************************************************************
//	�N���X��`(�I�u�W�F�N�g)
//*****************************************************************************
class CObject : public CSceneX
{
public:
	CObject();
	~CObject();

	static HRESULT Load(int MaxModel);				// ���[�h
	static void Unload(void);						// �A�����[�h
	static CObject *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int objType);		// ����

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int objType);			// ������
	void Uninit(void);														// �I��
	void Update(void);														// �X�V
	void Draw(void);														// �`��

	// �擾��ݒ�̊֐�
	static int GetNum() { return m_nNumAll; }			// ���f���̑������̎擾
	int GetType() { return m_nTypeNum; }				// ���f���̃^�C�v�̎擾
	int GetModelNum() { return m_nNum; }				// ���f���̔z�u�ԍ��̎擾
	void SetNum(int nNum) { m_nNum = nNum; }			// ���f���̔ԍ��ݒ�

private:
	static LPDIRECT3DTEXTURE9	*m_pTexture[MAX_MODEL];	// �e�N�X�`�����ւ̃|�C���^
	static LPD3DXMESH			m_pMesh[MAX_MODEL];		// ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER			m_pBuffMat[MAX_MODEL];	// �}�e���A�����ւ̃|�C���^
	static DWORD				m_nNumMat[MAX_MODEL];	// �}�e���A�����̐�

	static int m_nNumAll;			// �����z�u��
	int m_nTypeNum;					// �^�C�v�ԍ�
	int m_nNum;						// �z�u��
};

#endif // !_OBJECT_H_

