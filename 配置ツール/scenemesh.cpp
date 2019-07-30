//=======================================================//
//
//	�V�[�����b�V������ [SceneMesh.cpp]
// Author : Shun Yokomichi
//
//=======================================================//

//*******************************************************//
// �w�b�_�t�@�C���̃C���N���[�h
//*******************************************************//
#include "scenemesh.h"	// �V�[�����b�V��
#include "manager.h"	// �}�l�[�W���[
#include "renderer.h"	// �����_���[
#include "input.h"		// ����

//=======================================================//
// �R���X�g���N�^(CSceneMesh)
//=======================================================//
CSceneMesh::CSceneMesh(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	// �����l�ݒ�
	m_pTexture	= NULL;	// �e�N�X�`�����ւ̃|�C���^
	m_pVtxBuff	= NULL;	// �}�e���A�����ւ̃|�C���^
	m_pIdxBuff	= NULL;	// �C���f�b�N�X���ւ̃|�C���^

	m_meshType	= MESHTYPE_FIELD;					// ���b�V���^�C�v
	m_num		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
	//m_pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	//m_rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
	m_TexUV		= D3DXVECTOR2(0.0f, 0.0f);			// UV
	m_col		= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);// �F
}

//=======================================================//
// �I������(CSceneMesh)
//=======================================================//
void CSceneMesh::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL) { m_pVtxBuff->Release(); m_pVtxBuff = NULL; }

	// ���_�o�b�t�@�̔j��
	if (m_pIdxBuff != NULL) { m_pIdxBuff->Release(); m_pIdxBuff = NULL; }

	// �J������(���S�t���O)
	CScene::SetDeath();
}

