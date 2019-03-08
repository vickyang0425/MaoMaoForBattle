/*
 * mygame.h: セ郎纗笴栏セōclassinterface
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
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *   2006-02-08 V4.2
 *      1. Rename OnInitialUpdate() -> OnInit().
 *      2. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      3. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress.
*/

#include "CEraser.h"
#include "CBall.h"
#include "CBouncingBall.h"

namespace game_framework
{
/////////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////////

enum AUDIO_ID  				// ﹚竡贺絪腹
{
    AUDIO_DING,				// 0
    AUDIO_LAKE,				// 1
    AUDIO_NTUT				// 2
};

class CPractice
{
    public:
        CPractice();
        void LoadBitmap();
        void OnMove();
        void OnShow();
    private:
        CMovingBitmap pic;
        int x, y;

    private:
};
/////////////////////////////////////////////////////////////////////////////
// 这个class提供地图的构成
//看懂就可以改写成自己的程式了
/////////////////////////////////////////////////////////////////////////////

class CBounCBouncingBall;
class CGameMap
{
    public:
        CGameMap();
        void LoadBitmap();     //载入地图
        void OnShow();        //显示地图
		void OnMove();                                //播放弹跳球的动画
		void OnKeyDown(UINT);                     //处理按键按下后CGameMap的反应
		void RandomBouncingBall();                 //随机弹跳球的个数加入到Map中
		void InitializeBouncingBall(int,int,int); //初始化弹跳球
		~CGameMap();                              //析构函数
    protected:
        CMovingBitmap blue, green; //建立蓝色地图和绿色地图
        int map[4][5];              //建立一个地图矩阵的index
        const int X, Y;             //大地图的左上角x,y坐标
        const int MW, MH;           //每张小地图的宽度和高度
		CBouncingBall* bballs;      //CBouncingBall指针
		int random_num;            //随机个数


};




/////////////////////////////////////////////////////////////////////////////
// 硂class笴栏笴栏秨繷礶ン
// –Member functionImplementation常璶来
/////////////////////////////////////////////////////////////////////////////

class CGameStateInit : public CGameState
{
    public:
        CGameStateInit(CGame* g);
        void OnInit();  								// 笴栏の瓜砞﹚
        void OnBeginState();							// 砞﹚–Ω┮惠跑计
        void OnKeyUp(UINT, UINT, UINT); 				// 矪瞶龄絃Up笆
        void OnLButtonDown(UINT nFlags, CPoint point);  // 矪瞶菲公笆
    protected:
        void OnShow();									// 陪ボ硂篈笴栏礶
    private:
        CMovingBitmap logo;								// csielogo
};

/////////////////////////////////////////////////////////////////////////////
// 硂class笴栏笴栏磅︽ン璶笴栏祘Α常硂柑
// –Member functionImplementation常璶来
/////////////////////////////////////////////////////////////////////////////

class CGameStateRun : public CGameState
{
    public:
        CGameStateRun(CGame* g);
        ~CGameStateRun();
        void OnBeginState();							// 砞﹚–Ω┮惠跑计
        void OnInit();  								// 笴栏の瓜砞﹚
        void OnKeyDown(UINT, UINT, UINT);
        void OnKeyUp(UINT, UINT, UINT);
        void OnLButtonDown(UINT nFlags, CPoint point);  // 矪瞶菲公笆
        void OnLButtonUp(UINT nFlags, CPoint point);	// 矪瞶菲公笆
        void OnMouseMove(UINT nFlags, CPoint point);	// 矪瞶菲公笆
        void OnRButtonDown(UINT nFlags, CPoint point);  // 矪瞶菲公笆
        void OnRButtonUp(UINT nFlags, CPoint point);	// 矪瞶菲公笆
    protected:
        void OnMove();									// 簿笆笴栏じ
        void OnShow();									// 陪ボ硂篈笴栏礶
    private:
        const int		NUMBALLS;	// 瞴羆计
        CMovingBitmap	background;	// 璉春瓜
        CMovingBitmap	help;		// 弧瓜
        CBall*			ball;		// 瞴皚
        CMovingBitmap	corner;		// à辅瓜
        CEraser			eraser;		// ╃
        CInteger		hits_left;	// 逞疾阑计
        CBouncingBall   bball;		// は滦紆铬瞴

		CGameMap       gamemap;    //地图
        CPractice      c_practice;
        CMovingBitmap   practice;
        CMovingBitmap   practiceBlue;
        int picX, picY;


};

/////////////////////////////////////////////////////////////////////////////
// 硂class笴栏挡篈(Game Over)
// –Member functionImplementation常璶来
/////////////////////////////////////////////////////////////////////////////

class CGameStateOver : public CGameState
{
    public:
        CGameStateOver(CGame* g);
        void OnBeginState();							// 砞﹚–Ω┮惠跑计
        void OnInit();
    protected:
        void OnMove();									// 簿笆笴栏じ
        void OnShow();									// 陪ボ硂篈笴栏礶
    private:
        int counter;	// 计ぇ璸计竟
};

}