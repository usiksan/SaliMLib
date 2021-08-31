/*
   SaliMLib - cooperative Minimal Multitasking Library for 32-bit single-core Microcontrollers

   Author
     Sibilev A.S.

     www.salilab.ru
     www.salilab.com
   Description
     SaliMLib - is cooperative Minimal Multitasking Library for 32-bit single-core Microcontrollers

     SaliMLib is a library for use as a multitasking framework for embedded systems based on 32-bit
     single-core microcontrollers. This library uses cooperative multitasking. This means that each
     task is not allocated a pre-defined time quantum, but the task itself determines the appropriate
     time to transfer control to another task. This approach greatly simplifies task synchronization
     and eliminates the need to use critical sections.

     The main disadvantages of cooperative multitasking compared to preemprtive multitasking are the
     uncertainty of the response time to events, which is why such systems are not used in real-time
     systems. However, this limitation can be easily circumvented by using hardware interrupts, thus
     obtaining a complete analog of "hard time systems", and the actual "real time" can be achieved
     by configuring the microcontroller peripherals to automatically work in parallel with the
     microcontroller core.

     This library is an excellent alternative or addition to the traditional "state machines"for
     embedded systems. With this multitasking library, the implementation of many algorithms is
     radically simplified.

     Features
       - minimalism. This library consumes less resources than other RTOS
       - multitasking. The library provides cooperative multitasking
       - a two-level priority. In the library, all tasks are divided into critical and other tasks.
         Tasks with a critical priority level are managed first
       - a model without priorities can be used
       - no task scheduler. Based on the principle of cooperation all tasks of the same priority are
         performed sequentially
       - computer language. It uses C++ 11 for the interface and platform-independent part, as well
         as Assembler language for the platform-dependent part
       - simplicity. The entire library consists of three files: a kernel header file, a file with
         kernel source codes, and an assembler file with platform-dependent code
       - documentation. The source codes of the library are provided with comprehensive comments,
         as well as a guide
       - examples. There are usage examples for all parts of the library

   History
     v0.1  created library core as alternative to RTOS and state-machine algorithms
     v0.2  refactoring fixed containers to unified interface
     v0.3  appended smUpperRound template function
   */
#ifndef SALIMCORE_H
#define SALIMCORE_H


#define SM_VERSION_MAJOR 0
#define SM_VERSION_MINOR 3



#if defined (SM_NAMESPACE)
# define SM_USE_NAMESPACE using namespace ::SM_NAMESPACE;
# define SM_BEGIN_NAMESPACE namespace SM_NAMESPACE {
# define SM_END_NAMESPACE }
# define SM_NAMESPACE_PREPEND SM_NAMESPACE::
#else
# define SM_USE_NAMESPACE
# define SM_BEGIN_NAMESPACE
# define SM_END_NAMESPACE
# define SM_NAMESPACE_PREPEND
#endif

//#define __cplusplus

#ifdef __cplusplus
extern "C" {
#endif







//========================================================
//             C-part

/*! \defgroup cpart Interface with C programm
    \brief This elements used to integrate SaliMLib into your C-based project
    @{
  */

//!
//! \brief smTickCount System tick counter
//!
//! smTickCount used for implementing timers and other functions for working with time intervals
#ifndef smTickCount
extern volatile int smTickCount;
#endif

//!
//! \brief smInit        Initialise sheduler
//! \param stackCellSize Stack size for root task
//!
void smInit( unsigned stackCellSize );


//! @}

#ifdef __cplusplus
}







//========================================================
//             C++ -part
SM_BEGIN_NAMESPACE

/*! \defgroup CPlusPlusPart SaliMLib core library
 */


/*! \defgroup helperFunctions SaliMLib helper functions
    \ingroup CPlusPlusPart
    \brief helper inline functions are simplify some operations in the library
    @{
*/

//!
//! \brief smDigitToHexLow Converts digit to hex character
//! \param digit           Digit to convert
//! \return                Converted value
//!
inline char smDigitToHexLow( int digit ) { digit &= 0xf; return digit < 10 ? digit + '0' : digit + 'a' - 10; }



//!
//! \brief smAbs Template function to calculate absolute of signed value
//! \param val   Source signed value
//! \return      Absolute of signed value
//!
template <typename SmValue>
inline SmValue smAbs( SmValue val ) { return val < 0 ? -val : val; }



