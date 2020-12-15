#include "Networks.h"
//#include "ReplicationManagerClient.h"

// TODO(you): World state replication lab session

void ReplicationManagerClient::read(const InputMemoryStream& packet)
{
	while (packet.RemainingByteCount() > 0)
	{
		uint32 networkId;
		packet >> networkId;
		ReplicationAction action;
		packet >> action;

		if (action == ReplicationAction::Create)
		{
			if (App->modLinkingContext->getNetworkGameObject(networkId) == nullptr)
			{
				GameObject* go = Instantiate();
				App->modLinkingContext->registerNetworkGameObjectWithNetworkId(go, networkId);

				packet >> go->position.x;
				packet >> go->position.y;
				packet >> go->size.x;
				packet >> go->size.y;
				packet >> go->angle;

				BehaviourType type;
				packet >> type;

				if (type == BehaviourType::Spaceship)
				{
					go->sprite = App->modRender->addSprite(go);
					go->sprite->order = 5;
					go->sprite->texture = App->modResources->spacecraft1;//at the moment always type1
				}
				else if (type == BehaviourType::Laser)
				{
					go->sprite = App->modRender->addSprite(go);
					go->sprite->order = 4;
					go->sprite->texture = App->modResources->laser;
				}

				packet >> go->tag;
			}
			else
			{
				//object already exists
				action = ReplicationAction::Update;
			}
		}

		if (action == ReplicationAction::Update)
		{
			GameObject* go = App->modLinkingContext->getNetworkGameObject(networkId);

			packet >> go->position.x;
			packet >> go->position.y;
			packet >> go->size.x;
			packet >> go->size.y;
			packet >> go->angle;
		}
		else if (action == ReplicationAction::Destroy)
		{
			GameObject* go = App->modLinkingContext->getNetworkGameObject(networkId);
			if (go)
			{
				App->modLinkingContext->unregisterNetworkGameObject(go);
				Destroy(go);
			}
		}
	}
}
