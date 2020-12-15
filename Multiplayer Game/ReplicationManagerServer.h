// TODO(you): World state replication lab session
#include <map>

enum class ReplicationAction
{ None, Create, Update, Destroy};

struct ReplicationCommand
{
	ReplicationAction action;
	uint32 networkId;
};

class ReplicationManagerServer
{
public:
	void create(uint32 networkID);
	void update(uint32 networkID);
	void destroy(uint32 networkID);

	void write(OutputMemoryStream &packet);

	std::map<uint32, ReplicationAction> commands;
};