//!
//! \brief smMin Template function to determine minimum from two values
//! \param v1    First value to compare
//! \param v2    Second value to compare
//! \return      Minimum from v1 or v2
//!
template <typename SmValue>
inline SmValue smMin( SmValue v1, SmValue v2 ) { return v1 < v2 ? v1 : v2; }

//!
//! \brief smMax Template function to determine maximum from two values
//! \param v1    First value to compare
//! \param v2    Second value to compare
//! \return      Maximum from v1 or v2
//!
template <class SmValue>
inline SmValue smMax( SmValue v1, SmValue v2 ) { return v1 > v2 ? v1 : v2; }


//!
//! \brief smBound Bounds value val to min and max bounds
//! \param minVal  Minimum of bounds
//! \param val     Value to bounds
//! \param maxVal  Maximum of bounds
//! \return        Value bounded to min and max bounds
//!
template <class SmValue>
inline SmValue smBound( SmValue minVal, SmValue val, SmValue maxVal ) { return val < minVal ? minVal : (val > maxVal ? maxVal : val); }


//!
//! \brief smUpperRound Rounds value val inside bound. For example, if val = 15 and bound = 10 then rounded value will be 5.
//! \param val          Value to be need rounded
//! \param bound        Bound inside round
//! \return             Rounded value
//!
template <class SmValue>
inline SmValue smUpperRound( SmValue val, SmValue bound ) { return val >= bound ? val - bound : val; }

//! @} helperFunctions








/*! \defgroup tickFunctions SaliMLib Tick functions
    \ingroup CPlusPlusPart
    \brief This functions used for work with system tick count
    @{
    */


//!
//! \brief smTickFuture Helper function for calculate moment in the future. Is used for time out calculation.
//! \param tickOut      Time which added to current time to get future moment
//! \return             Time moment in the future
//!
inline
int  smTickFuture( int tickOut ) { return smTickCount + tickOut; }


//!
//! \brief smTickIsOut Helper function to detect when time out. This function indifferent to wrap smTickCount and will work correctly
//! \param futureTick  Moment in the future. When smTickCount rich this moment function will return true
//! \return            true - when smTickCount rich futureTime moment
//!
inline
bool smTickIsOut( int futureTick ) { return (smTickCount - futureTick) >= 0; }

//! @} tickFunctions









/*! \defgroup taskFunctions SaliMLib functions for task creation
    \ingroup CPlusPlusPart
    \brief This functions used for task creation
    @{
    */

//!
//! \brief SmTaskFunction Task function prototype. It take one argument as pointer to void and return nothing.
//!
using SmTaskFunction = void (*)(void *arg);


//!
//! \brief smTaskCreate  Creates new task with stackCellSize stack size and taskFunctor as task entry point
//! \param stackCellSize Task stack size in 32-bit cell
//! \param arg           Param for task, may any or nothing
//! \param taskFunction  Task entry point function
//! \param critic        Define priority level for task. All task devided into two sections: critic tasks and all other.
//!                      Critic task handled as fast as possible and suit for polling tasks.
//!
void smTaskCreate( unsigned stackCellSize, void *arg, SmTaskFunction taskFunction, bool critic = false );




//!
//! \brief smTaskCreateClass Template for automatic conversion of task function argument. It simply converts class pointer cls to void
//!                          for task creation argument and task entry function prototype
//! \param stackCellSize     Task stack size in 32-bit cell
//! \param cls               Class pointer param for task
//! \param taskFunction      Task entry point function with Class pointer as argument
//!
template <class SmClass>
void smTaskCreateClass( unsigned stackCellSize, SmClass *cls, void (*taskFunction)( SmClass *cls ) )
  {
  smTaskCreate( stackCellSize, cls, (SmTaskFunction)(taskFunction) );
  }

//! @} taskFunctions







/*! \defgroup waitFunctions SaliMLib functions for event waiting
    \ingroup CPlusPlusPart
    @{

    */

//!
//! \brief SmWaitFunction Wait function prototype. It take one argument as pointer to void and return bool as result.
//!                       If it return true then task resumed
//!
using SmWaitFunction = bool (*)(void *arg);


