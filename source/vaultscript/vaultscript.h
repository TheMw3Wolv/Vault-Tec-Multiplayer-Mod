#ifndef VAULTSCRIPT_H
#define VAULTSCRIPT_H

/*
 *  vaultscript.h
 *  Don't change anything here
 */

#ifdef __cplusplus
	#include <string>
	#include <vector>
	#include <unordered_set>
	#include <unordered_map>
	#include <cstdint>
	#define VAULTFUNCTION inline static
	#define VAULTAPI(name) _##name
	#define VAULTAPI_PREFIX '_'
	#define VAULTSPACE vaultmp::
	#define _CPP(expr) expr
	#define _C(expr)
#else
	#include <stdint.h>
	#define VAULTAPI(name) name
	#define VAULTAPI_PREFIX '\0'
	#define VAULTSPACE
	#define _CPP(expr)
	#define _C(expr) expr
#endif

#ifndef __WIN32__
	#ifndef __cdecl
		#define __cdecl __attribute__((__cdecl__))
	#endif
	#define VAULTVAR __attribute__ ((__visibility__("default")))
	#define VAULTSCRIPT VAULTVAR __cdecl
#else
	#define VAULTVAR __declspec(dllexport)
	#define VAULTSCRIPT VAULTVAR __cdecl
#endif

_CPP(
namespace vaultmp {
)
	typedef void Void;
	typedef char* RawString;
	typedef char RawChar;
	typedef const char* cRawString;
	typedef const char cRawChar;
	typedef int32_t Count;
	typedef uint32_t UCount;
	typedef double Value;
#ifndef __cplusplus
	typedef uint8_t Reason;
	typedef uint8_t Index;
	typedef uint8_t Type;
	typedef uint8_t State;
	typedef uint32_t Reference;
	typedef uint32_t Base;
	typedef uint32_t Cell;
	typedef uint32_t Interval;
	typedef uint64_t ID;
	typedef uint64_t Timer;
	typedef uint64_t Result;

	#define RawFunction(types)  Result (__cdecl*)(types)
	#define RawArray(type)		type*

	#define FALLOUT3			(0x01)
	#define NEWVEGAS			(FALLOUT3 << 1)
	#define FALLOUT_GAMES		(FALLOUT3 | NEWVEGAS)
	#define ALL_GAMES			(FALLOUT_GAMES)

	#define MAX_PLAYER_NAME		(16)
	#define MAX_PASSWORD_SIZE	(16)
	#define MAX_MESSAGE_LENGTH	(64)
	#define MAX_CHAT_LENGTH		(128)

	#define ID_REFERENCE		(0x01)
	#define ID_OBJECT			(ID_REFERENCE << 1)
	#define ID_ITEM				(ID_OBJECT << 1)
	#define ID_CONTAINER		(ID_ITEM << 1)
	#define ID_ACTOR			(ID_CONTAINER << 1)
	#define ID_PLAYER			(ID_ACTOR << 1)

	#define ALL_OBJECTS			(ID_OBJECT | ID_ITEM | ID_CONTAINER | ID_ACTOR | ID_PLAYER)
	#define ALL_CONTAINERS		(ID_CONTAINER | ID_ACTOR | ID_PLAYER)
	#define ALL_ACTORS			(ID_ACTOR | ID_PLAYER)
#else
	enum Reason : uint8_t;

	enum State : bool
	{
		True    =   true,
		False   =   false
	};

	enum Ref : uint32_t;
	enum Base : uint32_t;
	enum Cell : uint32_t;

	enum Interval : uint32_t
	{
		DEFAULT_PLAYER_RESPAWN  =   8000,
	};

	enum ID : uint64_t;
	enum Timer : uint64_t;
	enum Result : uint64_t;

	enum class Index : uint8_t
	{
		FALLOUT3            =   0x01,
		NEWVEGAS            =   FALLOUT3 << 1,
		FALLOUT_GAMES       =   FALLOUT3 | NEWVEGAS,
		ALL_GAMES           =   FALLOUT_GAMES,

		MAX_PLAYER_NAME     =   16,
		MAX_PASSWORD_SIZE   =   16,
		MAX_MESSAGE_LENGTH  =   64,
		MAX_CHAT_LENGTH		=	128,
	};

	enum class Type : uint8_t
	{
		ID_REFERENCE        =   0x01,
		ID_OBJECT           =   ID_REFERENCE << 1,
		ID_ITEM             =   ID_OBJECT << 1,
		ID_CONTAINER        =   ID_ITEM << 1,
		ID_ACTOR            =   ID_CONTAINER << 1,
		ID_PLAYER           =   ID_ACTOR << 1,

		ALL_OBJECTS         = 	(ID_OBJECT | ID_ITEM | ID_CONTAINER | ID_ACTOR | ID_PLAYER),
		ALL_CONTAINERS      = 	(ID_CONTAINER | ID_ACTOR | ID_PLAYER),
		ALL_ACTORS          = 	(ID_ACTOR | ID_PLAYER),
	};

	struct _hash_ID { inline size_t operator() (const ID& x) const { return std::hash<uint64_t>()((uint64_t) x); }};

	typedef std::string String;
	typedef std::vector<ID> IDVector;
	typedef std::unordered_set<ID, _hash_ID> IDSet;

	template <typename V>
	using IDHash = std::unordered_map<ID, V, _hash_ID>;

	template <typename T>
	using RawArray = T*;

	#define RawArray(type)		RawArray<type>

	template <typename... Types>
	using Function = Result (__cdecl*)(Types...) noexcept;

	#define RawFunction(types) 	Function<types>
}
#endif

