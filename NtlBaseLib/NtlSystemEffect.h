﻿//***********************************************************************************
//
//	File		:	NtlSystemEffect.h
//
//	Begin		:	2006-09-28
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	게임 데이타와 링크시키기 위한 프로그램 시스템 이펙트 코드를 정의한 파일 입니다.
//
//***********************************************************************************

//-----------------------------------------------------------------------------------
// 이 헤더파일은 직접 인클루드 하지 마세요!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//-----------------------------------------------------------------------------------

// 1 ~ 99
DEF_SYSTEM_EFFECT_CODE( PASSIVE_MAX_LP_UP,						false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_MAX_EP_UP,						false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_MAX_RP_DOWN,					false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_PHYSICAL_OFFENCE_UP,			false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_ENERGY_OFFENCE_UP,				false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_PHYSICAL_DEFENCE_UP,			false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_ENERGY_DEFENCE_UP,				false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_STR_UP,							false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_CON_UP,							false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_FOC_UP,							false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_DEX_UP,							false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_SOL_UP,							false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_ENG_UP,							false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_DASH,							false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_HOVERING,						false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_LP_REGENERATION,				false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_EP_REGENERATION,				false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_ATTACK_RATE_UP,					false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_DODGE_RATE_UP,					false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_BLOCK_RATE_UP,					false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_CURSE_SUCCESS_UP,				false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_CURSE_TOLERANCE_UP,				false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_PHYSICAL_CRITICAL_UP,			false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_ENERGY_CRITICAL_UP,				false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_RP_CHARGE_SPEED,				false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_MOVE_SPEED,						false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_ATTACK_SPEED_UP,				false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_SKILL_CASTING_TIME_DOWN,		false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_SKILL_COOL_TIME_DOWN,			false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_CHARGE,							false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_BLOCK_MODE,						false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_GUARD_COUNTERATTACK,			false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_DOT_VALUE_UP_ALL,				false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_DOT_TIME_UP_ALL,				false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_SKILL_EFFECT_VALUE_UP,			false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_SKILL_KEEP_TIME_UP,				false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_REQUIRE_EP_DOWN,				false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_CHANGE_APPLY_RANGE,				false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_APPLY_AREA_SIZE_UP,				false )
DEF_SYSTEM_EFFECT_CODE( PASSIVE_USE_RANGE_UP,					false )

// 100 ~ 199
DEF_SYSTEM_EFFECT_CODE( ACTIVE_DIRECT_DAMAGE,					true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_BACKSTAB,						true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_WARP_BACK_ATTACK,				false )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_THROUGH_ATTACK,					false )

// 200 ~ 299
DEF_SYSTEM_EFFECT_CODE( ACTIVE_DAMAGE_OVER_TIME,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_POISON,							true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_STOMACHACHE,						true )

// 300 ~ 399
DEF_SYSTEM_EFFECT_CODE( ACTIVE_DIRECT_HEAL,						true )

// 400 ~ 499
DEF_SYSTEM_EFFECT_CODE( ACTIVE_HEAL_OVER_TIME,					true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_EP_OVER_TIME,					true )

// 500 ~ 599
DEF_SYSTEM_EFFECT_CODE( ACTIVE_TELEPORT_BIND,					false )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_INVITE,							false )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_VISIT,							false )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_POISON_CURE,						true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_STOMACHACHE_CURE,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_PARALYZE_CURE,					true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_CURSE_DEBUFF,					true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_SUPER_SAIYAN,					true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_PURE_MAJIN,						true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_SUMMON,							true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_EP_UP,							true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_RP_UP,							true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_RESCUE,							true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_QUEST_TRIGGER,					true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_OBJECT_TRIGGER,					true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_GAMBLE_ITEM,						true )// [7/21/2008 SGpro]
DEF_SYSTEM_EFFECT_CODE( ACTIVE_KAIOKEN,							true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_VEHICLE,							true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_WARP,							true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_CLEAR_TARGET,					true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_RECIPE_UPDATE,					true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_CREATE_HOIPOI_MIX_MACHINE,		true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_CREATE_HOIPOI_HOUSE,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_MONSTER_TRANSFORM,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_GREAT_NAMEK,						true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_SCRIPT_SIGNAL_SPS,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_SCRIPT_SIGNAL_TQS,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_SCRIPT_SIGNAL_WPS,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_SCRIPT_SIGNAL_AIS,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_SCRIPT_EXECUTE_WPS_LOC,			true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_SHIFT_SEAL_STATE,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_TELEPORT_DOJO,					true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_SELF_RESCUE,						true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_CALL_PARTY_MEMBER,				true )

