/*
 * UPNDAY.h
 *  
 *  Copyright (c) 2019 hikyuu.org
 * 
 *  Created on: 2019-5-2
 *      Author: fasiondog
 */

#ifndef INDICATOR_CRT_UPNDAY_H_
#define INDICATOR_CRT_UPNDAY_H_

#include "EVERY.h"
#include "REF.h"

namespace hku {

/**
 * 连涨周期, UPNDAY(CLOSE,M)表示连涨M个周期
 * @ingroup Indicator
 */
Indicator UPNDAY(const Indicator& ind, int n = 3);

inline Indicator UPNDAY(const Indicator& ind, int n) {
    return EVERY(ind > REF(ind, 1), n);
}

} /* namespace */

#endif /* INDICATOR_CRT_UPNDAY_H_ */
