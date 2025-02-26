/*
 * SecOC_Cfg.c
 *
 *  Created on: 2021��9��7��
 *      Author: zhao chenyang
 */

#include "SecOC_Cfg.h"

SecOCintermediate_type SecOCintermediateTx [SECOC_NUM_OF_TX_IPDU] = {
	{
		.abc 		= 0,
		.addr_st 	= 0,
		.addr_cpy 	= 8,
		.type 		= 0,
		.slock 		= 0,
	},
	{
		.abc 		= 0,
		.addr_st 	= 8,
		.addr_cpy 	= 8,
		.type 		= 0,
		.slock 		= 0,
	},
};
SecOCintermediateRx_type SecOCintermediateRx [SECOC_NUM_OF_RX_IPDU] = {
	{
		.abc 		= 0,
		.addr_st 	= 0,
		.addr_cpy 	= 8,
		.type 		= 0,
		.slock 		= 0,
	},
	{
		.abc 		= 0,
		.addr_st 	= 8,
		.addr_cpy 	= 8,
		.type 		= 0,
		.slock 		= 0,
	},
};


SecOCRxPduProcessing_type SecOCRxPduProcessing[SECOC_NUM_OF_RX_IPDU] = {
	{
		.SecOCAuthDataFreshnessLen 				= 0,
		.SecOCAuthDataFreshnessStartPosition 	= 0,
		.SecOCAuthenticationBuildAttempts 		= 0,
		.SecOCAuthenticationVerifyAttempts		= 0,
		.SecOCAuthInfoTruncLength				= 0,
		.SecOCDataId							= 0,
		.SecOCFreshnessValueId					= 0,
		.SecOCFreshnessValueLength				= 0,
		.SecOCFreshnessValueTruncLength			= 0,
		.SecOCReceptionOverflowStrategy 		= 2,
		.SecOCUseAuthDataFreshness				= 0,
		.SecOCRxAuthenticPduLayer				= 1,
		.SecOCRxPduSecuredArea					= 0,
	},
	{
		.SecOCAuthDataFreshnessLen 				= 0,
		.SecOCAuthDataFreshnessStartPosition 	= 0,
		.SecOCAuthenticationBuildAttempts 		= 0,
		.SecOCAuthenticationVerifyAttempts		= 0,
		.SecOCAuthInfoTruncLength				= 0,
		.SecOCDataId							= 0,
		.SecOCFreshnessValueId					= 0,
		.SecOCFreshnessValueLength				= 0,
		.SecOCFreshnessValueTruncLength			= 0,
		.SecOCReceptionOverflowStrategy 		= 2,
		.SecOCUseAuthDataFreshness				= 0,
		.SecOCRxAuthenticPduLayer				= 1,
		.SecOCRxPduSecuredArea					= 0,
	}
};
