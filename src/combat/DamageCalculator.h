#ifndef FS_DAMAGE_CALCULATOR_H
#define FS_DAMAGE_CALCULATOR_H

#include "../combat.h"
#include "../game.h"

class DamageCalculator {
public:
    explicit DamageCalculator(Game& game);

    // Prepare critical hit values and set damage.critical when triggered
    void applyCritical(Player* casterPlayer, CombatDamage& damage, int32_t& primaryBonus, int32_t& secondaryBonus) const;

    bool applyDamage(Creature* caster, Creature* target, CombatDamage& damage, const CombatParams& params,
                     int32_t primaryBonus, int32_t secondaryBonus) const;

private:
    Game& game;
};

#endif // FS_DAMAGE_CALCULATOR_H
