#include <repast_hpc/AgentId.h>
#include <repast_hpc/SharedContext.h>
#include <repast_hpc/SharedNetwork.h>
#include <repast_hpc/SharedSpace.h>
#include <repast_hpc/DataSet.h>
#include <repast_hpc/RepastProcess.h>
#include <repast_hpc/Edge.h>
#include <repast_hpc/Point.h>
#include <repast_hpc/NCDataSetBuilder.h>

using namespace std;
using namespace repast;


class TestAgent: public Agent {

private:
	AgentId _id;
	int _state;

public:

	TestAgent(AgentId id, int state);
	virtual ~TestAgent();

	int state() const {
		return _state;
	}

	void state(int val) {
		_state = val;
	}

	AgentId& getId () {
		return _id;
	}

	const AgentId& getId () const {
		return _id;
	}

	void flipState();
};


struct TestAgentPackage {

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& id;
		ar& state;
	}

	AgentId id;
	int state;

	AgentId getId() const {
		return id;
	}

};


class TestEdge {
};


class Model {

private:

	int rank;

public:
	SharedContext<TestAgent> agents;
	SharedNetwork<TestAgent, TestEdge>* net;
	SharedGrids<TestAgent>::SharedWrappedGrid* grid;
	DataSet* dataSet;

	Model();
	virtual~ Model();
	void initSchedule();
	void step();
};


int main() {
	cout << "hello world" << endl;
}
