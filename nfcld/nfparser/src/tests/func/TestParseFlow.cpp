/*
 * TestParseFlow.cpp
 *
 *  Created on: 24.01.2011
 *      Author: wocson
 */

#include "TestParseFlow.h"
#include "srvfunc.h"


CPPUNIT_TEST_SUITE_REGISTRATION(TestParseFlow);

void TestParseFlow::setUp(void)
{
	try
	{
		m_mockCisco = new MockCisco("0.0.0.0","127.0.0.1",9993);
		m_mockVradius = new MockVradius();
		m_mockTemplateProc = new MockTemplateProc();

	} catch (const mt::Exception& me)
	{
		CPPUNIT_FAIL(me.displayText());
	}
}

void TestParseFlow::tearDown(void)
{
	delete m_mockCisco;
	delete m_mockVradius;
	delete m_mockTemplateProc;
	STOP_NFPARSER();
	sleep(1);
}


//4
void TestParseFlow::test_1FBdai_VRTmplN(void)
{
	test_RunTestNoRecvFromNfparser("nfparser.1FBdai.config.xml");
}
//5
void TestParseFlow::test_1FOKsi_VRTmplY(void)
{
	test_RunTestRecvFromNfparser("nfparser.1FOKsi.config.xml");
}
//6
void TestParseFlow::test_1FOKsi_VRTmplN(void)
{
	test_RunTestNoRecvFromNfparser("nfparser.1FBdsi.config.xml");
}
//7
void TestParseFlow::test_1FOKse_VRTmplN(void)
{
	test_RunTestNoRecvFromNfparser("nfparser.1FOKse.config.xml");
}
//8
void TestParseFlow::test_1FOKse_VRTmplY(void)
{
	test_RunTestRecvFromNfparser("nfparser.1FBdse.config.xml");
}
//9
void TestParseFlow::test_1FOKdi_VRTmplY(void)
{
	test_RunTestRecvFromNfparser("nfparser.1FOKdi.config.xml");
}
//10
void TestParseFlow::test_1FBddi_VRTmplN(void)
{
	test_RunTestNoRecvFromNfparser("nfparser.1FBddi.config.xml");
}
//11
void TestParseFlow::test_1FOKde_VRTmplN(void)
{
	test_RunTestNoRecvFromNfparser("nfparser.1FOKde.config.xml");
}
//12
void TestParseFlow::test_1FBdde_VRTmplY(void)
{
	test_RunTestRecvFromNfparser("nfparser.1FBdde.config.xml");
}
//13
void TestParseFlow::test_1FOKpti_VRTmplY(void)
{
	test_RunTestRecvFromNfparser("nfparser.1FOKpti.config.xml");
}
//14
void TestParseFlow::test_1FBdpti_VRTmplN(void)
{
	test_RunTestNoRecvFromNfparser("nfparser.1FBdpti.config.xml");
}
//15
void TestParseFlow::test_1FOKpte_VRTmplN(void)
{
	test_RunTestNoRecvFromNfparser("nfparser.1FOKpte.config.xml");
}

//16
void TestParseFlow::test_1FBdpte_VRTmplY(void)
{
	test_RunTestRecvFromNfparser("nfparser.1FBdpte.config.xml");
}
//17
void TestParseFlow::test_1FOkpri_VRTmplY(void)
{
	test_RunTestRecvFromNfparser("nfparser.1FOKpri.config.xml");
}
//18
void TestParseFlow::test_1FBdpri_VRTmplN(void)
{
	test_RunTestNoRecvFromNfparser("nfparser.1FBdpri.config.xml");
}
//19
void TestParseFlow::test_1FOKpre_VRTmplN(void)
{
	test_RunTestNoRecvFromNfparser("nfparser.1FOKpre.config.xml");
}
//20
void TestParseFlow::test_1FBdpre_VRTmplY(void)
{
	test_RunTestRecvFromNfparser("nfparser.1FBdpre.config.xml");
}
//21
void TestParseFlow::test_1FOKti_VRTmplY(void)
{
	test_RunTestRecvFromNfparser("nfparser.1FOKti.config.xml");
}
//22
void TestParseFlow::test_1FBdti_VRTmplN(void)
{
	test_RunTestNoRecvFromNfparser("nfparser.1FBdti.config.xml");
}
//23
void TestParseFlow::test_1FOKte_VRTmplN(void)
{
	test_RunTestNoRecvFromNfparser("nfparser.1FOKte.config.xml");
}
//24
void TestParseFlow::test_1FBdte_VRTmplY(void)
{
	test_RunTestRecvFromNfparser("nfparser.1FBdte.config.xml");
}
//25
void TestParseFlow::test_2FOKsiBdsipti_VRTmplY(void)
{
	test_RunTestRecvFromNfparser("nfparser.2FOKsiBdsipti.config.xml");
}
//26
void  TestParseFlow::test_2FOKsiptiBdsi_VRTmplY(void)
{
	test_RunTestRecvFromNfparser("nfparser.2FOKsiBdsipti.config.xml");
}