//=======================================================//
// �`�揈��(CSceneMesh)
//=======================================================//
void CSceneMesh::Draw(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X������
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]���f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu���f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �t�B�[���h�`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nMaxIdx, 0, m_nMaxPolygon);
}

//=======================================================//
// �����l�ݒ菈��(CSceneMesh)
//=======================================================//
void CSceneMesh::SetInitAll(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHTEX meshTex)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_meshType	= meshType;	// ���b�V���^�C�v
	m_meshTex	= meshTex;	// �e�N�X�`���^�C�v
	m_num		= num;		// ����
	m_pos		= pos;		// �ʒu
	m_rot		= rot;		// ����
	m_size		= size;		// �T�C�Y
	m_TexUV		= TexUV;	// UV
	m_col		= col;			// �F

	D3DXVECTOR2 tex;		// UV�v�Z�p
	int nCnt;				// YZ����

	switch (meshType)
	{// ���b�V���^�C�v
	case MESHTYPE_FIELD:	// �t�B�[���h
		m_nMaxVtx = ((int)num.x + 1) * ((int)num.z + 1);							// ���_��
		m_nMaxIdx = (((int)num.x + 1) * 2) * (int)num.z + (((int)num.z - 1) * 2);	// �C���f�b�N�X��
		m_nMaxPolygon = ((int)num.x * (int)num.z * 2) + (((int)num.z - 1) * 4);		// �|���S����

		nCnt = (int)num.z;	// Z����
		break;

	case MESHTYPE_WALL:		// ��
		m_nMaxVtx = ((int)num.x + 1) * ((int)num.y + 1);							// ���_��
		m_nMaxIdx = (((int)num.x + 1) * 2) * (int)num.y + (((int)num.y - 1) * 2);	// �C���f�b�N�X��
		m_nMaxPolygon = ((int)num.x * (int)num.y * 2) + (((int)num.y - 1) * 4);		// �|���S����

		nCnt = (int)num.y;	// Y����
		break;

	case MESHTYPE_CYLINDER:	// �V�����_�[
		m_nMaxVtx = ((int)num.x + 1) * ((int)num.y + 1);							// ���_��
		m_nMaxIdx = (((int)num.x + 1) * 2) * (int)num.y + (((int)num.y - 1) * 2);	// �C���f�b�N�X��
		m_nMaxPolygon = ((int)num.x * (int)num.y * 2) + (((int)num.y - 1) * 4);		// �|���S����

		if (size.z == 0.0f) size.z = 1.0f;	// �\����
		nCnt = (int)num.y;					// Y����
		break;
	}

	// ���_�o�b�t�@����
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nMaxVtx, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// �C���f�b�N�X�o�b�t�@����
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nMaxIdx, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIdxBuff, NULL);

	// ���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���ݒ�
	for (int nCnt_yz = 0; nCnt_yz < nCnt + 1; nCnt_yz++)
	{	// YZ�J�E���g
		for (int nCnt_x = 0; nCnt_x < num.x + 1; nCnt_x++)
		{	// X�J�E���g
			switch (meshType)
			{// ���b�V���^�C�v
			case MESHTYPE_FIELD:	// �t�B�[���h
				// ���_���ݒ�
				pVtx[0].pos = D3DXVECTOR3(-size.x + ((size.x * 2) / num.x) * nCnt_x, 0.0f, size.z - ((size.z * 2) / num.z) * nCnt_yz);
				//pVtx[0].pos.y = -12.0f + (float)(rand() % 12);

				switch (m_meshTex)
				{	// �e�N�X�`���^�C�v
				case MESHTEX_ALL:		// �S��
					tex = D3DXVECTOR2((TexUV.x / (num.x)) * (nCnt_x % ((int)num.x + 1)), (TexUV.y / (num.z)) * (nCnt_yz));
					break;

				case MESHTEX_NUMSIZE:	// ����
					tex = D3DXVECTOR2(TexUV.x * (nCnt_x % ((int)num.x + 1)), TexUV.y * (nCnt_yz));
					break;
				}
				// �e�N�X�`���ݒ�
				pVtx[0].tex = tex;

				// �@���ݒ�
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				break;
			case MESHTYPE_WALL:			// ��
				// ���_���ݒ�
				pVtx[0].pos = D3DXVECTOR3(-size.x + ((size.x * 2) / num.x) * nCnt_x, ((size.y * num.y) - (size.y * nCnt_yz)), 0.0f);

				switch (m_meshTex)
				{// �e�N�X�`���^�C�v
				case MESHTEX_ALL:		// �S��
					tex = D3DXVECTOR2((TexUV.x / (num.x)) * (nCnt_x % ((int)num.x + 1)), (TexUV.y / (num.z)) * (nCnt_yz));
					break;

				case MESHTEX_NUMSIZE:	// ����
					tex = D3DXVECTOR2(TexUV.x * (nCnt_x % ((int)num.x + 1)), TexUV.y * (nCnt_yz));
					break;
				}

				// �e�N�X�`���ݒ�
				pVtx[0].tex = tex;

				// �@���ݒ�
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				break;
			case MESHTYPE_CYLINDER:		// �V�����_�[
				// ���_���̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(sinf(D3DX_PI - ((D3DX_PI * (1.0f / (num.x / 2))) * nCnt_x) * size.z) * size.x,	// X
										((size.y) * num.y) - (size.y * (nCnt_yz)),											// YZ
										cosf(D3DX_PI - ((D3DX_PI * (1.0f / (num.x / 2))) * nCnt_x) * size.z) * size.x);		// X

				switch (m_meshTex)
				{// �e�N�X�`���^�C�v
				case MESHTEX_ALL:		// �S��
					tex = D3DXVECTOR2((TexUV.x / (num.x)) * (nCnt_x % ((int)num.x + 1)), (TexUV.y / (num.z)) * (nCnt_yz));
					break;

				case MESHTEX_NUMSIZE:	// ����
					tex = D3DXVECTOR2(TexUV.x * (nCnt_x % ((int)num.x + 1)), TexUV.y * (nCnt_yz));
					break;
				}
				// �e�N�X�`���ݒ�
				pVtx[0].tex = tex;

				// �@���ݒ�
				D3DXVECTOR3 vecNor;
				vecNor.x = pos.x - (pVtx[0].pos.x / pVtx[0].pos.x);	// X
				vecNor.z = pos.z - (pVtx[0].pos.z / pVtx[0].pos.z);	// Z
				D3DXVec3Normalize(&vecNor, &vecNor);				// ���K��
				pVtx[0].nor = D3DXVECTOR3(vecNor.x, 1.0f, vecNor.z);
				break;
			}

			// �J���[�ݒ�
			pVtx[0].col = col;

			// ���_�o�b�t�@�i�s
			pVtx += 1;
		}
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	// �@���v�Z
	SetVecNor();

	WORD *pIdx;	// �C���f�b�N�X�f�[�^�ւ̃|�C���^

	// �C���f�b�N�X�o�b�t�@�����b�N
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// �C���f�b�N�X�ݒ�
	for (int nCnt_yz = 0; nCnt_yz < nCnt; nCnt_yz++)
	{// YZ�J�E���g
		for (int nCnt_x = 0; nCnt_x < num.x + 1; nCnt_x++)
		{// X�J�E���g
		 // �C���f�b�N�X���ݒ�
			pIdx[0] = (WORD)(((nCnt_yz + 1) * (num.x + 1)) + nCnt_x);
			pIdx[1] = (WORD)(((nCnt_yz) * (num.x + 1)) + nCnt_x);

			pIdx += 2;	// �C���f�b�N�X�o�b�t�@�i�s

			if (nCnt > 1 && nCnt_yz != nCnt - 1 && nCnt_x == num.x)
			{// ���[���B��
				// �C���f�b�N�X���ݒ�i�k�ރ|���S���j
				pIdx[0] = (WORD)(((nCnt_yz) * (num.x + 1)) + nCnt_x);
				pIdx[1] = (WORD)(((nCnt_yz + 2) * (num.x + 1)));

				pIdx += 2;	// �C���f�b�N�X�o�b�t�@�i�s
			}
		}
	}
	// �C���f�b�N�X�o�b�t�@���A�����b�N
	m_pIdxBuff->Unlock();
}

