#include "vdp.h"

#define NS _NUMBER_OF_STATE_VARS_
int main(void)
{
  int mesh_size=5;
  MY_FLOAT x[NS] = {2.0086198608748438, 0.0};
  MY_FLOAT t=0.0, tf=6.663286859323084, ht;

  ht = tf*1.0/mesh_size;
  do {
      printf("% .15e % .15e % .15e\n", t/tf, x[0], x[1]);
    }
  while (taylor_uniform_step_vdp_eq(&t, x, +1, 2, -16, -16, &tf, &ht, NULL, NULL) != 1);

  return 0;
}
