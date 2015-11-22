/*
 * mmwave-enb-mac.h
 *
 *  Created on: May 1, 2015
 *      Author: root
 */

#ifndef SRC_MMWAVE_MODEL_MMWAVE_ENB_MAC_H_
#define SRC_MMWAVE_MODEL_MMWAVE_ENB_MAC_H_

#include "mmwave-mac.h"
#include "mmwave-enb-mac.h"
#include <ns3/lte-enb-cmac-sap.h>
#include <ns3/lte-mac-sap.h>
#include "mmwave-phy-mac-common.h"

namespace ns3
{

typedef std::vector < Ptr<PacketBurst> > DlHarqProcessesBuffer_t;

class MmWaveEnbMac : public Object
{
	friend class MmWaveEnbMacMemberEnbCmacSapProvider;
	friend class MmWaveMacEnbMemberPhySapUser;

public:
	static TypeId GetTypeId (void);
	MmWaveEnbMac (void);
	virtual ~MmWaveEnbMac (void);
	virtual void DoDispose (void);

/*	struct SchedConfigIndParameters
	{
		uint32_t m_sfn;
		TddSlotTypeList m_tddPattern;
		SfAllocInfo m_allocationList;
	};*/

  struct TransmitPduParameters
  {
    Ptr<Packet> pdu;  /**< the RLC PDU */
    uint16_t    rnti; /**< the C-RNTI identifying the UE */
    uint8_t     lcid; /**< the logical channel id corresponding to the sending RLC instance */
    uint8_t     layer; /**< the layer value that was passed by the MAC in the call to NotifyTxOpportunity that generated this PDU */
    uint8_t     harqProcessId; /**< the HARQ process id that was passed by the MAC in the call to NotifyTxOpportunity that generated this PDU */
  };

  struct ReportBufferStatusParameters
  {
    uint16_t rnti;  /**< the C-RNTI identifying the UE */
    uint8_t lcid;  /**< the logical channel id corresponding to the sending RLC instance */
    uint32_t txQueueSize;  /**< the current size of the RLC transmission queue */
    uint16_t txQueueHolDelay;  /**< the Head Of Line delay of the transmission queue */
    uint32_t retxQueueSize;  /**<  the current size of the RLC retransmission queue in bytes */
    uint16_t retxQueueHolDelay;  /**<  the Head Of Line delay of the retransmission queue */
    uint16_t statusPduSize;  /**< the current size of the pending STATUS RLC  PDU message in bytes */
  };

/*
  struct RachConfig
  {
  	uint8_t numberOfRaPreambles;
  	uint8_t preambleTransMax;
  	uint8_t raResponseWindowSize;
  };

  struct AllocateNcRaPreambleReturnValue
  {
  	bool valid; ///< true if a valid RA config was allocated, false otherwise
  	uint8_t raPreambleId; ///< random access preamble id
  	uint8_t raPrachMaskIndex; /// PRACH mask index
  };
*/

	void SetCofigurationParameters (Ptr<MmWavePhyMacCommon> ptrConfig);
	Ptr<MmWavePhyMacCommon> GetConfigurationParameters (void) const;

	// forwarded from LteMacSapProvider
	void DoTransmitPdu (LteMacSapProvider::TransmitPduParameters);
	void DoReportBufferStatus (LteMacSapProvider::ReportBufferStatusParameters);
	void DoUlCqiReport (MmWaveMacSchedSapProvider::SchedUlCqiInfoReqParameters ulcqi);

	void DoSubframeIndication (SfnSf sfnSf);

	void AssociateUeMAC (uint64_t imsi);

	void SetForwardUpCallback (Callback <void, Ptr<Packet> > cb);

//	void PhyPacketRx (Ptr<Packet> p);

	void DoReceivePhyPdu (Ptr<Packet> p);

	void DoReceiveControlMessage  (Ptr<MmWaveControlMessage> msg);

	void DoSchedConfigIndication (MmWaveMacSchedSapUser::SchedConfigIndParameters ind);

	MmWaveEnbPhySapUser* GetPhySapUser ();
	void SetPhySapProvider (MmWavePhySapProvider* ptr);

	MmWaveMacSchedSapUser* GetMmWaveMacSchedSapUser ();
	void SetMmWaveMacSchedSapProvider (MmWaveMacSchedSapProvider* ptr);