_CPP(extern "C" {)
	VAULTVAR VAULTSPACE RawChar vaultprefix = VAULTAPI_PREFIX;

	VAULTSCRIPT VAULTSPACE Void exec() _CPP(noexcept);

	VAULTSCRIPT VAULTSPACE Void OnSpawn(VAULTSPACE ID) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE Void OnCellChange(VAULTSPACE ID, VAULTSPACE Cell) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE Void OnContainerItemChange(VAULTSPACE ID, VAULTSPACE Base, VAULTSPACE Count, VAULTSPACE Value) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE Void OnActorValueChange(VAULTSPACE ID, VAULTSPACE Index, VAULTSPACE Value) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE Void OnActorBaseValueChange(VAULTSPACE ID, VAULTSPACE Index, VAULTSPACE Value) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE Void OnActorAlert(VAULTSPACE ID, VAULTSPACE State) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE Void OnActorSneak(VAULTSPACE ID, VAULTSPACE State) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE Void OnActorDeath(VAULTSPACE ID) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE Void OnActorEquipItem(VAULTSPACE ID, VAULTSPACE Base, VAULTSPACE Value) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE Void OnActorUnequipItem(VAULTSPACE ID, VAULTSPACE Base, VAULTSPACE Value) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE Void OnPlayerDisconnect(VAULTSPACE ID, VAULTSPACE Reason) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE Base OnPlayerRequestGame(VAULTSPACE ID) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE State OnPlayerChat(VAULTSPACE ID, VAULTSPACE RawString) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE State OnClientAuthenticate(VAULTSPACE cRawString, VAULTSPACE cRawString) _CPP(noexcept);

	VAULTSCRIPT VAULTSPACE Void (*VAULTAPI(timestamp))() _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE Timer (*VAULTAPI(CreateTimer))(VAULTSPACE RawFunction(), VAULTSPACE Interval) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE Timer (*VAULTAPI(CreateTimerEx))(VAULTSPACE RawFunction(), VAULTSPACE Interval, VAULTSPACE cRawString, ...) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE Void (*VAULTAPI(KillTimer))(VAULTSPACE Timer) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE Void (*VAULTAPI(MakePublic))(VAULTSPACE RawFunction(), VAULTSPACE cRawString, VAULTSPACE cRawString) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE Result (*VAULTAPI(CallPublic))(VAULTSPACE cRawString, ...) _CPP(noexcept);

	VAULTSCRIPT VAULTSPACE Void (*VAULTAPI(SetServerName))(VAULTSPACE cRawString) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE Void (*VAULTAPI(SetServerMap))(VAULTSPACE cRawString) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE Void (*VAULTAPI(SetServerRule))(VAULTSPACE cRawString, VAULTSPACE cRawString) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE Index (*VAULTAPI(GetGameCode))() _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE UCount (*VAULTAPI(GetMaximumPlayers))() _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE UCount (*VAULTAPI(GetCurrentPlayers))() _CPP(noexcept);

	VAULTSCRIPT VAULTSPACE cRawString (*VAULTAPI(ValueToString))(VAULTSPACE Index) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE cRawString (*VAULTAPI(AxisToString))(VAULTSPACE Index) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE cRawString (*VAULTAPI(AnimToString))(VAULTSPACE Index) _CPP(noexcept);

	VAULTSCRIPT VAULTSPACE State (*VAULTAPI(UIMessage))(VAULTSPACE ID, VAULTSPACE cRawString) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE State (*VAULTAPI(ChatMessage))(VAULTSPACE ID, VAULTSPACE cRawString) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE Void (*VAULTAPI(SetRespawn))(VAULTSPACE Interval) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE State (*VAULTAPI(IsValid))(VAULTSPACE ID) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE State (*VAULTAPI(IsObject))(VAULTSPACE ID) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE State (*VAULTAPI(IsItem))(VAULTSPACE ID) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE State (*VAULTAPI(IsContainer))(VAULTSPACE ID) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE State (*VAULTAPI(IsActor))(VAULTSPACE ID) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE State (*VAULTAPI(IsPlayer))(VAULTSPACE ID) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE Type (*VAULTAPI(GetType))(VAULTSPACE ID) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE UCount (*VAULTAPI(GetCount))(VAULTSPACE Type) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE UCount (*VAULTAPI(GetList))(VAULTSPACE Type, VAULTSPACE RawArray(VAULTSPACE ID)*) _CPP(noexcept);

	VAULTSCRIPT VAULTSPACE Ref (*VAULTAPI(GetReference))(VAULTSPACE ID) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE Base (*VAULTAPI(GetBase))(VAULTSPACE ID) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE cRawString (*VAULTAPI(GetName))(VAULTSPACE ID) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE Void (*VAULTAPI(GetPos))(VAULTSPACE ID, VAULTSPACE Value*, VAULTSPACE Value*, VAULTSPACE Value*) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE Void (*VAULTAPI(GetAngle))(VAULTSPACE ID, VAULTSPACE Value*, VAULTSPACE Value*, VAULTSPACE Value*) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE Cell (*VAULTAPI(GetCell))(VAULTSPACE ID) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE State (*VAULTAPI(IsNearPoint))(VAULTSPACE ID, VAULTSPACE Value, VAULTSPACE Value, VAULTSPACE Value, VAULTSPACE Value) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE UCount (*VAULTAPI(GetContainerItemCount))(VAULTSPACE ID, VAULTSPACE Base) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE Value (*VAULTAPI(GetActorValue))(VAULTSPACE ID, VAULTSPACE Index) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE Value (*VAULTAPI(GetActorBaseValue))(VAULTSPACE ID, VAULTSPACE Index) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE Index (*VAULTAPI(GetActorMovingAnimation))(VAULTSPACE ID) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE State (*VAULTAPI(GetActorAlerted))(VAULTSPACE ID) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE State (*VAULTAPI(GetActorSneaking))(VAULTSPACE ID) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE State (*VAULTAPI(GetActorDead))(VAULTSPACE ID) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE State (*VAULTAPI(IsActorJumping))(VAULTSPACE ID) _CPP(noexcept);

	VAULTSCRIPT VAULTSPACE State (*VAULTAPI(AddItem))(VAULTSPACE ID, VAULTSPACE Base, VAULTSPACE UCount, VAULTSPACE Value, VAULTSPACE State) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE UCount (*VAULTAPI(RemoveItem))(VAULTSPACE ID, VAULTSPACE Base, VAULTSPACE UCount, VAULTSPACE State) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE Void (*VAULTAPI(RemoveAllItems))(VAULTSPACE ID) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE Void (*VAULTAPI(SetActorValue))(VAULTSPACE ID, VAULTSPACE Index, VAULTSPACE Value) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE Void (*VAULTAPI(SetActorBaseValue))(VAULTSPACE ID, VAULTSPACE Index, VAULTSPACE Value) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE State (*VAULTAPI(EquipItem))(VAULTSPACE ID, VAULTSPACE Base, VAULTSPACE State, VAULTSPACE State) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE State (*VAULTAPI(UnequipItem))(VAULTSPACE ID, VAULTSPACE Base, VAULTSPACE State, VAULTSPACE State) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE Void (*VAULTAPI(KillActor))(VAULTSPACE ID) _CPP(noexcept);
	VAULTSCRIPT VAULTSPACE Void (*VAULTAPI(SetPlayerRespawn))(VAULTSPACE ID, VAULTSPACE Interval) _CPP(noexcept);
_CPP(})

