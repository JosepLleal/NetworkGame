// TODO(you): Reliability on top of UDP lab session
#include <map>
#include <list>

class DeliveryManager;

class DeliveryDelegate
{
public:
	virtual void onDeliverySuccess(DeliveryManager* deliveryManager) = 0;
	virtual void onDeliveryFailure(DeliveryManager* deliveryManager) = 0;

};

struct Delivery
{
	double dispatchtime = 0.0;
	double startTime = 0.0;
	DeliveryDelegate* delegate = nullptr;
};

class DeliveryManager 
{
public:
	//For senders to write a new seq. numbers into a packet
	Delivery* writeSequenceNumber(OutputMemoryStream& packet);

	//For recievers to process the seq. number from an incoming packet
	bool processSequenceNumber(const InputMemoryStream& packet);

	//For recievers to write ack'ed seq. numbers into a packet
	bool hasSequenceNumberspendingAck() const;
	void writeSequenceNumbersPendingAck(OutputMemoryStream& packet);

	//For senders to process ack'ed seq. numbers from a packet
	void processAckdSequenceNumbers(const InputMemoryStream& packet);
	void processTimedOutPackets();

	void clear();


private:
	//Sender side
	uint32 nextOutSeqNumber = 0;
	std::map<uint32,Delivery*> pendingDeliveries;

	//Reciever side
	uint32 nextExpSeqNumber = 0;
	std::list<uint32> pendingAck;
};

