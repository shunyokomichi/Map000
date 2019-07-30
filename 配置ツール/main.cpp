//=======================================================//
//
// ���C������ [main.cpp]
// Author : Shun Yokomichi
//
//=======================================================//

//*******************************************************//
// �w�b�_�t�@�C���̃C���N���[�h
//*******************************************************//
#include "main.h"		// ���C��
#include "manager.h"	// �}�l�[�W���[
#include "renderer.h"	// �����_���[


//*******************************************************//
// �}�N����`
//*******************************************************//
#define CLASS_NAME	"AppClass"							// �E�C���h�E�N���X��
#define WINDOW_NAME	"�}�b�v�z�u�c�[��				"	// �E�C���h�E�L���v�V������

//*******************************************************//
// �v���g�^�C�v�錾
//*******************************************************//
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

bool openReadFile(void);

bool ScreenShot(char *cBmpName);												// �X�N���[���V���b�g
void DrawCursor(HDC hdc);														// �J�[�\���`��
bool WriteBitmap(LPTSTR lpszFileName, int nWidth, int nHeight, LPVOID lpBits);	// �r�b�g�}�b�v����
HBITMAP CreateBackbuffer(int nWidth, int nHeight);								// �e�N�X�`������

//=======================================================//
// ���C���֐�
//=======================================================//
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ���������[�N���o
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// �}�l�[�W��������
	CManager *pManager = NULL;

#ifdef _DEBUG
	LPCSTR lpMenu = "IDR_MENU1";	// ���j���[�\��
#else
	LPCSTR lpMenu = NULL;			// ���j���[��\��
#endif

	lpMenu = NULL;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),								// WNDCLASSEX�̃������T�C�Y�w��
		CS_CLASSDC,										// �\������E�B���h�E�̃X�^�C���ݒ�
		WndProc,										// �E�B���h�E�v���V�[�W���̃A�h���X�w��
		0,												// �ʏ�͎w�肵�Ȃ�����"0"�w��
		0,												// �ʏ�͎w�肵�Ȃ�����"0"�w��
		hInstance,										// WinMain�̈����̃C���X�^���X�n���h���w��
		NULL,											// �^�X�N�o�[�Ɏg�p����A�C�R���w��
		LoadCursor(NULL, IDC_ARROW),					// �g�p����}�E�X�J�[�\���w��
		(HBRUSH)(COLOR_WINDOW + 1),						// �E�B���h�E�̃N���C�A���g�̈�̔w�i�F�ݒ�
		lpMenu,											// Windows�ɂ��郁�j���[�ݒ�
		CLASS_NAME,										// �E�B���h�E�N���X��
		NULL											// �t�@�C���A�C�R���Ɏg�p����A�C�R���w��
	};
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };	// �N���C�A���g�̈�ݒ�
	HWND hWnd;											// �E�B���h�E�n���h���i���ʎq�j
	MSG msg;											// ���b�Z�[�W�i�[�ϐ�
	DWORD dwCurrentTime;								// ���ݎ���
	DWORD dwExecLastTime;								// �ŏI��������
	DWORD dwFrameCount;									// FPS�l�J�E���g
	DWORD dwFPSLastTime;								// FPS�l�ŏI��������

	// �E�B���h�E�N���X�o�^
	RegisterClassEx(&wcex);

	// �w��N���C�A���g�̈�m�ۂ̂��߂ɕK�v�ȃE�B���h�E���W�v�Z
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, true);

	// �E�B���h�E�쐬
	hWnd = CreateWindow(CLASS_NAME,	// �E�B���h�E�N���X��
	WINDOW_NAME,					// �E�B���h�E��
	WS_OVERLAPPEDWINDOW,			// �E�B���h�E�X�^�C��
	CW_USEDEFAULT,					// �E�B���h�E�̍���X���W
	CW_USEDEFAULT,					// �E�B���h�E�̍���Y���W
	(rect.right - rect.left),		// �E�B���h�E�̕�
	(rect.bottom - rect.top),		// �E�B���h�E�̍���
	NULL,							// �e�E�B���h�E�̃n���h��
	NULL,							// ���j���[�n���h���܂��̓E�B���h�EID
	hInstance,						// �C���X�^���X�n���h��
	NULL);							// �E�B���h�E�쐬�f�[�^

	srand((unsigned int)time(0));	// �����_��

	// NULL�̏ꍇ�A�}�l�[�W������
	if (pManager == NULL) { pManager = new CManager; }

	if (pManager != NULL)
	{	// NULL�ȊO�̏ꍇ
		if (FAILED(pManager->Init(hInstance, hWnd, FALSE))) { return -1; }

		// ����\�ݒ�
		timeBeginPeriod(1);

		// �t���[���J�E���g������
		dwCurrentTime	= dwFrameCount	= 0;
		dwExecLastTime	= dwFPSLastTime = timeGetTime(); 	// ���ݎ������擾

		// �E�C���h�E�\��
		ShowWindow(hWnd, nCmdShow);	// �E�B���h�E�\���i�E�B���h�E�̕\����Ԑݒ�j
		UpdateWindow(hWnd);			// �E�B���h�E���f�i�E�B���h�E�̃N���C�A���g�̈�X�V�j

		// ���b�Z�[�W���[�v
		while (1)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// ���b�Z�[�W���擾��"0"��Ԃ�
			{	// Windows�̏���

				// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				if (msg.message == WM_QUIT) { break; }

				else
				{	// ���b�Z�[�W�|��ƃf�B�X�p�b�`
					TranslateMessage(&msg);	// ���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
					DispatchMessage(&msg);	// �E�B���h�E�v���V�[�W���փ��b�Z�[�W���o
				}
			}
			else
			{	// DirectX�̏���
				static int nCntFps = 0;
				dwCurrentTime = timeGetTime();		// ���ݎ����擾
				if ((dwCurrentTime - dwFPSLastTime) >= 500)
				{	// 30F���Ɏ��s
					nCntFps			= dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
					dwFPSLastTime	= dwCurrentTime;	// ���ݎ����ۑ�
					dwFrameCount	= 0;
				}

				if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
				{	// 60F�o��
					dwExecLastTime	= dwCurrentTime;	// ���ݎ����ۑ�

					// �X�V����
					pManager->Update();

					// �`�揈��
					pManager->Draw();

					dwFrameCount++;
				}
			}
		}
	}

	// �I������
	if (pManager != NULL)
	{	// NULL�ȊO�̏ꍇ
		pManager->Uninit();
		delete pManager;
		pManager = NULL;
	}

	// �E�B���h�E�N���X�̓o�^����
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// ����\��߂�
	timeEndPeriod(1);

	return (int)msg.wParam;
}

