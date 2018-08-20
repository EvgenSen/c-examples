#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <mtd/mtd-user.h>

/* default values */
#define DEF_MTD_NAME   "/dev/mtdblock0"
#define TEST_MTD_NAME  "/dev/mtdblock4"
#define DEF_OUT_NAME   "/tmp/rwd.txt"
#define DEF_MAX_COUNT  1
#define DEF_OFFSET     0x8

FILE *log_file;

#define print_dbg(...) fprintf (log_file, __VA_ARGS__)

void printProgress (int i, int max)
{
  int step = max / 10;
  if (i == max-1)
    printf(" [==========] 100%% | Count: %5d/%d\n", i+1, max);
  else if (max < 100)
    return;
  else if (i == step)
    printf(" [=         ] 10 %% | Count: %5d/%d\n", i, max);
  else if (i == step*2)
    printf(" [==        ] 20 %% | Count: %5d/%d\n", i, max);
  else if (i == step*3)
    printf(" [===       ] 30 %% | Count: %5d/%d\n", i, max);
  else if (i == step*4)
    printf(" [====      ] 40 %% | Count: %5d/%d\n", i, max);
  else if (i == step*5)
    printf(" [=====     ] 50 %% | Count: %5d/%d\n", i, max);
  else if (i == step*6)
    printf(" [======    ] 60 %% | Count: %5d/%d\n", i, max);
  else if (i == step*7)
    printf(" [=======   ] 70 %% | Count: %5d/%d\n", i, max);
  else if (i == step*8)
    printf(" [========  ] 80 %% | Count: %5d/%d\n", i, max);
  else if (i == step*9)
    printf(" [========= ] 90 %% | Count: %5d/%d\n", i, max);
}

int readMTD (int offset, int max_cnt, char * mtd_name)
{
  unsigned char buffer[512] = {0};
  int i = 0;

  for (i=0; i<max_cnt; i++)
  {
    int mtd = open(mtd_name, O_RDWR);
    if(mtd > 0)
    {
      if (max_cnt != 1)
        print_dbg("i: %5d\n", i);
      lseek(mtd, offset, SEEK_SET);
      read(mtd, buffer, sizeof(buffer));
      buffer[511] = '\0';
      print_dbg("Read addr %#010x: \"%s\"\n", offset, buffer);
      close(mtd);
    }
    else
    {
      perror("Can't open file");
      return 1;
    }
    if (max_cnt != 1)
      printProgress(i, max_cnt);
  }
  return 0;
}

int writeMTD (int offset, int max_cnt, char * mtd_name)
{
  int mtd = open(mtd_name, O_RDWR);
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
      printf("Successful write to addr %#010x on %s\n", offset, mtd_name);
    }
    fsync(mtd);
    close(mtd);
    return 0;
  }
  else
  {
    perror("Can't open file");
  }
  return 1;
}

