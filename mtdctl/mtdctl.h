/*
 * mtdctl.h
 *
 * Author: Evgeniy Sennikov <sennikov.work@ya.ru>
 *
 * Licence: GPLv3
 */

#ifndef MTDCTL_H
#define MTDCTL_H

#define BUFF_SIZE 2048

enum action_e
{
	ACT_NONE = 0,
	ACT_READ,
	ACT_WRITE,
	ACT_INFO,
	ACT_TEST,
};

// struct net_dev_priv
// {
// 	unsigned int flag;
// 	char data[64];
// };

#endif // MTDCTL_H
