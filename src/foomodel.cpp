#include "foomodel.h"

using namespace std;

namespace mpi = boost::mpi;

FooModel::FooModel(const string& propsFile, int argc, char* argv[], mpi::communicator* world) :
		props(propsFile, argc, argv, world) {

	repast::RepastProcess* rp = repast::RepastProcess::instance();
	rank = rp->rank();

	sizeX = repast::strToInt(props.getProperty("size.x"));
	sizeY = repast::strToInt(props.getProperty("size.y"));

	divisionX = repast::strToInt(props.getProperty("division.x"));
	divisionY = repast::strToInt(props.getProperty("division.y"));

	dimX = sizeX / divisionX;
	dimY = sizeY / divisionY;

	vector<int> procDim;
	procDim.push_back(divisionX);
	procDim.push_back(divisionY);

	stringstream m;
	m << "Creating grid (" << sizeX << "," << sizeY << ")...";
	Log4CL::instance()->get_logger("root").log(INFO, m.str());
	grid = new repast::SharedGrids<FooAgent>::SharedWrappedGrid(
			"grid ",
			repast::GridDimensions(repast::Point<int>(sizeX, sizeY)),
			procDim,
			1,
			world
		);
	agents.addProjection(grid);

	originX = grid->dimensions().origin().getX();
	originY = grid->dimensions().origin().getY();

	Log4CL::instance()->get_logger("root").log(INFO, "Creating agents...");
	FooAgent* fooagent;
	for (int i = 0; i < dimX && i < dimY; i++) {
		repast::AgentId id(i, rank, 0);
		fooagent = new FooAgent(id);
		agents.addAgent(fooagent);
		grid->moveTo(fooagent, repast::Point<int>(originX + i, originY + i));
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
	runner.scheduleStop(3);
	runner.scheduleEvent(1, 1, repast::Schedule::FunctorPtr(new repast::MethodFunctor<FooModel>(this, &FooModel::step)));

}

void FooModel::step() {
	vector<int> pos(2);
	FooAgent* thisAgent;

	repast::SharedContext<FooAgent>::const_local_iterator it;
	for (it = agents.localBegin(); it != agents.localEnd(); it++) {
		thisAgent = it->get();
		if (grid->getLocation(thisAgent->getId(), pos)) {
			int nx = pos[0] + 1;
			int ny = pos[1] + 1;
			stringstream m;
			m << "Moving " << thisAgent->getId() << " from " << pos[0] << "," << pos[1] << " to " << nx << "," << ny;
			Log4CL::instance()->get_logger("root").log(INFO, m.str());
			grid->moveTo(thisAgent, repast::Point<int>(nx, ny));
		}
	}

	synchAgents();

}

void FooModel::synchAgents() {
	repast::syncAgents<FooAgents>(*this, *this);
	grid->synchBuffer<FooAgents>(agents, *this, *this);
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
