/* SPDX-License-Identifier: BSD-2-Clause */

/*
 * Copyright (C) 2020, 2025 embedded brains GmbH & Co. KG
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

#include <rtems.h>

#ifndef _APPL_CONFIG_H
#define _APPL_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef RTEMS_SMP
#define APPL_PROCESSOR_COUNT 32
#else
#define APPL_PROCESSOR_COUNT 1
#endif

#define TASK_COUNT_PER_CPU 10

#define APPL_TASK_COUNT (1 + TASK_COUNT_PER_CPU * APPL_PROCESSOR_COUNT)

/* Defines the maximum thread-local storage size for this application */
#define APPL_MAX_TLS_SIZE RTEMS_ALIGN_UP(64, RTEMS_TASK_STORAGE_ALIGNMENT)

/*
 * The task storage size depends on task attributes.  This setting is used by
 * the application.
 */
#define APPL_TASK_ATTRIBUTES RTEMS_FLOATING_POINT

/*
 * Defines the task storage size for this application.  Uses two times the
 * APPL_MAX_TLS_SIZE to account for an architecture-specific thread-local
 * storage control block.
 */
#define APPL_TASK_STORAGE_SIZE                                                                     \
	RTEMS_TASK_STORAGE_SIZE(2 * APPL_MAX_TLS_SIZE + RTEMS_MINIMUM_STACK_SIZE,                  \
				APPL_TASK_ATTRIBUTES)

#define APPL_NAME "EXAMPLE TRACE"

/*
 * This option enables the event recording.  For each configured processor, a
 * ring buffer for event records is statically allocated.  Each ring buffer has
 * an element count configured by this option.
 */
#define CONFIGURE_RECORD_PER_PROCESSOR_ITEMS 1024

/*
 * This option enables the generation of event records for thread related
 * events.  This includes thread create, start, restart, delete, switch, begin,
 * exitted and terminate events.
 */
#define CONFIGURE_RECORD_EXTENSIONS_ENABLED

/*
 * This option enables the generation of event records for interrupt
 * entry and exit.
 */
#define CONFIGURE_RECORD_INTERRUPTS_ENABLED

/*
 * This option enables that the event records are dumped in a fatal error
 * handler.  The event records are serialized using a base64 encoding.
 */
#define CONFIGURE_RECORD_FATAL_DUMP_BASE64

int main(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _APPL_CONFIG_H */
