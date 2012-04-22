#ifndef ACTOR_H
#define ACTOR_H

#define TYPECLASS
#include "GameFactory.h"

#include <string>
#include <list>
#include <cmath>
#include <typeinfo>
#include <cstdlib>

#include "Value.h"
#include "Container.h"
#include "VaultException.h"
#include "VaultFunctor.h"
#include "Utils.h"
#include "API.h"
#include "PacketFactory.h"

const unsigned int FLAG_NOTSELF         = FLAG_DISABLED << 1;
const unsigned int FLAG_ALIVE           = FLAG_NOTSELF << 1;
const unsigned int FLAG_DEAD            = FLAG_ALIVE << 1;
const unsigned int FLAG_ISALERTED       = FLAG_DEAD << 1;
const unsigned int FLAG_NOTALERTED      = FLAG_ISALERTED << 1;

using namespace std;

/**
 * \brief Derives from Container class and represents an actor in-game
 *
 * Data specific to Actors are actor values, animations and various states
 */

class Actor : public Container
{
		friend class GameFactory;

	private:
		static Database Fallout3Actors;
		static Database FalloutNVActors;

		static Database Fallout3Creatures;
		static Database FalloutNVCreatures;

		static Database* Actors;
		static Database* Creatures;

		static Parameter param_ActorValues;

#ifdef VAULTMP_DEBUG
		static Debug* debug;
#endif

		Database::const_iterator data;

		unordered_map<unsigned char, Value<double> > actor_Values;
		unordered_map<unsigned char, Value<double> > actor_BaseValues;
		Value<unsigned char> anim_Moving;
		Value<unsigned char> state_MovingXY;
		Value<bool> state_Alerted;
		Value<bool> state_Sneaking;
		Value<bool> state_Dead;

        void initialize();

		Actor( const Actor& );
		Actor& operator=( const Actor& );

	protected:
		Actor( unsigned int refID, unsigned int baseID );
		Actor(const pDefault* packet);
		Actor(pDefault* packet);
		virtual ~Actor();

	public:

		/**
		 * \brief Retrieves a reference to a constant Parameter containing every available actor value string representation
		 *
		 * Used to pass actor values to the Interface
		 */
		static const Parameter& Param_ActorValues();

		/**
		 * \brief Creates a Parameter containing a VaultFunctor initialized with the given flags
		 *
		 * Used to pass Actor references matching the provided flags to the Interface
		 */
		static const Parameter CreateFunctor( unsigned int flags );

#ifdef VAULTMP_DEBUG
		static void SetDebugHandler( Debug* debug );
#endif

		/**
		 * \brief Retrieves the Actor's actor value specified by index (actor value hex code)
		 */
		double GetActorValue( unsigned char index ) const;
		/**
		 * \brief Retrieves the Actor's base actor value specified by index (actor value hex code)
		 */
		double GetActorBaseValue( unsigned char index ) const;
		/**
		 * \brief Retrieves the Actor's moving animation
		 */
		unsigned char GetActorMovingAnimation() const;
		/**
		 * \brief Retrieves the Actor's moving state
		 *
		 * 0x00 - the actor moves Forward / Backward
		 * 0x01 - the actor moves ForwardLeft / BackwardRight
		 * 0x02 - the actor moves ForwardRight / BackwardLeft
		 */
		unsigned char GetActorMovingXY() const;
		/**
		 * \brief Retrieves the Actor's alerted state
		 */
		bool GetActorAlerted() const;
		/**
		 * \brief Retrieves the Actor's sneaking state
		 */
		bool GetActorSneaking() const;
		/**
		 * \brief Retrieves the Actor's dead state
		 */
		bool GetActorDead() const;

		/**
		 * \brief Sets the Actor's actor value specified by index (actor value hex code)
		 */
		Lockable* SetActorValue( unsigned char index, double value );
		/**
		 * \brief Sets the Actor's base actor value specified by index (actor value hex code)
		 */
		Lockable* SetActorBaseValue( unsigned char index, double value );
		/**
		 * \brief Sets the Actor's moving animation
		 */
		Lockable* SetActorMovingAnimation( unsigned char index );
		/**
		 * \brief Sets the Actor's moving state
		 *
		 * 0x00 - the actor moves Forward / Backward
		 * 0x01 - the actor moves ForwardLeft / BackwardRight
		 * 0x02 - the actor moves ForwardRight / BackwardLeft
		 */
		Lockable* SetActorMovingXY( unsigned char moving );
		/**
		 * \brief Sets the Actor's alerted state
		 */
		Lockable* SetActorAlerted( bool state );
		/**
		 * \brief Sets the Actor's sneaking state
		 */
		Lockable* SetActorSneaking( bool state );
		/**
		 * \brief Sets the Actor's dead state
		 */
		Lockable* SetActorDead( bool state );

		/**
		 * \brief Returns true if the Actor is jumping
		 */
		bool IsActorJumping() const;

		/**
		 * \brief For network transfer
		 */
        virtual pDefault* toPacket();
};

class ActorFunctor : public VaultFunctor
{
	public:
		ActorFunctor( unsigned int flags ) : VaultFunctor( flags ) {};
		virtual ~ActorFunctor();

		virtual vector<string> operator()();
};

#endif
