
#include "SaliMCore.h"

#ifndef SM_TASK_MAX
  #define SM_TASK_MAX 8
#endif


SM_USE_NAMESPACE


//Task support
struct SmTaskBlock {
    unsigned       mTopOfStack;
    unsigned       mStackCellSize;
    SmTaskBlock   *mNextTask;
    void          *mArg;
    SmWaitFunction mWaitFunction;
    SmTaskFunction mTaskFunction;
    bool           mCritic;

    void buildTask( unsigned stackCellSize, void *arg, SmTaskFunction taskFunction, bool critic );

    void buildParialTask( void *arg, SmTaskFunction taskFunction, bool critic );

    bool isApply( unsigned stackCellSize ) const { return mTopOfStack == 0 && (mStackCellSize == 0 || stackCellSize <= mStackCellSize); }

    static bool criticUsed;
  };

using SmTaskBlockPtr = SmTaskBlock*;

static SmTaskBlock taskBlock[SM_TASK_MAX];

bool SmTaskBlock::criticUsed;

//C-interface
extern "C" {
  void smPortInitStack(void);
  void smPortSwitchContext(void);
  void smPortBuildStack(void);

  unsigned       smTopStack;
  SmTaskBlockPtr smCurrentTask;
  SmTaskBlockPtr smNextTask;

  volatile int   smTickCount;


  void smInit( unsigned stackCellSize )
    {
    //Copy current stack pointer to smTopStack
    smPortInitStack();
    //Init first task as main loop task
    smCurrentTask = taskBlock;
    smCurrentTask->mNextTask = smCurrentTask;
    smCurrentTask->mTopOfStack = smTopStack;
    smCurrentTask->mStackCellSize = stackCellSize;
    //Alloc stack for current main loop task
    smTopStack -= stackCellSize * 4;
    }



  void smTaskEntry(void)
    {
    while(true) {
      //Entry function must not return, but, if it return then exclude task from task list
      smCurrentTask->mTaskFunction( smCurrentTask->mArg );

      //Exclude task from task list
      for( smNextTask = smCurrentTask; smNextTask->mNextTask != smCurrentTask; smNextTask = smNextTask->mNextTask );
      smNextTask->mNextTask = smCurrentTask->mNextTask;
      smNextTask = smCurrentTask->mNextTask;

      //Switch to next available task
      for( smNextTask = smCurrentTask->mNextTask; !smNextTask->mWaitFunction( smNextTask->mArg ); smNextTask = smNextTask->mNextTask );
      //Switch context
      smPortSwitchContext();
      }
    }
}



static bool smWaitAlwaysTrue( void* ) { return true; }



void SmTaskBlock::buildTask(unsigned int stackCellSize, void *arg, SmTaskFunction taskFunction, bool critic)
  {
  //Entry function
  mArg          = arg;
  mWaitFunction = smWaitAlwaysTrue;
  mTaskFunction = taskFunction;
  //Stack
  mTopOfStack = smTopStack;
  smTopStack -= stackCellSize * 4;
  mStackCellSize = stackCellSize;
  //Critic
  mCritic = critic;
  //Link
  mNextTask = smCurrentTask->mNextTask;
  smCurrentTask->mNextTask = smNextTask = this;
  }




void SmTaskBlock::buildParialTask(void *arg, SmTaskFunction taskFunction, bool critic)
  {
  //Entry function
  mArg          = arg;
  mWaitFunction = smWaitAlwaysTrue;
  mTaskFunction = taskFunction;
  //Critic
  mCritic = critic;
  //Link
  mNextTask = smCurrentTask->mNextTask;
  smCurrentTask->mNextTask = smNextTask = this;
  }






void SM_NAMESPACE_PREPEND smTaskCreate(unsigned stackCellSize, void *arg, SmTaskFunction taskFunction , bool critic)
  {
  if( critic ) SmTaskBlock::criticUsed = true;
  for( int i = 0; i < SM_TASK_MAX; i++ )
    if( taskBlock[i].isApply(stackCellSize) ) {
      if( taskBlock[i].mStackCellSize )
        //Reused task block
        taskBlock[i].buildParialTask( arg, taskFunction, critic );
      else {
        //Fill task block
        taskBlock[i].buildTask( stackCellSize, arg, taskFunction, critic );
        //Build stack on stNextTask pointed task
        smPortBuildStack();
        }
      return;
      }
  }






void SM_NAMESPACE_PREPEND smWaitVoid(void *arg, SmWaitFunction waitFunction )
  {
  //Store wait functor for current task
  smCurrentTask->mArg          = arg;
  smCurrentTask->mWaitFunction = waitFunction;

  //First scan is for critic task
  if( SmTaskBlock::criticUsed ) {
    for( smNextTask = smCurrentTask->mNextTask; smNextTask != smCurrentTask; smNextTask = smNextTask->mNextTask )
      if( smNextTask->mCritic && smNextTask->mWaitFunction( smNextTask->mArg ) )
        //Available critic task found
        break;

    if( smNextTask == smCurrentTask )
      //Scan task list for available task
      for( smNextTask = smCurrentTask->mNextTask; !smNextTask->mWaitFunction( smNextTask->mArg ); smNextTask = smNextTask->mNextTask );
    }
  else {
    //Scan task list for available task
    for( smNextTask = smCurrentTask->mNextTask; !smNextTask->mWaitFunction( smNextTask->mArg ); smNextTask = smNextTask->mNextTask );
    }

  //Available task found. Switch to it if it is different task then current
  if( smNextTask != smCurrentTask )
    //Switch context
    smPortSwitchContext();
  }






void SM_NAMESPACE_PREPEND smYeld()
  {
  smWaitVoid( nullptr, smWaitAlwaysTrue );
  }






void SM_NAMESPACE_PREPEND smWaitTick( int timeOut )
  {
  //Calculate moment in the future
  timeOut = smTickFuture( timeOut );
  //Wait this moment
  smWait<int>( &timeOut, [] ( int *timeOutArg ) -> bool { return smTickIsOut( *timeOutArg ); } );
  }




