#ifndef CONTAINER_H
#define CONTAINER_H

#define TYPECLASS
#include "GameFactory.h"

#include <map>
#include <list>
#include <vector>
#include <algorithm>
#include <cstdlib>

struct Diff
{
	signed int count;
	double condition;
	signed int equipped;
	bool silent;
	bool stick;

	Diff()
	{
		count = 0;
		condition = 0.00;
		equipped = 0;
		silent = false;
		stick = false;
	}
};

typedef pair<list<NetworkID>, list<NetworkID> > ContainerDiff;
typedef list<pair<unsigned int, Diff> > GameDiff;
typedef pair<NetworkID, map<NetworkID, list<NetworkID> > > StripCopy;

#include "vaultmp.h"
#include "Data.h"
#include "Item.h"
#include "Object.h"
#include "PacketFactory.h"

#ifdef VAULTMP_DEBUG
#include "Debug.h"
#endif

using namespace Data;
using namespace std;

class Container : public Object
{
		friend class GameFactory;
		friend class Item;

	private:
		static Database Fallout3Items;
		static Database FalloutNVItems;

		static Database* Items;

#ifdef VAULTMP_DEBUG
		static Debug* debug;
#endif

		static bool Item_sort(NetworkID id, NetworkID id2);
		static bool Diff_sort(pair<unsigned int, Diff> diff, pair<unsigned int, Diff> diff2);

		list<NetworkID> container;
		Value<bool> flag_Lock;

		StripCopy Strip() const;

		void initialize();

		Container(const Container&);
		Container& operator=(const Container&);

	protected:
		Container(unsigned int refID, unsigned int baseID);
		Container(const pDefault* packet);
		Container(pDefault* packet);
		virtual ~Container();

	public:

#ifdef VAULTMP_DEBUG
		static void SetDebugHandler(Debug* debug);
#endif

		void AddItem(NetworkID id);
		ContainerDiff AddItem(unsigned int baseID, unsigned int count, double condition, bool silent) const;
		void RemoveItem(NetworkID id);
		ContainerDiff RemoveItem(unsigned int baseID, unsigned int count, bool silent) const;
		ContainerDiff RemoveAllItems() const;
		ContainerDiff EquipItem(unsigned int baseID, bool silent, bool stick) const;
		ContainerDiff UnequipItem(unsigned int baseID, bool silent, bool stick) const;

		ContainerDiff Compare(NetworkID id) const;
		NetworkID IsEquipped(unsigned int baseID) const;
		GameDiff ApplyDiff(ContainerDiff& diff);
		static void FreeDiff(ContainerDiff& diff);

		Lockable* getLock();
		bool IsEmpty() const;
		void PrintContainer() const;
		unsigned int GetItemCount(unsigned int baseID) const;
		const list<NetworkID>& GetItemList() const;

		void FlushContainer();
		NetworkID Copy() const;

		/**
		 * \brief For network transfer
		 */
		virtual pDefault* toPacket();
};

#endif