#ifdef __cplusplus
namespace vaultmp
{
	template<typename T, size_t t>
	struct TypeChar { static_assert(!t, "Unsupported type in variadic type list"); };

	template<typename T>
	struct TypeChar<T*, sizeof(void*)> { enum { value = 'p' }; };

	template<typename T>
	struct TypeChar<T, sizeof(uint8_t)> { enum { value = 'i' }; };

	template<typename T>
	struct TypeChar<T, sizeof(uint16_t)> { enum { value = 'i' }; };

	template<typename T>
	struct TypeChar<T, sizeof(uint32_t)> { enum { value = 'i' }; };

	template<typename T>
	struct TypeChar<T, sizeof(uint64_t)> { enum { value = 'l' }; };

	template<>
	struct TypeChar<Value, sizeof(Value)> { enum { value = 'f' }; };

	template<>
	struct TypeChar<cRawString, sizeof(cRawString)> { enum { value = 's' }; };

	template<>
	struct TypeChar<RawString, sizeof(RawString)> { enum { value = 's' }; };

	template<typename... Types>
	struct TypeString {
		static cRawChar value[sizeof...(Types) + 1];
	};

	template<typename... Types>
	cRawChar TypeString<Types...>::value[sizeof...(Types) + 1] = {
		TypeChar<typeof(Types), sizeof(Types)>::value...
	};

