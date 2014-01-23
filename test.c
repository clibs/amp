
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "amp.h"

int
main(){
  char *args[] = { "some", "stuff", "here" };

  // encode
  char *buf = amp_encode(args, 3);

  // header
  amp_t msg = {0};
  amp_decode(&msg, buf);
  assert(1 == msg.version);
  assert(3 == msg.argc);

  // args
  for (int i = 0; i < msg.argc; ++i) {
    char *arg = amp_decode_arg(&msg);
    switch (i) {
      case 0:
        assert(0 == strcmp("some", arg));
        break;
      case 1:
        assert(0 == strcmp("stuff", arg));
        break;
      case 2:
        assert(0 == strcmp("here", arg));
        break;
    }
  }

  printf("ok\n");

  return 0;
}