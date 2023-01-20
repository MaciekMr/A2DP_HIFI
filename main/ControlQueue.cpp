

#include "ControlQueue.h"

void ccall_queue_control(void * pvParameters)
{
    QueueControl::gethandler()->handlequeue(pvParameters);
}


QueueControl* QueueControl::object_handler;


QueueControl::QueueControl()
{
    object_handler = this;
}

QueueControl::~QueueControl()
{

}

void QueueControl::handlequeue(void* pvparams)
{

}

void QueueControl::starttask()
{
    if (control_task_queue == NULL) 
        control_task_queue = xQueueCreate(event_queue_size, sizeof(control_msg_t));
    
    void * ucParameterToPass = (void *) this;

    xTaskCreatePinnedToCore( ccall_queue_control, "CONTROL", STACK_SIZE, &ucParameterToPass, tskIDLE_PRIORITY, &xHandle, 0 );
}