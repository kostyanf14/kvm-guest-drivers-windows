/**********************************************************************
 * Copyright (c) 2008  Red Hat, Inc.
 *
 * File: ParaNdis6-Oid.c
 *
 * This file contains implementation of NDIS6 OID-related procedures.
 *
 * This work is licensed under the terms of the GNU GPL, version 2.  See
 * the COPYING file in the top-level directory.
 *
**********************************************************************/
#include "ParaNdis-Oid.h"
#include "ParaNdis6.h"

#if NDIS60_MINIPORT

#ifdef WPP_EVENT_TRACING
#include "ParaNdis6-Oid.tmh"
#endif

#define OIDENTRY(oid, el, xfl, xokl, flags) \
{ oid, el, xfl, xokl, flags, NULL }
#define OIDENTRYPROC(oid, el, xfl, xokl, flags, setproc) \
{ oid, el, xfl, xokl, flags, setproc }

/**********************************************************
Template for OID SET procedure
Parameters:
	context
	tOidDesc *pOid		descriptor of OID request
Return value:
	whatever
***********************************************************/
static NDIS_STATUS tbd(PARANDIS_ADAPTER *pContext, tOidDesc *pOid)
{
	DEBUG_ENTRY(0);
	return 	NDIS_STATUS_INVALID_OID;
}

/**********************************************************
Just fail the request for unsupported OID
Parameters:
	context
	tOidDesc *pOid		descriptor of OID request
Return value:
	whatever
***********************************************************/
static NDIS_STATUS OnSetInterruptModeration(PARANDIS_ADAPTER *pContext, tOidDesc *pOid)
{
	return 	NDIS_STATUS_INVALID_DATA;
}


static NDIS_STATUS OnSetOffloadParameters(PARANDIS_ADAPTER *pContext, tOidDesc *pOid);
static NDIS_STATUS OnSetOffloadEncapsulation(PARANDIS_ADAPTER *pContext, tOidDesc *pOid);

