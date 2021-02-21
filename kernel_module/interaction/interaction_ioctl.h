#ifndef INTERACTION_IOCTL_H
#define INTERACTION_IOCTL_H

#define MODULE_NAME          "interaction_ioctl"

#define INTER_MOD_MAJOR_NUM  100
#define INTER_MOD_DEV_NAME   MODULE_NAME

#define BUFFER_SIZE          32

typedef struct {
	int flag;
	char buffer[BUFFER_SIZE];
} ioctl_data_t;

#define GET_INTER_MOD_INT       _IOR (INTER_MOD_MAJOR_NUM, 1, int)
#define SET_INTER_MOD_INT       _IOW (INTER_MOD_MAJOR_NUM, 2, int)
#define GET_INTER_MOD_DATA      _IOR (INTER_MOD_MAJOR_NUM, 3, ioctl_data_t)
#define SET_INTER_MOD_DATA      _IOW (INTER_MOD_MAJOR_NUM, 4, ioctl_data_t)

#endif // INTERACTION_IOCTL_H
