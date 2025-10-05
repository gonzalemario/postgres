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

#endif							/* INSTRUMENT_NODE_H */