/**********************************************************
Structure defining how to support each OID
***********************************************************/
static const tOidWhatToDo OidsDB[] =
{
//												i f ok flags		set proc
OIDENTRY(OID_GEN_SUPPORTED_LIST,				2,2,4, ohfQueryStat		),
OIDENTRY(OID_GEN_HARDWARE_STATUS,				2,0,4, ohfQueryStat		),
OIDENTRY(OID_GEN_MEDIA_SUPPORTED,				2,0,4, ohfQueryStat		),
OIDENTRY(OID_GEN_MEDIA_IN_USE,					2,0,4, ohfQueryStat		),
OIDENTRY(OID_GEN_MAXIMUM_LOOKAHEAD,				2,0,4, ohfQuery			),
OIDENTRY(OID_GEN_MAXIMUM_FRAME_SIZE,			2,0,4, ohfQuery			),
OIDENTRY(OID_GEN_LINK_SPEED,					2,0,4, ohfQuery			),
OIDENTRY(OID_GEN_TRANSMIT_BUFFER_SPACE,			2,0,4, ohfQueryStat		),
OIDENTRY(OID_GEN_RECEIVE_BUFFER_SPACE,			2,0,4, ohfQueryStat		),
OIDENTRY(OID_GEN_TRANSMIT_BLOCK_SIZE,			2,0,4, ohfQueryStat		),
OIDENTRY(OID_GEN_RECEIVE_BLOCK_SIZE,			2,0,4, ohfQueryStat		),
OIDENTRY(OID_GEN_VENDOR_ID,						2,0,4, ohfQueryStat		),
OIDENTRY(OID_GEN_VENDOR_DESCRIPTION,			2,2,4, ohfQueryStat		),
OIDENTRYPROC(OID_GEN_CURRENT_PACKET_FILTER,		2,0,4, ohfQuerySet, ParaNdis_OnSetPacketFilter),
OIDENTRYPROC(OID_GEN_CURRENT_LOOKAHEAD,			2,0,4, ohfQuerySet, ParaNdis_OnSetLookahead),
OIDENTRY(OID_GEN_DRIVER_VERSION,				2,0,4, ohfQuery			),
OIDENTRY(OID_GEN_MAXIMUM_TOTAL_SIZE,			2,0,4, ohfQueryStat		),
OIDENTRY(OID_GEN_PROTOCOL_OPTIONS,				2,0,4, 0				),
OIDENTRY(OID_GEN_MAC_OPTIONS,					2,0,4, ohfQuery			),
OIDENTRY(OID_GEN_MEDIA_CONNECT_STATUS,			2,0,4, ohfQuery			),
OIDENTRY(OID_GEN_MAXIMUM_SEND_PACKETS,			2,0,4, ohfQueryStat		),
OIDENTRY(OID_GEN_VENDOR_DRIVER_VERSION,			2,0,4, ohfQueryStat		),
OIDENTRY(OID_GEN_SUPPORTED_GUIDS,				2,4,4, 0				),
OIDENTRYPROC(OID_GEN_NETWORK_LAYER_ADDRESSES,	2,2,4, ohfSet,		ParaNdis_OnOidSetNetworkAddresses),
OIDENTRY(OID_GEN_TRANSPORT_HEADER_OFFSET,		2,4,4, 0				),
OIDENTRY(OID_GEN_MEDIA_CAPABILITIES,			2,4,4, 0				),
OIDENTRY(OID_GEN_PHYSICAL_MEDIUM,				2,4,4, 0				),
OIDENTRY(OID_GEN_XMIT_OK,						3,4,4, ohfQueryStat3264	),
OIDENTRY(OID_GEN_RCV_OK,						3,4,4, ohfQueryStat3264	),
OIDENTRY(OID_GEN_XMIT_ERROR,					2,4,4, ohfQueryStat3264	),
OIDENTRY(OID_GEN_RCV_ERROR,						2,4,4, ohfQueryStat3264	),
OIDENTRY(OID_GEN_RCV_NO_BUFFER,					2,4,4, ohfQueryStat3264	),
OIDENTRY(OID_GEN_DIRECTED_BYTES_XMIT,			2,4,4, ohfQueryStat3264 ),
OIDENTRY(OID_GEN_DIRECTED_FRAMES_XMIT,			2,4,4, ohfQueryStat3264 ),
OIDENTRY(OID_GEN_MULTICAST_BYTES_XMIT,			2,4,4, ohfQueryStat3264 ),
OIDENTRY(OID_GEN_MULTICAST_FRAMES_XMIT,			2,4,4, ohfQueryStat3264 ),
OIDENTRY(OID_GEN_BROADCAST_BYTES_XMIT,			2,4,4, ohfQueryStat3264 ),
OIDENTRY(OID_GEN_BROADCAST_FRAMES_XMIT,			2,4,4, ohfQueryStat3264 ),
OIDENTRY(OID_GEN_DIRECTED_BYTES_RCV,			2,4,4, ohfQueryStat3264 ),
OIDENTRY(OID_GEN_DIRECTED_FRAMES_RCV,			2,4,4, ohfQueryStat3264 ),
OIDENTRY(OID_GEN_MULTICAST_BYTES_RCV,			2,4,4, ohfQueryStat3264 ),
OIDENTRY(OID_GEN_MULTICAST_FRAMES_RCV,			2,4,4, ohfQueryStat3264 ),
OIDENTRY(OID_GEN_BROADCAST_BYTES_RCV,			2,4,4, ohfQueryStat3264 ),
OIDENTRY(OID_GEN_BROADCAST_FRAMES_RCV,			2,4,4, ohfQueryStat3264 ),
OIDENTRY(OID_GEN_RCV_CRC_ERROR,					2,4,4, 0				),
OIDENTRY(OID_GEN_TRANSMIT_QUEUE_LENGTH,			2,0,4, ohfQuery			),
OIDENTRY(OID_GEN_GET_TIME_CAPS,					2,4,4, 0				),
OIDENTRY(OID_GEN_GET_NETCARD_TIME,				2,4,4, 0				),
OIDENTRY(OID_GEN_NETCARD_LOAD,					2,4,4, 0				),
OIDENTRY(OID_GEN_DEVICE_PROFILE,				2,4,4, 0				),
OIDENTRY(OID_GEN_INIT_TIME_MS,					2,4,4, 0				),
OIDENTRY(OID_GEN_RESET_COUNTS,					2,4,4, 0				),
OIDENTRY(OID_GEN_MEDIA_SENSE_COUNTS,			2,4,4, 0				),
OIDENTRY(OID_PNP_CAPABILITIES,					2,0,4, ohfQuery			),
OIDENTRYPROC(OID_PNP_SET_POWER,					2,0,4, ohfSet | ohfSetMoreOK, ParaNdis_OnSetPower),
OIDENTRY(OID_PNP_QUERY_POWER,					2,0,4, ohfQuery			),
OIDENTRYPROC(OID_PNP_ADD_WAKE_UP_PATTERN,		2,0,4, ohfSet,			ParaNdis_OnAddWakeupPattern),
OIDENTRYPROC(OID_PNP_REMOVE_WAKE_UP_PATTERN,	2,0,4, ohfSet,			ParaNdis_OnRemoveWakeupPattern),
OIDENTRYPROC(OID_PNP_ENABLE_WAKE_UP,			2,0,4, ohfQuerySet,		ParaNdis_OnEnableWakeup),
OIDENTRY(OID_802_3_PERMANENT_ADDRESS,			2,0,4, ohfQueryStat		),
OIDENTRY(OID_802_3_CURRENT_ADDRESS,				2,0,4, ohfQueryStat		),
OIDENTRYPROC(OID_802_3_MULTICAST_LIST,			2,0,4, ohfQuerySet,		ParaNdis_OnOidSetMulticastList),
OIDENTRY(OID_802_3_MAXIMUM_LIST_SIZE,			2,0,4, ohfQueryStat		),
OIDENTRY(OID_802_3_MAC_OPTIONS,					2,4,4, 0				),
OIDENTRY(OID_802_3_RCV_ERROR_ALIGNMENT,			2,4,4, ohfQueryStat3264	),
OIDENTRY(OID_802_3_XMIT_ONE_COLLISION,			2,4,4, ohfQueryStat3264	),
OIDENTRY(OID_802_3_XMIT_MORE_COLLISIONS,		2,4,4, ohfQueryStat3264	),
OIDENTRY(OID_802_3_XMIT_DEFERRED,				2,4,4, 0				),
OIDENTRY(OID_802_3_XMIT_MAX_COLLISIONS,			2,4,4, 0				),
OIDENTRY(OID_802_3_RCV_OVERRUN,					2,4,4, 0				),
OIDENTRY(OID_802_3_XMIT_UNDERRUN,				2,4,4, 0				),
OIDENTRY(OID_802_3_XMIT_HEARTBEAT_FAILURE,		2,4,4, 0				),
OIDENTRY(OID_802_3_XMIT_TIMES_CRS_LOST,			2,4,4, 0				),
OIDENTRY(OID_802_3_XMIT_LATE_COLLISIONS,		2,4,4, 0				),
OIDENTRY(OID_GEN_MACHINE_NAME,					2,4,4, 0				),
OIDENTRY(OID_GEN_STATISTICS,					3,4,4, ohfQueryStat		),
OIDENTRYPROC(OID_GEN_VLAN_ID,					0,4,4, ohfQueryStat | ohfSet, ParaNdis_OnSetVlanId),
OIDENTRYPROC(OID_GEN_INTERRUPT_MODERATION,		2,4,4, ohfQueryStat | ohfSet, OnSetInterruptModeration),
//TODO: turning link off, disable duplex etc
//OIDENTRYPROC(OID_GEN_LINK_PARAMETERS,			2,0,4, ohfQuerySet, OnSetLinkParameters),
OIDENTRY(OID_IP4_OFFLOAD_STATS,					4,4,4, 0),
OIDENTRY(OID_IP6_OFFLOAD_STATS,					4,4,4, 0),
OIDENTRYPROC(OID_TCP_OFFLOAD_PARAMETERS,		0,0,0, ohfSet, OnSetOffloadParameters),
OIDENTRYPROC(OID_OFFLOAD_ENCAPSULATION,			0,0,0, ohfQuerySet, OnSetOffloadEncapsulation),
// last entry, do not remove
OIDENTRY(0,										4,4,4, 0),
};