// 600 ~ 699
DEF_SYSTEM_EFFECT_CODE( ACTIVE_BIG_MONKEY,						true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_FUSION,							false )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_TRANSFORM,						true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_MAX_LP_UP,						true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_MAX_EP_UP,						true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_MAX_RP_UP,						true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_PHYSICAL_OFFENCE_UP,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_ENERGY_OFFENCE_UP,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_PHYSICAL_DEFENCE_UP,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_ENERGY_DEFENCE_UP,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_STR_UP,							true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_CON_UP,							true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_FOC_UP,							true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_DEX_UP,							true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_SOL_UP,							true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_ENG_UP,							true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_MOVE_SPEED_UP,					true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_ATTACK_SPEED_UP,					true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_ATTACK_RATE_UP,					true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_DODGE_RATE_UP,					true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_BLOCK_RATE_UP,					true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_HONEST_DEFENCE_UP,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_STRANGE_DEFENCE_UP,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_WILD_DEFENCE_UP,					true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_ELEGANCE_DEFENCE_UP,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_FUNNY_DEFENCE_UP,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_ENERGY_REFLECTION,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_LP_REGENERATION,					true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_EP_REGENERATION,					true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_RP_CHARGE_SPEED,					true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_CURSE_SUCCESS,					true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_CURSE_TOLERANCE,					true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_PHYSICAL_CRITICAL,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_ENERGY_CRITICAL,					true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_SKILL_CASTING_TIME_DOWN,			true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_SKILL_COOL_TIME_DOWN,			true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_ITEM_BREAK_RATE_DOWN,			true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_BARRIER,							true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_PARALYZE_TOLERANCE_UP,			true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_TERROR_TOLERANCE_UP,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_CONFUSE_TOLERANCE_UP,			true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_STONE_TOLERANCE_UP,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_CANDY_TOLERANCE_UP,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_PARALYZE_KEEPTIME_DOWN,			true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_TERROR_KEEPTIME_DOWN,			true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_CONFUSE_KEEPTIME_DOWN,			true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_STONE_KEEPTIME_DOWN,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_CANDY_KEEPTIME_DOWN,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_BLEEDING_KEEPTIME_DOWN,			true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_POISON_KEEPTIME_DOWN,			true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_STOMACHACHE_KEEPTIME_DOWN,		true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_CRITICAL_BLOCK_UP,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_NORMAL_SKILL_BLOCK_UP,			true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_CURSE_SKILL_BLOCK_UP,			true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_KNOCKDOWN_ATTACK_BLOCK_UP,		true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_HTB_SKILL_BLOCK_UP,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_SIT_DOWN_LP_REGENERATION_UP,		true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_SIT_DOWN_EP_REGENERATION_UP,		true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_PHYSICAL_CRITICAL_DAMAGE_UP,		true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_ENERGY_CRITICAL_DAMAGE_UP,		true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_ITEM_UPGRADE_RATE_UP,			true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_SPINNING_ATTACK,					true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_ATTACK_RANGE_UP,					true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_HIDE_KI,							true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_HIDE,							true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_FAKE_DEATH,						true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_CANT_BE_TARGETTED,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_BATTLE_ATTRIBUTE_UP,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_PHYSICAL_REFLECTION,				true )

// 700 ~ 799
DEF_SYSTEM_EFFECT_CODE( ACTIVE_EP_DOWN,							true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_RP_DOWN,							true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_BLESS_DEBUFF,					true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_LP_STEAL,						true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_EP_STEAL,						true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_PULL,							false )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_MAKE_LOWEST_AGGRO,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_AGGRO_DOWN,						true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_AGGRO_UP,						true )

// 800 ~ 899
DEF_SYSTEM_EFFECT_CODE( ACTIVE_TAUNT,							true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_PARALYZE,						false )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_CONFUSE,							true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_TERROR,							true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_STUN,							false )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_SLEEP,							false )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_STONE,							false )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_CANDY,							false )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_MAX_LP_DOWN,						true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_MAX_EP_DOWN,						true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_MAX_RP_DOWN,						true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_PHYSICAL_OFFENCE_DOWN,			true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_ENERGY_OFFENCE_DOWN,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_PHYSICAL_DEFENCE_DOWN,			true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_ENERGY_DEFENCE_DOWN,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_STR_DOWN,						true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_CON_DOWN,						true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_FOC_DOWN,						true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_DEX_DOWN,						true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_SOL_DOWN,						true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_ENG_DOWN,						true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_MOVE_SPEED_DOWN,					true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_ATTACK_SPEED_DOWN,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_ATTACK_RATE_DOWN,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_DODGE_RATE_DOWN,					true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_BLOCK_RATE_DOWN,					true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_HONEST_DEFENCE_DOWN,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_STRANGE_DEFENCE_DOWN,			true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_WILD_DEFENCE_DOWN,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_ELEGANCE_DEFENCE_DOWN,			true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_FUNNY_DEFENCE_DOWN,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_SKILL_CASTING_TIME_UP,			true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_SKILL_COOL_TIME_UP,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_ATTACK_RANGE_DOWN,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_TIED_UP,							true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_BAN_CHARGE,						true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_FREEZE,							true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_LP_STEAL_OVER_TIME,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_EP_STEAL_OVER_TIME,				true )
DEF_SYSTEM_EFFECT_CODE( ACTIVE_BATTLE_ATTRIBUTE_DOWN,			true )

//-----------------------------------------------------------------------------------