	MmWaveMacCschedSapUser* GetMmWaveMacCschedSapUser ();
	void SetMmWaveMacCschedSapProvider (MmWaveMacCschedSapProvider* ptr);

	LteMacSapProvider* GetUeMacSapProvider (void);
	LteEnbCmacSapProvider* GetEnbCmacSapProvider (void);

	void SetEnbCmacSapUser (LteEnbCmacSapUser* s);
	void ReceiveRachPreamble (uint32_t raId);

	// forwarded from FfMacCchedSapUser
	void DoCschedCellConfigCnf (MmWaveMacCschedSapUser::CschedCellConfigCnfParameters params);
	void DoCschedUeConfigCnf (MmWaveMacCschedSapUser::CschedUeConfigCnfParameters params);
	void DoCschedLcConfigCnf (MmWaveMacCschedSapUser::CschedLcConfigCnfParameters params);
	void DoCschedLcReleaseCnf (MmWaveMacCschedSapUser::CschedLcReleaseCnfParameters params);
	void DoCschedUeReleaseCnf (MmWaveMacCschedSapUser::CschedUeReleaseCnfParameters params);
	void DoCschedUeConfigUpdateInd (MmWaveMacCschedSapUser::CschedUeConfigUpdateIndParameters params);
	void DoCschedCellConfigUpdateInd (MmWaveMacCschedSapUser::CschedCellConfigUpdateIndParameters params);

private:
	// forwarded from LteEnbCmacSapProvider
	void DoConfigureMac (uint8_t ulBandwidth, uint8_t dlBandwidth);
	void DoAddUe (uint16_t rnti);
	void DoRemoveUe (uint16_t rnti);
	void DoAddLc (LteEnbCmacSapProvider::LcInfo lcinfo, LteMacSapUser* msu);
	void DoReconfigureLc (LteEnbCmacSapProvider::LcInfo lcinfo);
	void DoReleaseLc (uint16_t  rnti, uint8_t lcid);
	void UeUpdateConfigurationReq (LteEnbCmacSapProvider::UeConfig params);
	LteEnbCmacSapProvider::RachConfig DoGetRachConfig ();
	LteEnbCmacSapProvider::AllocateNcRaPreambleReturnValue DoAllocateNcRaPreamble (uint16_t rnti);
	uint8_t AllocateTbUid ();

	void DoDlHarqFeedback (DlHarqInfo params);
	void DoUlHarqFeedback (UlHarqInfo params);

	Ptr<MmWavePhyMacCommon> m_phyMacConfig;

	LteMacSapProvider* m_macSapProvider;
	LteEnbCmacSapProvider* m_cmacSapProvider;
	LteEnbCmacSapUser* m_cmacSapUser;

	uint32_t m_frameNum;
	uint32_t m_sfNum;
	uint32_t m_slotNum;

	uint8_t	m_tbUid;
	std::map<uint32_t, struct MacPduInfo> m_macPduMap;

	std::list <uint16_t> m_associatedUe;

	Callback <void, Ptr<Packet> > m_forwardUpCallback;

	std::vector <DlCqiInfo> m_dlCqiReceived;
	std::vector <MmWaveMacSchedSapProvider::SchedUlCqiInfoReqParameters> m_ulCqiReceived;
	std::vector <MacCeElement> m_ulCeReceived; // CE received (BSR up to now)

	MmWavePhySapProvider* m_phySapProvider;
	MmWaveEnbPhySapUser* m_phySapUser;

	MmWaveMacSchedSapProvider* m_macSchedSapProvider;
	MmWaveMacSchedSapUser* m_macSchedSapUser;
	MmWaveMacCschedSapProvider* m_macCschedSapProvider;
	MmWaveMacCschedSapUser* m_macCschedSapUser;

	std::map<uint8_t, uint32_t> m_receivedRachPreambleCount;

	std::map <uint16_t, std::map<uint8_t, LteMacSapUser*> > m_rlcAttached;

	std::vector <DlHarqInfo> m_dlHarqInfoReceived; // DL HARQ feedback received
	std::vector <UlHarqInfo> m_ulHarqInfoReceived; // UL HARQ feedback received
	std::map <uint16_t, DlHarqProcessesBuffer_t> m_miDlHarqProcessesPackets; // Packet under trasmission of the DL HARQ process

};

}



#endif /* SRC_MMWAVE_MODEL_MMWAVE_ENB_MAC_H_ */