	VAULTFUNCTION Void timestamp() noexcept { return VAULTAPI(timestamp)(); }
	VAULTFUNCTION Timer CreateTimer(Function<> function, Interval interval) noexcept { return VAULTAPI(CreateTimer)(function, interval); }

	template<typename... Types>
	VAULTFUNCTION Timer CreateTimer(Function<Types...> function, Interval interval, Types... values) noexcept
	{
		cRawString types = TypeString<Types...>::value;
		return VAULTAPI(CreateTimerEx)(reinterpret_cast<Function<>>(function), interval, types, values...);
	}

	VAULTFUNCTION Void KillTimer(Timer timer = static_cast<Timer>(0)) noexcept { return VAULTAPI(KillTimer)(timer); }

	template<typename... Types>
	VAULTFUNCTION Void MakePublic(Function<Types...> function, String name) noexcept
	{
		cRawString types = TypeString<Types...>::value;
		return VAULTAPI(MakePublic)(reinterpret_cast<Function<>>(function), name.c_str(), types);
	}

	template<typename... Types>
	VAULTFUNCTION Result CallPublic(String name, Types... values) noexcept
	{
		return VAULTAPI(CallPublic)(name.c_str(), values...);
	}

	VAULTFUNCTION Void SetServerName(String name) noexcept { return VAULTAPI(SetServerName)(name.c_str()); }
	VAULTFUNCTION Void SetServerMap(String map) noexcept { return VAULTAPI(SetServerMap)(map.c_str()); }
	VAULTFUNCTION Void SetServerRule(String key, String value) noexcept { return VAULTAPI(SetServerRule)(key.c_str(), value.c_str()); }
	VAULTFUNCTION Index GetGameCode() noexcept { return VAULTAPI(GetGameCode)(); }
	VAULTFUNCTION UCount GetMaximumPlayers() noexcept { return VAULTAPI(GetMaximumPlayers)(); }
	VAULTFUNCTION UCount GetCurrentPlayers() noexcept { return VAULTAPI(GetCurrentPlayers)(); }

	VAULTFUNCTION String ValueToString(Index index) noexcept { return String(VAULTAPI(ValueToString)(index)); }
	VAULTFUNCTION String AxisToString(Index index) noexcept { return String(VAULTAPI(AxisToString)(index)); }
	VAULTFUNCTION String AnimToString(Index index) noexcept { return String(VAULTAPI(AnimToString)(index)); }

