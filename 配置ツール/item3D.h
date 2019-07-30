//*****************************************************************************
//
// �A�C�e������ [item.h]
// Author : Shun Yokomichi
//
//*****************************************************************************
#ifndef _ITEM3D_H_
#define _ITEM3D_H_

#include "scene.h"
#include "model.h"

class CModel;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_ITEM_TYPE		(1)			// ���f����ނ̍ő吔
#define MAX_ITEM_PARTS		(2)			// �p�[�c��

//*****************************************************************************
//	�N���X��`(3D���f��)
//*****************************************************************************
class CItem3D : public CScene
{
public:
	//**********************
	// �\���̒�`
	//**********************
	typedef struct
	{
		D3DXVECTOR3 VtxMinModel, VtxMaxModel;	//���f���̍ŏ��l�A�ő�l
	} VTX;

	typedef struct
	{
		float fPosX;
		float fPosY;
		float fPosZ;
		float fRotX;
		float fRotY;
		float fRotZ;
	}KEY;

	typedef struct
	{
		int nFrame;
		KEY aKey[MAX_ITEM_PARTS];
	}KEY_INFO;

	typedef struct
	{	// ���[�V�����ɕK�v�ȕϐ�
		int nParts;		// �����p�[�c��
		int nIndex;		// �C���f�b�N�X
		int nParent;		// �y�A�����g
	}MOTION_ITEM;

	typedef enum
	{
		TYPE_NONE = 0,	// �����Ȃ�
		TYPE_KEY,		// ���擾
		TYPE_HP,		// ��
		TYPE_EXPLOSION,	// ����
		TYPE_MAX		// ����
	}TYPE;

	CItem3D(int nPriority = 3, OBJTYPE objType = CScene::OBJTYPE_ITEM3D);
	~CItem3D();
	
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void TxtLoad();
	char *ReadLine(FILE *pFile, char*pDst, int nPatten);
	char *SearchLine(char*pDst);

	static CItem3D * Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type);
	static HRESULT Load(void);	//	�ǂݍ��ފ֐�(�e�N�X�`��)
	static void Unload(void);	// �J������֐�(�e�N�X�`��)

	void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void SetType(TYPE type) { m_type = type; 	m_nTypeNum = type;}
	void SetNum(int nNum) { m_nNum = nNum; }			// ���f���̔ԍ��ݒ�
	void SetCol(D3DXCOLOR col) { m_col = col; }			// �F�̐ݒ�
	D3DXVECTOR3 GetPos() { return m_pos; }
	D3DXVECTOR3 GetRot() {return m_rot;}
	static int GetNum() { return m_nNumAll; }			// ���f���̑������̎擾
	int GetType() { return m_nTypeNum; }				// ���f���̃^�C�v�̎擾
	int GetModelNum() { return m_nNum; }				// ���f���̔z�u�ԍ��̎擾




private:
	D3DXVECTOR3 m_pos;						//�ʒu
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_rot;						//����
	D3DXCOLOR	m_col;						// �F
	D3DXMATRIX	m_mtxWorld;					//���[���h�}�g���b�N�X
	D3DXVECTOR3 m_VtxMinModel, m_VtxMaxModel;	//���f���̍ŏ��l�A�ő�l
	D3DXMATERIAL *m_pMat;
	VTX m_aModelVtx;
	static LPD3DXMESH	m_pMesh[MAX_ITEM_PARTS];
	static LPD3DXBUFFER m_pBuffMat[MAX_ITEM_PARTS];
	static DWORD		m_nNumMat[MAX_ITEM_PARTS];
	static LPDIRECT3DTEXTURE9	*m_pTexture[MAX_ITEM_PARTS];	// �e�N�X�`�����ւ̃|�C���^

	CModel *m_pModel[MAX_ITEM_PARTS];	// MODEL�̃|�C���^

	FILE *pFile;
	char *pStrCur;
	char aLine[256];
	char aStr[256];
	int m_aIndex[MAX_ITEM_PARTS];
	int m_aParts[MAX_ITEM_PARTS];
	D3DXVECTOR3 m_pos1;						//�ʒu
	D3DXVECTOR3 m_rot1;						//�ʒu

	MOTION_ITEM m_motionItem;
	TYPE m_type;

	static int m_nNumAll;			// �����z�u��
	int m_nTypeNum;					// �^�C�v�ԍ�
	int m_nNum;						// �z�u��

};

#endif