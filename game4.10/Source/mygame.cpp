/*
 * mygame.cpp: ���n�����[�����class��implementation
 * Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
 *
 * This file is part of game, a free game development framework for windows.
 *
 * game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * History:
 *   2002-03-04 V3.1
 *          Add codes to demostrate the use of CMovingBitmap::ShowBitmap(CMovingBitmap &).
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *      2. Demo the use of CInteger in CGameStateRun.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *      1. Add codes to display IDC_GAMECURSOR in GameStateRun.
 *   2006-02-08 V4.2
 *      1. Revise sample screens to display in English only.
 *      2. Add code in CGameStateInit to demo the use of PostQuitMessage().
 *      3. Rename OnInitialUpdate() -> OnInit().
 *      4. Fix the bug that OnBeginState() of GameStateInit is not called.
 *      5. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      6. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2006-12-30
 *      1. Bug fix: fix a memory leak problem by replacing PostQuitMessage(0) as
 *         PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0).
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress.
 *   2010-03-23 V4.6
 *      1. Demo MP3 support: use lake.mp3 to replace lake.wav.
*/

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"

namespace game_framework
{

/////////////////////////////////////////////////////////////////////////////
// ���@��class���[����[���ͼ�������
/////////////////////////////////////////////////////////////////////////////
CGameMap::CGameMap() : X(20), Y(40), MW(120), MH(100) //�����ͼ���Ͻ������Լ�ÿ��Сͼ��͸�
{
    int map_init[4][5] =        //�����ͼ���г�ֵ
    {
        {0, 0, 1, 0, 0},
        {0, 1, 2, 1, 0},
        {1, 2, 1, 2, 1},
        {2, 1, 2, 1, 2}
    };

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            map[i][j] = map_init[i][j];//��������map��
        }
    }
	bballs = NULL;
	random_num = 0;
}

void CGameMap::LoadBitmap()
{
    blue.LoadBitmap(IDB_B);
    green.LoadBitmap(IDB_R);
}

void CGameMap::OnShow()
{
	for (int i = 0; i < 5; i++)//������ʾ����ͼ
	{
		for (int j = 0; j < 4; j++)//������ʾ����ͼ
		{
			switch (map[j][i])
			{
			case 0:
				break;
			case 1:
				blue.SetTopLeft(X + (MW*i), Y + (MH*j));//�趨ÿ��ͼ����
				blue.ShowBitmap();                      //��ʾ�趨�������
				break;
			case 2:
				green.SetTopLeft(X + (MW*i), Y + (MH*j));//�趨ÿ��ͼ����
				green.ShowBitmap();                       //��ʾ�趨�������
				break;
			default:
				ASSERT(0);               //map���в��ó���0,1,2֮���ֵ
			}
		}
	}

	for (int i = 0; i < random_num; i++)
	{
		bballs[i].OnShow();
	}

}

void CGameMap::InitializeBouncingBall(int ini_index, int row, int column)
{
	const int VELOCITY = 0;
	const int BALL_PIC_HEIGHT = 15;
	int floor = Y + (row + 1)*MH - BALL_PIC_HEIGHT;

	bballs[ini_index].LoadBitmap();       //���뵯����Ķ���
	bballs[ini_index].SetFloor(floor);    //�趨��������ʼˮƽ��
	bballs[ini_index].SetVelocity(VELOCITY + column);//�趨��������ʼ�ٶȣ�Խ�ұߵĵ���Խ��
	bballs[ini_index].SetXY(X + column * MW + MW / 2, floor);//�趨�����ʼλ��X����Ϊ��mapһ���λ��

}
void CGameMap::RandomBouncingBall()
{
	const int MAX_RAND_NUM = 10;                //������������ֵ
	random_num = (rand() % MAX_RAND_NUM) + 1;    //���1~MAX_RAND_NUM
	delete[]bballs;
	bballs = new CBouncingBall[random_num];    //�������ֵ��̬����CBouncingBall����
	int ini_index = 0;                           //��ʼ����������
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 5; col++)
		{
			if (map[row][col] != 0 && ini_index < random_num)//ֻ��������ɫ�ĵ�ͼ�ҳ�ʼ����������������С������ĸ���
			{
				InitializeBouncingBall(ini_index, row, col);
				ini_index++;;                                //��ʼ�������м�һ
			}
		}
	}
}

void CGameMap::OnKeyDown(UINT nChar)
{
	const int KEY_SPACE = 0x20;
	if (nChar == KEY_SPACE)
	{
		RandomBouncingBall();//���հ׼����º����������
	}
}

void CGameMap::OnMove()
{
	for (int i = 0; i < random_num; i++)
	{
		bballs[i].OnMove();

	}
}

CGameMap::~CGameMap()
{
	delete []bballs;
}





