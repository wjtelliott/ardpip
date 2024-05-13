/***************************************************************************************
** File Name  :         CItems.h
** Author     :         WJTE
** Date       :         4/28/2024
** Description:         This file will be used to capture all our item / highlightable
**                      strings and data on the pip boy screen. Our util function here
**                      allows us to return our item data to other files.
***************************************************************************************/

// The arduino we are using doesn't have enough onboard RAM to support all of these in memory.
// We are going to instead create a bunch of macros and a switch to grab the needed character strings.
// This will keep the data on the flashed ROM instead.
#ifndef CItems_h
#define CItems_h
#include "CPipBoyPageNames.h"
#include "CLinkedList.h"

// we use these macros to make it easier to add new IDs without accidentally overwriting another one
// the precompiler won't make these stick around in mem
#define STAT_HEALTH_ID 1
#define STAT_RADIATION_ID 2
#define STAT_LEVEL_ID 3
#define SKILLS_BARTER_ID 4
#define SKILLS_BIG_GUNS_ID 5
#define SPECIAL_STRENGTH_ID 6
#define SPECIAL_PERCEPTION_ID 7
#define SPECIAL_ENDURANCE_ID 8
#define SPECIAL_CHARISMA_ID 9
#define SPECIAL_INTELLIGENCE_ID 10
#define SPECIAL_AGILITY_ID 11
#define SPECIAL_LUCK_ID 12
#define PERKS_ANT_SIGHT_ID 13
#define PERKS_COMMANDO_ID 14
#define GENERAL_QUESTS_COMPLETED_ID 15
#define GENERAL_LOCATIONS_DISCOVERED_ID 16

Item *getItem(uint16_t itemCode) {
  switch (itemCode) {
    default: return new Item();

    // Stat page - Stats
    case STAT_HEALTH_ID: return new Item("HEALTH    : 100/100", 1);
    case STAT_RADIATION_ID: return new Item("RADIATION : SEVERE", 1);
    case STAT_LEVEL_ID: return new Item("LEVEL     : 4", 1);

    // stat page - s.p.e.c.i.a.l.
    case SPECIAL_STRENGTH_ID: return new Item("STRENGTH     : 5", 1);
    case SPECIAL_PERCEPTION_ID: return new Item("PERCEPTION   : 7", 1);
    case SPECIAL_ENDURANCE_ID: return new Item("ENDURANCE    : 4", 1);
    case SPECIAL_CHARISMA_ID: return new Item("CHARISMA     : 2", 1);
    case SPECIAL_INTELLIGENCE_ID: return new Item("INTELLIGENCE : 8", 1);
    case SPECIAL_AGILITY_ID: return new Item("AGILITY      : 2", 1);
    case SPECIAL_LUCK_ID: return new Item("LUCK         : 1", 1);

    // stat page - skills
    case SKILLS_BARTER_ID: return new Item("Barter              30", 1);
    case SKILLS_BIG_GUNS_ID: return new Item("Big Guns            11", 1);

    case PERKS_ANT_SIGHT_ID: return new Item("Ant Sight", 1);
    case PERKS_COMMANDO_ID: return new Item("Commando", 1);

    case GENERAL_QUESTS_COMPLETED_ID: return new Item("Quests Completed                5", 1);
    case GENERAL_LOCATIONS_DISCOVERED_ID: return new Item("Locations Discovered           21", 1);
  }
}

#endif
