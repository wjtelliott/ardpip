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

// this _REALLY_ sucks, but it will work for now
#ifndef CItems_h
#define CItems_h
#include "CPipBoyPageNames.h"

// Things like stats / specials / radios / quests are considered items for now

// *************** STAT PAGE ****************** //
#define STAT STAT_CATEGORY1
#define STAT_HEALTH    { "HEALTH   : 100/100", STAT }
#define STAT_HEALTH_ID 1
#define STAT_RADIATION { "RADIATION: SEVERE", STAT }
#define STAT_RADIATION_ID 2
#define STAT_LEVEL     { "LEVEL    : 4", STAT }
#define STAT_LEVEL_ID 3

#define CONDITION STAT_CATEGORY2
#define CONDITION_RADIATION { "Severe radiation", CONDITION }
#define CONDITION_RADIATION_ID 4
#define CONDITION_POISONED { "Poisoned", CONDITION }
#define CONDITION_POISONED_ID 5

#define SPECIAL STAT_CATEGORY3
#define SPECIAL_STRENGTH     { "STRENGTH     : 5", SPECIAL }
#define SPECIAL_STRENGTH_ID 6
#define SPECIAL_PERCEPTION   { "PERCEPTION   : 7", SPECIAL }
#define SPECIAL_PERCEPTION_ID 7
#define SPECIAL_ENDURANCE    { "ENDURANCE    : 4", SPECIAL }
#define SPECIAL_ENDURANCE_ID 8
#define SPECIAL_CHARISMA     { "CHARISMA     : 2", SPECIAL }
#define SPECIAL_CHARISMA_ID 9
#define SPECIAL_INTELLIGENCE { "INTELLIGENCE : 8", SPECIAL }
#define SPECIAL_INTELLIGENCE_ID 10
#define SPECIAL_AGILITY      { "AGILITY      : 2", SPECIAL }
#define SPECIAL_AGILITY_ID 11
#define SPECIAL_LUCK         { "LUCK         : 1", SPECIAL }
#define SPECIAL_LUCK_ID 12

#define HEALTH STAT_CATEGORY4
#define HEALTH_BROKEN_LEG { "Crippled leg", HEALTH }
#define HEALTH_BROKEN_LEG_ID 13
#define HEALTH_BROKEN_ARM { "Crippled arm", HEALTH }
#define HEALTH_BROKEN_ARM_ID 14

#define STAT_1 STAT_CATEGORY5
#define STAT_1_BROKEN_LEG { "sample data", STAT_1 }
#define STAT_1_BROKEN_LEG_ID 15
#define STAT_1_BROKEN_ARM { "lorem ipsum", STAT_1 }
#define STAT_1_BROKEN_ARM_ID 16

// *************** INVENTORY PAGE ****************** //
#define WEAPON INVENTORY_CATEGORY1
#define WEAPON_PISTOL_32 { ".32 Pistol", WEAPON }
#define WEAPON_PISTOL_32_ID 200
#define WEAPON_PISTOL_WildBill { "Wild Bill's Sidearm", WEAPON }
#define WEAPON_PISTOL_WildBill_ID 201
#define WEAPON_PISTOL_10mm { "10mm pistol", WEAPON }
#define WEAPON_PISTOL_10mm_ID 202
#define WEAPON_PISTOL_CA10mm { "Colonel Autumn's 10mm pistol", WEAPON }
#define WEAPON_PISTOL_CA10mm_ID 203
#define WEAPON_PISTOL_S10mm { "Silenced 10mm pistol", WEAPON }
#define WEAPON_PISTOL_S10mm_ID 204
#define WEAPON_PISTOL_CP { "Chinese pistol", WEAPON }
#define WEAPON_PISTOL_CP_ID 205
#define WEAPON_PISTOL_ZRCP { "Zhu-Rong v418 Chinese pistol", WEAPON }
#define WEAPON_PISTOL_ZRCP_ID 206
#define WEAPON_PISTOL_Dart { "Dart gun", WEAPON }
#define WEAPON_PISTOL_Dart_ID 207
#define WEAPON_PISTOL_44 { "Scoped .44 Magnum", WEAPON }
#define WEAPON_PISTOL_44_ID 208
#define WEAPON_PISTOL_Blackhawk { "Blackhawk", WEAPON }
#define WEAPON_PISTOL_Blackhawk_ID 209
#define WEAPON_PISTOL_CMagnum { "Callahan's Magnum", WEAPON }
#define WEAPON_PISTOL_CMagnum_ID 210
#define WEAPON_PISTOL_PRevolver { "Paulson's revolver", WEAPON }
#define WEAPON_PISTOL_PRevolver_ID 211

