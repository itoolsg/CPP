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
	printf_s("照括馬室推 1俳鰍 惟績脊艦陥.\n");
	printf_s("源益企稽 1俳鰍税 持醗聖 馬澗 惟績戚倉.\n");
	printf_s("1俳鰍聖 走鎧檎辞 伽 乞窮 引鯉聖 呪戟馬獣奄 郊遇艦陥.\n");
	printf_s("引鯉(光)級戚 幻幻帖澗 省柔艦陥.\n");
	printf_s("号狽徹澗 戚疑  a澗 因維\n");
	printf_s("1俳奄 板鋼 奄綬 s徹(人陥陥陥 : 蒋掩聖 厳生檎 宋澗陥.) \n");
	printf_s("3俳奄 段鋼 奄綬 w徹(梱喰帖奄 : 雌馬疎酔 企唖識聖 厳壱 赤澗 引鯉聖 魁鎧獄鍵陥.) \n");
	printf_s("光級聖 陥 薦暗馬檎 亜舌 神献楕生稽 亜獣檎 陥製 舘域稽 角嬢逢艦陥. \n");
	printf_s("GOOD LUCK \n");
	//CreateTempMap();
	//CreateMobs();
	GetKey();
	GetStage(1);
}
//陥製 什砺戚走稽 亜閃神奄
void CGameManager::GetStage(int stage) {
	
	delete m_Map;//薄仙 己 肢薦廃陥.

	CGameMap* map = m_data.LoadMap(stage);//陥製 什砺戚走税 己聖 亜閃紳陥.
	m_Map = map;
	if(map == nullptr) {//殖漁
		m_flag = false;
		return;
	}

	
	m_stage = stage;

	m_PC->SetIsNextStage(false);//陥製 什砺戚走稽 角嬢亜虞澗 重硲研 昼社獣轍陥.
	m_PC->SetPosition(0, 6);//坦製 是帖稽 戚疑獣轍陥.

	m_Screen.Clean();
	printf_s("STAGE :: %d\n", m_Map->GetNo());
	GetKey();
}
void CGameManager::Run() {
	bool is_MonsterReadyToAttack = false;
	int chargeMPDelay = 0;// 10腰 鉢檎戚 飴重 吃 凶原陥 +1梢 辰錘陥.せせ
	if(m_Map == nullptr) {
		printf_s("己戚 稽球鞠走 省紹柔艦陥.\n");
		GetKey();
		return;
	}


	//m_flag 什砺戚走亜 魁猿走 穐澗走 溌昔
	while(inputProc() && m_flag) {
		Sleep(100);//0.1原陥 飴重
		is_MonsterReadyToAttack = !is_MonsterReadyToAttack;
		++chargeMPDelay;
		if(chargeMPDelay >=15) {//15腰 飴重 拝 凶原陥 mp 1 雌渋
			chargeMPDelay = 0;
			m_PC->SetMp(m_PC->GetMp() + 1);
		}
		m_ai.Move(m_PC, m_Map);//佼什斗亜 爽昔因拭惟 姶..せせ

		//2腰 飴重 拝凶原陥 凶険 呪 赤澗 映軒亜 持沿.せせせ
		//巷旋 雌殿 3腰 飴重猿走 巷獣.
		if(m_PC->GetUnBeatDelay() <= 0 && is_MonsterReadyToAttack)
			m_ai.Attack(m_PC, m_Map);
		else
			m_PC->MinusUnBeatDelay();

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
	m_Screen.Clean();
	printf_s("GAME OVER\n");
	printf_s("-_- 宋陥艦 食奄辞 匂奄拝 依昔亜... \n");
	GetKey();
}
void CGameManager::GameEnding() {
	m_Screen.Clean();
	printf_s("GAME Ending\n");
	printf_s("殖漁析 匝 硝紹走??\n");
	printf_s("2俳鰍 凶亀 戚群襖醤 せせせ\n");
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
	case 1072:// 1000 + 72 UP DIRECTION
		m_PC->ChangeDir(m_Map, DIR_UP);
		break;
	case 1075://1000+ 75 図楕 号狽徹
		m_PC->ChangeDir(m_Map, DIR_LEFT);
		break;
	case 1080://1000 + 80 焼掘号狽徹
		m_PC->ChangeDir(m_Map, DIR_DOWN);
		break;
	case 1077://1000 + 77 ->
		m_PC->ChangeDir(m_Map, DIR_RIGHT);
		break;
	case 'A':
		m_PC->Attack(m_Map);
		break;
	case 'S'://Straight Skill
		m_PC->AttackStraight(m_Map);
		break;
	case 'W'://Wide Attack Skill
		m_PC->AttackWide(m_Map);
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