	VAULTFUNCTION State UIMessage(ID id, String message) noexcept { return VAULTAPI(UIMessage)(id, message.c_str()); }
	VAULTFUNCTION State UIMessage(String message) noexcept { return VAULTAPI(UIMessage)(static_cast<ID>(0), message.c_str()); }
	VAULTFUNCTION State ChatMessage(ID id, String message) noexcept { return VAULTAPI(ChatMessage)(id, message.c_str()); }
	VAULTFUNCTION State ChatMessage(String message) noexcept { return VAULTAPI(ChatMessage)(static_cast<ID>(0), message.c_str()); }
	VAULTFUNCTION Void SetRespawn(Interval interval) noexcept { return VAULTAPI(SetRespawn)(interval); }
	VAULTFUNCTION State IsValid(ID id) noexcept { return VAULTAPI(IsValid)(id); }
	VAULTFUNCTION State IsObject(ID id) noexcept { return VAULTAPI(IsObject)(id); }
	VAULTFUNCTION State IsItem(ID id) noexcept { return VAULTAPI(IsItem)(id); }
	VAULTFUNCTION State IsContainer(ID id) noexcept { return VAULTAPI(IsContainer)(id); }
	VAULTFUNCTION State IsActor(ID id) noexcept { return VAULTAPI(IsActor)(id); }
	VAULTFUNCTION State IsPlayer(ID id) noexcept { return VAULTAPI(IsPlayer)(id); }
	VAULTFUNCTION Type GetType(ID id) noexcept { return VAULTAPI(GetType)(id); }
	VAULTFUNCTION UCount GetCount(Type type) noexcept { return VAULTAPI(GetCount)(type); }
	VAULTFUNCTION IDVector GetList(Type type) noexcept
	{
		RawArray<ID> data;
		UCount size = VAULTAPI(GetList)(type, &data);
		return IDVector(data, data + size);
	}

	VAULTFUNCTION Ref GetReference(ID id) noexcept { return VAULTAPI(GetReference)(id); }
	VAULTFUNCTION Base GetBase(ID id) noexcept { return VAULTAPI(GetBase)(id); }
	VAULTFUNCTION String GetName(ID id) noexcept { return String(VAULTAPI(GetName)(id)); }
	VAULTFUNCTION Void GetPos(ID id, Value& X, Value& Y, Value& Z) noexcept { return VAULTAPI(GetPos)(id, &X, &Y, &Z); }
	VAULTFUNCTION Void GetAngle(ID id, Value& X, Value& Y, Value& Z) noexcept { return VAULTAPI(GetAngle)(id, &X, &Y, &Z); }
	VAULTFUNCTION Cell GetCell(ID id) noexcept { return VAULTAPI(GetCell)(id); }
	VAULTFUNCTION State IsNearPoint(ID id, Value X, Value Y, Value Z, Value R) noexcept { return VAULTAPI(IsNearPoint)(id, X, Y, Z, R); }
	VAULTFUNCTION UCount GetContainerItemCount(ID id, Base base = static_cast<Base>(0)) noexcept { return VAULTAPI(GetContainerItemCount)(id, base); }
	VAULTFUNCTION Value GetActorValue(ID id, Index index) noexcept { return VAULTAPI(GetActorValue)(id, index); }
	VAULTFUNCTION Value GetActorBaseValue(ID id, Index index) noexcept { return VAULTAPI(GetActorBaseValue)(id, index); }
	VAULTFUNCTION Index GetActorMovingAnimation(ID id) noexcept { return VAULTAPI(GetActorMovingAnimation)(id); }
	VAULTFUNCTION State GetActorAlerted(ID id) noexcept { return VAULTAPI(GetActorAlerted)(id); }
	VAULTFUNCTION State GetActorSneaking(ID id) noexcept { return VAULTAPI(GetActorSneaking)(id); }
	VAULTFUNCTION State GetActorDead(ID id) noexcept { return VAULTAPI(GetActorDead)(id); }
	VAULTFUNCTION State IsActorJumping(ID id) noexcept { return VAULTAPI(IsActorJumping)(id); }

