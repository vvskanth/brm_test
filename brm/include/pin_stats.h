/*
 *	@(#)%Portal Version: pin_stats.h:PlatformR2Int:3:2006-Sep-12 03:27:56 %
 *      
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *      
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */
#ifndef _pin_stats_h_
#define _pin_stats_h_

#ifndef    _pin_os_locks_h_
  #include "pin_os_locks.h"
#endif
#ifndef    _pin_os_time_h_
  #include "pin_os_time.h"
#endif

/* Note: ts is used as an abbreviation for timestamp. */
typedef struct pin_stats_total_ops_per_thread {
	  /* first_ts
	   *    The timestamp of the first time the opcode was called.
	   */
	hrtime_t                first_ts;
	  /* last_ts
	   *    The timestamp of the last time the opcode was called.
	   */
	hrtime_t                last_ts;
	  /* total_cnt
	   *    The number of times this opcode has been called.
	   */
	u_int64                 total_cnt;
	  /* total_err_cnt
	   *    The total number of errors returned.
	   */
	u_int64                 total_err_cnt;
	  /* max_latency
	   *    The slowest elapsed time that any opcode took to complete
	   *    without returning an error. This is in nano-seconds.
	   */
	hrtime_t                max_latency;
	  /* max_latency_ts
	   *    The timestamp of when the opcode was recvd that had the
	   *    max latency.
	   */
	hrtime_t                max_latency_ts;
	  /* max_latency_opcode
	   *    The opcode that had the max latency.
	   */
	pin_opcode_t            max_latency_opcode;
} pin_stats_total_ops_per_thread_t;

/*
 * Performance testing shows that setting the individual
 * values is actually faster than using memset.
 */
#define PIN_STATS_TOTAL_OPS_PER_THREAD_T_CLEAR(ptr)     \
	(ptr)->first_ts = 0;                            \
	(ptr)->last_ts = 0;                             \
	(ptr)->total_cnt = 0;                           \
	(ptr)->total_err_cnt = 0;                       \
	(ptr)->max_latency = 0;                         \
	(ptr)->max_latency_ts = 0;                      \
	(ptr)->max_latency_opcode = 0;

#define PIN_MAX_QOS_BUCKETS_PER_OPCODE    8

/* One of these structures for each opcode */
typedef struct pin_opcode_qos {
	  /* next
	   *    This is a pointer used to make this a linked-list.
	   *    If not null, then this points to the next entry in the list.
	   *    A null value indicates the end of the list.
	   */
	struct pin_opcode_qos*  next;
	  /* opcode
	   *    The opcode related to this data.
	   */
	pin_opcode_t            opcode;
	  /* mutex
	   *    This is used to allow this data to be updated by
	   *    multiple threads at the same time.
	   */
	spin_lock_type          mutex;
	  /* base_ts
	   *   The starting timestamp of this interval.
	   */
	hrtime_t                base_ts;
	  /* total_cnt
	   *    The number of times this opcode has been called.
	   */
	u_int64                 total_cnt;
	  /* total_err_cnt
	   *   The number of errors this opcode returned an error.
	   */
	u_int64                 total_err_cnt;
	  /* min_latency
	   *   The fastest elapsed time that this opcode took to complete
	   *   without returning an error. This is in nano-seconds.
	   */
	hrtime_t                min_latency;
	  /* min_latency_ts
	   *   The timestamp of when the opcode was called that had the
	   *   min latency.
	   */
	hrtime_t                min_latency_ts;
	  /* max_latency
	   *   The slowest elapsed time that this opcode took to complete
	   *   without returning an error.  This is in nano-seconds.
	   */
	hrtime_t                max_latency;
	  /* max_latency_ts
	   *   The timestamp of when the opcode was called that had the 
	   *   max latency.
	   */
	hrtime_t                max_latency_ts;
	  /* total_latency
	   *   The total latency of all successful calls to this opcode.
	   *   Doesn't include the ones that returned an error.
	   *   This is in nano-seconds.
	   */
	hrtime_t                total_latency;
	  /* max_latency_input_flistp
	   *   This is the input flist that had the maximum latency.
	   */
	pin_flist_t*            max_latency_in_flistp;
	  /* qos_bucket_cnt
	   *   The number of active QOS buckets for this opcode.
	   */
	int32                   qos_bucket_cnt;
	  /* qos_cnts
	   *   This array contains the number of times the response time
	   *   of a successful call is less than or equal to the
	   *   corresponding qos_ns_times array entry.
	   */
	u_int64                 qos_cnts[PIN_MAX_QOS_BUCKETS_PER_OPCODE];
	  /* qos_ns_times
	   *   This array contains the maximum times in nano-seconds for
	   *   each QOS bucket. For example, let's say bucket is 0 and
	   *   qos_ns_times[0] is 5,000,000 (5 milli-seconds).
	   *   Then qos_cnts[0] will contain the number of times the
	   *   response time was less than or equal to 5 milli-seconds.
	   *   If qos_ns_times[1] is 10,000,000 (10 milli-seconds),
	   *   then qos_cnts[1] will contain the number of times the
	   *   response time was greater than 5 milli-seconds and
	   *   less than of equal to 10 milli-seconds.
	   */
	hrtime_t                qos_ns_times[PIN_MAX_QOS_BUCKETS_PER_OPCODE];
	  /* first_ts
	   *   The first recvd opcode timestamp.
	   */
	hrtime_t                first_ts;
	  /* last_ts
	   *   The last recvd opcode timestamp.
	   */
	hrtime_t                last_ts;
} pin_opcode_qos_t;

/*
 * Performance testing shows that setting the individual
 * values is actually faster than using memset.
 */
#define PIN_OPCODE_QOS_T_CLEAR(ptr)                     \
	(ptr)->next = NULL;                             \
	(ptr)->opcode = 0;                              \
	(ptr)->base_ts = 0;                             \
	(ptr)->total_cnt = 0;                           \
	(ptr)->total_err_cnt = 0;                       \
	(ptr)->min_latency = PIN_MAX_HRTIME_T;          \
	(ptr)->min_latency_ts = 0;                      \
	(ptr)->max_latency = 0;                         \
	(ptr)->max_latency_ts = 0;                      \
	(ptr)->total_latency = 0;                       \
	(ptr)->max_latency_in_flistp = NULL;            \
	(ptr)->qos_bucket_cnt = 0;                      \
	(ptr)->first_ts = 0;                            \
	(ptr)->last_ts = 0;
#endif
