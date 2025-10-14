/*
 * TestParseFlow.h
 *
 *  Created on: 24.01.2011
 *      Author: wocson
 */

#ifndef TESTPARSEFLOE_H_
#define TESTPARSEFLOE_H_

#include <cppunit/extensions/HelperMacros.h>
//#include <command/src/Command.h>

#include "utils/MockCisco.h"
#include "utils/MockVradius.h"
#include "utils/MockTemplateProc.h"

class TestParseFlow: public CPPUNIT_NS::TestFixture {

	CPPUNIT_TEST_SUITE(TestParseFlow);

	CPPUNIT_TEST(test_1FBdai_VRTmplN);//4
	CPPUNIT_TEST(test_1FOKsi_VRTmplY);//5
	CPPUNIT_TEST(test_1FOKsi_VRTmplN);//6
	CPPUNIT_TEST(test_1FOKse_VRTmplN);//7
	CPPUNIT_TEST(test_1FOKse_VRTmplY);//8
	CPPUNIT_TEST(test_1FOKdi_VRTmplY);//9
	CPPUNIT_TEST(test_1FBddi_VRTmplN);//10
	CPPUNIT_TEST(test_1FOKde_VRTmplN);//11
	CPPUNIT_TEST(test_1FBdde_VRTmplY);//12
	CPPUNIT_TEST(test_1FOKpti_VRTmplY);//13
	CPPUNIT_TEST(test_1FBdpti_VRTmplN);//14
	CPPUNIT_TEST(test_1FOKpte_VRTmplN);//15
	CPPUNIT_TEST(test_1FBdpte_VRTmplY);//16
	CPPUNIT_TEST(test_1FOkpri_VRTmplY);//17
	CPPUNIT_TEST(test_1FBdpri_VRTmplN);//18
	CPPUNIT_TEST(test_1FOKpre_VRTmplN);//19
	CPPUNIT_TEST(test_1FBdpre_VRTmplY);//20
	CPPUNIT_TEST(test_1FOKti_VRTmplY);//21
	CPPUNIT_TEST(test_1FBdti_VRTmplN);//22
	CPPUNIT_TEST(test_1FOKte_VRTmplN);//23
	CPPUNIT_TEST(test_1FBdte_VRTmplY);//24
	CPPUNIT_TEST(test_2FOKsiBdsipti_VRTmplY);//25
	CPPUNIT_TEST(test_2FOKsiptiBdsi_VRTmplY);//26

//====== VRadius Error ====================================================
	CPPUNIT_TEST(test_1FOKpte_NoVRadius);//27
	CPPUNIT_TEST(test_1FOKpte_VRadiusSendNotFindInfo);//28

//====== VRadius OK No Send to Tempate ====================================
	CPPUNIT_TEST(test_1FOKsi_VRadiusSendTemplPrcNoSend);//29

	CPPUNIT_TEST_SUITE_END();

protected:

	void test_1FBdai_VRTmplN(void);//4
	//src networks tests
	void test_1FOKsi_VRTmplY(void);//5
	void test_1FOKsi_VRTmplN(void);//6
	void test_1FOKse_VRTmplN(void);//7
	void test_1FOKse_VRTmplY(void);//8
	//dst networks tests
	void test_1FOKdi_VRTmplY(void);//9
	void test_1FBddi_VRTmplN(void);//10
	void test_1FOKde_VRTmplN(void);//11
	void test_1FBdde_VRTmplY(void);//12
	//ports tests
	void test_1FOKpti_VRTmplY(void);//13
	void test_1FBdpti_VRTmplN(void);//14
	void test_1FOKpte_VRTmplN(void);//15
	void test_1FBdpte_VRTmplY(void);//16
	//protocol tests
	void test_1FOkpri_VRTmplY(void);//17
	void test_1FBdpri_VRTmplN(void);//18
	void test_1FOKpre_VRTmplN(void);//19
	void test_1FBdpre_VRTmplY(void);//20
	//tos tests
	void test_1FOKti_VRTmplY(void);//21
	void test_1FBdti_VRTmplN(void);//22
	void test_1FOKte_VRTmplN(void);//23
	void test_1FBdte_VRTmplY(void);//24
	void test_2FOKsiBdsipti_VRTmplY(void);//25
	void test_2FOKsiptiBdsi_VRTmplY(void);//26

//=== VRAdius Error =====================================================
	void test_1FOKpte_NoVRadius(void);//27
	void test_1FOKpte_VRadiusSendNotFindInfo(void);//28
	void test_1FOKsi_VRadiusSendTemplPrcNoSend(void);//29

public:
	void setUp();
	void tearDown();

private:
	MockCisco *m_mockCisco;
	MockVradius *m_mockVradius;
	MockTemplateProc *m_mockTemplateProc;

	void test_RunTestNoRecvFromNfparser(const std::string config);
	void test_RunTestRecvFromNfparser(const std::string config);
};

#endif /* TESTPARSEFLOE_H_ */