//=======================================================//
// �E�C���h�E�v���V�[�W��
//=======================================================//
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nChackNum = 0;	// ���݂̑I�����j���[
	UINT uState = 0;	// ���j���[���擾�p

	switch (uMsg)
	{
	case WM_CREATE:
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:				// [ESC]�L�[�������ꂽ
			DestroyWindow(hWnd);	// �E�B���h�E��j������悤�w������
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=======================================================//
// �X�N���[���V���b�g
//=======================================================//
bool ScreenShot(char *cBmpName)
{
	HDC     hdc;
	HWND    hwndDesk;
	RECT    rc;
	BITMAP  bm;
	HBITMAP hbmp;
	HBITMAP hbmpPrev;
	bool bSuccess = false;				// ����������

	hwndDesk = GetForegroundWindow();	// �t�H�A�O���E���h�̃E�B���h�E�n���h���擾
	GetClientRect(hwndDesk, &rc);		// �N���C�A���g�̃E�B���h�E�T�C�Y�擾

	hdc = CreateCompatibleDC(NULL);

	hbmp = CreateBackbuffer((rc.right - rc.left), (rc.bottom - rc.top));

	hbmpPrev = (HBITMAP)SelectObject(hdc, hbmp);

	BitBlt(hdc, 0, 0, rc.right, rc.bottom, GetDC(hwndDesk), 0, 0, SRCCOPY);			// �r�b�g�}�b�v����
	//DrawCursor(hdc);

	GetObject(hbmp, sizeof(BITMAP), &bm);

	FILE *pFile = NULL;					// �t�@�C���|�C���^
	char cName[128];					// ���O�Ǘ�
	int nNum = 0;						// ���O�ԍ�

	while (1)
	{
		wsprintf(cName, "data\\TEXTURE\\ScreenShot\\%s_%d.bmp", cBmpName, nNum);	// ���O�쐬
		pFile = fopen(cName, "r");		// �t�@�C�����J��

		if (pFile == NULL)
		{// ���݂��Ȃ�
			break;
		}
		else
		{// ���݂���
			fclose(pFile);				// �t�@�C�������
			nNum++;						// �ԍ���i�߂�
		}
	}

	bSuccess = (bool)WriteBitmap(TEXT(cName), rc.right, rc.bottom, bm.bmBits);		// �e�N�X�`������

	SelectObject(hdc, hbmpPrev);
	DeleteObject(hbmp);
	DeleteDC(hdc);

	return bSuccess;
}

//=======================================================//
// �r�b�g�}�b�v����
//=======================================================//
HBITMAP CreateBackbuffer(int nWidth, int nHeight)
{
	LPVOID           lp;
	BITMAPINFO       bmi;
	BITMAPINFOHEADER bmiHeader;

	ZeroMemory(&bmiHeader, sizeof(BITMAPINFOHEADER));
	bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmiHeader.biWidth = nWidth;
	bmiHeader.biHeight = nHeight;
	bmiHeader.biPlanes = 1;
	bmiHeader.biBitCount = 24;

	bmi.bmiHeader = bmiHeader;

	return CreateDIBSection(NULL, (LPBITMAPINFO)&bmi, DIB_RGB_COLORS, &lp, NULL, 0);
}

//=======================================================//
// �J�[�\���`��
//=======================================================//
void DrawCursor(HDC hdc)
{
	int        x, y;
	CURSORINFO cursorInfo;
	ICONINFO   iconInfo;

	cursorInfo.cbSize = sizeof(CURSORINFO);
	GetCursorInfo(&cursorInfo);

	GetIconInfo(cursorInfo.hCursor, &iconInfo);

	x = cursorInfo.ptScreenPos.x - iconInfo.xHotspot;
	y = cursorInfo.ptScreenPos.y - iconInfo.yHotspot;
	DrawIcon(hdc, x, y, cursorInfo.hCursor);
}

//=======================================================//
// �e�N�X�`���쐬
//=======================================================//
bool WriteBitmap(LPTSTR lpszFileName, int nWidth, int nHeight, LPVOID lpBits)
{
	HANDLE           hFile;
	DWORD            dwResult;
	DWORD            dwSizeImage;
	BITMAPFILEHEADER bmfHeader;
	BITMAPINFOHEADER bmiHeader;

	hFile = CreateFile(lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	dwSizeImage = (nHeight) * ((3 * (nWidth) + 3) / 4) * 4;

	ZeroMemory(&bmfHeader, sizeof(BITMAPFILEHEADER));
	bmfHeader.bfType = *(LPWORD)"BM";
	bmfHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwSizeImage;
	bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	WriteFile(hFile, &bmfHeader, sizeof(BITMAPFILEHEADER), &dwResult, NULL);

	ZeroMemory(&bmiHeader, sizeof(BITMAPINFOHEADER));
	bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmiHeader.biWidth = (nWidth);
	bmiHeader.biHeight = (nHeight);
	bmiHeader.biPlanes = 1;
	bmiHeader.biBitCount = 24;
	bmiHeader.biSizeImage = dwSizeImage;
	bmiHeader.biCompression = BI_RGB;

	WriteFile(hFile, &bmiHeader, sizeof(BITMAPINFOHEADER), &dwResult, NULL);

	WriteFile(hFile, lpBits, dwSizeImage, &dwResult, NULL);

	CloseHandle(hFile);

	return true;
}

bool openReadFile()
{
	char    buf[1024];

	OPENFILENAME    ofn;
	char            filename[256], msg[300];
	HANDLE          hFile;
	DWORD           dwBytes;

	char Path[256];
	GetModuleFileName(NULL, Path, MAX_PATH);

	char drive[MAX_PATH + 1]
		, dir[MAX_PATH + 1]
		, fname[MAX_PATH + 1]
		, ext[MAX_PATH + 1];

	_splitpath(Path, drive, dir, fname, ext);//�p�X�����\���v�f�ɕ������܂�

	char cPassDir[MAX_PATH + 1];
	wsprintf(cPassDir, "%s%s", drive, dir);

	filename[0] = '\0';														//�Y���ƃf�t�H���g�t�@�C�����ɕςȕ����񂪕\�������
	memset(&ofn, 0, sizeof(OPENFILENAME));									//�\���̂�0�ŃN���A
	ofn.lStructSize = sizeof(OPENFILENAME);									//�t�@�C���̃T�C�Y
	ofn.lpstrInitialDir = cPassDir;
	ofn.lpstrFilter = "text file(*.txt)\0*.txt\0all file(*.*)\0*.*\0\0";	//�J���t�@�C���̎��
	ofn.lpstrFile = filename;												//��������t�@�C����
	ofn.nMaxFile = sizeof(filename);										//�J����t�@�C�������̍ő吔
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "txt";

	if (GetOpenFileName(&ofn) != TRUE)
		return FALSE;

	hFile = CreateFile(filename, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		wsprintf(msg, "%s failed to open.", filename);
		MessageBox(NULL, msg, "Section5.2 open file dialog", MB_OK);
		return FALSE;
	}
	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
	ReadFile(hFile, buf, sizeof(buf) - 1, &dwBytes, NULL);
	*((u_char*)buf + dwBytes) = 0;  //�ǂݏI����'\0'��}��
	CloseHandle(hFile);

	return TRUE;
}