static NDIS_OID SupportedOids[] =
{
		OID_GEN_SUPPORTED_LIST,
		OID_GEN_HARDWARE_STATUS,
		OID_GEN_MEDIA_SUPPORTED,
		OID_GEN_MEDIA_IN_USE,
		OID_GEN_MAXIMUM_LOOKAHEAD,
		OID_GEN_MAXIMUM_FRAME_SIZE,
		OID_GEN_LINK_SPEED,
		OID_GEN_LINK_SPEED_EX,
		OID_GEN_TRANSMIT_BUFFER_SPACE,
		OID_GEN_RECEIVE_BUFFER_SPACE,
		OID_GEN_TRANSMIT_BLOCK_SIZE,
		OID_GEN_RECEIVE_BLOCK_SIZE,
		OID_GEN_VENDOR_ID,
		OID_GEN_VENDOR_DESCRIPTION,
		OID_GEN_VENDOR_DRIVER_VERSION,
		OID_GEN_CURRENT_PACKET_FILTER,
		OID_GEN_CURRENT_LOOKAHEAD,
		OID_GEN_DRIVER_VERSION,
		OID_GEN_MAXIMUM_TOTAL_SIZE,
		OID_GEN_MAC_OPTIONS,
		OID_GEN_MEDIA_CONNECT_STATUS,
		OID_GEN_MAXIMUM_SEND_PACKETS,
		OID_GEN_NETWORK_LAYER_ADDRESSES,
		OID_GEN_INTERRUPT_MODERATION,
		OID_GEN_XMIT_ERROR,
		OID_GEN_RCV_ERROR,
		OID_GEN_RCV_NO_BUFFER,
		OID_802_3_PERMANENT_ADDRESS,
		OID_802_3_CURRENT_ADDRESS,
		OID_802_3_MULTICAST_LIST,
		OID_802_3_MAXIMUM_LIST_SIZE,
		OID_802_3_RCV_ERROR_ALIGNMENT,
		OID_802_3_XMIT_ONE_COLLISION,
		OID_802_3_XMIT_MORE_COLLISIONS,
		OID_GEN_STATISTICS,
		OID_PNP_CAPABILITIES,
		OID_PNP_SET_POWER,
		OID_PNP_QUERY_POWER,
		OID_GEN_XMIT_OK,
		OID_GEN_RCV_OK,
		OID_GEN_VLAN_ID,
		OID_OFFLOAD_ENCAPSULATION,
		OID_TCP_OFFLOAD_PARAMETERS
};


/**********************************************************
		For statistics header
***********************************************************/
static const ULONG SupportedStatisticsFlags =
	NDIS_STATISTICS_FLAGS_VALID_DIRECTED_FRAMES_RCV |
	NDIS_STATISTICS_FLAGS_VALID_DIRECTED_FRAMES_XMIT |
	NDIS_STATISTICS_FLAGS_VALID_DIRECTED_BYTES_RCV |
	NDIS_STATISTICS_FLAGS_VALID_DIRECTED_BYTES_XMIT |
	NDIS_STATISTICS_FLAGS_VALID_BYTES_RCV |
	NDIS_STATISTICS_FLAGS_VALID_BYTES_XMIT |
	NDIS_STATISTICS_FLAGS_VALID_MULTICAST_BYTES_RCV |
	NDIS_STATISTICS_FLAGS_VALID_BROADCAST_BYTES_RCV |
	NDIS_STATISTICS_FLAGS_VALID_MULTICAST_FRAMES_RCV |
	NDIS_STATISTICS_FLAGS_VALID_BROADCAST_FRAMES_RCV |
	NDIS_STATISTICS_FLAGS_VALID_MULTICAST_FRAMES_XMIT |
	NDIS_STATISTICS_FLAGS_VALID_BROADCAST_FRAMES_XMIT |
	NDIS_STATISTICS_FLAGS_VALID_MULTICAST_BYTES_RCV |
	NDIS_STATISTICS_FLAGS_VALID_BROADCAST_BYTES_RCV |
	NDIS_STATISTICS_FLAGS_VALID_RCV_ERROR |
	NDIS_STATISTICS_FLAGS_VALID_RCV_DISCARDS |
	NDIS_STATISTICS_FLAGS_VALID_XMIT_ERROR |
	NDIS_STATISTICS_FLAGS_VALID_XMIT_DISCARDS |
	NDIS_STATISTICS_FLAGS_VALID_MULTICAST_BYTES_XMIT |
	NDIS_STATISTICS_FLAGS_VALID_BROADCAST_BYTES_XMIT |
	0;

/**********************************************************
		For miniport registration
***********************************************************/
static const ULONG SupportedStatistics =
	NDIS_STATISTICS_XMIT_OK_SUPPORTED                   |
	NDIS_STATISTICS_RCV_OK_SUPPORTED                    |
	NDIS_STATISTICS_XMIT_ERROR_SUPPORTED                    |
	NDIS_STATISTICS_RCV_ERROR_SUPPORTED                     |
	NDIS_STATISTICS_RCV_NO_BUFFER_SUPPORTED                 |
	NDIS_STATISTICS_DIRECTED_BYTES_XMIT_SUPPORTED           |
	NDIS_STATISTICS_DIRECTED_FRAMES_XMIT_SUPPORTED          |
	NDIS_STATISTICS_MULTICAST_BYTES_XMIT_SUPPORTED          |
	NDIS_STATISTICS_MULTICAST_FRAMES_XMIT_SUPPORTED         |
	NDIS_STATISTICS_BROADCAST_BYTES_XMIT_SUPPORTED          |
	NDIS_STATISTICS_BROADCAST_FRAMES_XMIT_SUPPORTED         |
	NDIS_STATISTICS_DIRECTED_BYTES_RCV_SUPPORTED            |
	NDIS_STATISTICS_DIRECTED_FRAMES_RCV_SUPPORTED           |
	NDIS_STATISTICS_MULTICAST_BYTES_RCV_SUPPORTED           |
	NDIS_STATISTICS_MULTICAST_FRAMES_RCV_SUPPORTED          |
	NDIS_STATISTICS_BROADCAST_BYTES_RCV_SUPPORTED           |
	NDIS_STATISTICS_BROADCAST_FRAMES_RCV_SUPPORTED          |
	//NDIS_STATISTICS_RCV_CRC_ERROR_SUPPORTED               |
	NDIS_STATISTICS_TRANSMIT_QUEUE_LENGTH_SUPPORTED         |
	//NDIS_STATISTICS_BYTES_RCV_SUPPORTED                     |
	//NDIS_STATISTICS_BYTES_XMIT_SUPPORTED                    |
	NDIS_STATISTICS_RCV_DISCARDS_SUPPORTED                  |
	NDIS_STATISTICS_GEN_STATISTICS_SUPPORTED                |
	NDIS_STATISTICS_XMIT_DISCARDS_SUPPORTED					|
	0;