//--------------------------------------------------
CPractice::CPractice()
{
    x = y = 0;
}
void CPractice::OnMove()
{
    if (y <= SIZE_Y)
    {
        x += 3;
        y += 3;
    }
    else
    {
        x = y = 0;
    }
}
void CPractice::LoadBitmap()
{
    pic.LoadBitmap(IDB_PORING);
}
void CPractice::OnShow()
{
    pic.SetTopLeft(x, y);
    pic.ShowBitmap();
}

/////////////////////////////////////////////////////////////////////////////
// ���@��class���[����[���_�^�������
/////////////////////////////////////////////////////////////////////////////

CGameStateInit::CGameStateInit(CGame* g) : CGameState(g)
{
}

void CGameStateInit::OnInit()
{
    //
    // ���D�ܶ��r��OnInit�d�����еĈDҪ���ܶ��r�g����������[�����
    //     �ȵĲ��͟����[������F��Loading ...�����@ʾLoading���M�ȡ�
    //
    ShowInitProgress(0);	// һ�_ʼ��loading�M�Ȟ�0%
    //
    // �_ʼ�d���Y��
    //
    logo.LoadBitmap(IDB_BIGDOG);
    Sleep(300);				/// �������Ա㿴����M�ȣ����H�[��Ո�h����Sleep
    //
    // ��OnInit�������ӵ�CGameStaterRun::OnInit()�������M��߀�]��100%
    //
}

void CGameStateInit::OnBeginState()
{
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    const char KEY_ESC = 27;
    const char KEY_SPACE = ' ';

    if (nChar == KEY_SPACE)
        GotoGameState(GAME_STATE_RUN);						// �ГQ��GAME_STATE_RUN
    else if (nChar == KEY_ESC)								// Demo �P�]�[��ķ���
        PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// �P�]�[��
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{
    GotoGameState(GAME_STATE_RUN);		// �ГQ��GAME_STATE_RUN
}

void CGameStateInit::OnShow()
{
    //
    // �N��logo
    //
    logo.SetTopLeft((SIZE_X - logo.Width()) / 2, SIZE_Y / 8);
    logo.ShowBitmap();
    //
    // DemoΞĻ���͵�ʹ�ã����^�_�l�rՈ�M������ֱ��ʹ�����ͣ�����CMovingBitmap���^��
    //
    CDC* pDC = CDDraw::GetBackCDC();			// ȡ�� Back Plain �� CDC
    CFont f, *fp;
    f.CreatePointFont(160, "Times New Roman");	// �a�� font f; 160��ʾ16 point���֦r
    fp = pDC->SelectObject(&f);					// �x�� font f
    pDC->SetBkColor(RGB(0, 0, 0));
    pDC->SetTextColor(RGB(255, 255, 0));
    pDC->TextOut(120, 220, "Please click mouse or press SPACE to begin.");
    pDC->TextOut(5, 395, "Press Ctrl-F to switch in between window mode and full screen mode.");

    if (ENABLE_GAME_PAUSE)
        pDC->TextOut(5, 425, "Press Ctrl-Q to pause the Game.");

    pDC->TextOut(5, 455, "Press Alt-F4 or ESC to Quit.");
    pDC->SelectObject(fp);						// ��ŵ� font f (ǧ�f��Ҫ©�˷ŵ�)
    CDDraw::ReleaseBackCDC();					// ��ŵ� Back Plain �� CDC
}



/////////////////////////////////////////////////////////////////////////////
// ��class������̬(Game Over)
/////////////////////////////////////////////////////////////////////////////

CGameStateOver::CGameStateOver(CGame* g)
    : CGameState(g)
{
}

void CGameStateOver::OnMove()
{
    counter--;

    if (counter < 0)
        GotoGameState(GAME_STATE_INIT);
}

void CGameStateOver::OnBeginState()
{
    counter = 30 * 5; // 5 seconds
}

void CGameStateOver::OnInit()
{
    //
    // ���D�ܶ��r��OnInit�d�����еĈDҪ���ܶ��r�g����������[�����
    //     �ȵĲ��͟����[������F��Loading ...�����@ʾLoading���M�ȡ�
    //
    ShowInitProgress(66);	// �ӂ�ǰһ����B���M�ȣ���̎�M��ҕ��66%
    //
    // �_ʼ�d���Y��
    //
    Sleep(300);				// �������Ա㿴����M�ȣ����H�[��Ո�h����Sleep
    //
    // ��K�M�Ȟ�100%
    //
    ShowInitProgress(100);
}

void CGameStateOver::OnShow()
{
    CDC* pDC = CDDraw::GetBackCDC();			// ȡ�� Back Plain �� CDC
    CFont f, *fp;
    f.CreatePointFont(160, "Times New Roman");	// �a�� font f; 160��ʾ16 point���֦r
    fp = pDC->SelectObject(&f);					// �x�� font f
    pDC->SetBkColor(RGB(0, 0, 0));
    pDC->SetTextColor(RGB(255, 255, 0));
    char str[80];								// Demo ���֌��ִ����D�Q
    sprintf(str, "Game Over ! (%d)", counter / 30);
    pDC->TextOut(240, 210, str);
    pDC->SelectObject(fp);						// ��ŵ� font f (ǧ�f��Ҫ©�˷ŵ�)
    CDDraw::ReleaseBackCDC();					// ��ŵ� Back Plain �� CDC
}




/////////////////////////////////////////////////////////////////////////////
// ���@��class���[����[������������Ҫ���[���ʽ�����@�e
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame* g)
    : CGameState(g), NUMBALLS(28)
{
    ball = new CBall[NUMBALLS];
    picX = picY = 0;
}