//!
//! \brief smWaitVoid   Main wait function. Wait while waitFuncion return true. While task is in wait state cpu switch to other tasks
//! \param arg          Any argument for waitFuncion
//! \param waitFunction Pointer to wait function
//!
void smWaitVoid( void *arg, SmWaitFunction waitFunction );


//!
//! \brief smWait       Template for automatic conversion of waitFunction argument. It simply converts specified pointer to void
//!                     for smWait and waitFunction
//! \param arg          WaitFunction argument
//! \param waitFunction Pointer to wait function
//!
template <class SmArg>
void smWait( SmArg *arg, bool (*waitFunction)( SmArg *farg ) )
  {
  smWaitVoid( arg, (SmWaitFunction)(waitFunction) );
  }


template <class SmArg>
void smWaitRef( SmArg &arg, bool (*waitFunction)( SmArg *farg ) )
  {
  smWaitVoid( &arg, (SmWaitFunction)(waitFunction) );
  }



//!
//! \brief smWaitClass Template for automatic using class operator() member-function for stop wait event.
//!                    smClass must have member-function with prototype "bool operator () () { ... }".
//!                    When this function return true waiting stop and task resumed.
//! \param cls         Any class object which has member-function operator().
//!
template <class SmClass>
void smWaitClass( SmClass cls )
  {
  smWait<SmClass>( &cls, [] ( SmClass *ptr ) -> bool { return ptr(); } );
  }


//!
//! \brief smWaitClassPtr Template for automatic using class operator() member-function for stop wait event.
//!                       SmClass must have member-function with prototype "bool operator () () { ... }".
//!                       When this function return true waiting stop and task resumed.
//! \param cls            Any class object which has member-function operator().
//!
template <class SmClass>
void smWaitClassPtr( SmClass *cls )
  {
  smWait<SmClass>( cls, [] ( SmClass *ptr ) -> bool { return (*ptr)(); } );
  }



//!
//! \brief smWaitBoolTrue Helper function which wait until bool is set to true
//! \param arg            Testing bool
//!
inline void smWaitBoolTrue( bool *arg )
  {
  smWait<bool>( arg, [] ( bool *arg ) -> bool { return *arg; } );
  }



//!
//! \brief smWaitBoolFalse Helper function which wait until bool is set to false
//! \param arg             Testing bool
//!
inline void smWaitBoolFalse( bool *arg )
  {
  smWait<bool>( arg, [] ( bool *arg ) -> bool { return !*arg; } );
  }


//!
//! \brief smWaitIntUntilZero Helper function which wait until int is set to 0
//! \param arg                Testing int
//!
inline void smWaitIntUntilZero( int *arg )
  {
  smWait<int>( arg, [] ( int *arg ) -> bool { return *arg == 0; } );
  }


//!
//! \brief smWaitIntUntilNotZero Helper function which wait until int is set to not 0
//! \param arg                   Tesming int
//!
inline void smWaitIntUntilNotZero( int *arg )
  {
  smWait<int>( arg, [] ( int *arg ) -> bool { return *arg != 0; } );
  }


//!
//! \brief smWaitTick Helper function for waiting specified timeOut. It resume this task after timeOut elapsed
//! \param timeOut    Waiting time out
//!
void smWaitTick( int timeOut );


//!
//! \brief smYeld Simple funtion which switch to another task and resume this task when task loop round
//!
void smYeld();

//! @} waitFunctions








/*! \defgroup mutex SaliMLib Mutex
    \ingroup CPlusPlusPart
    @{

    */

//!
//! \brief The SmMutex class Helper class for guard some resource against sharing
//!
class SmMutex {
    bool mBusy; //!< Variable to indicate resource is busy
  public:
    //!
    //! \brief SmMutex Construct initialy not busy resource
    //!
    SmMutex() : mBusy(false) {}

    //!
    //! \brief isLocked Check if resource is busy
    //! \return         true when resource is busy
    //!
    bool isLocked() const { return mBusy; }

    //!
    //! \brief lock Try lock resource. If resource is busy then it wait until it will be free.
    //!             If resource is free it locked
    //!
    void lock() {
      if( mBusy )
        smWaitBoolFalse( &mBusy );
      mBusy = true;
      }

    //!
    //! \brief unlock Unlocks resource
    //!
    void unlock() { mBusy = false; }
  };



