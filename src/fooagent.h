#ifndef FOOAGENT_H_INCLUDED
#define FOOAGENT_H_INCLUDED

#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include <repast_hpc/AgentId.h>

using namespace std;

struct FooAgents {

	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar & id;
		ar & proc;
		ar & type;
	}

	int id;
	int proc;
	int type;

	repast::AgentId getId() const {
		return repast::AgentId(id, proc, type);
	}

};

class FooAgent: public repast::Agent {

	friend class boost::serialization::access;

private:
	repast::AgentId id;

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar & boost::serialization::base_object<Agent>(*this);
		ar & id;
	}

public:
	FooAgent(repast::AgentId);
	virtual ~FooAgent();

	virtual repast::AgentId& getId() {
		return id;
	}

	virtual const repast::AgentId& getId() const {
		return id;
	}
};

#endif // FOOAGENT_H_INCLUDED
