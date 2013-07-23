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

	vector<int> division;
	division.push_back(divisionX);
	division.push_back(divisionY);

	stringstream m;
	m << "Creating grid (" << sizeX << "," << sizeY << ")...";
	Log4CL::instance()->get_logger("root").log(INFO, m.str());
	grid = new repast::SharedGrids<FooAgent>::SharedWrappedGrid(
			"grid ",
			repast::GridDimensions(repast::Point<int>(sizeX, sizeY)),
			division,
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
		grid->moveTo(fooagent->getId(), repast::Point<int>(originX + i, originY + i));
	}

	Log4CL::instance()->get_logger("root").log(INFO, "Sync buffer...");
	grid->initSynchBuffer(agents);
	grid->synchBuffer<FooPackage>(agents, *this, *this);

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
			int nx = pos[0] + 1;
			int ny = pos[1] + 1;
			grid->moveTo(thisAgent->getId(), repast::Point<int>(nx, ny));
			stringstream m;
			m << thisAgent->getId() << " moves from " << pos[0] << "," << pos[1] << " to " << nx << "," << ny;
			Log4CL::instance()->get_logger("root").log(INFO, m.str());
		}
	}

	synchAgents();

}

void FooModel::synchAgents() {
	Log4CL::instance()->get_logger("root").log(INFO, "Sync...");
	repast::syncAgents<FooPackage>(*this, *this);
	grid->initSynchBuffer(agents);
	grid->synchBuffer<FooPackage>(agents, *this, *this);
	repast::RepastProcess::instance()->syncAgentStatus<FooAgent,FooPackage>(agents, *this, *this);
	grid->synchMove();
}

FooAgent* FooModel::createAgent(FooPackage content) {
	return new FooAgent(content.getId());
}

void FooModel::createAgents(vector<FooPackage>& contents, vector<FooAgent*>& out) {
	for (vector<FooPackage>::iterator agent = contents.begin(); agent != contents.end(); ++agent) {
		out.push_back(new FooAgent(agent->getId()));
	}
}

void FooModel::provideContent(FooAgent* agent, vector<FooPackage>& out) {
	repast::AgentId id = agent->getId();
	FooPackage agents = { id.id(), id.startingRank(), id.agentType() };
	out.push_back(agents);
}

void FooModel::provideContent(const repast::AgentRequest& request, vector<FooPackage>& out) {
	const vector<repast::AgentId>& ids = request.requestedAgents();
	for (int i = 0, size = ids.size(); i < size; i++) {
		repast::AgentId id = ids[i];

		if (agents.contains(id)) {
			FooAgent* fooagent = agents.getAgent(id);
			FooPackage content = { id.id(), id.startingRank(), id.agentType() };
			out.push_back(content);
		}
	}
}

void FooModel::updateAgent(const FooPackage& content) {
	repast::AgentId id = content.getId();

	stringstream m;
	m << "Update agent " << id;
	Log4CL::instance()->get_logger("root").log(INFO, m.str());

	if (agents.contains(id)) {
		FooAgent* copy = agents.getAgent(id);
		// ...
	}
}