//!
//! \brief The SmMutexLocker class Helper class for automatic lock and unlock resource by using SmMutex
//!
class SmMutexLocker {
    SmMutex &mMutex; //!< Mutex which guarding resource
  public:
    //!
    //! \brief SmMutexLocker Constructor. It get mutex and try lock it. If resource is busy then it wait until it free
    //! \param mutex         Mutex which guarding resource
    //!
    SmMutexLocker( SmMutex &mutex ) : mMutex(mutex) { mMutex.lock(); }

    //!
    //! \brief ~SmMutexLocker Destructor automaticly unlock mutex when leave visibility zone
    //!
    ~SmMutexLocker() { mMutex.unlock(); }
  };

//! @} mutex






/*! \defgroup semaphor SaliMLib Semaphor
    \ingroup CPlusPlusPart
    @{

    */

//!
//! \brief The SmSemaphor class Helper class for guard some resource against multiple sharing
//!
class SmSemaphor {
    int mCount; //!< Variable for indicate resource is busy. Resource is busy when mCount reaches 0. If mCount greater 0 then resource is free.
  public:
    SmSemaphor( int cnt ) : mCount(cnt) {}

    //!
    //! \brief isLocked Check if resource is busy
    //! \return         true when resource is busy
    //!
    bool isLocked() const { return mCount == 0; }

    //!
    //! \brief lock Try lock resource. If resource is busy then it wait until it will be free.
    //!             If resource is free it locked
    //!
    void lock() {
      if( mCount == 0 )
        smWaitIntUntilNotZero( &mCount );
      mCount--;
      }

    //!
    //! \brief unlock Unlocks resource
    //!
    void unlock() { mCount++; }
  };



//!
//! \brief The SmSemaphorLocker class Helper class for automatic lock and unlock resource by using SmSemaphor
//!
class SmSemaphorLocker {
    SmSemaphor &mSemaphor; //!< Semaphor which guarding resource
  public:
    //!
    //! \brief SmSemaphorLocker Constructor. It get semaphor and try lock it. If resource is busy then it wait until it free
    //! \param sema             Semaphor which guarding resource
    //!
    SmSemaphorLocker( SmSemaphor &sema ) : mSemaphor(sema) { mSemaphor.lock(); }

    //!
    //! \brief ~SmSemaphorLocker Destructor automaticly unlock semaphor when leave visibility zone
    //!
    ~SmSemaphorLocker() { mSemaphor.unlock(); }
  };

//! @} semaphor









/*! \defgroup fixedContainers SaliMLib Containers with fixed size
    \ingroup CPlusPlusPart
    @{

    */


//!
//! \brief The SmFixedContainerWait struct Template class to implement waiting for a specified quantity
//!
template <class SmPointer, typename SmValue>
struct SmPointerAndValue {
    SmPointer *mPointer; //!< Pointer to some object
    SmValue    mValue;   //!< Some value

    SmPointerAndValue( SmPointer *pointer, SmValue val ) : mPointer(pointer), mValue(val) {}
  };


template <class SmFixedContainer>
inline void smFixedWaitItem( SmFixedContainer *container )
  {
  if( container->itemCount() == 0 )
    smWait<SmFixedContainer>( container, [] ( SmFixedContainer *q ) -> bool { return q->itemCount() != 0; } );
  }

template <class SmFixedContainer>
inline void smFixedWaitItemCount( SmFixedContainer *container, int count )
  {
  using SmFixedContainerAndValue = SmPointerAndValue<SmFixedContainer,int>;
  if( container->itemCount() < count )
    smWait<SmFixedContainerAndValue>( SmFixedContainerAndValue( container, count ), [] ( SmFixedContainerAndValue *q ) -> bool { return q->mPointer->itemCount() >= q->mValue; } );
  }

template <class SmFixedContainer>
inline void smFixedWaitEmpty( SmFixedContainer *container )
  {
  if( container->emptyCount() == 0 )
    smWait<SmFixedContainer>( container, [] ( SmFixedContainer *q ) -> bool { return q->emptyCount() != 0; } );
  }

