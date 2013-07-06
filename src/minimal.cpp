#include <iostream>
#include <repast_hpc/AgentId.h>

using namespace std;
using namespace repast;

int main(int argc, char** argv){
    AgentId a(1,1,1);
    cout << a << endl;
}