CGameStateRun::~CGameStateRun()
{
    delete[] ball;
}

void CGameStateRun::OnBeginState()
{
    const int BALL_GAP = 90;
    const int BALL_XY_OFFSET = 45;
    const int BALL_PER_ROW = 7;
    const int HITS_LEFT = 10;
    const int HITS_LEFT_X = 590;
    const int HITS_LEFT_Y = 0;
    const int BACKGROUND_X = 60;
    const int ANIMATION_SPEED = 15;

    for (int i = 0; i < NUMBALLS; i++)  				// �O�������ʼ����
    {
        int x_pos = i % BALL_PER_ROW;
        int y_pos = i / BALL_PER_ROW;
        ball[i].SetXY(x_pos * BALL_GAP + BALL_XY_OFFSET, y_pos * BALL_GAP + BALL_XY_OFFSET);
        ball[i].SetDelay(x_pos);
        ball[i].SetIsAlive(true);
    }

    eraser.Initialize();
    background.SetTopLeft(BACKGROUND_X, 0);				// �O����������ʼ����
    help.SetTopLeft(0, SIZE_Y - help.Height());			// �O���f���D����ʼ����
    hits_left.SetInteger(HITS_LEFT);					// ָ��ʣ�µ�ײ����
    hits_left.SetTopLeft(HITS_LEFT_X, HITS_LEFT_Y);		// ָ��ʣ��ײ����������
    CAudio::Instance()->Play(AUDIO_LAKE, true);			// �ܷ� WAVE
    CAudio::Instance()->Play(AUDIO_DING, false);		// �ܷ� WAVE
    CAudio::Instance()->Play(AUDIO_NTUT, true);			// �ܷ� MIDI
}

void CGameStateRun::OnMove()							// �Ƅ��[��Ԫ��

{
    //practice.SetTopLeft(10, 10);
    practiceBlue.SetTopLeft(100, 100);

    if (picX <= SIZE_Y)
    {
        picX = +5;
        picY = +5;
    }
    else
    {
        picX = picY = 0;
    }

    practice.SetTopLeft(picX, picY);
    c_practice.OnMove();

	gamemap.OnMove();

    //ָ����ϰ�D�ε�����
    //
    // ���ϣ���޸�cursor�Ę�ʽ���t�������ʽ��commmentȡ������
    //
    // SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
    //
    // �Ƅӱ����D������
    //
    if (background.Top() > SIZE_Y)
        background.SetTopLeft(60, -background.Height());

    background.SetTopLeft(background.Left(), background.Top() + 1);
    //
    // �Ƅ���
    //
    int i;

    for (i = 0; i < NUMBALLS; i++)
        ball[i].OnMove();

    //
    // �ƄӲ���
    //
    eraser.OnMove();

    //
    // �Д�����Ƿ�������
    //
    for (i = 0; i < NUMBALLS; i++)
        if (ball[i].IsAlive() && ball[i].HitEraser(&eraser))
        {
            ball[i].SetIsAlive(false);
            CAudio::Instance()->Play(AUDIO_DING);
            hits_left.Add(-1);

            //
            // ��ʣ�N��ײ�Δ���0���t����Game Over��B
            //
            if (hits_left.GetInteger() <= 0)
            {
                CAudio::Instance()->Stop(AUDIO_LAKE);	// ���� WAVE
                CAudio::Instance()->Stop(AUDIO_NTUT);	// ���� MIDI
                GotoGameState(GAME_STATE_OVER);
            }
        }

    //
    // �Ƅӏ�������
    //
    bball.OnMove();
}