template <class SmFixedContainer>
inline void smFixedWaitEmptyCount( SmFixedContainer *container, int count )
  {
  using SmFixedContainerAndValue = SmPointerAndValue<SmFixedContainer,int>;
  if( container->emptyCount() < count )
    smWait<SmFixedContainerAndValue>( SmFixedContainerAndValue( container, count ), [] ( SmFixedContainerAndValue *q ) -> bool { return q->mPointer->emptyCount() >= q->mValue; } );
  }



//!
//! \brief The SmFixedQueue Templace class for building queue with fixed size
//!
template <class Item, int queueSize>
class SmFixedQueue {
    using SmFixedQueueObject = SmFixedQueue<Item,queueSize>;

    int  mHead;              //!< Index to extract Item
    int  mTail;              //!< Index to append Item
    Item mBuffer[queueSize]; //!< Item buffer
  public:
    SmFixedQueue() : mHead(0), mTail(0) {}

    //!
    //! \brief itemCount Returns item count in the queue (Common fixedContainer interface)
    //! \return          Item count in the queue
    //!
    int   itemCount() const { return mHead <= mTail ? mTail - mHead : queueSize - mHead + mTail; }

    //!
    //! \brief emptyCount Returns count of free places in the queue (Common fixedContainer interface)
    //! \return           Count of free places
    //!
    int   emptyCount() const { return queueSize - 1 - itemCount(); }

    //!
    //! \brief clear Clear queue contents (Common fixedContainer interface)
    //!
    void  clear() { mHead = mTail = 0; }

    //!
    //! \brief at    Return item at index beginning from mHead. index value must not exceed elements count (Common fixedContainer interface)
    //! \param index index is value from 0 to count. When index eq 0 then return head element
    //! \return      Element with index
    //!
    Item &at( int index ) { return mBuffer[smUpperRound(mHead + index, queueSize)]; }

    //!
    //! \brief waitItem Waits until there is at least one element in the container (Common fixedContainer interface)
    //!
    void  waitItem() { smFixedWaitItem<SmFixedQueueObject>( this ); }

    //!
    //! \brief waitItem Waits until there is at least count elements in the container (Common fixedContainer interface)
    //!
    void  waitItemCount( int count ) { smFixedWaitItemCount<SmFixedQueueObject>( this, count ); }

    //!
    //! \brief waitEmpty Waits until there is space for at least one element (Common fixedContainer interface)
    //!
    void  waitEmpty() { smFixedWaitEmpty<SmFixedQueueObject>( this ); }

    //!
    //! \brief waitEmptyCount Waits until there is space for at least count elements (Common fixedContainer interface)
    //!
    void  waitEmptyCount( int count ) { smFixedWaitEmptyCount<SmFixedQueueObject>( this, count ); }

    //!
    //! \brief waitContinueItemWaits until there is at least one element as continued block in the container
    //!
    void  waitContinueItem() {
      if( continueCount() == 0 )
        smWait<SmFixedQueueObject>( this, [] ( SmFixedQueueObject *q ) -> bool { return q->continueCount() != 0; } );
      }

    //!
    //! \brief head Return head element
    //! \return     Element at head
    //!
    Item &head() { return mBuffer[mHead]; }

    //!
    //! \brief deque Retrieves an item from the queue
    //! \return      Retrived item
    //!
    Item  deque() { waitItem(); return mBuffer[headNext()]; }

    //!
    //! \brief enque Puts an item in the queue
    //! \param item  Item to put
    //!
    void  enque( Item item ) { waitEmpty(); mBuffer[tailNext()] = item; }

    //!
    //! \brief continueCount Returns the size of a continuous section
    //! \return              Size of a continuous section
    //!
    int   continueCount() const { return mHead <= mTail ? mTail - mHead : queueSize - mHead;}

    //!
    //! \brief continueBuffer Returns pointer to a continuous section
    //! \return               Pointer to a continuous section
    //!
    Item *continueBuffer() { return mBuffer + mHead; }

    //!
    //! \brief continueDeque Remove block of count elements from queue
    //! \param count         Count of removed elements
    //!
    void  continueDeque( int count ) { mHead = smUpperRound( mHead + count, queueSize ); }

  private:
    int   headNext() { int ptr = mHead; mHead = smUpperRound( mHead + 1, queueSize ); return ptr; }

    int   tailNext() { int ptr = mTail; mTail = smUpperRound( mTail + 1, queueSize ); return ptr; }
  };





