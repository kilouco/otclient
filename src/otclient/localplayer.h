/*
 * Copyright (c) 2010-2012 OTClient <https://github.com/edubart/otclient>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef LOCALPLAYER_H
#define LOCALPLAYER_H

#include "player.h"

// @bindclass
class LocalPlayer : public Player
{
    enum {
        PREWALK_TIMEOUT = 1000
    };

public:
    LocalPlayer();

    void unlockWalk() { m_walkLockExpiration = 0; }
    void lockWalk(int millis = 250);
    bool canWalk(Otc::Direction direction);

    void setStates(int states);
    void setSkill(Otc::Skill skill, int level, int levelPercent);
    void setHealth(double health, double maxHealth);
    void setFreeCapacity(double freeCapacity);
    void setExperience(double experience);
    void setLevel(double level, double levelPercent);
    void setMana(double mana, double maxMana);
    void setMagicLevel(double magicLevel, double magicLevelPercent);
    void setSoul(double soul);
    void setStamina(double stamina);
    void setKnown(bool known) { m_known = known; }
    void setInventoryItem(Otc::InventorySlot inventory, const ItemPtr& item);
    void setVocation(int vocation);
    void setPremium(bool premium);

    int getStates() { return m_states; }
    int getSkillLevel(Otc::Skill skill) { return m_skillsLevel[skill]; }
    int getSkillLevelPercent(Otc::Skill skill) { return m_skillsLevelPercent[skill]; }
    int getVocation() { return m_vocation; }
    double getWalkPing();
    double getHealth() { return m_health; }
    double getMaxHealth() { return m_maxHealth; }
    double getFreeCapacity() { return m_freeCapacity; }
    double getExperience() { return m_experience; }
    double getLevel() { return m_level; }
    double getLevelPercent() { return m_levelPercent; }
    double getMana() { return m_mana; }
    double getMaxMana() { return m_maxMana; }
    double getMagicLevel() { return m_magicLevel; }
    double getMagicLevelPercent() { return m_magicLevelPercent; }
    double getSoul() { return m_soul; }
    double getStamina() { return m_stamina; }
    ItemPtr getInventoryItem(Otc::InventorySlot inventory) { return m_inventoryItems[inventory]; }

    bool hasSight(const Position& pos);
    bool isKnown() { return m_known; }
    bool isPreWalking() { return m_preWalking; }
    bool isAutoWalking() { return m_autoWalking; }
    bool isPremium() { return m_premium; }

    LocalPlayerPtr asLocalPlayer() { return static_self_cast<LocalPlayer>(); }
    bool isLocalPlayer() { return true; }

    virtual void onAppear();

protected:
    void walk(const Position& oldPos, const Position& newPos);
    void preWalk(Otc::Direction direction);
    void cancelWalk(Otc::Direction direction = Otc::InvalidDirection);
    void stopWalk();

    friend class Game;

protected:
    void updateWalkOffset(int totalPixelsWalked);
    void updateWalk();
    void terminateWalk();

private:
    // walk related
    bool m_preWalking;
    bool m_lastPrewalkDone;
    bool m_autoWalking;
    bool m_premium;
    Position m_lastPrewalkDestionation;
    ItemPtr m_inventoryItems[Otc::LastInventorySlot];
    ScheduledEventPtr m_autoWalkEndEvent;
    stdext::boolean<false> m_waitingWalkPong;
    Timer m_walkPingTimer;
    std::deque<int> m_lastWalkPings;

    std::array<int, Otc::LastSkill> m_skillsLevel;
    std::array<int, Otc::LastSkill> m_skillsLevelPercent;

    bool m_known;
    int m_states;
    int m_vocation;
    ticks_t m_walkLockExpiration;

    double m_health;
    double m_maxHealth;
    double m_freeCapacity;
    double m_experience;
    double m_level;
    double m_levelPercent;
    double m_mana;
    double m_maxMana;
    double m_magicLevel;
    double m_magicLevelPercent;
    double m_soul;
    double m_stamina;
};

#endif
