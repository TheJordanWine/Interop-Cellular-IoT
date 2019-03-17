/**
 * Meter Main Application File for C:  MeterMain.c
 *
 * This file is the main source code file for the client application.
 * It utilizes the OS-IoT-BG96 C library to communicate using oneM2M
 * standards for a cellular network.  This class represents the Utility
 * Meter reader and stores values related to such.
 *
 * Instructions:
 *     See docs/client.md for the instructions on how to compile and execute
 *         this client application.
 *
 * @author Team 6
 *
 * @version 1.0
 */

// Imports
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tx_api.h>
#include "onem2m_http.h"

/**
* These are example addresses
*/
#define HEAP_START 0x10008000
#define HEAP_END 0x1000FFFF

// Use built-in atomicc compare and swap routine.
#define atomic_compare_and_swap __sync_val_compare_and_swap

// Define functions
static void * init_malloc(size_t size);
static void * do_malloc(size_t size);

/**
 * Declarations
 */

// Initialize ThreadX internal memory pool stucture
static TX_BYTE_POOL malloc_pool_ = {0};

/*
* Flag that is used in do_malloc() to cause competing threads to wait until
* initialization is completed before allocating memory.
*/
volatile static bool initialized_ = false;

// initialize volatile function for malloc pointer
volatile malloc_ptr_t p_malloc = &init_malloc;

// Set parameter values
const char* from = "FromValue";   // From Value
const char* host = "127.0.0.1";   // Server host IP value
uint16_t port = 8080;             // Server host Port values
const size_t malloc_size = 9001;  // Malloc allocation size

/**
  * Main function, program entry point.
  */
int main (int argc, char* argv[]) {

  // Set dynamic memory allocation functions using ThreadX BytePool resource
  onem2m_set_allocation_functions( init_malloc(size) , free() );

  // Start HTTP service
  onem2m_http_start();

  // Start an HTTP session
  onem2m_http_sess_start();

  // Configure the from value
  onem2m_set_from( from );

  // Start an HTTP connection
  onem2m_http_connect( host , port );

  // Create meter resource object

  // Perform oneM2M operations using onem2m_http_retrieve(),
  // onem2m_http_create(), and onem2m_http_update()

  // Use the callback function provided to process the results of the operation

  // Once the callback has been processed release the HTTP connection
  if (true) {
    onem2m_http_disconnect();
  }

  // HTTP sessions can be kept open for reuse or closed
  if (false) {
    onem2m_http_sess_end();

  }

} // End of main function

/**
  * Initialize malloc of size
  */
void * init_malloc(size_t size)
{
	assert(size > 0);

	uintptr_t heap_start = (uintptr_t)HEAP_START;
	uintptr_t heap_size = (uintptr_t)HEAP_END - heap_start;

	/**
	* Check the current value of the malloc pointer
	* If it's still init_malloc, swap the value to do_malloc and return true.
	* If the value is do_malloc, another thread has priority; run do_malloc(),
  * skipping initialization.
	*/
	if(atomic_compare_and_swap(&p_malloc, &init_malloc, &do_malloc))
	{
		uint8_t r;

		/**
    * ThreadX's API to create a byte pool using a memory block.
		*/
		r = tx_byte_pool_create(&malloc_pool_, "Heap Memory Pool",
				(void *)heap_start,
				heap_size);
		assert(r == TX_SUCCESS);

		//Signal to any threads waiting on do_malloc that this thread is done
		initialized_ = true;
	}

	/*
	* Two situations happen here:
	* 1) malloc initialized on the first call, and then passed through to normal malloc
	*	2) two threads raced to init. One initializes, and the other falls through to malloc
	*/
	return do_malloc(size);
}

/**
  * Function to act upon malloc. Used in init_malloc(size_t size)
  */
void * do_malloc(size_t size)
{
	void * ptr = NULL;

	/**
	* Since multiple threads could be racing to malloc, if this thread lost the race,
	* make sure the ThreadX pool has been created before memory is allocated
	*/
	while(!initialized_)
	{
		tx_thread_sleep(1);
	}

	if(size > 0)
	{
		// Wrap the threadX call into a standard form
		uint8_t r = tx_byte_allocate(&malloc_pool_, &ptr, size, TX_WAIT_FOREVER);

		assert(r == TX_SUCCESS && "malloc failed");
	} //else NULL if there was an error

	return ptr;
}

/**
  * Function to free malloc.
  */
void free(void * ptr)
{
	//free should not be called before malloc is init'd
	assert(initialized_);

	if(ptr) {
		//Wrap the threadX call into a standard form
		uint8_t r = tx_byte_release(ptr);
		ptr = NULL;
		assert(r == TX_SUCCESS);
	}
}
