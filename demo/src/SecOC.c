/*
 * SecOC.c
 *
 *  Created on: 2021��9��6��
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

bool is_legal(PduIdType TxPduId){
	// �ж�SecOC�Ƿ��ѳ�ʼ��(_secOCState)
	if (SECOC_INIT != _secOCState)
	{
		return false; // δ��ʼ��
	}

	// �ж�id�Ϸ��ԣ��������ͱ��ĵ���������
	if (TxPduId >= SECOC_NUM_OF_TX_IPDU)
	{
		return false; //��secoc��txpduid
	}
}

Std_ReturnType SecOC_IfTransmit(PduIdType TxPduId, const PduInfoType *PduInfoPtr){
	if(!is_legal(TxPduId)){
		return E_NOT_OK;
	}
	uint8 len = PduInfoPtr->SduLength;
	uint8 *src = PduInfoPtr->SduDataPtr;

	SecOCintermediate_type intermediate = SecOCintermediateTx[TxPduId];
	uint8 *dst = apduTx + intermediate.addr_st;
	intermediate.apduBlen = len;

	memcpy(dst,src,len);

	return E_OK;
}


Std_ReturnType authenticate(SecOCintermediate_type SecOCintermidate,PduInfoType *PduInfoPtr){
	//������֤
	return E_OK;
}

void SecOC_MainFunctionTx(void) {
    // SecOC�Ƿ��ѳ�ʼ��
    if (_secOCState != SECOC_INIT)
        return;

    // ��������apdu�洢�ռ�
    uint8 idx = 0;
    Std_ReturnType result;
    PduInfoType PduInfo;
    for (; idx < SECOC_NUM_OF_TX_IPDU; idx++)
    {
        SecOCintermediate_type intermediate = SecOCintermediateTx[idx];
        // ������ֵ��spduδ��
        if (intermediate.apduBlen > 0 && intermediate.slock == 0)
        {
            authenticate(intermediate, &PduInfo);
            if (PduInfo.SduLength > 0)
            { //authenticate �ɹ�
                result = PduR_SecOCTransmit(idx, &PduInfo);
            }
        }
    }
}


void SecOC_TxConfirmation(PduIdType TxPduId, Std_ReturnType result){

	if(!is_legal(TxPduId))
		return;

	//�ͷ�spdu
	SecOCintermediate_type intermediate = SecOCintermediateTx[TxPduId];
	intermediate.spduBlen = 0;

	// ��������
	if (result == E_OK) {
		PduR_SecOCIfTxConfirmation(TxPduId, result);
	}

}


void  SecOC_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr){

	if(!is_legal(RxPduId))
		return;

	uint8 idx,AuthStartPosition,AuthLen;
	SecOCRxPduProcessing_type tmpSecOCRxPduProcessing = SecOCRxPduProcessing[RxPduId];

	//�ж��Ƿ������ݲ���������ֵ
	if(tmpSecOCRxPduProcessing->SecOCUseAuthDataFreshness == true){
		AuthStartPosition = tmpSecOCRxPduProcessing->SecOCAuthDataFreshnessStartPosition;
		AuthLen = tmpSecOCRxPduProcessing->SecOCAuthDataFreshnessLen;
	}

	//�ݶ�replace�����Ƿ�ʽ	������s-pdu��a-pdu���Լ����ǵĳ���
	switch (tmpSecOCRxPduProcessing->SecOCReceptionOverflowStrategy)
	{
	case REPLACE:
		memcpy((uint8 *)(SecOCintermediateRx[RxPduId].spduBlen + spduRx), PduInfoPtr.SduDataPtr, PduInfoPtr.SduLength);
		SecOCintermediateRx[RxPduId].spduBlen = PduInfoPtr.SduLength;
		SecOCintermediateRx[RxPduId].abc = 0;
		SecOCintermediateRx[RxPduId].avac = 0;
		break;
	case REJECT: // ������ʹ�õ�secure pdu�� �ܾ�����������
		if(SecOCintermediateRx[RxPduId].spduBlen == 0){
			memcpy((uint8 *)(SecOCintermediateRx[RxPduId].spduBlen + spduRx), PduInfoPtr.SduDataPtr, PduInfoPtr.SduLength);
			SecOCintermediateRx[RxPduId].spduBlen = PduInfoPtr.SduLength;
		}
		break;
	case QUEUE: // ������ʹ�õ�secure pdu�� �Ŷ����������ݣ�SecOCReceptionQueueSizeΪ����
		/*
		if(SecOCintermidateRx[RxPduId] δ��������){  // ��Ҫ����������ͣ�ѭ�����У���
			�Ŷ�
		}
		*/
		break;

	default:
		printf("throw err to get wrong SecOCReceptionOverflowStrategy");
		break;
	}
}
void verify(){

}
void SecOC_MainFunctionRx(){

}


