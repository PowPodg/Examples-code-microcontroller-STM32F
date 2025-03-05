#include "SampleFilter.h"
SampleFilter sf;

static int filter_taps[SAMPLEFILTER_TAP_NUM] = {
		-2152,
		  -2870,
		  -3216,
		  -1977,
		  1197,
		  5897,
		  10965,
		  14879,
		  16351,
		  14879,
		  10965,
		  5897,
		  1197,
		  -1977,
		  -3216,
		  -2870,
		  -2152
};

void SampleFilter_init(SampleFilter* f) {
  int i;
  for(i = 0; i < SAMPLEFILTER_TAP_NUM; ++i)
    f->history[i] = -1091;
  f->last_index = 0;
}

void SampleFilter_put(SampleFilter* f, int input) {
  f->history[f->last_index++] = input;
  if(f->last_index == SAMPLEFILTER_TAP_NUM)
    f->last_index = 0;
}

int SampleFilter_get(SampleFilter* f) {
  long acc = 0;
  int index = f->last_index, i;
  for(i = 0; i < SAMPLEFILTER_TAP_NUM; ++i) {
    index = index != 0 ? index-1 : SAMPLEFILTER_TAP_NUM-1;
    acc += (long)f->history[index] * filter_taps[i];
  };
  return acc >> 16;
}
