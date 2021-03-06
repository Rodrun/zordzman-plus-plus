#pragma once

#include "Entity.hpp"
#include "Mob.hpp"
#include "weapons/weapon.hpp"
#include "weapons/weaponList.hpp"

namespace client {
class Player : public mob::Mob {
public:
    /// Initialize the player with x, y, and speed.
    ///
    /// @param x X position
    /// @param y Y Position
    /// @speed The speed, by default 1.0f.
    Player(std::string username, float x, float y, float speed = 1.0f);
    /// Render the player.
    void render() const override;
    /// Update logic and variables, like position n shit
    void tick() override;
    /// Clone the player
    Player * clone() const override;
    /// Return the player's m_username.
    std::string getUsername() const;

    /// Get the combat weapon the player has equipped.
    ///
    /// It returns a pointer to a const BaseWeapon.
    weapon::BaseWeapon * getCombatWeapon();

    /// Change the combat weapon the player has.
    void setCombatWeapon(weapon::BaseWeapon * b);

    /// Return if the player is holding their combat weapon.
    bool holdingCombatWeapon();

    /// Get the special weapon the player has equipped.
    ///
    /// It returns a pointer to a const BaseWeapon.
    weapon::BaseWeapon * getSpecialWeapon();

    /// Change the special weapon the player has.
    void setSpecialWeapon(weapon::BaseWeapon * b);

    /// Return whether the player is holding their special weapon.
    bool holdingSpecialWeapon();

    /// Get the current weapon the player is holding.
    ///
    /// Returns pointer to a const BaseWeapon.
    weapon::BaseWeapon * getCurrentWeapon();

    /// Change whether the player is holding their combat or special.
    void setCurrentWeapon(weapon::WeaponSlot slot);

    // The delay between using a weapon
    int weapon_delay = 0;

private:
    /// Check for input form the keyboard.
    void input();
    /// Move the player up.
    void moveUp();
    /// Move the player down.
    void moveDown();
    /// Move the player left.
    void moveLeft();
    /// Move the player right.
    void moveRight();

    std::string m_username = "Player";
    // How many "pixels" the player has walked.
    float m_distanceWalked = 0;

    weapon::BaseWeapon * m_combat_weapon = weaponList::BlankWeapon;
    weapon::BaseWeapon * m_special_weapon = weaponList::BlankWeapon;
    char m_current_weapon = 0;
};
} // namespace client