int main(int argc, const char *argv[])
{
  int ret_value = -1;
  int i = 0;
  int offset  = DEF_OFFSET;
  int max_cnt = DEF_MAX_COUNT;
  char mtd_name[128] = DEF_MTD_NAME;
  char output_name[256] = {0};
  log_file = stderr;

  if (argc >= 10 || argc == 1)
    goto __print_usage;

  /* Parsing args */
  for (i=1; i<argc; i++)
  {
    if (!strcmp(argv[i], "-o") || !strcmp(argv[i], "--offset"))
    {
      sscanf(argv[i+1], "%x", &offset);
    }
    else if (!strcmp(argv[i], "-f") || !strcmp(argv[i], "--file"))
    {
      strcpy(output_name, argv[i+1]);
      if ((log_file = fopen(output_name, "w"))==NULL) 
      {
        perror("Can't create output file");
        goto __exit;
      }
    }
    else if (!strcmp(argv[i], "-m") || !strcmp(argv[i], "--mtd"))
    {
      strcpy(mtd_name, argv[i+1]);
    }
    else if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--count"))
    {
      max_cnt = atoi(argv[i+1]);
    }
    else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help") || !strcmp(argv[i], "?"))
    {
      goto __print_usage;
    }
  }

  /* Start action */
  if(!strcmp(argv[1], "-r") || !strcmp(argv[1], "read"))
  {
    fprintf(stderr, "Start read from: %s Offset: %#010x Count: %d\n", mtd_name, offset, max_cnt);
    ret_value = readMTD (offset, max_cnt, mtd_name);
    goto __exit;
  }
  else if(!strcmp(argv[1], "-t") || !strcmp(argv[1], "test"))
  {
    if(!strcmp(argv[2], "dm"))
    {
      if (max_cnt == 1)
        max_cnt = 100;
      ret_value = 0;
      fprintf(stderr, "Start test Offset: %#010x Count: %d\n", offset, max_cnt);
      fprintf(stderr, "First dev: %s\n", DEF_MTD_NAME);
      fprintf(stderr, "Second dev: /dev/mtdblock4\n");
      for (i=0; i<max_cnt; i++)
      {
        print_dbg("i: %5d\n", i);
        ret_value += readMTD (offset, 1, DEF_MTD_NAME);
        //usleep(50);
        ret_value += readMTD (offset, 1, TEST_MTD_NAME);
        printProgress(i, max_cnt);
      }
    } 
    else if (!strcmp(argv[2], "rw"))
    {
      // TODO: write, read and compare
    }
    else
    {
      fprintf(stderr, "Usage:\n");
      fprintf(stderr, "    test dm - Write to different mtd\n");
      //fprintf(stderr, "    test rw - Read and write operation\n");
    }
    goto __exit;
  }
  else if(!strcmp(argv[1], "-i") || !strcmp(argv[1], "info"))
  {
    mtd_info_t mtd_info;
    int fd = open(mtd_name, O_RDWR);
    ioctl(fd, MEMGETINFO, &mtd_info);
 
    printf("MTD type: %u\n", mtd_info.type);
    printf("MTD total size : %u bytes\n", mtd_info.size);
    printf("MTD erase size : %u bytes\n", mtd_info.erasesize);

    ret_value = 0;
    goto __exit;
  }
  else if (!strcmp(argv[1], "-w") || !strcmp(argv[1], "write"))
  {
    fprintf(stderr, "Start write to: %s Offset: %#010x Count: %d\n", mtd_name, offset, max_cnt);
    ret_value = writeMTD (offset, max_cnt, mtd_name);
    goto __exit;
  }

__print_usage:
  fprintf(stderr, "Usage:\n");
  fprintf(stderr, "    %s [action] [args]\n", argv[0]);
  fprintf(stderr, "action:\n");
  fprintf(stderr, "    -r, read   read form mtd device\n");
  fprintf(stderr, "    -w, write  write to mtd device\n");
  fprintf(stderr, "    -i, info   get information of mtd device\n");
  fprintf(stderr, "    -t, test   start testing mtd device\n");
  fprintf(stderr, "args:\n");
  fprintf(stderr, "    -o, --offset  offset in HEX from the beginning of dev (default:%#010x)\n", DEF_OFFSET);
  fprintf(stderr, "    -f, --file    output file (default:stderr)\n");
  fprintf(stderr, "    -m, --mtd     mtd device name (default:%s)\n", DEF_MTD_NAME);
  fprintf(stderr, "    -c, --count   number of iterations (default:%d)\n", DEF_MAX_COUNT);
  fprintf(stderr, "\n");
  fprintf(stderr, "Example\n");
  fprintf(stderr, "    %s -r -o %#010x -m %s -f /tmp/rwd.txt -c %d\n", argv[0], DEF_OFFSET, DEF_MTD_NAME, DEF_MAX_COUNT);
  fprintf(stderr, "\n");
__exit:
  if(log_file && log_file != stderr)
  {
    fclose((FILE *)log_file);
    log_file = NULL;
  }
  //fprintf(stderr, "Program exit with code %d\n", ret_value);
  return ret_value;
}