/**********************************************************
For common query provides array of supported OID
***********************************************************/
void ParaNdis_GetSupportedOid(PVOID *pOidsArray, PULONG pLength)
{
	*pOidsArray		= SupportedOids;
	*pLength		= sizeof(SupportedOids);
}

/**********************************************************
statistics support information for statistics structure
(it is different from bitmask provided at miniport registration)
***********************************************************/
ULONG ParaNdis6_GetSupportedStatisticsFlags()
{
	return SupportedStatisticsFlags;
}


/**********************************************************
OID support information for miniport registration
***********************************************************/
void ParaNdis6_GetSupportedOid(NDIS_MINIPORT_ADAPTER_GENERAL_ATTRIBUTES *pGenAttributes)
{
	pGenAttributes->SupportedOidList = SupportedOids;
	pGenAttributes->SupportedOidListLength = sizeof(SupportedOids);
	pGenAttributes->SupportedStatistics = SupportedStatistics;
}

/*****************************************************************
Handles NDIS6 specific OID, all the rest handled by common handler
*****************************************************************/
static NDIS_STATUS ParaNdis_OidQuery(PARANDIS_ADAPTER *pContext, tOidDesc *pOid)
{
	union _tagtemp
	{
		NDIS_LINK_SPEED							LinkSpeed;
		NDIS_INTERRUPT_MODERATION_PARAMETERS	InterruptModeration;
		NDIS_LINK_PARAMETERS					LinkParameters;
	} u;
	NDIS_STATUS  status = NDIS_STATUS_SUCCESS;
	PVOID pInfo  = NULL;
	ULONG ulSize = 0;
	BOOLEAN bFreeInfo = FALSE;

#define SETINFO(field, value) pInfo = &u.##field; ulSize = sizeof(u.##field); u.##field = (value)
	switch(pOid->Oid)
	{
		case OID_GEN_LINK_SPEED_EX:
			{
				ULONG64 speed = pContext->bConnected ? PARANDIS_FORMAL_LINK_SPEED : NDIS_LINK_SPEED_UNKNOWN;
				u.LinkSpeed.RcvLinkSpeed = speed;
				u.LinkSpeed.XmitLinkSpeed = speed;
				pInfo = &u.LinkSpeed;
				ulSize = sizeof(u.LinkSpeed);
			}
			break;
		case OID_GEN_STATISTICS:
			pInfo  = &pContext->Statistics;
			ulSize = sizeof(pContext->Statistics);
			break;
		case OID_GEN_INTERRUPT_MODERATION:
			u.InterruptModeration.Header.Type = NDIS_OBJECT_TYPE_DEFAULT;
			u.InterruptModeration.Header.Size = sizeof(u.InterruptModeration);
			u.InterruptModeration.Header.Revision = NDIS_INTERRUPT_MODERATION_PARAMETERS_REVISION_1;
			u.InterruptModeration.Flags = 0;
			u.InterruptModeration.InterruptModeration = NdisInterruptModerationNotSupported;
			pInfo = &u.InterruptModeration;
			ulSize = sizeof(u.InterruptModeration);
			break;
		default:
			return ParaNdis_OidQueryCommon(pContext, pOid);
	}
	if (status == NDIS_STATUS_SUCCESS)
	{
		status = ParaNdis_OidQueryCopy(pOid, pInfo, ulSize, bFreeInfo);
	}
	return status;
}

/**********************************************************
NDIS required procedure for OID support
Parameters:
	context
	PNDIS_OID_REQUEST  pNdisRequest			NDIS oid request
Return value:
	NDIS status, as returned from supporting procedure
	NDIS_STATUS_NOT_SUPPORTED if support for get/set is not defined in the table
***********************************************************/
NDIS_STATUS ParaNdis6_OidRequest(
	NDIS_HANDLE miniportAdapterContext,
	PNDIS_OID_REQUEST  pNdisRequest)
{
	NDIS_STATUS  status = NDIS_STATUS_NOT_SUPPORTED;
	tOidWhatToDo Rules;
	PARANDIS_ADAPTER *pContext = (PARANDIS_ADAPTER *)miniportAdapterContext;
	tOidDesc _oid;
	_oid.Reserved = pNdisRequest;
	ParaNdis_GetOidSupportRules(pNdisRequest->DATA.SET_INFORMATION.Oid, &Rules, OidsDB);
	_oid.ulToDoFlags = Rules.Flags;

	ParaNdis_DebugHistory(pContext, hopOidRequest, NULL, pNdisRequest->DATA.SET_INFORMATION.Oid, pNdisRequest->RequestType, 1);
	DPrintf(Rules.nEntryLevel, ("[%s] OID type %d, id 0x%X(%s) of %d", __FUNCTION__,
				pNdisRequest->RequestType,
				pNdisRequest->DATA.SET_INFORMATION.Oid,
				Rules.name,
				pNdisRequest->DATA.SET_INFORMATION.InformationBufferLength));

	if (pContext->bSurprizeRemoved) status = NDIS_STATUS_NOT_ACCEPTED;
	else switch(pNdisRequest->RequestType)
	{
		case NdisRequestQueryStatistics:
			if (Rules.Flags & ohfQueryStatOnly)
			{
				// fall to query;
			}
			else
			{
				status = NDIS_STATUS_NOT_SUPPORTED;
				break;
			}
		case NdisRequestQueryInformation:
			if (Rules.Flags & ohfQuery)
			{
				_oid.InformationBuffer = pNdisRequest->DATA.QUERY_INFORMATION.InformationBuffer;
				_oid.InformationBufferLength = pNdisRequest->DATA.QUERY_INFORMATION.InformationBufferLength;
				_oid.Oid = pNdisRequest->DATA.QUERY_INFORMATION.Oid;
				_oid.pBytesWritten = &pNdisRequest->DATA.QUERY_INFORMATION.BytesWritten;
				_oid.pBytesNeeded = &pNdisRequest->DATA.QUERY_INFORMATION.BytesNeeded;
				_oid.pBytesRead = &pNdisRequest->DATA.QUERY_INFORMATION.BytesWritten;
				status = ParaNdis_OidQuery(pContext, &_oid);
			}
			break;
		case NdisRequestSetInformation:
			if (Rules.Flags & ohfSet)
			{
				if (Rules.OidSetProc)
				{
					_oid.InformationBuffer = pNdisRequest->DATA.SET_INFORMATION.InformationBuffer;
					_oid.InformationBufferLength = pNdisRequest->DATA.SET_INFORMATION.InformationBufferLength;
					_oid.Oid = pNdisRequest->DATA.SET_INFORMATION.Oid;
					_oid.pBytesWritten = &pNdisRequest->DATA.SET_INFORMATION.BytesRead;
					_oid.pBytesNeeded = &pNdisRequest->DATA.SET_INFORMATION.BytesNeeded;
					_oid.pBytesRead = &pNdisRequest->DATA.SET_INFORMATION.BytesRead;
					status = Rules.OidSetProc(pContext, &_oid);
				}
				else
				{
					DPrintf(0, ("Error: Inconsistent OIDDB, oid %s", Rules.name));
				}
			}
			break;
		default:
			DPrintf(Rules.nExitFailLevel, ("Error: Unsupported OID type %d, id 0x%X(%s)",
				pNdisRequest->RequestType, Rules.oid, Rules.name));
			status = NDIS_STATUS_NOT_SUPPORTED;
			break;
	}
	ParaNdis_DebugHistory(pContext, hopOidRequest, NULL, pNdisRequest->DATA.SET_INFORMATION.Oid, status, 0);
	if (status != NDIS_STATUS_PENDING)
	{
		DPrintf(((status != NDIS_STATUS_SUCCESS) ? Rules.nExitFailLevel : Rules.nExitOKLevel),
			("[%s] OID type %d, id 0x%X(%s) (%X)", __FUNCTION__,
			pNdisRequest->RequestType, Rules.oid, Rules.name, status));
	}
	return status;
}

