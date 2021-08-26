#define MAXN		100
#define MAXL		8
#include <map>
#include <queue>
#include <vector>
#include <string>
#include <unordered_map>

int duTime = 0;

class bicycle {
public:
	int startstamp = 0;
	int ridetime = 0;
	bool checkvalid() {
		return ridetime > duTime ? false : true;
	}
};
struct cmp {
	bool operator () (const bicycle *b1,const bicycle* b2){
		return b1->ridetime > b2->ridetime;
	}
};
class station {
public:
	std::priority_queue<bicycle*,std::vector<bicycle*>,cmp>bicycles;
	int deTime = 0;
	std::queue<int>orderqueue;
	void order(int curTime) {
		while (!orderqueue.empty()&& curTime > orderqueue.front()) {
			bicycles.emplace(new bicycle());
			orderqueue.pop();
		}
	}
};

std::vector<station*>stations;
class customer {
public:
	int ticket = 0,ticketfrom = 0,ticketend = 0;
	bicycle* b = nullptr;
	bool rent(int timestamp,int pID) {
		updateTicket(timestamp);
		if (ticket != 0 && b == nullptr && !stations[pID]->bicycles.empty()) {
			b = stations[pID]->bicycles.top();
			b->startstamp = timestamp;
			stations[pID]->bicycles.pop();
			return true;
		}
		return false;
	}
	void updateTicket(int curTime) {
		ticket -= (curTime - ticketfrom);
		if (ticket <= 0)
			ticket = 0;
		ticketfrom = curTime;
	}
};

std::unordered_map<std::string, customer*>customers;
void init(int N, int durableTime, int deliveryTimes[MAXN])
{
	stations.clear();
	customers.clear();
	duTime = durableTime;
	for (int i = 0; i < N; i++) {
		station* tmp = new station();
		tmp->deTime = deliveryTimes[i];
		stations.push_back(tmp);
	}
}

void addBicycle(int cTimestamp, int pID, int bicycleNum)
{
	for (int i = 0; i < bicycleNum; i++)
		stations[pID]->bicycles.emplace(new bicycle());
}

void buyTicket(int cTimestamp, char uName[MAXL], int validTime)
{
	std::string name = uName;
	if (customers.find(name) == customers.end())
		customers.emplace(std::make_pair(name, new customer()));
	if (customers.find(name) != customers.end()) {
		customers.find(name)->second->updateTicket(cTimestamp);
		customers.find(name)->second->ticket += validTime;
		customers.find(name)->second->ticketend = customers.find(name)->second->ticket + customers.find(name)->second->ticketfrom;
	}
}

int rentBicycle(int cTimestamp, char uName[MAXL], int pID)
{
	std::string name = uName;
	stations[pID]->order(cTimestamp);
	if (customers.find(name) == customers.end())
		customers.emplace(std::make_pair(name, new customer()));
	if (customers.find(name)->second->rent(cTimestamp, pID))
		return customers.find(name)->second->b->ridetime;
	return -1;
}

int returnBicycle(int cTimestamp, char uName[MAXL], int pID)
{
	std::string name = uName;
	if (customers.find(name) == customers.end())
		customers.emplace(std::make_pair(name, new customer()));
	if (customers.find(name) != customers.end()) {
		if (customers.find(name)->second->b == nullptr)
			return -1;
		customers.find(name)->second->b->ridetime += cTimestamp - customers.find(name)->second->b->startstamp;
		if (customers.find(name)->second->b->checkvalid())
			stations[pID]->bicycles.emplace(customers.find(name)->second->b);
		else
			stations[pID]->orderqueue.emplace(cTimestamp + stations[pID]->deTime - 1);
		customers.find(name)->second->b = nullptr;
		if (customers.find(name)->second->ticketend > cTimestamp)
			return 0;
		else
		{
			int res = cTimestamp - customers.find(name)->second->ticketend + 1;
			customers.find(name)->second->updateTicket(cTimestamp);
			return res;
		}
	}
}