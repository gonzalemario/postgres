/*-------------------------------------------------------------------------
 *
 * instrument_node.h
 *	  Definitions for node-specific instrumentation
 *
 *
 * Portions Copyright (c) 1996-2025, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * src/include/executor/instrument_node.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef INSTRUMENT_NODE_H
#define INSTRUMENT_NODE_H



/* ---------------------
 * per-worker aggregate information
 * ---------------------
 */
typedef struct AggregateInstrumentation
{
	Size	hash_mem_peak; /* peak hash table memory usage */
	uint64	hash_disk_used; /*kB of disk space used */
	int		hash_batches_used; /* batches used during entire execution */
} AggregateInstrumentation;

/* ----------------
 *    Shared memory container for per-worker aggregate information
 * ----------------
 */
typedef struct SharedAgginfo
{
	int	num_workers;
	AggregateInstrumentation sinstrument[FLEXIBLE_ARRAY_MEMBER];
} SharedAggInfo;
/*
 * Struct for statistics maintained by amgettuple and amgetbitmap
 *
 * Note: IndexScanInstrumentation can't contain any pointers, since it is
 * copied into a SharedIndexScanInstrumentation during parallel scans
 */
typedef struct IndexScanInstrumentation
{
	/* Index search count (incremented with pgstat_count_index_scan call) */
	uint64		nsearches;
} IndexScanInstrumentation;

/*
 * Struct for every worker's IndexScanInstrumentation, stored in shared memory
 */
typedef struct SharedIndexScanInstrumentation
{
	int			num_workers;
	IndexScanInstrumentation winstrument[FLEXIBLE_ARRAY_MEMBER];
} SharedIndexScanInstrumentation;

/*
 *	 BitmapHeapScanInstrumentation information
 *
 *		exact_pages		   total number of exact pages retrieved
 *		lossy_pages		   total number of lossy pages retrieved
 */
typedef struct BitmapHeapScanInstrumentation
{
	uint64		exact_pages;
	uint64		lossy_pages;
} BitmapHeapScanInstrumentation;

/*
 *	 Instrumentation data for a parallel bitmap heap scan.
 *
 * A shared memory struct that each parallel worker copies its
 * BitmapHeapScanInstrumentation information into at executor shutdown to
 * allow the leader to display the information in EXPLAIN ANALYZE.
 */
typedef struct SharedBitmapHeapInstrumentation
{
	int			num_workers;
	BitmapHeapScanInstrumentation sinstrument[FLEXIBLE_ARRAY_MEMBER];
} SharedBitmapHeapInstrumentation;


typedef struct MemoizeInstrumentation
{
	uint64 cache_hits;		/* number of rescans where we've found the
							 * scan parameters values to be cached */
	uint64 cache_misses;	/* number of rescans where we've not found the
							 * scan parameters values to be cached */
	uint64 cache_evictions;	/* number of cache entries removed due to
							 * the need to free memory */
	uint64 cache_overflows;	/* number of times we've had to bypass the
							 * cache when filling it due to not being
							 * able to free enough space to store the
							 * current scan's tuples */
	uint64	mem_peak;		/* peak memory usage in bytes */

} MemoizeInstrumentation;

/* ----------------
 *  Shared memory container for per-worker memoize information
 *  ----------------
 */
typedef struct SharedMemoizeInfo
{
	int			num_workers;
	MemoizeInstrumentation sinstrument[FLEXIBLE_ARRAY_MEMBER];
} SharedMemoizeInfo;

typedef enum
{
	SORT_SPACE_TYPE_DISK,
	SORT_SPACE_TYPE_MEMORY,
} TuplesortSpaceType;

typedef enum
{
	SORT_TYPE_STILL_IN_PROGRESS = 0,
	SORT_TYPE_TOP_N_HEAPSORT = 1 << 0,
	SORT_TYPE_QUICKSORT = 1 << 1,
	SORT_TYPE_EXTERNAL_SORT = 1 << 2,
	SORT_TYPE_EXTERNAL_MERGE = 1 << 3
} TuplesortMethod;

typedef struct TuplesortInstrumentation
{
	TuplesortMethod sortMethod;		/* sort algorithm used */
	TuplesortSpaceType spaceType;	/* type of space spaceUsed represents */
	int64       spaceUsed;			/* space consumption, in kB */
} TuplesortInstrumentation;

/* ----------------
 *  Shared memory container for per-worker sort information
 *  ----------------
 */
typedef struct SharedSortInfo
{
	int num_workers;
	TuplesortInstrumentation sinstrument[FLEXIBLE_ARRAY_MEMBER];
} SharedSortInfo;

/* ----------------
 *   Values displayed by EXPLAIN ANALYZE
 * ----------------
 */
typedef struct HashInstrumentation
{
	int		nbuckets;			/* number of buckets at end of execution */
	int		nbuckets_original;	/* planned number of buckets */
	int		nbatch;				/* number of batches at end of execution */
	int		nbatch_original;	/* planned number of batches */
	Size	space_peak;			/* peak memory usage in bytes */
} HashInstrumentation;

/* ----------------
 *   Shared memory container for per-worker hash information
 * ----------------
 */
typedef struct SharedHashInfo
{
	int			num_workers;
	HashInstrumentation hinstrument[FLEXIBLE_ARRAY_MEMBER];
} SharedHashInfo;

/* ----------------
 *   Instrumentation information for IncrementalSort
 * ----------------
 */
typedef struct IncrementalSortGroupInfo
{
	int64		groupCount;
	int64		maxDiskSpaceUsed;
	int64		totalDiskSpaceUsed;
	int64		maxMemorySpaceUsed;
	int64		totalMemorySpaceUsed;
	bits32		sortMethods;	/* bitmask of TuplesortMethod */
} IncrementalSortGroupInfo;

typedef struct IncrementalSortInfo
{
	IncrementalSortGroupInfo fullsortGroupInfo;
	IncrementalSortGroupInfo prefixsortGroupInfo;
} IncrementalSortInfo;

/* ----------------
 *   Shared memory container for per-worker incremental sort information
 * ----------------
 */
typedef struct SharedIncrementalSortInfo
{
	int			num_workers;
	IncrementalSortInfo sinfo[FLEXIBLE_ARRAY_MEMBER];
} SharedIncrementalSortInfo;

#endif							/* INSTRUMENT_NODE_H */