#define APPAREL INVENTORY_CATEGORY2
#define APPAREL_VAULT_SUIT_101 { "Vault 101 Suit", APPAREL }
#define APPAREL_VAULT_SUIT_101_ID 212
#define APPAREL_SHADY_SUNGLASSES { "Shady sunglasses", APPAREL }
#define APPAREL_SHADY_SUNGLASSES_ID 213

#define AID INVENTORY_CATEGORY3
#define AID_STIMPACK { "Stimpack (18)", AID }
#define AID_STIMPACK_ID 214
#define AID_BUFFOUT { "Buffout", AID }
#define AID_BUFFOUT_ID 215
#define AID_JET { "Jet (35)", AID }
#define AID_JET_ID 216

#define AMMO INVENTORY_CATEGORY4
#define AMMO_10mm { "10mm ammo", AMMO }
#define AMMO_10mm_ID 217
#define AMMO_FATMAN { "Mini nuke", AMMO }
#define AMMO_FATMAN_ID 218
#define AMMO_DART { "Dart (9)", AMMO }
#define AMMO_DART_ID 219

#define MISC_ITEM INVENTORY_CATEGORY5
#define MISC_ITEM_TED_BEAR { "Teddy bear", MISC_ITEM }
#define MISC_ITEM_TED_BEAR_ID 220
#define MISC_ITEM_DINNER_PLATE { "Dinner plate", MISC_ITEM }
#define MISC_ITEM_DINNER_PLATE_ID 221
#define MISC_ITEM_DESK_LAMP { "Broken desk light", MISC_ITEM }
#define MISC_ITEM_DESK_LAMP_ID 222

// *************** DATA PAGE ****************** //

// data page 1 + 2 are not currently implemented

#define QUEST DATA_CATEGORY3
#define QUEST_FIND_DAD { "Find Dad", QUEST }
#define QUEST_FIND_DAD_ID 800
#define QUEST_YGSEITH { "You Gotta Shot 'em In The Head", QUEST }
#define QUEST_YGSEITH_ID 801
#define QUEST_ESCAPE { "Escape!", QUEST }
#define QUEST_ESCAPE_ID 802

#define NOTE DATA_CATEGORY4
#define NOTE_FROM_DAD { "Note from Dad", NOTE }
#define NOTE_FROM_DAD_ID 803
#define NOTE_SCHEM_ROCKIT { "Schematic: Rock-It-Launcher", NOTE }
#define NOTE_SCHEM_ROCKIT_ID 804

#define RADIO DATA_CATEGORY5
#define RADIO_ENCLAVE { "Enclave radio", RADIO }
#define RADIO_ENCLAVE_ID 805
#define RADIO_THREEDOG { "Galaxy New radio", RADIO }
#define RADIO_THREEDOG_ID 806

// ******************************************* //

struct BaseItem {
  char* name;
  char* categoryName;
};

