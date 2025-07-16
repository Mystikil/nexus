#include "../otpch.h"
#include "DamageCalculator.h"
#include "../player.h"

DamageCalculator::DamageCalculator(Game& game) : game(game) {}

void DamageCalculator::applyCritical(Player* casterPlayer, CombatDamage& damage, int32_t& primaryBonus, int32_t& secondaryBonus) const
{
    primaryBonus = 0;
    secondaryBonus = 0;
    if (!damage.critical && damage.primary.type != COMBAT_HEALING && casterPlayer && damage.origin != ORIGIN_CONDITION) {
        uint16_t chance = casterPlayer->getSpecialSkill(SPECIALSKILL_CRITICALHITCHANCE);
        uint16_t skill = casterPlayer->getSpecialSkill(SPECIALSKILL_CRITICALHITAMOUNT);
        if (chance > 0 && skill > 0 && uniform_random(1, 100) <= chance) {
            primaryBonus = std::round(damage.primary.value * (skill / 10000.));
            secondaryBonus = std::round(damage.secondary.value * (skill / 10000.));
            damage.critical = true;
        }
    }
}

bool DamageCalculator::applyDamage(Creature* caster, Creature* target, CombatDamage& damage, const CombatParams& params,
                                   int32_t primaryBonus, int32_t secondaryBonus) const
{
    Player* casterPlayer = caster ? caster->getPlayer() : nullptr;
    bool playerCombatReduced = false;

    if ((damage.primary.value < 0 || damage.secondary.value < 0) && caster) {
        Player* targetPlayer = target->getPlayer();
        if (casterPlayer && targetPlayer && casterPlayer != targetPlayer && targetPlayer->getSkull() != SKULL_BLACK) {
            damage.primary.value /= 2;
            damage.secondary.value /= 2;
            playerCombatReduced = true;
        }
    }

    if (damage.critical) {
        damage.primary.value += playerCombatReduced ? primaryBonus / 2 : primaryBonus;
        damage.secondary.value += playerCombatReduced ? secondaryBonus / 2 : secondaryBonus;
        game.addMagicEffect(target->getPosition(), CONST_ME_CRITICAL_DAMAGE);
    }

    bool success = false;
    if (damage.primary.type != COMBAT_MANADRAIN) {
        if (game.combatBlockHit(damage, caster, target, params.blockedByShield, params.blockedByArmor, params.itemId != 0, params.ignoreResistances)) {
            return false;
        }
        success = game.combatChangeHealth(caster, target, damage);
    } else {
        success = game.combatChangeMana(caster, target, damage);
    }
    return success;
}
