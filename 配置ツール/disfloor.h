//=============================================================================
//
// ���\������ [disfloor.h]
// Author : Shun Yokomichi
//
//=============================================================================
#ifndef _DISFLOOR_H_
#define _DISFLOOR_H_

#include "sceneX.h"		//	X���f��
#include "object.h"		// �I�u�W�F�N�g
#include "meshfield.h"	// ���b�V���t�B�[���h
#include "display.h"

class CDisplay;
//*****************************************************************************
//	�N���X��`(���b�V���t�B�[���h�p)
//*****************************************************************************
class CDisFloor : public CDisplay, CSceneMesh
{
public:
	CDisFloor();
	~CDisFloor() {};

	static HRESULT Load(int MaxModel);				// ���[�h
	static void Unload(void);						// �A�����[�h
	static CDisFloor *Create(D3DXVECTOR3 Num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHTEX meshTex, int nTex);	// ����

	HRESULT Init(D3DXVECTOR3 Num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHTEX meshTex, int nTex);			// ������
	void Uninit(void);														// �I��
	void Update(void);														// �X�V
	void Draw(void);														// �`��
	void SelectOperation();													// ���������߂�
	void SelectNum(int nNum);												// �I��ԍ�
	void MeshSize();														// ���b�V���t�B�[���h�̖ʐ�
	void FloorCreate();

	// �擾 & �ݒ�̊֐�
	void SetSizeChange();						// �T�C�Y�̐ݒ�
	void SelectTex(void);									// Texture�I������
	static int GetNum() { return m_nNum; }					// Texture�ԍ��̎擾


private:
	static LPDIRECT3DTEXTURE9	m_pTexture[MAX_MODEL];		// �e�N�X�`�����ւ̃|�C���^
	static int m_nNum;										// Texture�ԍ�
	D3DXVECTOR3 num;										// ���̖�������(X��Z)
	int							m_SelectNum;				// �I�����Ă���ԍ�(�ԍ����������߂�)
	D3DXVECTOR3					m_MeshSize;					// ���b�V���t�B�[���h�̖ʐ�
	
};

#endif

