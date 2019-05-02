/*
 * test_VARP.cpp
 * 
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2013-4-18
 *      Author: fasiondog
 */

#ifdef TEST_ALL_IN_ONE
    #include <boost/test/unit_test.hpp>
#else
    #define BOOST_TEST_MODULE test_hikyuu_indicator_suite
    #include <boost/test/unit_test.hpp>
#endif

#include <fstream>
#include <hikyuu/StockManager.h>
#include <hikyuu/indicator/crt/KDATA.h>
#include <hikyuu/indicator/crt/VARP.h>
#include <hikyuu/indicator/crt/PRICELIST.h>

using namespace hku;

/**
 * @defgroup test_indicator_VARP test_indicator_VARP
 * @ingroup test_hikyuu_indicator_suite
 * @{
 */

/** @par 检测点 */
BOOST_AUTO_TEST_CASE( test_VARP ) {
    /** @arg n > 1 的正常情况 */
    PriceList d;
    for (size_t i = 0; i < 15; ++i) {
        d.push_back(i+1);
    }
    d[5] = 4.0;
    d[7] = 4.0;
    d[11] = 6.0;

    Indicator ind = PRICELIST(d);
    Indicator dev = VARP(ind, 10);
    BOOST_CHECK(dev.name() == "VARP");
    BOOST_CHECK(dev.size() == 15);
    BOOST_CHECK(dev[8] == Null<price_t>());
    BOOST_CHECK(std::fabs(dev[9] - 7.69) < 0.01 );
    BOOST_CHECK(std::fabs(dev[10] - 8.89) < 0.01 );
    BOOST_CHECK(std::fabs(dev[11] - 7.21) < 0.01 );
    BOOST_CHECK(std::fabs(dev[12] - 9.61) < 0.01 );
    BOOST_CHECK(std::fabs(dev[13] - 12.01) < 0.01 );
    BOOST_CHECK(std::fabs(dev[14] - 14.41) < 0.01 );

    /** @arg n = 1时 */
    dev = VARP(ind, 1);
    BOOST_CHECK(dev.name() == "VARP");
    BOOST_CHECK(dev.size() == 15);
    for (size_t i = 0; i < dev.size(); ++i) {
        BOOST_CHECK(dev[i] == Null<price_t>());
    }

    /** @arg operator() */
    Indicator expect = VARP(ind, 10);
    dev = VARP(10);
    Indicator result = dev(ind);
    BOOST_CHECK(result.size() == expect.size());
    for (size_t i = 0; i < expect.size(); ++i) {
        BOOST_CHECK(result[i] == expect[i]);
    }
}


//-----------------------------------------------------------------------------
// test export
//-----------------------------------------------------------------------------
#if HKU_SUPPORT_SERIALIZATION

/** @par 检测点 */
BOOST_AUTO_TEST_CASE( test_VARP_export ) {
    StockManager& sm = StockManager::instance();
    string filename(sm.tmpdir());
    filename += "/VARP.xml";

    Stock stock = sm.getStock("sh000001");
    KData kdata = stock.getKData(KQuery(-20));
    Indicator ma1 = VARP(CLOSE(kdata), 10);
    {
        std::ofstream ofs(filename);
        boost::archive::xml_oarchive oa(ofs);
        oa << BOOST_SERIALIZATION_NVP(ma1);
    }

    Indicator ma2;
    {
        std::ifstream ifs(filename);
        boost::archive::xml_iarchive ia(ifs);
        ia >> BOOST_SERIALIZATION_NVP(ma2);
    }

    BOOST_CHECK(ma2.name() == "VARP");
    BOOST_CHECK(ma1.size() == ma2.size());
    BOOST_CHECK(ma1.discard() == ma2.discard());
    BOOST_CHECK(ma1.getResultNumber() == ma2.getResultNumber());
    for (size_t i = 0; i < ma1.size(); ++i) {
        BOOST_CHECK_CLOSE(ma1[i], ma2[i], 0.00001);
    }
}
#endif /* #if HKU_SUPPORT_SERIALIZATION */

/** @} */


