#pragma once

namespace SCENE
{
	enum ID { SCENE_MENU, SCENE_CHARSELECT, SCENE_STAGE, SCENE_QUIT, SCENE_END};
}

namespace OBJ
{
	enum ID { OBJECT, CASTLE, NOCREATE, WALL, BUTTON, UI, FONT ,EFFECT, LEADER, BUILDING , MONSTER, PLATER, BULLET, CRAD, GOLD, OBJ_END };
	enum STATE { LIVE ,DEAD, STATE_END};
}

namespace PLAYER
{
	enum STATE { STAND, MOVE, ATTACK, STATE_END};
	enum SOIDIER { MILITIA, SHIELD, LONGBOW, SOLDER_END };
	enum TYPE { MERCHANT,GENERAL, BUILDER, TYPE_END};
}

namespace CRAD
{
	enum TYPE { ECONOMY_CRAD, MILITARY_CRAD, BUILDING_CRAD, TECHNOLOGY_CRAD, CRAD_END};
	enum ECONOMY { GRAIN, CHEESE, POTTER, BUILDINGTAX, MEMBRANELABOR, ECONOMY_END};
	//0.곡물 1.치즈 2.도공 3.건물세 4.막노동
	enum MILITARY { MILITIA, SHIELD, LongBow, MILITARY_END};
	//0.민병대 1.방패병 2.장궁병
	enum BUILDING { HOUSE, DEFENSEWALL, FACTORY, MONASTERY, BARN , APARTMENTHOUSE, FARM};
	//0.집 1.방어벽 2. 공장 3.수도원 4.축사 5.공동주택 6.사육장
	enum MYTECHNOLOGY {};
}

namespace BUTTON
{
	enum LIST { LEADER_SKILL, BAG, RESET, CEMETRY, MENU, STATE_STOP, EXPERT };
}