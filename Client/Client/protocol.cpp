#include"protocol.h"

#include<string.h>
#include<QDebug>


PDU *mkPDU(uint uiMsgLen){
    uint uiPDULen = sizeof (PDU) + uiMsgLen;
    PDU *pdu = (PDU*)malloc(uiPDULen);
    if(NULL==pdu){
        exit(1);
    }
    memset(pdu,0,uiPDULen);
    pdu->uiMsgLen = uiMsgLen;
    pdu->uiPDULen = uiPDULen;
    return pdu;
}
