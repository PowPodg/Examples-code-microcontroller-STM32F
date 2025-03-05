#ifndef SAMPLEFILTER_H_
#define SAMPLEFILTER_H_



#define SAMPLEFILTER_TAP_NUM 17

typedef struct {
  int history[SAMPLEFILTER_TAP_NUM];
  unsigned int last_index;
} SampleFilter;

extern SampleFilter sf;
void SampleFilter_init(SampleFilter* f);
void SampleFilter_put(SampleFilter* f, int input);
int SampleFilter_get(SampleFilter* f);

#endif
