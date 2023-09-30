#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>

#ifndef _KFMAP_H_
#define _KFMAP_H_
#ifdef __cplusplus
extern "C" {
#endif

#define DEVICE_FILE "/dev/flipover"
#define PAGE_SIZE 4096

struct Vector3
{
  float x, y, z;
};
struct view_matrix_t
{
  float matrix[4][4];
};
struct player
{
  struct Vector3 pos;
  int health;
  int team;
  char name[128];
};

struct shared
{
  struct player players[32];
  struct view_matrix_t matrix;
};
static int fd;

int map(struct shared **players);
int unmap(struct shared **players);
#ifdef __cplusplus
}
#endif
#endif