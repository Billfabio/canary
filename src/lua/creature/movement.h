/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2019  Mark Samman <mark.samman@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef SRC_LUA_CREATURE_MOVEMENT_H_
#define SRC_LUA_CREATURE_MOVEMENT_H_

#include "declarations.hpp"
#include "items/item.h"
#include "lua/functions/events/move_event_functions.hpp"
#include "lua/scripts/scripts.h"
#include "creatures/players/vocations/vocation.h"


class MoveEvent;
using MoveEvent_ptr = std::unique_ptr<MoveEvent>;

struct MoveEventList {
	std::list<MoveEvent> moveEvent[MOVE_EVENT_LAST];
};

using VocEquipMap = std::map<uint16_t, bool>;

class MoveEvents final : public Scripts {
	public:
		MoveEvents() = default;

		// non-copyable
		MoveEvents(const MoveEvents&) = delete;
		MoveEvents& operator=(const MoveEvents&) = delete;

		static MoveEvents& getInstance() {
			// Guaranteed to be destroyed
			static MoveEvents instance;
			// Instantiated on first use
			return instance;
		}

		uint32_t onCreatureMove(Creature* creature, const Tile* tile, MoveEvent_t eventType);
		uint32_t onPlayerEquip(Player* player, Item* item, Slots_t slot, bool isCheck);
		uint32_t onPlayerDeEquip(Player* player, Item* item, Slots_t slot);
		uint32_t onItemMove(Item* item, Tile* tile, bool isAdd);

		MoveEvent* getEvent(Item* item, MoveEvent_t eventType);

		bool isRegistered(uint32_t itemid);

		bool registerLuaEvent(MoveEvent* event);
		bool registerLuaFunction(MoveEvent* event);
		void clear();

	private:
		void clearMap(std::map<int32_t, MoveEventList>& map) const;

		void addEvent(MoveEvent moveEvent, int32_t id, std::map<int32_t, MoveEventList>& map);

		void addEvent(MoveEvent moveEvent, const Position& pos, std::map<Position, MoveEventList>& map);
		MoveEvent* getEvent(const Tile* tile, MoveEvent_t eventType);

		MoveEvent* getEvent(Item* item, MoveEvent_t eventType, Slots_t slot);

		std::map<int32_t, MoveEventList> uniqueIdMap;
		std::map<int32_t, MoveEventList> actionIdMap;
		std::map<int32_t, MoveEventList> itemIdMap;
		std::map<Position, MoveEventList> positionMap;
};

constexpr auto g_moveEvents = &MoveEvents::getInstance;

class MoveEvent final : public Script {
	public:
		explicit MoveEvent(LuaScriptInterface* interface);

		MoveEvent_t getEventType() const;
		void setEventType(MoveEvent_t type);

		uint32_t fireStepEvent(Creature* creature, Item* item, const Position& pos);
		uint32_t fireAddRemItem(Item* item, Item* tileItem, const Position& pos);
		uint32_t fireEquip(Player* player, Item* item, Slots_t slot, bool isCheck);

		uint32_t getSlot() const {
			return slot;
		}

		//scripting
		bool executeStep(Creature* creature, Item* item, const Position& pos);
		bool executeEquip(Player* player, Item* item, Slots_t slot, bool isCheck);
		bool executeAddRemItem(Item* item, Item* tileItem, const Position& pos);
		//

		//onEquip information
		uint32_t getReqLevel() const {
			return reqLevel;
		}
		uint32_t getReqMagLv() const {
			return reqMagLevel;
		}
		bool isPremium() const {
			return premium;
		}
		const std::string& getVocationString() const {
			return vocationString;
		}
		void setVocationString(const std::string& str) {
			vocationString = str;
		}
		uint32_t getWieldInfo() const {
			return wieldInfo;
		}
		const VocEquipMap& getVocEquipMap() const {
			return vocEquipMap;
		}
		void addVocEquipMap(std::string vocName) {
			int32_t vocationId = g_vocations().getVocationId(vocName);
			if (vocationId != -1) {
				vocEquipMap[vocationId] = true;
			}
		}
		bool getTileItem() const {
			return tileItem;
		}
		void setTileItem(bool b) {
			tileItem = b;
		}
		std::vector<uint32_t> getItemIdRange() {
			return itemIdRange;
		}
		void addItemId(uint32_t id) {
			itemIdRange.emplace_back(id);
		}
		std::vector<uint32_t> getActionIdRange() {
			return actionIdRange;
		}
		void addActionId(uint32_t id) {
			actionIdRange.emplace_back(id);
		}
		std::vector<uint32_t> getUniqueIdRange() {
			return uniqueIdRange;
		}
		void addUniqueId(uint32_t id) {
			uniqueIdRange.emplace_back(id);
		}
		std::vector<Position> getPosList() {
			return posList;
		}
		void addPosList(Position pos) {
			posList.emplace_back(pos);
		}
		const std::string& getFileName() const {
			return fileName;
		}
		void setFileName(const std::string& scriptName) {
			fileName = scriptName;
		}
		void setSlot(uint32_t s) {
			slot = s;
		}
		uint32_t getRequiredLevel() {
			return reqLevel;
		}
		void setRequiredLevel(uint32_t level) {
			reqLevel = level;
		}
		uint32_t getRequiredMagLevel() {
			return reqMagLevel;
		}
		void setRequiredMagLevel(uint32_t level) {
			reqMagLevel = level;
		}
		bool needPremium() {
			return premium;
		}
		void setNeedPremium(bool b) {
			premium = b;
		}
		uint32_t getWieldInfo() {
			return wieldInfo;
		}
		void setWieldInfo(WieldInfo_t info) {
			wieldInfo |= info;
		}

		static uint32_t StepInField(Creature* creature, Item* item, const Position& pos);
		static uint32_t StepOutField(Creature* creature, Item* item, const Position& pos);

		static uint32_t AddItemField(Item* item, Item* tileItem, const Position& pos);
		static uint32_t RemoveItemField(Item* item, Item* tileItem, const Position& pos);

		static uint32_t EquipItem(MoveEvent* moveEvent, Player* player, Item* item, Slots_t slot, bool boolean);
		static uint32_t DeEquipItem(MoveEvent* moveEvent, Player* player, Item* item, Slots_t slot, bool boolean);

	private:
		std::string getScriptTypeName() const override;

		uint32_t slot = SLOTP_WHEREEVER;

		MoveEvent_t eventType = MOVE_EVENT_NONE;
		/// Step function
		std::function<uint32_t(
			Creature* creature,
			Item* item,
			const Position& pos
		)> stepFunction;
		// Move function
		std::function<uint32_t(
			Item* item,
			Item* tileItem,
			const Position& pos
		)> moveFunction;
		// equipFunction
		std::function<uint32_t(
			MoveEvent* moveEvent,
			Player* player,
			Item* item,
			Slots_t slot,
			bool boolean
		)> equipFunction;

		//onEquip information
		uint32_t reqLevel = 0;
		uint32_t reqMagLevel = 0;
		bool premium = false;
		std::string vocationString;
		uint32_t wieldInfo = 0;
		VocEquipMap vocEquipMap;
		bool tileItem = false;

		std::vector<uint32_t> itemIdRange;
		std::vector<uint32_t> actionIdRange;
		std::vector<uint32_t> uniqueIdRange;
		std::vector<Position> posList;

		std::string fileName;

		friend class MoveEventFunctions;
};

#endif  // SRC_LUA_CREATURE_MOVEMENT_H_