	VAULTFUNCTION State AddItem(ID id, Base base, UCount count = 1, Value condition = 100.0, State silent = True) noexcept { return VAULTAPI(AddItem)(id, base, count, condition, silent); }
	VAULTFUNCTION UCount RemoveItem(ID id, Base base, UCount count = 1, State silent = True) noexcept { return VAULTAPI(RemoveItem)(id, base, count, silent); }
	VAULTFUNCTION Void RemoveAllItems(ID id) noexcept { return VAULTAPI(RemoveAllItems)(id); }
	VAULTFUNCTION Void SetActorValue(ID id, Index index, Value value) noexcept { return VAULTAPI(SetActorValue)(id, index, value); }
	VAULTFUNCTION Void SetActorBaseValue(ID id, Index index, Value value) noexcept { return VAULTAPI(SetActorBaseValue)(id, index, value); }
	VAULTFUNCTION State EquipItem(ID id, Base base, State silent = True, State stick = True) noexcept { return VAULTAPI(EquipItem)(id, base, silent, stick); }
	VAULTFUNCTION State UnequipItem(ID id, Base base, State silent = True, State stick = True) noexcept { return VAULTAPI(UnequipItem)(id, base, silent, stick); }
	VAULTFUNCTION Void KillActor(ID id) noexcept { return VAULTAPI(KillActor)(id); }
	VAULTFUNCTION Void SetPlayerRespawn(ID id, Interval interval) noexcept { return VAULTAPI(SetPlayerRespawn)(id, interval); }

	class Reference {
		protected:
			ID id;
			Ref refID;
			Base baseID;
			Type type;

			Reference(ID id, Type type) noexcept : id(id), refID(id ? vaultmp::GetReference(id) : static_cast<Ref>(0)), baseID(id ? vaultmp::GetBase(id) : static_cast<Base>(0)), type(type) {}
			virtual ~Reference() noexcept {}

		public:
			State IsValid() const noexcept { return id ? True : False; }
			explicit operator bool() const noexcept { return IsValid(); }
			explicit operator State() const noexcept { return IsValid(); }
			bool operator==(const Reference& R) const noexcept { return IsValid() && this->id == R.id; }
			bool operator!=(const Reference& R) const noexcept { return !operator==(R); }

			ID GetID() const noexcept { return id; }
			Ref GetReference() const noexcept { return refID; }
			Base GetBase() const noexcept { return baseID; }
			Type GetType() const noexcept { return type; }

			State Revalidate() noexcept
			{
				if (id && vaultmp::IsValid(id))
					return True;
				id = static_cast<ID>(0);
				return False;
			}

			static UCount GetCount() noexcept { return vaultmp::GetCount(Type::ID_REFERENCE); }
			static IDVector GetList() noexcept { return vaultmp::GetList(Type::ID_REFERENCE); }
	};

	class Object : public Reference {
		protected:
			Object(ID id, Type type) noexcept : Reference(id, type) {}

		public:
			Object(ID id) noexcept : Reference(vaultmp::IsObject(id) ? id : static_cast<ID>(0), Type::ID_OBJECT) {}
			virtual ~Object() noexcept {}

			String GetName() const noexcept { return vaultmp::GetName(id); }
			Void GetPos(Value& X, Value& Y, Value& Z) const noexcept { return vaultmp::GetPos(id, X, Y, Z); }
			Void GetAngle(Value& X, Value& Y, Value& Z) const noexcept { return vaultmp::GetAngle(id, X, Y, Z); }
			Cell GetCell() const noexcept { return vaultmp::GetCell(id); }
			State IsNearPoint(Value& X, Value& Y, Value& Z, Value& R) const noexcept { return vaultmp::IsNearPoint(id, X, Y, Z, R); }

			static UCount GetCount() noexcept { return vaultmp::GetCount(Type::ID_OBJECT); }
			static IDVector GetList() noexcept { return vaultmp::GetList(Type::ID_OBJECT); }
	};

	class Item : public Object {
		protected:
			Item(ID id, Type type) noexcept : Object(id, type) {}

		public:
			Item(ID id) noexcept : Object(vaultmp::IsItem(id) ? id : static_cast<ID>(0), Type::ID_ITEM) {}
			virtual ~Item() noexcept {}

