/* SPDX-License-Identifier: BSD-3-Clause */

/*
 * Copyright 2024 NXP
 */

#ifndef __DFE_APP_H__
#define __DFE_APP_H__

#include <la9310.h>

#define UNUSED(x)	(void)(x)

/* trace string description size */
#define MAX_TRACE_STRING_DESC 8

/* number of trace entries */
#define MAX_TS 80

typedef enum e_scs {
	SCS_kHz15 = 0,
	SCS_kHz30 = 1,
	SCS_MAX
} eSCS;


// /*  __________________________________________________________________
//  * |          |          |          |          |           |          |
//  * | dl_slots | g1_slots | ul_slots | g2_slots | ul2_slots | g3_slots |
//  * |__________|__________|__________|__________|___________|__________|
//  * 
//  */
// typedef struct sPattern {
// 	uint8_t dl1_slots;
// 	uint8_t g1_slots;
// 	uint8_t ul1_slots;
// 	uint8_t dl2_slots;
// 	uint8_t g2_slots;
// 	uint8_t ul2_slots;
// 	uint8_t pad[2];
// } tPattern;

typedef struct sSlot {
	uint8_t is_dl;
	uint8_t is_ul;
	uint8_t start_symbol_dl;
	uint8_t end_symbol_dl;
	uint8_t start_symbol_ul;
	uint8_t end_symbol_ul;
	//uint8_t pad[2];
} tSlot;

#if 0 /* future: optimize memory footprint */
#define SLOT_DL     (1<<0)
#define SLOT_UL     (1<<1)
#define SLOT_MIXED  (SLOT_DL | SLOT_UL)

#define SLOT_IS_DL(s) !!((s.is_dl_ul & SLOT_DL) == SLOT_DL) 
#define SLOT_IS_UL(s) !!((s.is_dl_ul & SLOT_UL) == SLOT_UL) 
#define SLOT_IS_MIXED(s) !!((s.is_dl_ul & SLOT_MIXED) == SLOT_MIXED) 

#define SLOT_MASK(size) \
			((~(unsigned int)0) >> (32 - (size)))

#define SLOT_SET_FIELD(word, val, size, shift) \
			(word |= ((val) & SLOT_MASK(size)) << shift)

#define SLOT_GET_FIELD(word, size, shift) \
			((word >> shift) & SLOT_MASK(size))
#endif

// typedef struct {
// 	eSCS scs;
// 	tPattern p;
// } tDFEPatternConfig;

/* tracing related defines & structs */
typedef enum {
	TRACE_INVALID = 0,
	TRACE_TICK,
	TRACE_AXIQ_TX,
	TRACE_AXIQ_RX,
	TRACE_PHYTIMER,
	TRACE_SLOT,
	TRACE_SLOT_UL,
	TRACE_SLOT_DL,
	TRACE_VSPA,
	TRACE_MAX
} eTraceEvent;

const char eTraceEventString[TRACE_MAX][MAX_TRACE_STRING_DESC] = {
	"Invalid",
	"Tick",
	"AXIQ_UL",
	"AXIQ_DL",
	"PhyTmr",
	"Slot",
	"Slot_UL",
	"Slot_DL",
	"VspaMsg",
};

typedef struct {
	uint32_t timestamp;
	uint32_t event;
	uint32_t param1;
	uint32_t param2;
} sTraceEntry;

typedef struct {
	uint32_t target_timestamp;
	enum ePhyTimerComparatorTrigger tx_allowed_state;
	enum ePhyTimerComparatorTrigger rx_allowed_state;
	uint32_t exec_after_timestamp;
} sTimeAgentMessage;

extern sTraceEntry app_logging[];
extern uint32_t debug_ts[];
extern uint32_t ulTotalTicks;
extern uint32_t ulVspaMsgCnt;
extern uint32_t ulVspaMsgRxCnt;
extern uint32_t ulVspaMsgTxCnt;
extern bool debug;
extern bool stop;

extern const uint32_t ofdm_short_sym_time[];
extern const uint32_t ofdm_long_sym_time[];
extern const uint32_t slot_duration[];
extern const uint32_t tick_interval[];
extern uint32_t uRxAntennaComparator;
extern const uint32_t uTxAntennaComparator;

int vDFEInit(void);

void vTraceEventRecord(eTraceEvent event, uint32_t p1, uint32_t p2);
void vTraceEventShow();

void vSetupTddPattern(uint32_t cfg_scs);
void vVSPADebugBreakPoint();
void prvConfigTddTask(void *pvParameters);
void vConfigFddStart();
void vConfigFddStop();
void vSetTimeAdvance(int32_t iVal);
void vPhyTimerDelay(uint32_t sleep_phy_timer_ticks);
int iBenchmarkVspaTest(uint32_t size, uint32_t mode, uint32_t parallel_dma, uint32_t iterations, uint32_t *gbits, uint32_t *mbits);
int iSetRxAntenna(uint32_t antenna);
void vSetTxRxBuffers(uint32_t rxa, uint32_t txa, uint32_t sym_size, uint32_t rxs, uint32_t txs);
void vSetQecParam(uint32_t txrx, uint32_t mode, uint32_t idx, uint32_t value);

void vTddStop(void);
int iTddStart(void);
void vFddStartStop(uint32_t start);

static void prvSendMsgToHost(uint32_t type, uint32_t status, int args_count, ...);

void switch_rf(uint32_t mode);

void dump_slots();

#endif /* __DFE_APP_H__ */
