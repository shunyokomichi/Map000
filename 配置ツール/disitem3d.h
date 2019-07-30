//*****************************************************************************
//
// �A�C�e���\������ [disitem.h]
// Author : Shun Yokomichi
//
//*****************************************************************************
#ifndef _DISITEM3D_H_
#define _DISITEM3D_H_

#include "scene.h"
#include "model.h"
#include "item3D.h"
#include "display.h"

class CModel;

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
//	�N���X��`(3D���f��)
//*****************************************************************************
class CDisItem3D : public CDisplay, CScene
{
public:
	//**********************
	// �\���̒�`
	//**********************
	typedef struct
	{
		D3DXVECTOR3 VtxMinModel, VtxMaxModel;	//���f���̍ŏ��l�A�ő�l
	} VTX;

	typedef enum
	{
		TYPE_NONE = 0,	// �����Ȃ�
		TYPE_KEY,		// ���擾
		TYPE_HP,		// ��
		TYPE_EXPLOSION,	// ����
		TYPE_MAX		// ����
	}TYPE;

	typedef struct
	{	// ���[�V�����ɕK�v�ȕϐ�
		int nParts;		// �����p�[�c��
		int nIndex;		// �C���f�b�N�X
		int nParent;		// �y�A�����g

	}MOTION_ITEM;


	CDisItem3D(int nPriority = 3, OBJTYPE objType = CScene::OBJTYPE_SAID);
	~CDisItem3D() ;
	
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void TxtLoad();
	char *ReadLine(FILE *pFile, char*pDst, int nPatten);
	char *SearchLine(char*pDst);

	static CDisItem3D * Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type);
	static HRESULT Load(void);	//	�ǂݍ��ފ֐�(�e�N�X�`��)
	static void Unload(void);	// �J������֐�(�e�N�X�`��)

	void SetCol(int nNum);
	void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void SetType(TYPE type) { m_type = type; }

	void SelectType(void);									// ���f���I������
	void ModelCreate(void);

	// �擾 & �ݒ�̊֐�
	static int GetNum() { return m_nNum; }					// ���f���ԍ��̎擾
	D3DXVECTOR3 SetModelSize(int nSize);


private:
	D3DXVECTOR3 m_pos;						//�ʒu
	D3DXVECTOR3 m_rot;						//����
	D3DXCOLOR	m_col;
	D3DXMATRIX	m_mtxWorld;					//���[���h�}�g���b�N�X
	D3DXVECTOR3 m_VtxMinModel, m_VtxMaxModel;	//���f���̍ŏ��l�A�ő�l
	D3DXMATERIAL *m_pMat;
	VTX m_aModelVtx;
	static LPD3DXMESH	m_pMesh[MAX_ITEM_PARTS];
	static LPD3DXBUFFER m_pBuffMat[MAX_ITEM_PARTS];
	static DWORD		m_nNumMat[MAX_ITEM_PARTS];
	static LPDIRECT3DTEXTURE9	*m_pTexture[MAX_ITEM_PARTS];	// �e�N�X�`�����ւ̃|�C���^

	CModel *m_pModel[MAX_ITEM_PARTS];	// MODEL�̃|�C���^
	static int m_nNum;			// ���f���ԍ�

	FILE *pFile;
	char *pStrCur;
	char aLine[256];
	char aStr[256];
	int m_aIndex[MAX_ITEM_PARTS];
	int m_aParts[MAX_ITEM_PARTS];
	MOTION_ITEM m_motionItem;
	TYPE m_type;
	D3DXVECTOR3 m_pos1;						//�ʒu
	D3DXVECTOR3 m_rot1;						//�ʒu


};

#endif