//=================================================================
//27
void TestParseFlow::test_1FOKpte_NoVRadius(void)
{
	START_NFPARSER("nfparser.1FOKpti_NoVRadius.config.xml");
	CPPUNIT_ASSERT(m_mockCisco->sendFlow("../../nfparser/src/tests/func/data/nfgood.bin"));
}
//28
void TestParseFlow::test_1FOKpte_VRadiusSendNotFindInfo(void)
{
	START_NFPARSER("nfparser.1FOKpti_VRadSendNFI.config.xml");
	m_mockCisco->sendFlow("../../nfparser/src/tests/func/data/nfgood.bin");

	//recv from NfParcer to VRadius
	mt::ByteBuffer buffRecvfNfpar = m_mockVradius->recvRequestFromNfparser();
	CPPUNIT_ASSERT(buffRecvfNfpar.getSize());

	FileData nfpar2Vradius("../../nfparser/src/tests/func/data/vradius_req.bin");
	mt::ByteBuffer buffVradSaved = nfpar2Vradius.getDataFromFile();
	CPPUNIT_ASSERT(memcmp(buffRecvfNfpar+22,buffVradSaved+22,40) == 0);

	// No send response from VRadius to Nfparser
	FileData Vradius2nfpar("../../nfparser/src/tests/func/data/vradius_error_resp.bin");
	mt::ByteBuffer buffVradResponse = Vradius2nfpar.getDataFromFile();
	CPPUNIT_ASSERT(m_mockVradius->sendResponse2NfParser(buffVradResponse));
/*
	// recv info for MockTemplateProc from NFparser
	CommandPtr cmd = m_mockTemplateProc->recvCommandFromNfparser();
	CPPUNIT_ASSERT(!cmd);
*/
}

//29 Send|Recv VRadius d'nt send cmd to TemplateProcessor
void TestParseFlow::test_1FOKsi_VRadiusSendTemplPrcNoSend(void)
{
	START_NFPARSER("nfparser.1FOKsi.config.xml");

	m_mockCisco->sendFlow("../../nfparser/src/tests/func/data/nfgood.bin");

	//Check send request from NfParcer to VRadius - Recv is OK
	FileData nfpar2Vradius("../../nfparser/src/tests/func/data/vradius_req.bin");
	mt::ByteBuffer buffVradSaved2File = nfpar2Vradius.getDataFromFile();

	mt::ByteBuffer buffVradRecvfNfpar = m_mockVradius->recvRequestFromNfparser();
	CPPUNIT_ASSERT(buffVradRecvfNfpar.getSize());
	CPPUNIT_ASSERT(memcmp(buffVradSaved2File+22,buffVradRecvfNfpar+22,40) == 0);

	//Check  send response from VRadius to Nfparser
	FileData Vradius2nfpar("../../nfparser/src/tests/func/data/vradius_success_resp.bin");
	mt::ByteBuffer buffVradResponse = Vradius2nfpar.getDataFromFile();
	CPPUNIT_ASSERT(m_mockVradius->sendResponse2NfParser(buffVradResponse));
}


//======== Common case ====================
// No send to VRadius/TemplateProccessor
void TestParseFlow::test_RunTestNoRecvFromNfparser(const std::string config)
{
	START_NFPARSER(config);
	m_mockCisco->sendFlow("../../nfparser/src/tests/func/data/nfgood.bin");

	//Check send requirst from NfParcer to VRadius - NoRecv is OK
	mt::ByteBuffer buffRecvfNfpar = m_mockVradius->recvRequestFromNfparser();
	CPPUNIT_ASSERT_EQUAL(0,(int)buffRecvfNfpar.getSize());
/*
	CommandPtr cmd = m_mockTemplateProc->recvCommandFromNfparser();
	CPPUNIT_ASSERT(!cmd);
*/
}

// Send to VRadius/TemplateProccessor
void TestParseFlow::test_RunTestRecvFromNfparser(const std::string config)
{
	START_NFPARSER(config);

	m_mockCisco->sendFlow("../../nfparser/src/tests/func/data/nfgood.bin");

	//Check send request from NfParcer to VRadius - Recv is OK
	FileData nfpar2Vradius("../../nfparser/src/tests/func/data/vradius_req.bin");
	mt::ByteBuffer buffVradSaved2File = nfpar2Vradius.getDataFromFile();

	mt::ByteBuffer buffVradRecvfNfpar = m_mockVradius->recvRequestFromNfparser();
	CPPUNIT_ASSERT(buffVradRecvfNfpar.getSize());
	CPPUNIT_ASSERT(memcmp(buffVradSaved2File+22,buffVradRecvfNfpar+22,40) == 0);

	//Check  send response from VRadius to Nfparser
	FileData Vradius2nfpar("../../nfparser/src/tests/func/data/vradius_success_resp.bin");
	mt::ByteBuffer buffVradResponse = Vradius2nfpar.getDataFromFile();
	CPPUNIT_ASSERT(m_mockVradius->sendResponse2NfParser(buffVradResponse));
/*
	// recv info for MockTemplateProc from NFparser
	CommandPtr cmd = m_mockTemplateProc->recvCommandFromNfparser();

    CPPUNIT_ASSERT(cmd);
	CPPUNIT_ASSERT_EQUAL(cmd->commandType(),Command::GET_SETTING_WITH_SEARCH_BASE);
	CPPUNIT_ASSERT_EQUAL(std::string("NFPARSER1"),cmd->sourceId());
	CPPUNIT_ASSERT_EQUAL(std::string("89211234567"),cmd->msisdn());
	CPPUNIT_ASSERT_EQUAL(std::string("internet"),cmd->base());
	CPPUNIT_ASSERT_EQUAL(std::string("wap,mms;wap"),cmd->searchBase());
*/
}