void CGameStateRun::OnInit()  								// �[��ĳ�ֵ���D���O��
{
    //
    // ���D�ܶ��r��OnInit�d�����еĈDҪ���ܶ��r�g����������[�����
    // �ȵĲ��͟����[������F��Loading ...�����@ʾLoading���M�ȡ�
    //
    ShowInitProgress(33);	// �ӂ�ǰһ����B���M�ȣ���̎�M��ҕ��33%
    //
    // �_ʼ�d���Y��
    //
    int i;

    for (i = 0; i < NUMBALLS; i++)
        ball[i].LoadBitmap();								// �d���i����ĈD��

    eraser.LoadBitmap();
    background.LoadBitmap(IDB_BACKGROUND);					// �d�뱳���ĈD��
    //
    // ��ɲ���Loading����������M��
    //
    ShowInitProgress(50);
    Sleep(300);// �������Ա㿴����M�ȣ����H�[��Ո�h����Sleep
    //
    // �^�m�d�������Y��
    //
    help.LoadBitmap(IDB_HELP, RGB(255, 255, 255));				// �d���f���ĈD��
    corner.LoadBitmap(IDB_CORNER);							// �d�����D��
    corner.ShowBitmap(background);							// ��corner�N��background
    bball.LoadBitmap();										// �d��D��
    hits_left.LoadBitmap();
    CAudio::Instance()->Load(AUDIO_DING, "sounds\\ding.wav");	// �d�뾎̖0����ding.wav
    CAudio::Instance()->Load(AUDIO_LAKE, "sounds\\lake.mp3");	// �d�뾎̖1����lake.mp3
    CAudio::Instance()->Load(AUDIO_NTUT, "sounds\\ntut.mid");	// �d�뾎̖2����ntut.mid
    //
    // ��OnInit��CGameStaterOver::OnInit()�A��100%



	gamemap.LoadBitmap();       //�����ͼ
    c_practice.LoadBitmap();
    practice.LoadBitmapA(IDB_PORING);
    practiceBlue.LoadBitmapA(IDB_R, RGB(255, 255, 255));
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    const char KEY_LEFT = 0x25; // keyboard����^
    const char KEY_UP = 0x26; // keyboard�ϼ��^
    const char KEY_RIGHT = 0x27; // keyboard�Ҽ��^
    const char KEY_DOWN = 0x28; // keyboard�¼��^

    if (nChar == KEY_LEFT)
        eraser.SetMovingLeft(true);

    if (nChar == KEY_RIGHT)
        eraser.SetMovingRight(true);

    if (nChar == KEY_UP)
        eraser.SetMovingUp(true);

    if (nChar == KEY_DOWN)
        eraser.SetMovingDown(true);

	gamemap.OnKeyDown(nChar);
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    const char KEY_LEFT = 0x25; // keyboard����^
    const char KEY_UP = 0x26; // keyboard�ϼ��^
    const char KEY_RIGHT = 0x27; // keyboard�Ҽ��^
    const char KEY_DOWN = 0x28; // keyboard�¼��^

    if (nChar == KEY_LEFT)
        eraser.SetMovingLeft(false);

    if (nChar == KEY_RIGHT)
        eraser.SetMovingRight(false);

    if (nChar == KEY_UP)
        eraser.SetMovingUp(false);

    if (nChar == KEY_DOWN)
        eraser.SetMovingDown(false);
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)   // ̎����Ą���
{
    eraser.SetMovingLeft(true);
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// ̎����Ą���
{
    eraser.SetMovingLeft(false);
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// ̎����Ą����@
{
    // �]�¡������Ҫ̎�����Ƅӵ�Ԓ����code���@�e
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // ̎����Ą���
{
    eraser.SetMovingRight(true);
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// ̎����Ą���
{
    eraser.SetMovingRight(false);
}

void CGameStateRun::OnShow()
{
    //
    //  ע�⣺Show�e��ǧ�f��Ҫ�Ƅ��κ���������ˣ��Ƅ����˵Ĺ�������Move���Ō���
    //        ��t��ҕ�������L�D�r(OnDraw)������͕��Ƅӣ���������ܹ֡��Q���g�Z
    //        �f��Moveؓ؟MVC�е�Model��Showؓ؟View����View��������Model��
    //
    //
    //  �N�ϱ����D��ײ�������򡢲��ӡ���������
    //
    background.ShowBitmap();			// �N�ϱ����D
    help.ShowBitmap();					// �N���f���D
    hits_left.ShowBitmap();

    for (int i = 0; i < NUMBALLS; i++)
        ball[i].OnShow();				// �N�ϵ�i̖��

    bball.OnShow();						// �N�Ϗ�������
    eraser.OnShow();					// �N�ϲ���
    //
    //  �N�����ϼ����½���ĈD
    //
    corner.SetTopLeft(0, 0);
    corner.ShowBitmap();
    corner.SetTopLeft(SIZE_X - corner.Width(), SIZE_Y - corner.Height());
    corner.ShowBitmap();

	gamemap.OnShow();                  //����ͼ�N��ΞĻ
    practice.ShowBitmap();             //���D���N��ΞĻ
    practiceBlue.ShowBitmap();
    c_practice.OnShow();
	                  
}

}