//!
//! \brief The SmFixedStack class
//!
template <class Item, int stackSize>
class SmFixedStack {
    using SmFixedStackObject = SmFixedStack<Item,stackSize>;

    int  mTop;               //!< Top index of stack
    Item mBuffer[stackSize]; //!< Stack buffer
  public:
    SmFixedStack() : mTop(stackSize) {}


    //!
    //! \brief itemCount Returns item count in the stack (Common fixedContainer interface)
    //! \return          Item count in the stack
    //!
    int   itemCount() const { return stackSize - mTop; }

    //!
    //! \brief emptyCount Returns count of free places in the stack (Common fixedContainer interface)
    //! \return           Count of free stack
    //!
    int   emptyCount() const { return mTop; }

    //!
    //! \brief clear Clear stack contents (Common fixedContainer interface)
    //!
    void  clear() { mTop = stackSize; }

    //!
    //! \brief at    Return item at index beginning from top of stack. index value must not exceed elements count (Common fixedContainer interface)
    //! \param index index is value from 0 to count. When index eq 0 then return top stack element
    //! \return      Element with index
    //!
    Item &at( int index ) { return mBuffer[mTop + index]; }

    //!
    //! \brief waitItem Waits until there is at least one element in the container (Common fixedContainer interface)
    //!
    void  waitItem() { smFixedWaitItem<SmFixedStackObject>( this ); }

    //!
    //! \brief waitItem Waits until there is at least count elements in the container (Common fixedContainer interface)
    //!
    void  waitItemCount( int count ) { smFixedWaitItemCount<SmFixedStackObject>( this, count ); }

    //!
    //! \brief waitEmpty Waits until there is space for at least one element (Common fixedContainer interface)
    //!
    void  waitEmpty() { smFixedWaitEmpty<SmFixedStackObject>( this ); }

    //!
    //! \brief waitEmptyCount Waits until there is space for at least count elements (Common fixedContainer interface)
    //!
    void  waitEmptyCount( int count ) { smFixedWaitEmptyCount<SmFixedStackObject>( this, count ); }

    //!
    //! \brief head Return top stack element
    //! \return     Element at top of stack
    //!
    Item &top() const { mBuffer[mTop]; }

    //!
    //! \brief pop Pop item from stack
    //! \return    Item from top of stack
    //!
    Item pop() {
      waitItem();
      return mBuffer[mTop++];
      }

    //!
    //! \brief push Push item on to stack
    //! \param item Pushed item
    //!
    void  push( Item item ) {
      waitEmpty();
      mBuffer[--mTop] = item;
      }


  };




//!
//! \brief The SmFixedBuffer class
//!
template <class Item, int bufferSize>
class SmFixedBuffer {
    using SmFixedBufferObject = SmFixedBuffer<Item,bufferSize>;

    int  mCount;              //!< Element count
    Item mBuffer[bufferSize]; //!< Element buffer
  public:
    SmFixedBuffer() : mCount(0) {}



    //!
    //! \brief itemCount Returns item count in the buffer (Common fixedContainer interface)
    //! \return          Item count in the buffer
    //!
    int   itemCount() const { return mCount; }

    //!
    //! \brief emptyCount Returns count of free places in the buffer (Common fixedContainer interface)
    //! \return           Count of free places
    //!
    int   emptyCount() const { return bufferSize - mCount; }

    //!
    //! \brief clear Clear buffer contents (Common fixedContainer interface)
    //!
    void  clear() { mCount = 0; }

    //!
    //! \brief at    Return item at index beginning from begin of buffer. index value must not exceed elements count (Common fixedContainer interface)
    //! \param index index is value from 0 to count. When index eq 0 then return first element
    //! \return      Element with index
    //!
    Item &at( int index ) { return mBuffer[index]; }

    //!
    //! \brief waitItem Waits until there is at least one element in the container (Common fixedContainer interface)
    //!
    void  waitItem() { smFixedWaitItem<SmFixedBufferObject>( this ); }

    //!
    //! \brief waitItem Waits until there is at least count elements in the container (Common fixedContainer interface)
    //!
    void  waitItemCount( int count ) { smFixedWaitItemCount<SmFixedBufferObject>( this, count ); }

