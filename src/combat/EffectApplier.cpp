#include "../otpch.h"
#include "EffectApplier.h"
#include "../player.h"

EffectApplier::EffectApplier(Game& game) : game(game) {}

void EffectApplier::applyAfterHit(Creature* caster, Creature* target, const CombatDamage& damage,
                                  const CombatParams& params, Player* casterPlayer,
                                  int32_t totalDamage, size_t targetsCount) const
{
    if (damage.blockType == BLOCK_NONE || damage.blockType == BLOCK_ARMOR) {
        for (const auto& condition : params.conditionList) {
            if (caster == target || !target->isImmune(condition->getType())) {
                Condition* conditionCopy = condition->clone();
                if (caster) {
                    conditionCopy->setParam(CONDITION_PARAM_OWNER, caster->getID());
                }
                target->addCombatCondition(conditionCopy);
            }
        }
    }

    if (casterPlayer && !damage.leeched && damage.primary.type != COMBAT_HEALING && damage.origin != ORIGIN_CONDITION) {
        if (casterPlayer->getHealth() < casterPlayer->getMaxHealth()) {
            uint16_t chance = casterPlayer->getSpecialSkill(SPECIALSKILL_LIFELEECHCHANCE);
            uint16_t skill = casterPlayer->getSpecialSkill(SPECIALSKILL_LIFELEECHAMOUNT);
            if (chance > 0 && skill > 0 && normal_random(1, 100) <= chance) {
                CombatDamage leechCombat;
                leechCombat.origin = ORIGIN_NONE;
                leechCombat.leeched = true;
                leechCombat.primary.value = std::ceil(totalDamage * ((skill / 10000.) + ((targetsCount - 1) * ((skill / 10000.) / 10.))) / static_cast<double>(targetsCount));
                game.combatChangeHealth(nullptr, casterPlayer, leechCombat);
                casterPlayer->sendMagicEffect(casterPlayer->getPosition(), CONST_ME_MAGIC_RED);
            }
        }

        if (casterPlayer->getMana() < casterPlayer->getMaxMana()) {
            uint16_t chance = casterPlayer->getSpecialSkill(SPECIALSKILL_MANALEECHCHANCE);
            uint16_t skill = casterPlayer->getSpecialSkill(SPECIALSKILL_MANALEECHAMOUNT);
            if (chance > 0 && skill > 0 && normal_random(1, 100) <= chance) {
                CombatDamage leechCombat;
                leechCombat.origin = ORIGIN_NONE;
                leechCombat.leeched = true;
                leechCombat.primary.value = std::ceil(totalDamage * ((skill / 10000.) + ((targetsCount - 1) * ((skill / 10000.) / 10.))) / static_cast<double>(targetsCount));
                game.combatChangeMana(nullptr, casterPlayer, leechCombat);
                casterPlayer->sendMagicEffect(casterPlayer->getPosition(), CONST_ME_MAGIC_BLUE);
            }
        }
    }

    if (params.dispelType == CONDITION_PARALYZE) {
        target->removeCondition(CONDITION_PARALYZE);
    } else {
        target->removeCombatCondition(params.dispelType);
    }

    if (params.targetCallback) {
        params.targetCallback->onTargetCombat(caster, target);
    }
}
