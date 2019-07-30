//=======================================================//
//
// ���b�V���t�B�[���h���� [Meshfield.h]
// Author : Shun Yokomichi
//
//=======================================================//
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

//*******************************************************//
// �w�b�_�t�@�C���̃C���N���[�h
//*******************************************************//
#include "main.h"		// ���C��
#include "Scenemesh.h"	// �V�[�����b�V��

#define MAX_TEX_FIELD	(5)

//*******************************************************//
// �N���X��`
//*******************************************************//
class CMeshField : public CSceneMesh
{// CMeshField�i�e�FCSceneMesh�j
public:
	CMeshField() : CSceneMesh(CScene::PRIORITY_1, CScene::OBJTYPE_MESHFIELD) {};	// �R���X�g���N�^(���g����)
	~CMeshField() {};																// �f�X�g���N�^(���g����)

	// ��������
	static CMeshField *Create(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHTEX meshTex, int nTex);
	static HRESULT Load(int nMaxTex);	// �e�N�X�`���ǂݍ��ݏ���
	static void Unload(void);	// �e�N�X�`���J������

	// ����������
	HRESULT Init(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHTEX meshTex);
	void Uninit(void)	{ m_nNumAll--; CSceneMesh::Uninit(); };	// �I������
	void Update(void){};							// �X�V����(���g����)
	void Draw(void)		{ CSceneMesh::Draw(); };	// �`�揈��

	// �擾 or �ݒ�̊֐�
	static int GetNum() { return m_nNumAll; }			// ���f���̑������̎擾
	int GetModelNum() { return m_nNum; }				// ���f���̔z�u�ԍ��̎擾
	void SetNum(int nNum) { m_nNum = nNum; }			// ���f���̔ԍ��ݒ�
	int GetTex() { return m_nTex; }


private:
	static int m_nNumAll;			// �����z�u��
	static LPDIRECT3DTEXTURE9	m_pTexture[MAX_TEX_FIELD];	// �e�N�X�`�����ւ̃|�C���^
	int							m_nNum;		// �z�u��
	int							m_nTex;

};
#endif