			static UCount GetCount() noexcept { return vaultmp::GetCount(Type::ID_ITEM); }
			static IDVector GetList() noexcept { return vaultmp::GetList(Type::ID_ITEM); }
	};

	class Container : public Object {
		protected:
			Container(ID id, Type type) noexcept : Object(id, type) {}

		public:
			Container(ID id) noexcept : Object(vaultmp::IsContainer(id) ? id : static_cast<ID>(0), Type::ID_CONTAINER) {}
			virtual ~Container() noexcept {}

			UCount GetContainerItemCount(Base base = static_cast<Base>(0)) const noexcept { return vaultmp::GetContainerItemCount(id, base); }

			State AddItem(Base base, UCount count = 1, Value condition = 100.0, State silent = True) noexcept { return vaultmp::AddItem(id, base, count, condition, silent); }
			UCount RemoveItem(Base base, UCount count = 1, State silent = True) noexcept { return vaultmp::RemoveItem(id, base, count, silent); }
			Void RemoveAllItems() noexcept { return vaultmp::RemoveAllItems(id); }

			static UCount GetCount() noexcept { return vaultmp::GetCount(Type::ID_CONTAINER); }
			static IDVector GetList() noexcept { return vaultmp::GetList(Type::ID_CONTAINER); }
	};

	class Actor : public Container {
		protected:
			Actor(ID id, Type type) noexcept : Container(id, type) {}

		public:
			Actor(ID id) noexcept : Container(vaultmp::IsActor(id) ? id : static_cast<ID>(0), Type::ID_ACTOR) {}
			virtual ~Actor() noexcept {}

			Value GetActorValue(Index index) const noexcept { return vaultmp::GetActorValue(id, index); }
			Value GetActorBaseValue(Index index) const noexcept { return vaultmp::GetActorBaseValue(id, index); }
			Index GetActorMovingAnimation() const noexcept { return vaultmp::GetActorMovingAnimation(id); }
			State GetActorAlerted() const noexcept { return vaultmp::GetActorAlerted(id); }
			State GetActorSneaking() const noexcept { return vaultmp::GetActorSneaking(id); }
			State GetActorDead() const noexcept { return vaultmp::GetActorDead(id); }
			State IsActorJumping() const noexcept { return vaultmp::IsActorJumping(id); }

			Void SetActorValue(Index index, Value value) noexcept { return vaultmp::SetActorValue(id, index, value); }
			Void SetActorBaseValue(Index index, Value value) noexcept { return vaultmp::SetActorBaseValue(id, index, value); }
			State EquipItem(Base base, State silent = True, State stick = True) noexcept { return vaultmp::EquipItem(id, base, silent, stick); }
			State UnequipItem(Base base, State silent = True, State stick = True) noexcept { return vaultmp::UnequipItem(id, base, silent, stick); }
			Void KillActor() noexcept { return vaultmp::KillActor(id); }

			static UCount GetCount() noexcept { return vaultmp::GetCount(Type::ID_ACTOR); }
			static IDVector GetList() noexcept { return vaultmp::GetList(Type::ID_ACTOR); }
	};

	class Player : public Actor {
		public:
			Player(ID id) noexcept : Actor(vaultmp::IsPlayer(id) ? id : static_cast<ID>(0), Type::ID_PLAYER) {}
			virtual ~Player() noexcept {}

			Void SetPlayerRespawn(Interval interval) noexcept { return vaultmp::SetPlayerRespawn(id, interval); }
			State UIMessage(String message) noexcept { return vaultmp::UIMessage(id, message); }
			State ChatMessage(String message) noexcept { return vaultmp::ChatMessage(id, message); }

			Player& operator<<(const String& message) noexcept
			{
				ChatMessage(message);
				return *this;
			}

			static UCount GetCount() noexcept { return vaultmp::GetCount(Type::ID_PLAYER); }
			static IDVector GetList() noexcept { return vaultmp::GetList(Type::ID_PLAYER); }
	};

	class _Chat {
		public:
			_Chat& operator<<(const String& message) noexcept
			{
				ChatMessage(message);
				return *this;
			}
	} Chat;
}
#endif

#endif
