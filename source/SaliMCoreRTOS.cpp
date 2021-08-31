
#include "SaliMCoreRTOS.h"


//!
//! \brief smTaskCreate  Creates new task with stackCellSize stack size and taskFunctor as task entry point
//! \param stackCellSize Task stack size in 32-bit cell
//! \param arg           Param for task, may any or nothing
//! \param taskFunction  Task entry point function
//! \param critic        Define priority level for task. All task devided into two sections: critic tasks and all other.
//!                      Critic task handled as fast as possible and suit for polling tasks.
//!
void smTaskCreate( unsigned stackCellSize, void *arg, SmTaskFunction taskFunction, bool critic )
  {
  static int c = 0;
  char taskName[8];
  taskName[0] = 'T';
  taskName[1] = 'a';
  taskName[2] = 's';
  taskName[3] = 'k';
  taskName[4] = '0' + c / 10;
  taskName[5] = '0' + c % 10;
  taskName[6] = 0;
  c++;
  TaskHandle_t xHandle = NULL;
  xTaskCreate( taskFunction, taskName, stackCellSize, arg, tskIDLE_PRIORITY, &xHandle );
  configASSERT( xHandle );
  }


//!
//! \brief smWaitVoid   Main wait function. Wait while waitFuncion return true. While task is in wait state cpu switch to other tasks
//! \param arg          Any argument for waitFuncion
//! \param waitFunction Pointer to wait function
//!
void smWaitVoid( void *arg, SmWaitFunction waitFunction )
  {
  while( !waitFunction(arg) )
    taskYIELD();
  }





//!
//! \brief smWaitTick Helper function for waiting specified timeOut. It resume this task after timeOut elapsed
//! \param timeOut    Waiting time out
//!
void smWaitTick( int timeOut )
  {
  vTaskDelay( timeOut / (1000 / configTICK_RATE_HZ) );
  }


//!
//! \brief smYeld Simple funtion which switch to another task and resume this task when task loop round
//!
void smYeld()
  {
  taskYIELD();
  }