/**********************************************************
NDIS required procedure for OID cancel
May be used only for OID returning PENDING
Parameters:
	irrelevant
***********************************************************/
VOID ParaNdis6_OidCancelRequest(
		NDIS_HANDLE hMiniportAdapterContext,
		PVOID pRequestId)
{

}

static void OnSetPowerWorkItem(PVOID  WorkItemContext, NDIS_HANDLE  NdisIoWorkItemHandle)
{
	tPowerWorkItem *pwi = (tPowerWorkItem *)WorkItemContext;
	PARANDIS_ADAPTER *pContext = pwi->pContext;
	PNDIS_OID_REQUEST pRequest = (PNDIS_OID_REQUEST)pwi->request;
	NDIS_STATUS status = NDIS_STATUS_SUCCESS;
#ifdef DEBUG_TIMING
	LARGE_INTEGER TickCount;
	LARGE_INTEGER SysTime;

	KeQueryTickCount(&TickCount);
	NdisGetCurrentSystemTime(&SysTime);
	DPrintf(0, ("\n%s>> CPU #%d, perf-counter %I64d, tick count %I64d, NDIS_sys_time %I64d\n", __FUNCTION__, KeGetCurrentProcessorNumber(), KeQueryPerformanceCounter(NULL).QuadPart,TickCount.QuadPart, SysTime.QuadPart));
#endif

	if (pwi->state == NetDeviceStateD0)
	{
		ParaNdis_PowerOn(pContext);
	}
	else
	{
		ParaNdis_PowerOff(pContext);
	}
	NdisFreeMemory(pwi, 0, 0);
	NdisFreeIoWorkItem(NdisIoWorkItemHandle);
	ParaNdis_DebugHistory(pContext, hopOidRequest, NULL, pRequest->DATA.SET_INFORMATION.Oid, status, 2);
	NdisMOidRequestComplete(pContext->MiniportHandle, pRequest, status);
}


/**********************************************************
NDIS6.X handler of power management
***********************************************************/
NDIS_STATUS ParaNdis_OnSetPower(PARANDIS_ADAPTER *pContext, tOidDesc *pOid)
{
	NDIS_STATUS status;
	NDIS_DEVICE_POWER_STATE newState;
#ifdef DEBUG_TIMING
	LARGE_INTEGER TickCount;
	LARGE_INTEGER SysTime;

	KeQueryTickCount(&TickCount);
	NdisGetCurrentSystemTime(&SysTime);
	DPrintf(0, ("\n%s>> CPU #%d, perf-counter %I64d, tick count %I64d, NDIS_sys_time %I64d\n", __FUNCTION__, KeGetCurrentProcessorNumber(), KeQueryPerformanceCounter(NULL).QuadPart,TickCount.QuadPart, SysTime.QuadPart));
#endif

	DEBUG_ENTRY(0);

	status = ParaNdis_OidSetCopy(pOid, &newState, sizeof(newState));
	if (status == NDIS_STATUS_SUCCESS)
	{
		NDIS_HANDLE hwo = NdisAllocateIoWorkItem(pContext->MiniportHandle);
		tPowerWorkItem *pwi = ParaNdis_AllocateMemory(pContext, sizeof(tPowerWorkItem));
		status = NDIS_STATUS_FAILURE;
		if (pwi && hwo)
		{
			pwi->pContext = pContext;
			pwi->state    = newState;
			pwi->WorkItem = hwo;
			pwi->request  = pOid->Reserved;
			NdisQueueIoWorkItem(hwo, OnSetPowerWorkItem, pwi);
			status = NDIS_STATUS_PENDING;
		}
		else
		{
			if (pwi) NdisFreeMemory(pwi, 0, 0);
			if (hwo) NdisFreeIoWorkItem(hwo);
		}
	}
	return status;
}

