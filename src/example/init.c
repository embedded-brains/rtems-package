/* SPDX-License-Identifier: BSD-2-Clause */

/*
 * Copyright (C) 2020 embedded brains GmbH & Co. KG
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems.h>
#include <rtems/bspIo.h>

/*
 * This example application shows an RTEMS configuration which only uses
 * statically allocated resources.  The initialization task sends messages to a
 * message consumer task which prints out the message content (one character)
 * via rtems_putc().  The example uses up to two processors.
 */

#define ASSERT_SUCCESS( sc ) \
  do { \
    if ( ( sc ) != RTEMS_SUCCESSFUL ) { \
      rtems_fatal( RTEMS_FATAL_SOURCE_EXIT, __LINE__ ); \
    } \
  } while ( 0 )

#define MAX_TLS_SIZE RTEMS_ALIGN_UP( 64, RTEMS_TASK_STORAGE_ALIGNMENT )

#define TASK_ATTRIBUTES RTEMS_DEFAULT_ATTRIBUTES

#define TASK_STORAGE_SIZE \
  RTEMS_TASK_STORAGE_SIZE( \
    MAX_TLS_SIZE + RTEMS_MINIMUM_STACK_SIZE, \
    TASK_ATTRIBUTES \
  )

#define MSG_MAX_PENDING 4

#define MSG_SIZE sizeof( char )

static const char hello_world[] = "Hello, world!\n";

static rtems_id putc_done;

static rtems_id msg_queue;

RTEMS_ALIGNED( RTEMS_TASK_STORAGE_ALIGNMENT )
static char msg_task_storage[ TASK_STORAGE_SIZE ];

static const rtems_task_config msg_task_config = {
  .name = rtems_build_name( 'R', 'U', 'N', ' ' ),
  .initial_priority = 2,
  .storage_area = msg_task_storage,
  .storage_size = sizeof( msg_task_storage ),
  .maximum_thread_local_storage_size = MAX_TLS_SIZE,
  .initial_modes = RTEMS_DEFAULT_MODES,
  .attributes = TASK_ATTRIBUTES
};

static RTEMS_MESSAGE_QUEUE_BUFFER( MSG_SIZE ) msg_buffers[ MSG_MAX_PENDING ];

static const rtems_message_queue_config msg_queue_config = {
  .name = rtems_build_name( 'M', 'S', 'G', 'Q' ),
  .maximum_pending_messages = RTEMS_ARRAY_SIZE( msg_buffers ),
  .maximum_message_size = MSG_SIZE,
  .storage_area = msg_buffers,
  .storage_size = sizeof( msg_buffers ),
  .attributes = RTEMS_DEFAULT_ATTRIBUTES
};

static void msg_task( rtems_task_argument arg )
{
  (void) arg;

  while ( true ) {
    rtems_status_code sc;
    char c;
    size_t n;

    n = 0;
    sc = rtems_message_queue_receive(
      msg_queue,
      &c,
      &n,
      RTEMS_WAIT,
      RTEMS_NO_TIMEOUT
    );
    ASSERT_SUCCESS( sc );

    if ( n != sizeof( char ) ) {
      rtems_fatal( RTEMS_FATAL_SOURCE_EXIT, __LINE__ );
    }

    rtems_putc( c );

    sc = rtems_semaphore_release( putc_done );
    ASSERT_SUCCESS( sc );
  }
}

static void wait_putc_done( void )
{
  rtems_status_code sc;

  sc = rtems_semaphore_obtain( putc_done, RTEMS_WAIT, RTEMS_NO_TIMEOUT );
  ASSERT_SUCCESS( sc );
}

static void Init( rtems_task_argument arg )
{
  rtems_id id;
  rtems_status_code sc;
  const char *c;
  size_t putc_count;

  (void) arg;

  sc = rtems_semaphore_create(
    rtems_build_name( 'P', 'U', 'T', 'C' ),
    0,
    RTEMS_COUNTING_SEMAPHORE,
    0,
    &putc_done
  );
  ASSERT_SUCCESS( sc );

  sc = rtems_message_queue_construct( &msg_queue_config, &msg_queue );
  ASSERT_SUCCESS( sc );

  sc = rtems_task_construct( &msg_task_config, &id );
  ASSERT_SUCCESS( sc );

  sc = rtems_task_start( id, msg_task, 0 );
  ASSERT_SUCCESS( sc );

  c = hello_world;
  putc_count = 0;

  while ( *c != '\0' ) {
    if ( putc_count >= MSG_MAX_PENDING ) {
      wait_putc_done();
      --putc_count;
    }

    sc = rtems_message_queue_send( msg_queue, c, sizeof( *c ) );
    ASSERT_SUCCESS( sc );

    ++c;
    ++putc_count;
  }

  while ( putc_count > 0 ) {
    wait_putc_done();
    --putc_count;
  }

  rtems_fatal( RTEMS_FATAL_SOURCE_EXIT, 0 );
}

#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MAXIMUM_PROCESSORS 2

#define CONFIGURE_MAXIMUM_BARRIERS 3

#define CONFIGURE_MAXIMUM_MESSAGE_QUEUES 3

#define CONFIGURE_MAXIMUM_PARTITIONS 3

#define CONFIGURE_MAXIMUM_PERIODS 3

#define CONFIGURE_MAXIMUM_SEMAPHORES 3

#define CONFIGURE_MAXIMUM_TASKS 3

#define CONFIGURE_MINIMUM_TASKS_WITH_USER_PROVIDED_STORAGE \
  CONFIGURE_MAXIMUM_TASKS

#define CONFIGURE_MAXIMUM_TIMERS 3

#define CONFIGURE_MAXIMUM_USER_EXTENSIONS 3

#define CONFIGURE_MICROSECONDS_PER_TICK 1000

#define CONFIGURE_MAXIMUM_FILE_DESCRIPTORS 0

#define CONFIGURE_DISABLE_NEWLIB_REENTRANCY

#define CONFIGURE_APPLICATION_DISABLE_FILESYSTEM

#define CONFIGURE_MAXIMUM_THREAD_LOCAL_STORAGE_SIZE MAX_TLS_SIZE

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INIT_TASK_ATTRIBUTES TASK_ATTRIBUTES

#define CONFIGURE_INIT_TASK_INITIAL_MODES RTEMS_DEFAULT_MODES

#define CONFIGURE_INIT_TASK_CONSTRUCT_STORAGE_SIZE TASK_STORAGE_SIZE

#define CONFIGURE_IDLE_TASK_STORAGE_SIZE \
  ( MAX_TLS_SIZE + RTEMS_MINIMUM_STACK_SIZE )

#define CONFIGURE_INIT

#include <rtems/confdefs.h>
