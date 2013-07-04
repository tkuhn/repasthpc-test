#include <repast_hpc/AgentId.h>
#include <repast_hpc/SharedContext.h>
#include <repast_hpc/SharedNetwork.h>
#include <repast_hpc/SharedSpace.h>
#include <repast_hpc/DataSet.h>
#include <repast_hpc/RepastProcess.h>
#include <repast_hpc/Edge.h>
#include <repast_hpc/Point.h>
#include <repast_hpc/NCDataSetBuilder.h>


class TestAgent: public repast::Agent {

private:
	repast::AgentId _id;
	int _state;

public:

	TestAgent(repast::AgentId id, int state);
	virtual ~TestAgent();

	int state() const {
		return _state;
	}

	void state(int val) {
		_state = val;
	}

	repast::AgentId& getId () {
		return _id;
	}

	const repast::AgentId& getId () const {
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

	repast::AgentId id;
	int state;

	repast::AgentId getId() const {
		return id;
	}

};


class TestEdge {
};


class Model {

private:

	int rank;

public:
	repast::SharedContext<TestAgent> agents;
	repast::SharedNetwork<TestAgent, TestEdge>* net;
	repast::SharedGrids<TestAgent>::SharedWrappedGrid* grid;
	repast::DataSet* dataSet;

	Model();
	virtual~ Model();
	void initSchedule();
	void step();
};


int main() {
}
