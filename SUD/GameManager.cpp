#include "stdafx.h"
#include "GameManager.h"
#include "Mob.h"
#include "Block.h"
#include <iostream>
#include <time.h>
#include <conio.h>
#include <windows.h>

CGameManager::CGameManager(void)
{
	m_PC = new CPC();
	CLog::GetInstance()->reset();
	m_stage = 1;
	m_flag = true;
}


CGameManager::~CGameManager(void)
{
	delete m_PC;
	if(m_Map != nullptr)
		delete m_Map;
}

void CGameManager::Init() {
	srand((unsigned int)time(NULL));


	printf_s("[Game Start!!]\n");
	m_PC->SetPosition(5, 5);
	m_PC->PrintPosition();
	printf_s("照括馬室推 1俳奄惟績脊艦陥.\n");
	printf_s("源益企稽 1俳奄税 持醗聖 馬澗 惟績戚倉.\n");
	printf_s("1俳奄研 走鎧檎辞 伽 乞窮 引鯉聖 呪戟馬獣奄 郊遇艦陥.\n");
	printf_s("引鯉(光)級戚 幻幻帖澗 省柔艦陥.\n");
	printf_s("是 w 焼掘 s 図 a 神献 d  因維 e \n");
	printf_s("光級聖 陥 薦暗馬檎 亜舌 神献楕生稽 亜獣檎 陥製 舘域稽 角嬢逢艦陥. \n");
	printf_s("GOOD LUCK \n");
	//CreateTempMap();
	//CreateMobs();
	GetKey();
	GetStage(1);
	GetKey();
}
void CGameManager::GetStage(int stage) {
	delete m_Map;
	CGameMap* map = m_data.LoadMap(stage);
	if(map == nullptr)
		m_flag = false;

	m_Map = map;
	m_stage = stage;
	m_PC->SetIsNextStage(false);
	m_PC->SetPosition(0, 6);
}
void CGameManager::Run() {
	bool is_ReadyAttack = false;

	if(m_Map == nullptr) {
		printf_s("己戚 稽球鞠走 省紹柔艦陥.\n");
		GetKey();
		return;
	}

	//m_flag 什砺戚走亜 魁猿走 穐澗走 溌昔
	while(inputProc() && m_flag) {
		Sleep(100);//0.1段原陥 飴重
		is_ReadyAttack = !is_ReadyAttack;

		m_ai.Move(m_PC, m_Map);//佼什斗亜 爽昔因拭惟 姶..せせ

		//0.2段原陥 凶険 呪 赤澗 映軒亜 持沿.せせせ
		//左什 凶 刃穿 宋製.... 陥陥陥陥...
		if(is_ReadyAttack)
			m_ai.Attack(m_PC, m_Map);

		m_Screen.Clean();//鉢檎 適軒嬢
		m_Screen.Show(m_Map, m_PC);//鉢檎 益形爽壱.

		if(m_PC->Is_Over() )
			break;
	}
	if(!m_flag)
		GameEnding();

	if(m_PC->Is_Over() )
		GameOver();

}
void CGameManager::GameOver() {
	printf_s("GAME OVER\n");
	GetKey();
}
void CGameManager::GameEnding() {
	printf_s("GAME Ending\n");
	printf_s("殖漁析 匝 硝紹走??\n");
	printf_s("2俳奄 凶亀 戚群襖醤 せせせ\n");
	GetKey();
}
void CGameManager::Release() {

}
int CGameManager::GetKey() {
	int key = _getch();
	

	// 奄管徹研 刊研 獣 224研 胡煽 閤壱 陥獣 廃腰 希 閤澗陥. alt, control, shift 去 せせ
	if(key == 224)
		key = 1000 + _getch();

	//社庚切研 企庚切稽 搭析廃陥.
	if(key>=97 && key <= 122)
		key = key - 32;
	return key;
}

bool CGameManager::inputProc()
{
	//徹左球 脊径聖 閤焼辞
	// 朕固球研 督縮馬壱, 限惟 坦軒廃陥.
	// q研 脊径 閤生檎 曽戟吉陥.
	if(!_kbhit())
		return true;

	std::string strInput;
	//getline(std::cin, strInput);
	int key = GetKey();

	switch(key) {
	case 'W':
		m_PC->ChangeDir(m_Map, DIR_UP);
		break;
	case 'A':
		m_PC->ChangeDir(m_Map, DIR_LEFT);
		break;
	case 'S':
		m_PC->ChangeDir(m_Map, DIR_DOWN);
		break;
	case 'D':
		m_PC->ChangeDir(m_Map, DIR_RIGHT);
		break;
	case 'E':
		m_PC->Attack(m_Map);
		break;
	case 'Q':
		printf_s("惟績聖 曽戟馬獣畏柔艦猿? (Y/N)\n");
		int quitKey = GetKey();
		if(quitKey == 'Y')
			return false;

	}

	//PC拭辞 神献楕生稽 魁猿走 亜檎 学什闘什砺戚走稽 亜虞壱 廃陥.
	if(m_PC->GetIsNextStage()) {
		GetStage(m_stage + 1);
	}

	return true;
}
