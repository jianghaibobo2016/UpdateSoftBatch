/*
 * PrintBuff.h
 *
 *  Created on: Oct 19, 2018
 *      Author: jhb
 */

#ifndef SRC_PRINTBUFF_H_
#define SRC_PRINTBUFF_H_

#include <stdio.h>
#include "dp_sd_type.h"

namespace muduo {
class PrintBuff {
public:
	void static printBufferByHex(const DP_CHAR *note, const void *buff,
			DP_U32 len) {
		printf("%s\n", note);
		DP_S32 iPos = 0;
		for (DP_U32 iPos = 0; iPos < len; iPos++) {
			if (iPos % 10 == 0 && iPos != 0)
				printf("\n");
			printf("%02x ", ((DP_U8 *) buff)[iPos]);
		}
		printf("\n");
	}
};

}

#endif /* SRC_PRINTBUFF_H_ */
