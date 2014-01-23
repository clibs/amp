
//
// amp.c
//
// Copyright (c) 2014 TJ Holowaychuk <tj@vision-media.ca>
//

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "amp.h"

/*
 * Read u32be.
 */

static uint32_t
read_u32_be(char *buf) {
  uint32_t n = 0;
  n |= buf[0] << 24;
  n |= buf[1] << 16;
  n |= buf[2] << 8;
  n |= buf[3];
  return n;
}

/*
 * Write u32be.
 */

static void
write_u32_be(char *buf, uint32_t n) {
  buf[0] = n >> 24 & 0xff;
  buf[1] = n >> 16 & 0xff;
  buf[2] = n >> 8 & 0xff;
  buf[3] = n & 0xff;
}

/*
 * Decode the `msg` header in `buf`.
 */

void
amp_decode(amp_t *msg, char *buf) {
  msg->version = buf[0] >> 4;
  msg->argc = buf[0] & 0xf;
  msg->buf = buf + 1;
}

/*
 * Decode `msg` argument, returning a buffer
 * that must be freed by the user and progressing
 * the msg->buf cursor.
 */

char *
amp_decode_arg(amp_t *msg) {
  uint32_t len = read_u32_be(msg->buf);
  msg->buf += 4;

  char *buf = malloc(len);
  if (!buf) return NULL;

  memcpy(buf, msg->buf, len);
  msg->buf += len;
  return buf;
}

/*
 * Encode the AMP message argv.
 *
 *         0        1 2 3 4     <length>    ...
 *   +------------+----------+------------+
 *   | <ver/argc> | <length> | <data>     | additional arguments
 *   +------------+----------+------------+
 *
 */

char *
amp_encode(char **argv, int argc) {
  size_t len = 1;
  size_t lens[argc];

  // length
  for (int i = 0; i < argc; ++i) {
    len += 4;
    lens[i] = strlen(argv[i]);
    len += lens[i];
  }

  // alloc
  char *buf = malloc(len);
  char *ret = buf;
  if (!buf) return NULL;

  // ver/argc
  *buf++ = AMP_VERSION << 4 | argc;

  // encode
  for (int i = 0; i < argc; ++i) {
    size_t len = lens[i];

    write_u32_be(buf, len);
    buf += 4;

    memcpy(buf, argv[i], len);
    buf += len;
  }

  return ret;
}