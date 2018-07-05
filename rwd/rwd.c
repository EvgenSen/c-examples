#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, const char *argv[])
{
  unsigned char buffer[512] = {0};
  int offset;
  sscanf(argv[2], "%x", &offset);

  if (argc != 4)
    goto __print_usage;

  if(!strcmp(argv[1], "r"))
  {
    int mtd = open(argv[3], O_RDWR);
    if(mtd > 0)
    {
      lseek(mtd, offset, SEEK_SET);
      read(mtd, buffer, sizeof(buffer));
      printf(" Read addr %#010x: \"%s\" \n", offset, buffer);
      close(mtd);
      return 0;
    }
    else
    {
      perror("Can't open file");
      return 1;
    }
  }
  else if (!strcmp(argv[1], "w"))
  {
    int mtd = open(argv[3], O_RDWR);
    size_t sz = 0;
    if(mtd > 0)
    {
      lseek(mtd, offset, SEEK_SET);
      if((sz = write(mtd, "my test msg for flash", 128) ) < 0)
      {
        perror("Can't write to file");
        return 1;
      }
      else
      {
        printf("Successful write to addr 0x%x \n",offset);
      }
      fsync(mtd);
      close(mtd);
      return 0;
    }
    else
    {
      perror("Can't open file");
      return 1;
    }
  }

__print_usage:
  fprintf(stderr, "Usage:\n");
  fprintf(stderr, "    %s r/w offset dev\n", argv[0]);
  fprintf(stderr, "\n");
  return 1;
}
