/*
 * unuseless.cpp
 *
 *  Created on: Nov 25, 2018
 *      Author: jhb
 */
#if 0
#include <iostream>

int partition(int data[], int length, int start, int end) {
	if (data == NULL || length <= 0 || start < 0 || end >= length)
		return -1;

	int index = random(start, end);
	swap(&data[index], &data[end]);
	int small = start;
	for (index = start; index < end; ++index) {
		if (data[index] < data[end]) {
			++small;
			if (small != index)
				swap(&data[small], &data[index]);
		}
	}
	++small;
	swap(&data[small], &data[end]);
}
void quickSort(int data[], int length, int start, int end) {
	if (start == end)
		return;
	int index = partition(data, length, start, end);
	if (index > start)
		quickSort(data, length, start, index - 1);
	if (index < end)
		quickSort(data, length, index + 1, end);
}
#endif


//
//
//
//