static void DumpOffloadStructure(NDIS_OFFLOAD *po, LPCSTR message)
{
	int level = 1;
	ULONG *pul;
	DPrintf(level, ("[%s](%s)", __FUNCTION__, message));
	pul = (ULONG *)&po->Checksum.IPv4Transmit;
	DPrintf(level, ("CSV4TX:(%d,%d)", pul[0], pul[1]));
	pul = (ULONG *)&po->Checksum.IPv4Receive;
	DPrintf(level, ("CSV4RX:(%d,%d)", pul[0], pul[1]));
	pul = (ULONG *)&po->Checksum.IPv6Transmit;
	DPrintf(level, ("CSV6TX:(%d,%d)", pul[0], pul[1], pul[2], pul[3], pul[4]));
	pul = (ULONG *)&po->Checksum.IPv6Receive;
	DPrintf(level, ("CSV6RX:(%d,%d)", pul[0], pul[1], pul[2], pul[3], pul[4]));
	pul = (ULONG *)&po->LsoV1;
	DPrintf(level, ("LSOV1 :(%d,%d,%d,%d)", pul[0], pul[1], pul[2], pul[3]));
	pul = (ULONG *)&po->LsoV2.IPv4;
	DPrintf(level, ("LSO4V2:(%d,%d,%d)", pul[0], pul[1], pul[2]));
	pul = (ULONG *)&po->LsoV2.IPv6;
	DPrintf(level, ("LSO6V2:(%d,%d,%d,%d)", pul[0], pul[1], pul[2], pul[3]));
}

static void FillOffloadStructure(NDIS_OFFLOAD *po, tOffloadSettingsFlags f)
{
	NDIS_TCP_IP_CHECKSUM_OFFLOAD *pcso = &po->Checksum;
	NDIS_TCP_LARGE_SEND_OFFLOAD_V1 *plso = &po->LsoV1;
	NDIS_TCP_LARGE_SEND_OFFLOAD_V2 *plso2 = &po->LsoV2;
	NdisZeroMemory(po, sizeof(*po));
	po->Header.Type = NDIS_OBJECT_TYPE_OFFLOAD;
	po->Header.Revision = NDIS_OFFLOAD_REVISION_1;
	po->Header.Size = NDIS_SIZEOF_NDIS_OFFLOAD_REVISION_1;
	pcso->IPv4Transmit.Encapsulation = NDIS_ENCAPSULATION_IEEE_802_3;
	pcso->IPv4Transmit.IpChecksum = f.fTxIPChecksum ? NDIS_OFFLOAD_SUPPORTED : NDIS_OFFLOAD_NOT_SUPPORTED;
	pcso->IPv4Transmit.IpOptionsSupported = f.fTxIPOptions ? NDIS_OFFLOAD_SUPPORTED : NDIS_OFFLOAD_NOT_SUPPORTED;
	pcso->IPv4Transmit.TcpChecksum = f.fTxTCPChecksum ? NDIS_OFFLOAD_SUPPORTED : NDIS_OFFLOAD_NOT_SUPPORTED;
	pcso->IPv4Transmit.TcpOptionsSupported = f.fTxTCPOptions ? NDIS_OFFLOAD_SUPPORTED : NDIS_OFFLOAD_NOT_SUPPORTED;
	pcso->IPv4Transmit.UdpChecksum = f.fTxUDPChecksum ? NDIS_OFFLOAD_SUPPORTED : NDIS_OFFLOAD_NOT_SUPPORTED;
	plso->IPv4.Encapsulation = f.fTxLso ? NDIS_ENCAPSULATION_IEEE_802_3 : NDIS_ENCAPSULATION_NOT_SUPPORTED;
	plso->IPv4.TcpOptions = (f.fTxLsoTCP && f.fTxLso) ? NDIS_OFFLOAD_SUPPORTED : NDIS_OFFLOAD_NOT_SUPPORTED;
	plso->IPv4.IpOptions  = (f.fTxLsoIP  && f.fTxLso) ? NDIS_OFFLOAD_SUPPORTED : NDIS_OFFLOAD_NOT_SUPPORTED;
	plso->IPv4.MaxOffLoadSize = f.fTxLso ? PARANDIS_MAX_LSO_SIZE : 0;
	plso->IPv4.MinSegmentCount = f.fTxLso ? PARANDIS_MIN_LSO_SEGMENTS : 0;
	plso2->IPv4.Encapsulation = plso->IPv4.Encapsulation;
	plso2->IPv4.MaxOffLoadSize = plso->IPv4.MaxOffLoadSize;
	plso2->IPv4.MinSegmentCount = plso->IPv4.MinSegmentCount;
	plso2->IPv6.Encapsulation = NDIS_ENCAPSULATION_NOT_SUPPORTED;
}

void ParaNdis6_FillOffloadConfiguration(PARANDIS_ADAPTER *pContext)
{
	NDIS_OFFLOAD *po = &pContext->ReportedOffloadConfiguration;
	FillOffloadStructure(po, pContext->Offload.flags);
}

void ParaNdis6_FillOffloadCapabilities(PARANDIS_ADAPTER *pContext)
{
	tOffloadSettingsFlags f;
	NDIS_OFFLOAD *po = &pContext->ReportedOffloadCapabilities;
	ParaNdis_ResetOffloadSettings(pContext, &f, NULL);
	FillOffloadStructure(po, f);
}

static const NDIS_TCP_IP_CHECKSUM_OFFLOAD CheckSumAllOff =
{
	{
		//IPV4Tx
		NDIS_ENCAPSULATION_IEEE_802_3,
		NDIS_OFFLOAD_SET_NO_CHANGE,
		NDIS_OFFLOAD_SET_NO_CHANGE,
		NDIS_OFFLOAD_SET_NO_CHANGE,
		NDIS_OFFLOAD_SET_NO_CHANGE,
		NDIS_OFFLOAD_SET_NO_CHANGE
	},
	{
		//IPV4Rx
		NDIS_ENCAPSULATION_NOT_SUPPORTED,
		NDIS_OFFLOAD_SET_NO_CHANGE,
		NDIS_OFFLOAD_SET_NO_CHANGE,
		NDIS_OFFLOAD_SET_NO_CHANGE,
		NDIS_OFFLOAD_SET_NO_CHANGE,
		NDIS_OFFLOAD_SET_NO_CHANGE
	},
	{
		//IPV6Tx
		NDIS_ENCAPSULATION_NOT_SUPPORTED,
		NDIS_OFFLOAD_SET_NO_CHANGE,
		NDIS_OFFLOAD_SET_NO_CHANGE,
		NDIS_OFFLOAD_SET_NO_CHANGE,
		NDIS_OFFLOAD_SET_NO_CHANGE
	},
	{
		//IPV6Rx
		NDIS_ENCAPSULATION_NOT_SUPPORTED,
		NDIS_OFFLOAD_SET_NO_CHANGE,
		NDIS_OFFLOAD_SET_NO_CHANGE,
		NDIS_OFFLOAD_SET_NO_CHANGE,
		NDIS_OFFLOAD_SET_NO_CHANGE
	}
};