BaseItem getItemData(uint16_t itemCode) {
  switch (itemCode) {
    case 0:
    default:
      Serial.print("Got bad item code: "); Serial.println(itemCode);
      return { "", "" };

    // stat page
    case STAT_HEALTH_ID: return STAT_HEALTH;
    case STAT_RADIATION_ID: return STAT_RADIATION;
    case STAT_LEVEL_ID: return STAT_LEVEL;
  
    case CONDITION_RADIATION_ID: return CONDITION_RADIATION;
    case CONDITION_POISONED_ID: return CONDITION_POISONED;
  
    case SPECIAL_STRENGTH_ID: return SPECIAL_STRENGTH;
    case SPECIAL_PERCEPTION_ID: return SPECIAL_PERCEPTION;
    case SPECIAL_ENDURANCE_ID: return SPECIAL_ENDURANCE;
    case SPECIAL_CHARISMA_ID: return SPECIAL_CHARISMA;
    case SPECIAL_INTELLIGENCE_ID: return SPECIAL_INTELLIGENCE;
    case SPECIAL_AGILITY_ID: return SPECIAL_AGILITY;
    case SPECIAL_LUCK_ID: return SPECIAL_LUCK;
  
    case HEALTH_BROKEN_LEG_ID: return HEALTH_BROKEN_LEG;
    case HEALTH_BROKEN_ARM_ID: return HEALTH_BROKEN_ARM;
    
    case STAT_1_BROKEN_LEG_ID: return STAT_1_BROKEN_LEG;
    case STAT_1_BROKEN_ARM_ID: return STAT_1_BROKEN_ARM;

    // inv page
    case WEAPON_PISTOL_32_ID: return WEAPON_PISTOL_32;
    case WEAPON_PISTOL_WildBill_ID: return WEAPON_PISTOL_WildBill;
    case WEAPON_PISTOL_10mm_ID: return WEAPON_PISTOL_10mm;
    case WEAPON_PISTOL_CA10mm_ID: return WEAPON_PISTOL_CA10mm;
    case WEAPON_PISTOL_S10mm_ID: return WEAPON_PISTOL_S10mm;
    case WEAPON_PISTOL_CP_ID: return WEAPON_PISTOL_CP;
    case WEAPON_PISTOL_ZRCP_ID: return WEAPON_PISTOL_ZRCP;
    case WEAPON_PISTOL_Dart_ID: return WEAPON_PISTOL_Dart;
    case WEAPON_PISTOL_44_ID: return WEAPON_PISTOL_44;
    case WEAPON_PISTOL_Blackhawk_ID: return WEAPON_PISTOL_Blackhawk;
    case WEAPON_PISTOL_CMagnum_ID: return WEAPON_PISTOL_CMagnum;
    case WEAPON_PISTOL_PRevolver_ID: return WEAPON_PISTOL_PRevolver;

    case APPAREL_VAULT_SUIT_101_ID: return APPAREL_VAULT_SUIT_101;
    case APPAREL_SHADY_SUNGLASSES_ID: return APPAREL_SHADY_SUNGLASSES;

    case AID_STIMPACK_ID: return AID_STIMPACK;
    case AID_BUFFOUT_ID: return AID_BUFFOUT;
    case AID_JET_ID: return AID_JET;

    case AMMO_10mm_ID: return AMMO_10mm;
    case AMMO_FATMAN_ID: return AMMO_FATMAN;
    case AMMO_DART_ID: return AMMO_DART;

    case MISC_ITEM_TED_BEAR_ID: return MISC_ITEM_TED_BEAR;
    case MISC_ITEM_DINNER_PLATE_ID: return MISC_ITEM_DINNER_PLATE;
    case MISC_ITEM_DESK_LAMP_ID: return MISC_ITEM_DESK_LAMP;

    // data page
    case QUEST_FIND_DAD_ID: return QUEST_FIND_DAD;
    case QUEST_YGSEITH_ID: return QUEST_YGSEITH;
    case QUEST_ESCAPE_ID: return QUEST_ESCAPE;

    case NOTE_FROM_DAD_ID: return NOTE_FROM_DAD;
    case NOTE_SCHEM_ROCKIT_ID: return NOTE_SCHEM_ROCKIT;

    case RADIO_ENCLAVE_ID: return RADIO_ENCLAVE;
    case RADIO_THREEDOG_ID: return RADIO_THREEDOG;
  }
}

#endif
