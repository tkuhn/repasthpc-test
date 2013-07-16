#ifndef FOOMODEL_H_INCLUDED
#define FOOMODEL_H_INCLUDED

#include <repast_hpc/AgentId.h>
#include <repast_hpc/AgentRequest.h>
#include <repast_hpc/Point.h>
#include <repast_hpc/Properties.h>
#include <repast_hpc/RepastProcess.h>
#include <repast_hpc/Schedule.h>
#include <repast_hpc/SharedContext.h>
#include <repast_hpc/SharedSpace.h>
#include <repast_hpc/SVDataSetBuilder.h>
#include <repast_hpc/Utilities.h>

#include "fooagent.h"

using namespace std;

namespace mpi = boost::mpi;

class FooModel {

private:
	int rank;
	repast::SharedContext<FooAgent> agents;
	repast::SharedGrids<FooAgent>::SharedWrappedGrid* grid;
	repast::Properties props;

public:
	FooModel(const string& propsFile, int argc, char* argv[], mpi::communicator* world);

	virtual ~FooModel();

	void init();

	void initSchedule();

	void step();

	void synchAgents();

	FooAgent* createAgent(FooAgents& content);

	void createAgents(vector<FooAgents>& contents, vector<FooAgent*>& out);

	void provideContent(FooAgent* agent, vector<FooAgents>& out);

	void provideContent(const repast::AgentRequest& request, vector<FooAgents>& out);

	void updateAgent(const FooAgents& content);
};

#endif // FOOMODEL_H_INCLUDED