static void BuildOffloadChangeReport(
	const NDIS_OFFLOAD *Previous,
	const NDIS_OFFLOAD *Current,
	NDIS_OFFLOAD *diff)
{
	RtlCopyMemory(&diff->Checksum, &CheckSumAllOff, sizeof(diff->Checksum));
	/*
	if (Current->Checksum.IPv4Transmit.IpChecksum == NDIS_OFFLOAD_SUPPORTED)
		diff->Checksum.IPv4Transmit.IpChecksum = NDIS_OFFLOAD_SET_ON;
	if (Current->Checksum.IPv4Transmit.IpOptionsSupported == NDIS_OFFLOAD_SUPPORTED)
		diff->Checksum.IPv4Transmit.IpOptionsSupported = NDIS_OFFLOAD_SET_ON;
	if (Current->Checksum.IPv4Transmit.TcpChecksum == NDIS_OFFLOAD_SUPPORTED)
		diff->Checksum.IPv4Transmit.TcpChecksum = NDIS_OFFLOAD_SET_ON;
	if (Current->Checksum.IPv4Transmit.TcpOptionsSupported == NDIS_OFFLOAD_SUPPORTED)
		diff->Checksum.IPv4Transmit.TcpOptionsSupported = NDIS_OFFLOAD_SET_ON;
	if (Current->Checksum.IPv4Transmit.UdpChecksum == NDIS_OFFLOAD_SUPPORTED)
		diff->Checksum.IPv4Transmit.UdpChecksum = NDIS_OFFLOAD_SET_ON;
	*/
}

static void SendStatusIndication(PARANDIS_ADAPTER *pContext, NDIS_OFFLOAD *Prev, tOidDesc *pOid)
{
	NDIS_STATUS_INDICATION	indication;
	PNDIS_OID_REQUEST pRequest = NULL;
	NDIS_OFFLOAD offload = pContext->ReportedOffloadConfiguration;
	BuildOffloadChangeReport(Prev, &pContext->ReportedOffloadConfiguration, &offload);
	NdisZeroMemory(&indication, sizeof(indication));
	indication.Header.Type = NDIS_OBJECT_TYPE_STATUS_INDICATION;
	indication.Header.Revision = NDIS_STATUS_INDICATION_REVISION_1;
	indication.Header.Size = sizeof(indication);
	indication.SourceHandle = pContext->MiniportHandle;
	indication.StatusCode = NDIS_STATUS_TASK_OFFLOAD_CURRENT_CONFIG;
	indication.StatusBuffer = &offload;
	indication.StatusBufferSize = sizeof(offload);
	// we shall not do that for offload indications,
	// it is only for case we return NDIS_STATUS_INDICATION_REQUIRED from OID Handler
	// if (pOid) pRequest = (PNDIS_OID_REQUEST)pOid->Reserved;
	if (pRequest)
	{
		indication.RequestId = pRequest->RequestId;
		indication.DestinationHandle = pRequest->RequestHandle;
	}
	DPrintf(0, ("Indicating offload change"));
	NdisMIndicateStatusEx(pContext->MiniportHandle , &indication);
}

static ULONG __inline SetOffloadField(ULONG current, ULONG isSupported, UCHAR TxRxValue, BOOLEAN *pbFailed)
{
	switch(TxRxValue)
	{
		case NDIS_OFFLOAD_PARAMETERS_TX_RX_DISABLED:
			return 0;
		case NDIS_OFFLOAD_PARAMETERS_TX_RX_ENABLED:
			*pbFailed = TRUE;
			return 0;
		case NDIS_OFFLOAD_PARAMETERS_TX_ENABLED_RX_DISABLED:
			if (!isSupported) *pbFailed = TRUE;
			return isSupported;
		case NDIS_OFFLOAD_PARAMETERS_RX_ENABLED_TX_DISABLED:
			*pbFailed = TRUE;
			return 0;
		case NDIS_OFFLOAD_PARAMETERS_NO_CHANGE:
		default:
			break;
	}
	return current;
}

static NDIS_STATUS ApplyOffloadConfiguration(PARANDIS_ADAPTER *pContext,
	NDIS_OFFLOAD_PARAMETERS *pop, tOidDesc *pOid)
{
	BOOLEAN bFailed = FALSE;
	NDIS_STATUS status = NDIS_STATUS_SUCCESS;
	tOffloadSettingsFlags fSupported;
	tOffloadSettingsFlags fPresent = pContext->Offload.flags;
	NDIS_OFFLOAD PreviousOffload = pContext->ReportedOffloadConfiguration;
	tOffloadSettingsFlags *pf = &fPresent;
	ParaNdis_ResetOffloadSettings(pContext, &fSupported, NULL);
	pf->fTxIPChecksum = SetOffloadField(pf->fTxIPChecksum, fSupported.fTxIPChecksum, pop->IPv4Checksum, &bFailed);
	pf->fTxIPOptions = pf->fTxIPChecksum;
	pf->fTxTCPChecksum = SetOffloadField(pf->fTxTCPChecksum, fSupported.fTxTCPChecksum, pop->TCPIPv4Checksum, &bFailed);
	pf->fTxTCPOptions = pf->fTxTCPChecksum;
	pf->fTxUDPChecksum = SetOffloadField(pf->fTxUDPChecksum, fSupported.fTxUDPChecksum, pop->UDPIPv4Checksum, &bFailed);
	if (NDIS_OFFLOAD_PARAMETERS_LSOV1_DISABLED == pop->LsoV1 || NDIS_OFFLOAD_PARAMETERS_LSOV2_DISABLED == pop->LsoV2IPv4)
	{
		pf->fTxLso = 0;
	}
	else if (NDIS_OFFLOAD_PARAMETERS_LSOV1_ENABLED == pop->LsoV1 || NDIS_OFFLOAD_PARAMETERS_LSOV2_ENABLED  == pop->LsoV2IPv4)
	{
		if (fSupported.fTxLso) pf->fTxLso = 1;
		else
			bFailed = TRUE;
	}
	if ((NDIS_OFFLOAD_PARAMETERS_LSOV1_DISABLED == pop->LsoV1 && NDIS_OFFLOAD_PARAMETERS_LSOV2_ENABLED  == pop->LsoV2IPv4) ||
		(NDIS_OFFLOAD_PARAMETERS_LSOV1_ENABLED == pop->LsoV1 && NDIS_OFFLOAD_PARAMETERS_LSOV2_DISABLED  == pop->LsoV2IPv4))
	{
		bFailed = TRUE;
	}
	if (pop->LsoV2IPv6 == NDIS_OFFLOAD_PARAMETERS_LSOV2_ENABLED)
	{
		bFailed = TRUE;
	}

	if (bFailed && pOid)
	{
		status = NDIS_STATUS_INVALID_PARAMETER;
	}
	else
	{
		pContext->Offload.flags = fPresent;
		ParaNdis6_FillOffloadConfiguration(pContext);
		if (pOid)
		{
			//DumpOffloadStructure(&pContext->ReportedOffloadConfiguration, "Updated");
			SendStatusIndication(pContext, &PreviousOffload, pOid);
		}
	}
	return status;
}

