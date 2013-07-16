#include <iostream>

#include "fooagent.h"
#include "foomodel.h"

#include <repast_hpc/RepastProcess.h>
#include <repast_hpc/Schedule.h>
#include <repast_hpc/AgentId.h>

using namespace std;

namespace mpi = boost::mpi;

int main(int argc, char* argv[]) {
	if (argc < 3) {
		cout << "ERROR. Two arguments needed: <repast config file> <model config file>" << endl;
		return -1;
	}

	mpi::environment env(argc, argv);
	mpi::communicator world;

	string config = argv[1];
	string props = argv[2];

	repast::RepastProcess::init(config, &world);

	Log4CL::instance()->get_logger("root").log(INFO, "Starting...");

	Log4CL::instance()->get_logger("root").log(INFO, "Creating model...");
	FooModel* model = new FooModel(props, argc, argv, &world);

	Log4CL::instance()->get_logger("root").log(INFO, "Init model...");
	model->init();

	Log4CL::instance()->get_logger("root").log(INFO, "Init scheduling...");
	model->initSchedule();

	world.barrier();

	repast::ScheduleRunner& runner = repast::RepastProcess::instance()->getScheduleRunner();
	runner.run();

	Log4CL::instance()->get_logger("root").log(INFO, "Stopping...");

	world.barrier();

	repast::RepastProcess::instance()->done();

	return 0;
}
