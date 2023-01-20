
#include <stdint.h>
#include "freertos/xtensa_api.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#define STACK_SIZE 2048

enum msg_type_t {CONTROL = 0, GENERAL = 1, OTHER = 3};

struct control_msg_t
{
    msg_type_t msgtype;
    int16_t    params[4];
};


void ccall_queue_control(void * pvParameters);



class QueueControl
{

friend void ccall_queue_control(void *);

protected:
    static QueueControl * object_handler;
    TaskHandle_t xHandle = NULL;
    QueueHandle_t control_task_queue = NULL;
    uint16_t event_queue_size = 20;
public:
    QueueControl();
    ~QueueControl();
    void handlequeue(void* pvparams);
    void starttask();
    void stoptask();

/*static*/
public:
    static QueueControl* gethandler() { return(object_handler);};
};