//=======================================================//
// UV�ړ��l�ݒ菈��(CSceneMesh)
//=======================================================//
void CSceneMesh::SetTex(D3DXVECTOR2 TexMoveUV)
{
	int nCnt;	// YZ����

	switch (m_meshType)
	{// ���b�V���^�C�v
	case MESHTYPE_FIELD:		// �t�B�[���h
		nCnt = (int)m_num.z;	// Z����
		break;

	case MESHTYPE_WALL:			// ��
		nCnt = (int)m_num.y;	// Y����
		break;

	case MESHTYPE_CYLINDER:		// �V�����_�[
		nCnt = (int)m_num.y;	// Y����
		break;
	}

	// ���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt_yz = 0; nCnt_yz < nCnt + 1; nCnt_yz++)
	{// YZ�J�E���g
		for (int nCnt_x = 0; nCnt_x < m_num.x + 1; nCnt_x++)
		{// X�J�E���g
			// �e�N�X�`���ݒ�
			switch (m_meshTex)
			{// �e�N�X�`���^�C�v
			case MESHTEX_ALL:		// �S��
				pVtx[0].tex = D3DXVECTOR2((m_TexUV.x / (m_num.x)) * (nCnt_x % ((int)m_num.x + 1)) + TexMoveUV.x, (m_TexUV.y / (m_num.y)) * (nCnt_yz)+TexMoveUV.y);
				break;

			case MESHTEX_NUMSIZE:	// ����
				pVtx[0].tex = D3DXVECTOR2(m_TexUV.x * (nCnt_x % ((int)m_num.x + 1)) + TexMoveUV.x, m_TexUV.y * (nCnt_yz)+TexMoveUV.y);
				break;
			}
			pVtx += 1;	// ���_�o�b�t�@�i�s
		}
	}
	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=======================================================//
// ���_�擾����(CSceneMesh)
//=======================================================//
D3DXVECTOR3 CSceneMesh::GetVtx(int nNum)
{
	D3DXVECTOR3 pos;	// �ʒu

	// ���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���ݒ�
	pos = pVtx[nNum].pos;

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return pos;
}

//=======================================================//
// �@���擾����(CSceneMesh)
//=======================================================//
D3DXVECTOR3 CSceneMesh::GetNor(int nNum)
{
	D3DXVECTOR3 nor;	// �@��

	// ���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �@���ݒ�
	nor = pVtx[nNum].nor;

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return nor;
}

//=======================================================//
// ����g�p�L���̊m�F����(CSceneMesh)
//=======================================================//
bool CSceneMesh::ColInMesh(D3DXVECTOR3 pos)
{
	// ���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	bool		bLand = false;	// �g�p�̗L��
	D3DXVECTOR3 vecA, vecC;		// �v�Z�p�x�N�g���i���_���m�A���_��POS�j
	D3DXVECTOR3 posmtx = pos - m_pos;

	/*
	// �x�N�g���v�Z
	vecA = pVtx[(int)(m_num.x)].pos - pVtx[(int)(0)].pos;
	vecC = posmtx - pVtx[(int)(0)].pos;

	if (0 < ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
	{// �͈͓��i�E���j�ɂ��邱�Ƃ��m�F
	 // �x�N�g���v�Z
		vecA = pVtx[(int)(m_nMaxVtx - 1)].pos - pVtx[(int)(m_num.x)].pos;
		vecC = posmtx - pVtx[(int)(m_num.x)].pos;

		if (0 < ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
		{// �͈͓��i�E���j�ɂ��邱�Ƃ��m�F
		 // �x�N�g���v�Z
			vecA = pVtx[(int)((m_nMaxVtx - 1) - (m_num.x))].pos - pVtx[(int)(m_nMaxVtx - 1)].pos;
			vecC = posmtx - pVtx[(int)(m_nMaxVtx - 1)].pos;

			if (0 < ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
			{// �͈͓��i�E���j�ɂ��邱�Ƃ��m�F
			 // �x�N�g���v�Z
				vecA = pVtx[(int)(0)].pos - pVtx[(int)((m_nMaxVtx - 1) - (m_num.x))].pos;
				vecC = posmtx - pVtx[(int)((m_nMaxVtx - 1) - (m_num.x))].pos;

				if (0 < ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
				{// �͈͓��i�E���j�ɂ��邱�Ƃ��m�F
					bLand = true;
				}
			}
		}
	}*/

	if (m_pos.x - m_size.x <= pos.x && pos.x <= m_pos.x + m_size.x &&
		m_pos.z - m_size.z <= pos.z && pos.z <= m_pos.z + m_size.z)
	{
		bLand = true;
	}

	return bLand;
}

//=======================================================//
// �T�C���X�V����(CSceneMesh)
//=======================================================//
float CSceneMesh::GetUpDown(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// ���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int			nCntVtxNum[4];				// �g�p���_�̔ԍ��ۑ�
	D3DXVECTOR3 Hitnor[2];					// �g�p���_�̖@���ۑ�
	bool		bRange_Block	= false;	// �u���b�N�͈͓�����
	float		fUpDown			= 0.0f;		// �X��

	// ���݂̃u���b�N���m�F
	int nHitBlock =(int)((pos.x + m_size.x) / ((m_size.x * 2.0f) / m_num.x)) +
	(int)((int)(((pos.z - m_size.z) / ((m_size.z * 2.0f) / m_num.z)) * -1) * m_num.x);

	// �e���C���`�F�b�N
	int nCntLand_Block = 0;

	if (0 <= nHitBlock && nHitBlock < (m_nMaxPolygon + (((int)m_num.z - 1) * 4)) / 2)
	{
		for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
		{	// �u���b�N����4���_���J�E���g
			D3DXVECTOR3 vecA, vecC;						// �v�Z�p�x�N�g���i���_���m�A���_��POS�j
			int nData = 0, nChack1 = 0, nChack2 = 0;	// �l�����p�i���̃J�E���g�A�덷�����p�j
			int nVtxNum_N = 0, nVtxNum = 0;				// �g�p���钸�_�ԍ��i���̒��_�ԍ��A���݂̒��_�ԍ��j

			nData = (nCntVtx + 1) % 4;					// ���J�E���g�m�F

			// �J�E���g���̌덷�C���i2*2 = 0134; for = 0143 �� 0134�j
			if		(nCntVtx == 2)	{ nChack1 = 1; }
			else if (nCntVtx == 3)	{ nChack1 = -1; }
			if		(nData == 2)	{ nChack2 = 1; }
			else if (nData == 3)	{ nChack2 = -1; }

			// ���_�ԍ��v�Z
			nVtxNum_N = (nData % 2) + (((int)m_num.x + 1) * (nData / 2)) + nChack2 + (nHitBlock + (nHitBlock / ((int)m_num.x)));
			nVtxNum = (nCntVtx % 2) + (((int)m_num.x + 1) * (nCntVtx / 2)) + nChack1 + (nHitBlock + (nHitBlock / ((int)m_num.x)));

			// ���_�ԍ��ۑ�
			nCntVtxNum[nCntVtx] = nVtxNum - nChack1;

			// �x�N�g���v�Z
			vecA = pVtx[nVtxNum_N].pos - pVtx[nVtxNum].pos;
			vecC = pos - pVtx[nVtxNum].pos;

			if (0 <= ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
			{	// �͈͓��i�E���j�ɑ��݊m�F
				nCntLand_Block++;	// �`�F�b�N���J�E���g

				if (nCntLand_Block == 4)
				{// �`�F�b�N����4�ɓ��B
					bRange_Block = true;	// �u���b�N���ɑ��݊m��

					D3DXVECTOR3 nor[2], vec0[2], vec1[2];	// �@���v�Z�p�i���K���p�A�x�N�g���j

					// ����
					vec0[0] = pVtx[nCntVtxNum[0]].pos - pVtx[nCntVtxNum[2]].pos;
					vec0[1] = pVtx[nCntVtxNum[3]].pos - pVtx[nCntVtxNum[2]].pos;

					// �E��
					vec1[0] = pVtx[nCntVtxNum[3]].pos - pVtx[nCntVtxNum[1]].pos;
					vec1[1] = pVtx[nCntVtxNum[0]].pos - pVtx[nCntVtxNum[1]].pos;

					// ���όv�Z
					D3DXVec3Cross(&nor[0], &vec0[0], &vec0[1]);
					D3DXVec3Cross(&nor[1], &vec1[0], &vec1[1]);

					// ���K��
					D3DXVec3Normalize(&nor[0], &nor[0]);
					D3DXVec3Normalize(&nor[1], &nor[1]);

					// �@���ݒ�
					Hitnor[0] = nor[0];
					Hitnor[1] = nor[1];

					break;
				}
			}
		}

		if (bRange_Block == true)
		{	// �u���b�N���ɑ��ݎ�
			D3DXVECTOR3 vecA, vecC;	// �v�Z�p�x�N�g���i���_���m�A���_��POS�j
			int nVtxNum = 0;		// �g�p���钸�_�ԍ�

			// �x�N�g���v�Z
			vecA = pVtx[nCntVtxNum[3]].pos - pVtx[nCntVtxNum[0]].pos;
			vecC = pos - pVtx[nCntVtxNum[0]].pos;

			if (0 <= ((vecA.z * vecC.x) - (vecA.x * vecC.z)))	{ nVtxNum = 2; }	// �͈͓��i�E���j�ɑ��݊m�F
			else												{ nVtxNum = 1; }	// �͈͓��i�����j�ɑ��݊m�F

			// �x�N�g���v�Z
			D3DXVECTOR3 vecP = D3DXVECTOR3(pos.x + sinf(rot.y), pos.y, pos.z + cosf(rot.y)) - pos;

			// �X�Όv�Z
			fUpDown = ((pVtx[nCntVtxNum[nVtxNum]].nor.x * vecP.x) + (pVtx[nCntVtxNum[nVtxNum]].nor.y * vecP.y) + (pVtx[nCntVtxNum[nVtxNum]].nor.z * vecP.z)) /
			(sqrtf((float)(pow((double)pVtx[nCntVtxNum[nVtxNum]].nor.x, 2) + pow((double)pVtx[nCntVtxNum[nVtxNum]].nor.y, 2) +
			pow((double)pVtx[nCntVtxNum[nVtxNum]].nor.z, 2))) *sqrtf((float)(pow((double)vecP.x, 2) + pow((double)vecP.y, 2) + pow(vecP.z, 2))));
		}
	}
	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return fUpDown;
}

//=======================================================//
// �u���b�N�@���擾����(CSceneMesh)
//=======================================================//
float CSceneMesh::GetBlockNor(D3DXVECTOR3 pos)
{
	// ���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nCntVtxNum[4];				// �g�p���_�̔ԍ��ۑ�
	bool bRange_Block	= false;	// �u���b�N�͈͓�����
	float fNor			= 0.0f;		// �X��

	// ���݂̃u���b�N���m�F
	int nHitBlock =(int)((pos.x + m_size.x) / ((m_size.x * 2.0f) / m_num.x)) +
	(int)((int)(((pos.z - m_size.z) / ((m_size.z * 2.0f) / m_num.z)) * -1) * m_num.x);

	int nCntLand_Block = 0;	// �e���C���`�F�b�N

	if (0 <= nHitBlock && nHitBlock < (m_nMaxPolygon + (((int)m_num.z - 1) * 4)) / 2)
	{
		for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
		{// �u���b�N����4���_���J�E���g
			D3DXVECTOR3 vecA, vecC;						// �v�Z�p�x�N�g���i���_���m�A���_��POS�j
			int nData = 0, nChack1 = 0, nChack2 = 0;	// �l�����p�i���̃J�E���g�A�덷�����p�j
			int nVtxNum_N = 0, nVtxNum = 0;				// �g�p���钸�_�ԍ��i���̒��_�ԍ��A���݂̒��_�ԍ��j

			nData = (nCntVtx + 1) % 4;	// ���J�E���g�m�F

			// �J�E���g���̌덷�C���i2*2 = 0134; for = 0143 �� 0134�j
			if		(nCntVtx == 2)	{ nChack1 = 1; }
			else if (nCntVtx == 3)	{ nChack1 = -1; }
			if		(nData == 2)	{ nChack2 = 1; }
			else if (nData == 3)	{ nChack2 = -1; }

			// ���_�ԍ��v�Z
			nVtxNum_N = (nData % 2) + (((int)m_num.x + 1) * (nData / 2)) + nChack2 + (nHitBlock + (nHitBlock / ((int)m_num.x)));
			nVtxNum = (nCntVtx % 2) + (((int)m_num.x + 1) * (nCntVtx / 2)) + nChack1 + (nHitBlock + (nHitBlock / ((int)m_num.x)));

			// ���_�ԍ��ۑ�
			nCntVtxNum[nCntVtx] = nVtxNum - nChack1;

			// �x�N�g���v�Z
			vecA = pVtx[nVtxNum_N].pos - pVtx[nVtxNum].pos;
			vecC = pos - pVtx[nVtxNum].pos;

			if (0 <= ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
			{	// �͈͓��i�E���j�ɑ��݊m�F
				nCntLand_Block++;	// �`�F�b�N���J�E���g

				// �`�F�b�N����4�ɓ��B
				if (nCntLand_Block == 4) { bRange_Block = true;	break; }	// �u���b�N���ɑ��݊m��
			}
		}

		if (bRange_Block == true)
		{// �u���b�N���ɑ���
			D3DXVECTOR3 vecA, vecC;	// �v�Z�p�x�N�g���i���_���m�A���_��POS�j
			int nVtxNum = 0;		// �g�p���钸�_�ԍ�

			// �x�N�g���v�Z
			vecA = pVtx[nCntVtxNum[3]].pos - pVtx[nCntVtxNum[0]].pos;
			vecC = pos - pVtx[nCntVtxNum[0]].pos;

			if (0 <= ((vecA.z * vecC.x) - (vecA.x * vecC.z)))	{ nVtxNum = 2; }	// �͈͓��i�E���j�ɑ��݊m�F
			else												{ nVtxNum = 1; }	// �͈͓��i�����j�ɑ��݊m�F

			fNor = pVtx[nCntVtxNum[nVtxNum]].nor.y;	// �X�Ζ@��(Y)
		}
	}
	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return fNor;
}

//=======================================================//
// �����ݒ菈��(CSceneMesh)
//=======================================================//
void CSceneMesh::SetHeight(D3DXVECTOR3 pos, float fValue, float fRange)
{
	// ���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���ݒ�
	for (int nCnt_yz = 0; nCnt_yz < m_num.z + 1; nCnt_yz++)
	{	// YZ�J�E���g
		for (int nCnt_x = 0; nCnt_x < m_num.x + 1; nCnt_x++)
		{	// X�J�E���g
			float fLength = sqrtf((float)(pow((double)(pVtx[0].pos.x - pos.x), 2) + pow((double)(pVtx[0].pos.z - pos.z), 2)));

			if (fLength < fRange)
			{
				float fPercent = fLength / fRange;

				pVtx[0].pos.y += cosf(((D3DX_PI * 0.5f) * (fPercent))) * fValue;
			}
			pVtx += 1;	// ���_�o�b�t�@�i�s
		}
	}
}

//=======================================================//
// �����擾����(CSceneMesh)
//=======================================================//
float CSceneMesh::GetHeight(D3DXVECTOR3 pos)
{
	// ���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int			nCntVtxNum[4];					// �g�p���_�̔ԍ��ۑ�
	D3DXVECTOR3 Hitnor[2];						// �g�p���_�̖@���ۑ�
	bool		bRange_Block	= false;		// �u���b�N�͈͓�����
	float		fHeight			= (-500.0f);	// ����

	D3DXVECTOR3 posmtx = pos - m_pos;

	// ���݂̃u���b�N���m�F
	int nHitBlock =(int)((posmtx.x + m_size.x) / ((m_size.x * 2.0f) / m_num.x)) +
	(int)((int)(((posmtx.z - m_size.z) / ((m_size.z * 2.0f) / m_num.z)) * -1) * m_num.x);

	int nCntLand_Block = 0;	// �e���C���`�F�b�N

	if (0 <= nHitBlock && nHitBlock < (m_nMaxPolygon + (((int)m_num.z - 1) * 4)) / 2)
	{
		for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
		{	// �u���b�N����4���_���J�E���g
			D3DXVECTOR3 vecA, vecC;						// �v�Z�p�x�N�g���i���_���m�A���_��POS�j
			int nData = 0, nChack1 = 0, nChack2 = 0;	// �l�����p�i���̃J�E���g�A�덷�����p�j
			int nVtxNum_N = 0, nVtxNum = 0;				// �g�p���钸�_�ԍ��i���̒��_�ԍ��A���݂̒��_�ԍ��j

			nData = (nCntVtx + 1) % 4;	// ���J�E���g�m�F

			// �J�E���g���̌덷�C���i2*2 = 0134; for = 0143 �� 0134�j
			if		(nCntVtx == 2)	{ nChack1 = 1; }
			else if (nCntVtx == 3)	{ nChack1 = -1; }
			if		(nData == 2)	{ nChack2 = 1; }
			else if (nData == 3)	{ nChack2 = -1; }

			// ���_�ԍ��v�Z
			nVtxNum_N = (nData % 2) + (((int)m_num.x + 1) * (nData / 2)) + nChack2 + (nHitBlock + (nHitBlock / ((int)m_num.x)));
			nVtxNum = (nCntVtx % 2) + (((int)m_num.x + 1) * (nCntVtx / 2)) + nChack1 + (nHitBlock + (nHitBlock / ((int)m_num.x)));

			// ���_�ԍ��ۑ�
			nCntVtxNum[nCntVtx] = nVtxNum - nChack1;

			// �x�N�g���v�Z
			vecA = pVtx[nVtxNum_N].pos - pVtx[nVtxNum].pos;
			vecC = posmtx - pVtx[nVtxNum].pos;

			if (0 <= ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
			{// �͈͓��i�E���j�ɑ��݊m�F
				nCntLand_Block++;	// �`�F�b�N���J�E���g

				if (nCntLand_Block == 4)
				{// �`�F�b�N����4�ɓ��B
					bRange_Block = true;	// �u���b�N���ɑ��݊m��

					// �@���v�Z�p�i���K���p�A�x�N�g���j
					D3DXVECTOR3 nor[2], vec0[2], vec1[2];

					// ����
					vec0[0] = pVtx[nCntVtxNum[0]].pos - pVtx[nCntVtxNum[2]].pos;
					vec0[1] = pVtx[nCntVtxNum[3]].pos - pVtx[nCntVtxNum[2]].pos;

					// �E��
					vec1[0] = pVtx[nCntVtxNum[3]].pos - pVtx[nCntVtxNum[1]].pos;
					vec1[1] = pVtx[nCntVtxNum[0]].pos - pVtx[nCntVtxNum[1]].pos;

					// ���όv�Z
					D3DXVec3Cross(&nor[0], &vec0[0], &vec0[1]);
					D3DXVec3Cross(&nor[1], &vec1[0], &vec1[1]);

					// ���K��
					D3DXVec3Normalize(&nor[0], &nor[0]);
					D3DXVec3Normalize(&nor[1], &nor[1]);

					// �@���ݒ�
					Hitnor[0] = nor[0];
					Hitnor[1] = nor[1];

					break;
				}
			}
		}

		if (bRange_Block == true)
		{	// �u���b�N���ɑ���
			D3DXVECTOR3 vecA, vecC;	// �v�Z�p�x�N�g���i���_���m�A���_��POS�j
			int nVtxNum = 0;		// �g�p���钸�_�ԍ�

			// �x�N�g���v�Z
			vecA = pVtx[nCntVtxNum[3]].pos - pVtx[nCntVtxNum[0]].pos;
			vecC = posmtx - pVtx[nCntVtxNum[0]].pos;

			if (0 <= ((vecA.z * vecC.x) - (vecA.x * vecC.z)))	{ nVtxNum = 2; }	// �͈͓��i�E���j�ɑ��݊m�F
			else												{ nVtxNum = 1; }	// �͈͓��i�����j�ɑ��݊m�F

			// �x�N�g���v�Z
			D3DXVECTOR3 vecP = posmtx - pVtx[nCntVtxNum[nVtxNum]].pos;
			vecP.y -= pVtx[nCntVtxNum[nVtxNum]].pos.y;

			// �����v�Z
			fHeight = (((Hitnor[nVtxNum / 2 ^ 1].x * vecP.x) + (Hitnor[nVtxNum / 2 ^ 1].z * vecP.z)) / -Hitnor[nVtxNum / 2 ^ 1].y) + pVtx[nCntVtxNum[nVtxNum]].pos.y;
			fHeight += m_pos.y;
		}
	}
	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return fHeight;
}

//=======================================================//
// �@���v�Z����(CSceneMesh)
//=======================================================//
void CSceneMesh::SetVecNor(void)
{
	/*VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	typedef struct
	{// �e�u���b�N�̖@���v�Z�p
		D3DXVECTOR3 nor[4];
	}NORDATA;

	// �u���b�N�����������m��
	NORDATA *norData = NULL;
	norData = new NORDATA[((m_nMaxPolygon + (((int)m_num.z - 1) * 4)) / 2)];

	if (norData != NULL)
	{// NULL�ȊO��������
		for (int nCntBlock = 0; nCntBlock < (m_nMaxPolygon + (((int)m_num.z - 1) * 4)) / 2; nCntBlock++)
		{// �u���b�N���J�E���g
			int nCntVtxNum[4];		// ���_�̔ԍ��v�Z�p

			// �u���b�N�̒��_�𒲂ׂ�
			nCntVtxNum[0] = (0 % 2) + (((int)m_num.x + 1) * (0 / 2)) + (nCntBlock + (nCntBlock / ((int)m_num.x)));
			nCntVtxNum[1] = (1 % 2) + (((int)m_num.x + 1) * (1 / 2)) + (nCntBlock + (nCntBlock / ((int)m_num.x)));
			nCntVtxNum[2] = (2 % 2) + (((int)m_num.x + 1) * (2 / 2)) + (nCntBlock + (nCntBlock / ((int)m_num.x)));
			nCntVtxNum[3] = (3 % 2) + (((int)m_num.x + 1) * (3 / 2)) + (nCntBlock + (nCntBlock / ((int)m_num.x)));

			D3DXVECTOR3 nor[2], vec0[2], vec1[2];			// �@���v�Z�p�i���K���p�A�x�N�g���j

			// ����
			vec0[0] = pVtx[nCntVtxNum[0]].pos - pVtx[nCntVtxNum[2]].pos;
			vec0[1] = pVtx[nCntVtxNum[3]].pos - pVtx[nCntVtxNum[2]].pos;
			// �E��
			vec1[0] = pVtx[nCntVtxNum[3]].pos - pVtx[nCntVtxNum[1]].pos;
			vec1[1] = pVtx[nCntVtxNum[0]].pos - pVtx[nCntVtxNum[1]].pos;
			// ���όv�Z
			D3DXVec3Cross(&nor[0], &vec0[0], &vec0[1]);
			D3DXVec3Cross(&nor[1], &vec1[0], &vec1[1]);
			// ���K��
			D3DXVec3Normalize(&nor[0], &nor[0]);
			D3DXVec3Normalize(&nor[1], &nor[1]);

			// �@���̕ۑ�
			norData[nCntBlock].nor[0] = (nor[0] + nor[1]) * 0.5f;
			norData[nCntBlock].nor[1] = nor[1];
			norData[nCntBlock].nor[2] = nor[0];
			norData[nCntBlock].nor[3] = (nor[0] + nor[1]) * 0.5f;
		}

		for (int nCntVtx = 0; nCntVtx < m_nMaxVtx; nCntVtx++)
		{// ���_���J�E���g
			D3DXVECTOR3 nor;	// �@���v�Z����
			int SetBlock[4];	// �u���b�N�ԍ�

			// ���_�ɐڂ���u���b�N�𒲂ׂ�
			SetBlock[0] = (nCntVtx - (int)(m_num.x + 1)) - (nCntVtx / (int)(m_num.x + 1));
			SetBlock[1] = (nCntVtx - (int)(m_num.x)) - (nCntVtx / (int)(m_num.x + 1));
			SetBlock[2] = (nCntVtx - 1) - (nCntVtx / (int)(m_num.x + 1));
			SetBlock[3] = (nCntVtx)-(nCntVtx / (int)(m_num.x + 1));

			if (nCntVtx != 0 && nCntVtx != 0 + m_num.x && nCntVtx != (m_nMaxVtx - 1) - m_num.x && nCntVtx != (m_nMaxVtx - 1))
			{// �p�ȊO
				if (nCntVtx <= m_num.x)
				{// �㉡���C��
					nor = (norData[SetBlock[2]].nor[1] + norData[SetBlock[3]].nor[2] + norData[SetBlock[3]].nor[1]) / 3;
				}
				else if (nCntVtx % (int)(m_num.x + 1) == 0)
				{// ���c���C��
					nor = (norData[SetBlock[1]].nor[2] + norData[SetBlock[3]].nor[1] + norData[SetBlock[3]].nor[2]) / 3;
				}
				else if (nCntVtx % (int)(m_num.x + 1) == (int)(m_num.x))
				{// �E�c���C��
					nor = (norData[SetBlock[0]].nor[1] + norData[SetBlock[1]].nor[2] + norData[SetBlock[2]].nor[1]) / 3;
				}
				else if ((m_nMaxVtx - 1) - m_num.x <= nCntVtx)
				{// �������C��
					nor = (norData[SetBlock[0]].nor[2] + norData[SetBlock[0]].nor[1] + norData[SetBlock[1]].nor[2]) / 3;
				}
				else
				{// ���̑�����
					nor = (norData[SetBlock[0]].nor[2] + norData[SetBlock[0]].nor[1] + norData[SetBlock[1]].nor[2] +
						norData[SetBlock[2]].nor[1] + norData[SetBlock[3]].nor[1] + norData[SetBlock[3]].nor[2]) / 6;
				}
			}
			else
			{// �p
				if (nCntVtx == 0)
				{// ����
					nor = norData[SetBlock[3]].nor[0];
				}
				else if (nCntVtx == 0 + m_num.x)
				{// �E��
					nor = norData[SetBlock[2]].nor[1];
				}
				else if (nCntVtx == (m_nMaxVtx - 1) - m_num.x)
				{// ����
					nor = norData[SetBlock[1]].nor[2];
				}
				else if (nCntVtx == (m_nMaxVtx - 1))
				{// �E��
					nor = norData[SetBlock[0]].nor[3];
				}
			}

			// �@���̐ݒ�
			pVtx[nCntVtx].nor = nor;
		}

		if (norData != NULL)
		{// NULL�ȊO�̏ꍇ
			delete[] norData;	// �j��
			norData = NULL;		// NULL��
		}
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();*/
}

//=======================================================//
// �T�C���X�V����(����)(CSceneMesh)
//=======================================================//
void CSceneMesh::SinUpdate(SINTYPE sinType, SINSTART sinStart, D3DXVECTOR3 pos, float frot, float fUp, float fLine)
{
	// ���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �i�[�p
	D3DXVECTOR3 posStart;

	switch (sinStart)
	{
	case SINSTART_POS:		posStart = pos;												break;	// �ʒu
	case SINSTART_CENTER:	posStart = m_size;											break;	// �����
	case SINSTART_LU:		posStart = pVtx[0].pos;										break;	// ����
	case SINSTART_RU:		posStart = pVtx[(int)(m_num.x + 1)].pos;					break;	// �E��
	case SINSTART_LD:		posStart = pVtx[(m_nMaxVtx - 1) - (int)(m_num.x + 1)].pos;	break;	// �����
	case SINSTART_RD:		posStart = pVtx[(m_nMaxVtx - 1)].pos;						break;	// �E���
	}

	// �v�Z�p
	float fMax;

	switch (sinType)
	{
	case SINTYPE_CIRCLE:fMax = sqrtf((float)(pow((double)m_size.x, 2) + pow((double)m_size.z, 2))); break;	// �~��
	case SINTYPE_RIGHT:	fMax = m_size.x * 2.0f; break;														// �E
	case SINTYPE_LEFT:	fMax = m_size.x * 2.0f; break;														// ��
	case SINTYPE_UP:	fMax = m_size.z * 2.0f; break;														// ��
	case SINTYPE_DOWN:	fMax = m_size.z * 2.0f; break;														// ��
	}

	// ���_���ݒ�
	for (int nCnt_yz = 0; nCnt_yz < m_num.z + 1; nCnt_yz++)
	{	// YZ�J�E���g
		for (int nCnt_x = 0; nCnt_x < m_num.x + 1; nCnt_x++)
		{	// X�J�E���g
			float fnow = 0.0f;

			switch (sinType)
			{
			// �~��
			case SINTYPE_CIRCLE:	fnow = sqrtf((float)(pow((double)(pVtx[0].pos.x - posStart.x), 2) + pow((double)(pVtx[0].pos.z - posStart.z), 2))); break;
			case SINTYPE_RIGHT:		fnow = pVtx[0].pos.x - posStart.x; frot *= -1;	break;	// X
			case SINTYPE_LEFT:		fnow = pVtx[0].pos.x - posStart.x;				break;	// X
			case SINTYPE_UP:		fnow = pVtx[0].pos.z - posStart.z;				break;	// Z
			case SINTYPE_DOWN:		fnow = pVtx[0].pos.z - posStart.z;				break;	// Z
			}

			float fPP = fnow / fMax;
			pVtx[0].pos.y = sinf(((D3DX_PI) * (fPP * fLine)) + frot) * (fUp/* * (1.2f - fPP)*/);

			pVtx += 1;	// ���_�o�b�t�@�i�s
		}
	}

	// �@���v�Z
	SetVecNor();
}

//=======================================================//
// �T�C���X�V����(�͈�)(CSceneMesh)
//=======================================================//
void CSceneMesh::SinUpdate(SINTYPE sinType, SINSTART sinStart, D3DXVECTOR3 pos, float frot, float fUp, float fLine, float fRange)
{
	// ���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �i�[�p
	D3DXVECTOR3 posStart;

	switch (sinStart)
	{
	case SINSTART_POS:		posStart = pos - m_pos;										break;	// �ʒu
	case SINSTART_CENTER:	posStart = m_size;											break;	// �����
	case SINSTART_LU:		posStart = pVtx[0].pos;										break;	// ����
	case SINSTART_RU:		posStart = pVtx[(int)(m_num.x + 1)].pos;					break;	// �E��
	case SINSTART_LD:		posStart = pVtx[(m_nMaxVtx - 1) - (int)(m_num.x + 1)].pos;	break;	// �����
	case SINSTART_RD:		posStart = pVtx[(m_nMaxVtx - 1)].pos;						break;	// �E���
	}

	// �͈̓T�C�Y�i�[�p
	float fMax;

	switch (sinType)
	{
	case SINTYPE_CIRCLE:	fMax = sqrtf((float)(pow((double)m_size.x, 2) + pow((double)m_size.z, 2))); break;	// �~��
	case SINTYPE_RIGHT:		fMax = m_size.x * 2.0f; break;														// �E
	case SINTYPE_LEFT:		fMax = m_size.x * 2.0f; break;														// ��
	case SINTYPE_UP:		fMax = m_size.z * 2.0f; break;														// ��
	case SINTYPE_DOWN:		fMax = m_size.z * 2.0f; break;														// ��
	}

	// ���_���ݒ�
	for (int nCnt_yz = 0; nCnt_yz < m_num.z + 1; nCnt_yz++)
	{	// YZ�J�E���g
		for (int nCnt_x = 0; nCnt_x < m_num.x + 1; nCnt_x++)
		{	// X�J�E���g
			float fLength = sqrtf((float)(pow((double)(pVtx[0].pos.x - posStart.x), 2) + pow((double)(pVtx[0].pos.z - posStart.z), 2)));

			if (fLength < fRange)
			{
				float fPercent	= fLength / fRange;
				float fnow		= 0.0f;

				pVtx[0].pos.y = sinf(((D3DX_PI) * (fPercent * fLine)) + frot) * (fUp * (1.0f - fPercent));
			}

			pVtx += 1;	// ���_�o�b�t�@�i�s
		}
	}
	// �@���v�Z
	SetVecNor();
}

//=======================================================//
// �T�C�Y�̐ݒ�
//=======================================================//
void CSceneMesh::SetSize(D3DXVECTOR3 size, D3DXVECTOR3 nNum)
{
	// ���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR2 tex;		// UV�v�Z�p
	int nCnt;				// YZ����
	nCnt = (int)m_num.z;	// Z����

	// ���_���ݒ�
	for (int nCnt_yz = 0; nCnt_yz < nCnt + 1; nCnt_yz++)
	{	// YZ�J�E���g
		for (int nCnt_x = 0; nCnt_x < m_num.x + 1; nCnt_x++)
		{	// X�J�E���g

			// ���_���ݒ�
			pVtx[0].pos = D3DXVECTOR3(-size.x + ((size.x * 2) / m_num.x) * nCnt_x, 0.0f, size.z - ((size.z * 2) / m_num.z) * nCnt_yz);
			//pVtx[0].pos.y = -12.0f + (float)(rand() % 12);

			tex = D3DXVECTOR2(m_TexUV.x * (nCnt_x % ((int)m_num.x + 1)), m_TexUV.y * (nCnt_yz));

			// �e�N�X�`���ݒ�
			pVtx[0].tex = tex;

			// �@���ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// �@���ݒ�
			D3DXVECTOR3 vecNor;
			vecNor.x = m_pos.x - (pVtx[0].pos.x / pVtx[0].pos.x);	// X
			vecNor.z = m_pos.z - (pVtx[0].pos.z / pVtx[0].pos.z);	// Z
			D3DXVec3Normalize(&vecNor, &vecNor);				// ���K��
			pVtx[0].nor = D3DXVECTOR3(vecNor.x, 1.0f, vecNor.z);

			// �J���[�ݒ�
			pVtx[0].col = m_col;

			// ���_�o�b�t�@�i�s
			pVtx += 1;
		}
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

}