NDIS_STATUS OnSetOffloadParameters(PARANDIS_ADAPTER *pContext, tOidDesc *pOid)
{
	NDIS_STATUS status;
	NDIS_OFFLOAD_PARAMETERS op;
	status = ParaNdis_OidSetCopy(pOid, &op, sizeof(op));
	if (status == NDIS_STATUS_SUCCESS)
	{
		status = ApplyOffloadConfiguration(pContext, &op, pOid);
	}
	return status;
}

void ParaNdis6_ApplyOffloadPersistentConfiguration(PARANDIS_ADAPTER *pContext)
{
	// 0/1 =>NDIS_OFFLOAD_PARAMETERS_LSOV1_DISABLED/NDIS_OFFLOAD_PARAMETERS_LSOV1_ENABLED
	pContext->InitialOffloadParameters.LsoV1++;
	pContext->InitialOffloadParameters.LsoV2IPv4++;
	pContext->InitialOffloadParameters.LsoV2IPv6++;
	pContext->InitialOffloadParameters.IPv4Checksum++;
	pContext->InitialOffloadParameters.TCPIPv4Checksum++;
	pContext->InitialOffloadParameters.TCPIPv6Checksum++;
	pContext->InitialOffloadParameters.UDPIPv4Checksum++;
	pContext->InitialOffloadParameters.UDPIPv6Checksum++;
	ApplyOffloadConfiguration(pContext,&pContext->InitialOffloadParameters, NULL);
	ParaNdis6_FillOffloadCapabilities(pContext);
}


NDIS_STATUS OnSetOffloadEncapsulation(PARANDIS_ADAPTER *pContext, tOidDesc *pOid)
{
	NDIS_OFFLOAD_ENCAPSULATION encaps;
	NDIS_STATUS status;
	status = ParaNdis_OidSetCopy(pOid, &encaps, sizeof(encaps));
	if (status == NDIS_STATUS_SUCCESS)
	{
		ULONG *pul = (ULONG *)&encaps;
		status = NDIS_STATUS_INVALID_PARAMETER;
		DPrintf(1, ("[%s] %08lX, %08lX, %08lX, %08lX, %08lX, %08lX, %08lX", __FUNCTION__,
				pul[0],pul[1],pul[2],pul[3],pul[4],pul[5],pul[6]));
		if (encaps.Header.Size == sizeof(encaps) &&
			encaps.Header.Revision == NDIS_OFFLOAD_ENCAPSULATION_REVISION_1 &&
			encaps.Header.Type == NDIS_OBJECT_TYPE_OFFLOAD_ENCAPSULATION &&
			(encaps.IPv4.Enabled != NDIS_OFFLOAD_SET_ON || (encaps.IPv4.EncapsulationType & NDIS_ENCAPSULATION_IEEE_802_3)) &&
			(encaps.IPv6.Enabled != NDIS_OFFLOAD_SET_ON || (encaps.IPv6.EncapsulationType & NDIS_ENCAPSULATION_IEEE_802_3))
			)
		 {
			DPrintf(0, ("[%s] V4 types 0x%lX, header %d, enabled %d", __FUNCTION__,
				encaps.IPv4.EncapsulationType,
				encaps.IPv4.HeaderSize,
				encaps.IPv4.Enabled));
			DPrintf(0, ("[%s] V6 types 0x%lX, header %d, enabled %d", __FUNCTION__,
				encaps.IPv6.EncapsulationType,
				encaps.IPv6.HeaderSize,
				encaps.IPv6.Enabled));
			if (encaps.IPv4.Enabled == NDIS_OFFLOAD_SET_ON)
			{
				pContext->Offload.ipHeaderOffset = encaps.IPv4.HeaderSize;
				pContext->bOffloadEnabled = TRUE;
			}
			else if (encaps.IPv4.Enabled == NDIS_OFFLOAD_SET_OFF)
			{
				pContext->bOffloadEnabled = FALSE;
			}
			status = NDIS_STATUS_SUCCESS;
		 }
	}
	return status;
}


/*******************************************************************
Provides startup information about offload capabilities and settings
********************************************************************/
NDIS_STATUS ParaNdis6_GetRegistrationOffloadInfo(
		PARANDIS_ADAPTER *pContext,
		NDIS_MINIPORT_ADAPTER_OFFLOAD_ATTRIBUTES *poa)
{
	NDIS_STATUS  status = NDIS_STATUS_NOT_SUPPORTED;
	NdisZeroMemory(poa, sizeof(*poa));
	/* something supported? */
	if (pContext->Offload.flagsValue)
	{
		poa->Header.Type = NDIS_OBJECT_TYPE_MINIPORT_ADAPTER_OFFLOAD_ATTRIBUTES;
		poa->Header.Revision = NDIS_MINIPORT_ADAPTER_OFFLOAD_ATTRIBUTES_REVISION_1;
		poa->Header.Size = sizeof(*poa);
		poa->DefaultOffloadConfiguration = &pContext->ReportedOffloadConfiguration;
		poa->HardwareOffloadCapabilities = &pContext->ReportedOffloadCapabilities;
		DumpOffloadStructure(poa->HardwareOffloadCapabilities, "Initial Capabilities");
		DumpOffloadStructure(poa->DefaultOffloadConfiguration, "Initial Configuration");
		status = NDIS_STATUS_SUCCESS;
	}
	return status;
}

#endif //NDIS6_MINIPORT
