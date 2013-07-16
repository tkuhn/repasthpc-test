#include "foomodel.h"

using namespace std;

namespace mpi = boost::mpi;

FooModel::FooModel(const string& propsFile, int argc, char* argv[], mpi::communicator* world) :
		props(propsFile, argc, argv, world) {

	repast::RepastProcess* rp = repast::RepastProcess::instance();
	rank = rp->rank();

	vector<int> procDim;
	procDim.push_back(1);
	procDim.push_back(1);

	Log4CL::instance()->get_logger("root").log(INFO, "Creating grid...");
	grid = new repast::SharedGrids<FooAgent>::SharedWrappedGrid(
			"grid ",
			repast::GridDimensions(repast::Point<int>(10, 10)),
			procDim,
			1,
			world
		);
	agents.addProjection(grid);

	Log4CL::instance()->get_logger("root").log(INFO, "Creating agents...");
	FooAgent* fooagent;
	for (int i = 1; i < 5; i++) {
		repast::AgentId id(i, rank, 0);
		fooagent = new FooAgent(id);
		agents.addAgent(fooagent);
		grid->moveTo(fooagent, repast::Point<int>(i*2, i*2));
	}

	Log4CL::instance()->get_logger("root").log(INFO, "Sync buffer...");
	grid->initSynchBuffer(agents);
	grid->synchBuffer<FooAgents>(agents, *this, *this);

}

FooModel::~FooModel() {
}

void FooModel::init() {
}

void FooModel::initSchedule() {
	repast::ScheduleRunner& runner = repast::RepastProcess::instance()->getScheduleRunner();
	runner.scheduleStop(5);
	runner.scheduleEvent(1, 1, repast::Schedule::FunctorPtr(new repast::MethodFunctor<FooModel>(this, &FooModel::step)));

}

void FooModel::step() {
	vector<int> pos(2);
	FooAgent* thisAgent;

	repast::SharedContext<FooAgent>::const_local_iterator it;
	for (it = agents.localBegin(); it != agents.localEnd(); it++) {
		thisAgent = it->get();
		if (grid->getLocation(thisAgent->getId(), pos)) {
			stringstream m;
			m << "Handling " << thisAgent->getId() << " at position " << pos[0] << "," << pos[1];
			Log4CL::instance()->get_logger("root").log(INFO, m.str());
		}
	}

}

void FooModel::synchAgents() {
}

FooAgent* FooModel::createAgent(FooAgents& content) {
	return new FooAgent(content.getId());
}

void FooModel::createAgents(vector<FooAgents>& contents, vector<FooAgent*>& out) {
	for (vector<FooAgents>::iterator agent = contents.begin(); agent != contents.end(); ++agent) {
		out.push_back(new FooAgent(agent->getId()));
	}
}

void FooModel::provideContent(FooAgent* agent, vector<FooAgents>& out) {
	repast::AgentId id = agent->getId();
	FooAgents agents = { id.id(), id.startingRank(), id.agentType() };
	out.push_back(agents);
}

void FooModel::provideContent(const repast::AgentRequest& request, vector<FooAgents>& out) {
	const vector<repast::AgentId>& ids = request.requestedAgents();
	for (int i = 0, size = ids.size(); i < size; i++) {
		repast::AgentId id = ids[i];

		if (agents.contains(id)) {
			FooAgent* fooagent = agents.getAgent(id);
			FooAgents content = { id.id(), id.startingRank(), id.agentType() };
			out.push_back(content);
		}
	}
}

void FooModel::updateAgent(const FooAgents& content) {
	repast::AgentId id = content.getId();

	if (agents.contains(id)) {
		FooAgent* copy = agents.getAgent(id);
	}
}
