#include "CppUTestExt/MockSupport.h"
#include "task.hpp"

extern "C"
{

}


Task::Task(const char *name, uint32_t _stackSize, UBaseType_t prio)
{
    mock().actualCall("Task")
        .withStringParameter("name", name)
        .withParameter("stackSize", _stackSize)
        .withParameter("priority", prio);
}

Task::~Task()
{
    mock().actualCall("~Task");
}

Status_t Task::initTask(void)
{
    return static_cast<Status_t>(mock().actualCall("initTask").returnIntValueOrDefault(STATUS_OKAY));
}

void Task::taskRun()
{
    
}