/* Vault-Tec Multiplayer */

// Constants

const Index: {
        FALLOUT3            =   0x00000001,
        NEWVEGAS            =   FALLOUT3 << FALLOUT3,
        FALLOUT_GAMES       =   FALLOUT3 | NEWVEGAS,
        ALL_GAMES           =   FALLOUT_GAMES,

        MAX_PLAYER_NAME     =   16,
        MAX_PASSWORD_SIZE   =   16,
        MAX_MESSAGE_LENGTH  =   64,
        MAX_CHAT_LENGTH     =   128,
}

const Type: {
        ID_REFERENCE        =   0x00000001,
        ID_OBJECT           =   ID_REFERENCE << ID_REFERENCE,
        ID_ITEM             =   ID_OBJECT << ID_REFERENCE,
        ID_CONTAINER        =   ID_ITEM << ID_REFERENCE,
        ID_ACTOR            =   ID_CONTAINER << ID_REFERENCE,
        ID_PLAYER           =   ID_ACTOR << ID_REFERENCE,

        ALL_OBJECTS         =   (ID_OBJECT | ID_ITEM | ID_CONTAINER | ID_ACTOR | ID_PLAYER),
        ALL_CONTAINERS      =   (ID_CONTAINER | ID_ACTOR | ID_PLAYER),
        ALL_ACTORS          =   (ID_ACTOR | ID_PLAYER),
}

const Interval: {
	DEFAULT_PLAYER_RESPAWN 	= 8000,
}

// Callbacks

forward OnSpawn(ID);
forward OnCellChange(ID, cell);
forward OnContainerItemChange(ID, base, count, Float:value);
forward OnActorValueChange(ID, index, Float:value);
forward OnActorBaseValueChange(ID, index, Float:value);
forward OnActorAlert(ID, Bool:alerted);
forward OnActorSneak(ID, Bool:sneaking);
forward OnActorDeath(ID);
forward OnActorEquipItem(ID, base, Float:value);
forward OnActorUnequipItem(ID, base, Float:value);
forward OnPlayerDisconnect(ID, reason);
forward OnPlayerRequestGame(ID);
forward OnPlayerChat(ID, message{});
forward OnClientAuthenticate(const name{}, const pwd{});

// Functions

native timestamp();
native CreateTimer(const func{}, interval);
native CreateTimerEx(const func{}, interval, const def{}, {Fixed,Float,_}:...);
native KillTimer(timer);
native MakePublic(const func{}, const name{}, const def{});
native CallPublic(const name{}, {Fixed,Float,_}:...);

native SetServerName(const name{});
native SetServerMap(const map{});
native SetServerRule(const rule{}, const value{});
native GetGameCode();
native GetMaximumPlayers();
native GetCurrentPlayers();

native ValueToString(index, value{});
native AxisToString(index, value{});
native AnimToString(index, value{});

native Bool:UIMessage(ID, const message{});
native Bool:ChatMessage(ID, const message{});
native SetRespawn(interval);
native Bool:IsValid(ID);
native Bool:IsObject(ID);
native Bool:IsItem(ID);
native Bool:IsContainer(ID);
native Bool:IsActor(ID);
native Bool:IsPlayer(ID);
native Type:GetType(ID);
native GetCount(Type:type);
native GetList(Type:type, id[]);

native GetReference(ID);
native GetBase(ID);
native GetName(ID, name{});
native GetPos(ID, &Float:X, &Float:Y, &Float:Z);
native GetAngle(ID, &Float:X, &Float:Y, &Float:Z);
native GetCell(ID);
native Bool:IsNearPoint(ID, Float:X, Float:Y, Float:Z, Float:R);
native GetContainerItemCount(ID, base);
native Float:GetActorValue(ID, index);
native Float:GetActorBaseValue(ID, index);
native GetActorMovingAnimation(ID);
native Bool:GetActorAlerted(ID);
native Bool:GetActorSneaking(ID);
native Bool:GetActorDead(ID);
native Bool:IsActorJumping(ID);

native Bool:AddItem(ID, base, count, Float:value, Bool:silent);
native RemoveItem(ID, base, count, Bool:silent);
native RemoveAllItems(ID);
native SetActorValue(ID, index, Float:value);
native SetActorBaseValue(ID, index, Float:value);
native Bool:EquipItem(ID, base, Bool:silent, Bool:stick);
native Bool:UnequipItem(ID, base, Bool:silent, Bool:stick);
native KillActor(ID);
native SetPlayerRespawn(ID, interval);
