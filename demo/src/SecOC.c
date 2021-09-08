/*
 * SecOC.c
 *
 *  Created on: 2021年9月6日
 *      Author: zhao chenyang
 */

#include "SecOC.h"
#include "PduR_SecOC.h"
#include "SecOC_Cfg.h"

uint8 apduTx[8*SECOC_NUM_OF_TX_IPDU];
uint8 spduTx[8*SECOC_NUM_OF_TX_IPDU];
uint8 apduRx[8*SECOC_NUM_OF_RX_IPDU];
uint8 spduRx[8*SECOC_NUM_OF_RX_IPDU];

SecOC_StateType _secOCState = SECOC_UNINIT;

void init(){
	_secOCState = SECOC_INIT;
}

bool is_legal(PduIdType TxPduId){
	// 判断SecOC是否已初始化(_secOCState)
	if (SECOC_INIT != _secOCState)
	{
		return false; // 未初始化
	}

	// 判断id合法性，超过发送报文的配置数量
	if (TxPduId >= SECOC_NUM_OF_TX_IPDU)
	{
		return false; //非secoc内txpduid
	}
	return true;
}

Std_ReturnType SecOC_IfTransmit(PduIdType TxPduId, const PduInfoType *PduInfoPtr){
//	if(is_legal(TxPduId) == 0){
//		return E_NOT_OK;
//	}
	uint8 len = PduInfoPtr->SduLength;
	uint8 *src = PduInfoPtr->SduDataPtr;

	SecOCintermediate_type* intermediate = &SecOCintermediateTx[TxPduId];
	uint8 *dst = apduTx + intermediate->addr_st;
	intermediate->apduBlen = len;

	memcpy(dst,src,len);

	return E_OK;
}

#define bit(i) (1 << i)
#define addr2uint8p(address, index) (address + (index + 8 - 1) / 8)
#define testbit(data, index) (bit(index) & data)
#define setbit(address, index) *addr2uint8p(address, index) |= bit(index % 8)
#define clearbit(address, index) *addr2uint8p(address, index) &= !bit(index % 8)

Std_ReturnType authenticate(SecOCintermediate_type* SecOCintermediate){
	Std_ReturnType result;
	//调用验证
	return E_OK;
}

void SecOC_MainFunctionTx(void) {
    // SecOC是否已初始化
    if (_secOCState != SECOC_INIT)
        return;

    // 遍历所有apdu存储空间
    uint8 idx = 0;
    Std_ReturnType result;
    PduInfoType PduInfo;
    for (; idx < SECOC_NUM_OF_TX_IPDU; idx++)
    {
        SecOCintermediate_type intermediate = SecOCintermediateTx[idx];
        // 若存在值且spdu未锁
        if (intermediate.apduBlen > 0 && intermediate.slock == 0)
        {
        	authenticate(&intermediate);
        	PduInfo.SduLength = intermediate.apduBlen;
        	PduInfo.SduDataPtr = apduTx + intermediate.addr_st;
            if (PduInfo.SduLength > 0)
            { //authenticate 成功
                result = PduR_SecOCTransmit(idx, &PduInfo);
            }
        }
    }
}


void SecOC_TxConfirmation(PduIdType TxPduId, Std_ReturnType result){

	if(!is_legal(TxPduId))
		return;

	//释放spdu
	SecOCintermediate_type intermediate = SecOCintermediateTx[TxPduId];
	intermediate.spduBlen = 0;

	// 继续传递
	if (result == E_OK) {
		PduR_SecOCIfTxConfirmation(TxPduId, result);
	}

}


void  SecOC_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr){

	if(!is_legal(RxPduId))
		return;

	uint8 idx,AuthStartPosition,AuthLen;
	SecOCRxPduProcessing_type *tmpSecOCRxPduProcessing = &SecOCRxPduProcessing[RxPduId];

	//判断是否有数据部分做新鲜值
	if(tmpSecOCRxPduProcessing->SecOCUseAuthDataFreshness == true){
		AuthStartPosition = tmpSecOCRxPduProcessing->SecOCAuthDataFreshnessStartPosition;
		AuthLen = tmpSecOCRxPduProcessing->SecOCAuthDataFreshnessLen;
	}

	//暂定replace，覆盖方式	保存了s-pdu和a-pdu，以及它们的长度
	switch (tmpSecOCRxPduProcessing->SecOCReceptionOverflowStrategy)
	{
	case REPLACE:
		memcpy((uint8 *)(SecOCintermediateRx[RxPduId].apduBlen + spduRx), PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength);
		SecOCintermediateRx[RxPduId].spduBlen = PduInfoPtr->SduLength;
		SecOCintermediateRx[RxPduId].abc = 0;
		SecOCintermediateRx[RxPduId].avac = 0;
		break;
	case REJECT: // 当有在使用的secure pdu， 拒绝新来的数据
		if(SecOCintermediateRx[RxPduId].spduBlen == 0){
			memcpy((uint8 *)(SecOCintermediateRx[RxPduId].spduBlen + spduRx), PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength);
			SecOCintermediateRx[RxPduId].spduBlen = PduInfoPtr->SduLength;
		}
		break;
	case QUEUE: // 当有在使用的secure pdu， 排队新来的数据，SecOCReceptionQueueSize为上限
		/*
		if(SecOCintermidateRx[RxPduId] 未用满队列){  // 需要设计数据类型（循环队列？）
			排队
		}
		*/
		break;

	default:
		printf("throw err to get wrong SecOCReceptionOverflowStrategy");
		break;
	}
}


void verify() {
}


void SecOC_MainFunctionRx() {
	// SecOC是否已初始化
	if (_secOCState != SECOC_INIT)
		return;

	// 遍历所有apdu存储空间
	uint8 idx = 0;
	Std_ReturnType result;
	PduInfoType *PduInfoPtr;
	for (idx; idx < SECOC_NUM_OF_TX_IPDU; idx++)
	{
		SecOCintermediateRx_type intermediate = SecOCintermediateRx[idx];
		// 若存在值且spdu未锁
		if (intermediate.apduBlen > 0 && intermediate.slock == 0)
		{
			verify();
			if (PduInfoPtr->SduLength > 0)
			{ // verify 成功
				result = PduR_SecOCTransmit(idx, PduInfoPtr);
			}
		}
	}
}