    //!
    //! \brief waitEmpty Waits until there is space for at least one element (Common fixedContainer interface)
    //!
    void  waitEmpty() { smFixedWaitEmpty<SmFixedBufferObject>( this ); }

    //!
    //! \brief waitEmptyCount Waits until there is space for at least count elements (Common fixedContainer interface)
    //!
    void  waitEmptyCount( int count ) { smFixedWaitEmptyCount<SmFixedBufferObject>( this, count ); }


    //!
    //! \brief append Append one item to end of buffer
    //! \param item   Item to append
    //!
    void  append( Item item ) {
      //Wait for emty space for one item
      waitEmpty();
      //Place item
      mBuffer[mCount++] = item;
      }

    //!
    //! \brief append Append block of count items to end of buffer
    //! \param items  Pointer to block of items
    //! \param count  Count of appended items
    //!
    void  append( Item *items, int count ) {
      //Wait for emty space for count items
      waitEmptyCount( count );
      //Place items
      while( count-- )
        mBuffer[mCount++] = *items++;
      }

    //!
    //! \brief insert Inserts one item in specified position of buffer
    //! \param item   Inserted item
    //! \param pos    Position to which item will be inserted
    //!
    void  insert( Item item, int pos ) {
      //Wait for emty space for one item
      waitEmpty();
      mCount++;
      //Free space to item. We shift all right-stand items to one position to right
      for( int i = mCount - 1; i > pos; i-- )
        mBuffer[i] = mBuffer[i - 1];
      //Place item
      mBuffer[pos] = item;
      }

    //!
    //! \brief insert Inserts block of count items in specified position of buffer
    //! \param items  Inserted item block
    //! \param pos    Position to which item block will be inserted
    //! \param count  Count of inserted items
    //!
    void  insert( Item *items, int pos, int count ) {
      //Wait for emty space for count items
      waitEmptyCount(count);
      mCount += count;
      //Free space to item block. We shift all right-stand items to count positions to right
      for( int i = mCount - 1; i > pos; i-- )
        mBuffer[i] = mBuffer[i - 1];
      //Place items
      while( count-- )
        mBuffer[pos++] = *items++;
      }

    //!
    //! \brief remove Removes one item from specified position of buffer
    //! \param pos    Position of removed item
    //!
    void  remove( int pos ) {
      waitItem();
      for( pos++; pos < mCount; pos++ )
        mBuffer[pos - 1] = mBuffer[pos];
      mCount--;
      }

    //!
    //! \brief remove Removes block of items begining from specified position of buffer
    //! \param pos    Position of removed item block
    //! \param count  Count of removed items
    //!
    void  remove( int pos, int count ) {
      waitItemCount( count );
      for( pos += count; pos < mCount; pos++ )
        mBuffer[pos - count] = mBuffer[pos];
      mCount -= count;
      }

  };

//! @} fixedContainers







/*! \defgroup containerAlgorithms SaliMLib Algorithms for fixed-size containers
    \ingroup CPlusPlusPart
    @{

    */

//!
//! \brief The SmContainerItemWaiter class
//!
template<class SmItem, class SmContainer>
class SmContainerItemWaiter {
    using SmContainerItemWaiterObject = SmContainerItemWaiter<SmItem,SmContainer>;

    SmContainer &mContainer; //!< Container
    int          mLastCount; //!< Last count of scanned items in container
    int          mCountMax;  //!< Max count of scanned items
    SmItem       mItemEoln;  //!< Item end of line representation
  public:
    SmContainerItemWaiter( SmItem item, SmContainer &container, int countMax ) : mContainer(container), mLastCount(0), mCountMax(countMax), mItemEoln(item) {}

    int  countNetto() const { return mLastCount; }

    int  countBrutto() const { return mLastCount + 1; }

    bool isMaxReached() const { return mLastCount == mCountMax; }

    bool operator () () {
      while( mLastCount < mContainer.itemCount() ) {
        if( mLastCount >= mCountMax ) return true;
        if( mContainer.at(mLastCount) == mItemEoln ) return true;
        else mLastCount++;
        }
      return false;
      }

    void wait() {
      smWaitClassPtr<SmContainerItemWaiterObject>( this );
      }
  };

//! @} containerAlgorithms



SM_END_NAMESPACE

#endif


#endif // SALIMCORE_H
