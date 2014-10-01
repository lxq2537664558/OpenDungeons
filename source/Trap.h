/*
 *  Copyright (C) 2011-2014  OpenDungeons Team
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TRAP_H
#define TRAP_H

#include "ODPacket.h"

#include <string>
#include <vector>
#include <istream>
#include <ostream>
#include <map>

class GameMap;
class Player;
class Seat;
class Tile;
class RoomObject;

#include "Building.h"

/*! \class Trap Trap.h
 *  \brief Defines a trap
 */
class Trap : public Building
{
 /* TODO: Trap and room share a lot of things, so we might want to make a shared
 *  base-class, like "Building" or something.
 */
public:
    enum TrapType
    {
        nullTrapType = 0,
        cannon,
        spike,
        boulder
    };

    Trap(GameMap* gameMap);
    virtual ~Trap()
    {}

    virtual std::string getOgreNamePrefix() { return "Trap_"; }

    /*! \brief Creates a type specific subclass of trap and returns a pointer to it.
     * This function also sets up some of the room's properties. If nameToUse is empty,
     * a new unique name will be generated. If not, the given one will be used
     */
    //TODO: Use something better than a void pointer for params.
    static Trap* createTrap(GameMap* gameMap, TrapType nType, const std::vector<Tile*> &nCoveredTiles,
        Seat *seat, bool forceName = false, const std::string& name = "", void* params = NULL);

    /** \brief Adds the trap newTrap to the game map
     */
    static void setupTrap(GameMap* gameMap, Trap* newTrap);

    static Trap* createTrapFromStream(GameMap* gameMap, const std::string& trapMeshName, std::istream &is,
        const std::string& trapName);
    static Trap* createTrapFromPacket(GameMap* gameMap, const std::string& trapMeshName, ODPacket &is,
        const std::string& trapName);

    inline const TrapType& getType() const
    { return mType; }

    static const char* getTrapNameFromTrapType(TrapType t);
    static const char* getMeshNameFromTrapType(TrapType t);
    static TrapType getTrapTypeFromMeshName(std::string s);

    static int costPerTile(TrapType t);

    // Functions which can be overridden by child classes.
    virtual void doUpkeep();

    std::string getNameTile(Tile* tile);

    virtual bool shoot(Tile* tile)
    {
        return true;
    }

    virtual void addCoveredTile(Tile* t, double nHP = mDefaultTileHP);
    virtual void removeCoveredTile(Tile* t);
    virtual Tile* getCoveredTile(int index);
    std::vector<Tile*> getCoveredTiles();
    virtual unsigned int numCoveredTiles();
    virtual void clearCoveredTiles();

    static std::string getFormat();
    friend std::istream& operator>>(std::istream& is, Trap *t);
    friend std::ostream& operator<<(std::ostream& os, Trap *t);
    friend ODPacket& operator>>(ODPacket& is, Trap *t);
    friend ODPacket& operator<<(ODPacket& os, Trap *t);

    // Methods inherited from AttackableObject.
    double getHP(Tile *tile) const;
    double getDefense() const;
    void takeDamage(GameEntity* attacker, double damage, Tile *tileTakingDamage);
    void receiveExp(double experience);
    virtual bool shouldDisplayMeshOnGround()
    {
        return true;
    }
    // TODO : this should be shared between room and trap (probably in building ?)
    void addRoomObject(Tile* targetTile, RoomObject* roomObject);
    void removeRoomObject(Tile* tile);
    void removeRoomObject(RoomObject* roomObject);
    void removeAllRoomObject();
    RoomObject* getRoomObjectFromTile(Tile* tile);
    RoomObject* loadRoomObject(GameMap* gameMap, const std::string& meshName,
        Tile* targetTile, double rotationAngle);
    RoomObject* loadRoomObject(GameMap* gameMap, const std::string& meshName,
        Tile* targetTile, double x, double y, double rotationAngle);
    virtual void updateActiveSpots();
    virtual RoomObject* notifyActiveSpotCreated(Tile* tile);
    virtual void notifyActiveSpotRemoved(Tile* tile);

protected:
    virtual void createMeshLocal();
    virtual void destroyMeshLocal();
    virtual void deleteYourselfLocal();
    int mReloadTime;
    double mMinDamage;
    double mMaxDamage;
    const static double mDefaultTileHP;// = 10.0;

    std::map<Tile*, int> mReloadTimeCounters;
    std::map<Tile*, RoomObject*> mRoomObjects;
    std::vector<Tile*> mCoveredTiles;
    std::map<Tile*, double> mTileHP;
    TrapType mType;
};

#endif // TRAP_H
