#include "TestSmartPtr.h"
#include "mt/common/SmartPtr.h"

CPPUNIT_TEST_SUITE_REGISTRATION( TestSmartPtr);

class BaseClass {
public:
	virtual ~BaseClass() {
	}
	virtual std::string str() {
		return "BaseClass";
	}
};

class DerivedClass: public BaseClass {
public:
	virtual std::string str() {
		return "DerivedClass";
	}
};

class BadCastClass {
};

void TestSmartPtr::setUp() {
}

void TestSmartPtr::tearDown() {
}

void TestSmartPtr::testInitSmartPtr() {

	mt::SmartPtr<BaseClass> a_ptr = new BaseClass;
	CPPUNIT_ASSERT_EQUAL(1, (int) a_ptr.count());
}

void TestSmartPtr::testDecrementCount() {
	mt::SmartPtr<BaseClass> a_ptr = new BaseClass;
	CPPUNIT_ASSERT_EQUAL(1, (int) a_ptr.count());
	{
		mt::SmartPtr<BaseClass> a_ptr2 = a_ptr;
		CPPUNIT_ASSERT_EQUAL(2, (int) a_ptr.count());
		CPPUNIT_ASSERT_EQUAL(2, (int) a_ptr2.count());
	}
	CPPUNIT_ASSERT_EQUAL(1, (int) a_ptr.count());
}

void TestSmartPtr::testCast() {
	mt::SmartPtr<BaseClass> a_ptr = new DerivedClass;
	CPPUNIT_ASSERT_EQUAL(1, (int) a_ptr.count());
	{
		mt::SmartPtr<DerivedClass> b_ptr = a_ptr.cast<DerivedClass> ();
		CPPUNIT_ASSERT_EQUAL(2, (int) a_ptr.count());
		CPPUNIT_ASSERT(a_ptr.data() == b_ptr.data());
	}
	CPPUNIT_ASSERT_EQUAL(1, (int) a_ptr.count());
}

void TestSmartPtr::testBadCast() {
	mt::SmartPtr<BaseClass> a_ptr = new BaseClass;
	mt::SmartPtr<BadCastClass> ptr = a_ptr.cast<BadCastClass> ();
	CPPUNIT_ASSERT(!ptr);
}

void TestSmartPtr::testCastFromDerivedToBase() {
	mt::SmartPtr<DerivedClass> a_ptr = new DerivedClass;
	CPPUNIT_ASSERT_EQUAL(1, (int) a_ptr.count());
	{
		mt::SmartPtr<BaseClass> b_ptr = a_ptr.cast<BaseClass> ();
		CPPUNIT_ASSERT_EQUAL(2, (int) a_ptr.count());
		CPPUNIT_ASSERT(a_ptr.data() == b_ptr.data());
		CPPUNIT_ASSERT_EQUAL(std::string("DerivedClass"), b_ptr->str());
	}
	CPPUNIT_ASSERT_EQUAL(1, (int) a_ptr.count());
}

void TestSmartPtr::testSmartPtrInVector() {

	typedef mt::SmartPtr<BaseClass> BaseClassPtr;


		BaseClassPtr a, b;

		std::vector<BaseClassPtr> *v = new std::vector<BaseClassPtr>;

		a = new BaseClass();
		b = new BaseClass();
		CPPUNIT_ASSERT_EQUAL(1, (int) a.count());
		CPPUNIT_ASSERT_EQUAL(1, (int) b.count());

		v->push_back(a);
		v->push_back(b);

		CPPUNIT_ASSERT_EQUAL(2, (int) a.count());
		CPPUNIT_ASSERT_EQUAL(2, (int) b.count());

		delete v;

		CPPUNIT_ASSERT_EQUAL(1, (int) a.count());
		CPPUNIT_ASSERT_EQUAL(1, (int) b.count());


}



void TestSmartPtr::testSmartPtrInMap() {

	typedef mt::SmartPtr<BaseClass> BaseClassPtr;


		BaseClassPtr a, b;

		std::map<std::string, BaseClassPtr> *m = new std::map<std::string, BaseClassPtr>;

		a = new BaseClass();
		b = new BaseClass();

		CPPUNIT_ASSERT_EQUAL(1, (int) a.count());
		CPPUNIT_ASSERT_EQUAL(1, (int) b.count());


		(*m)[std::string("one")] = a;
		(*m)[std::string("two")] = b;

		CPPUNIT_ASSERT_EQUAL(2, (int) a.count());
		CPPUNIT_ASSERT_EQUAL(2, (int) b.count());

		delete m;

		CPPUNIT_ASSERT_EQUAL(1, (int) a.count());
		CPPUNIT_ASSERT_EQUAL(1, (int) b.count());


}


