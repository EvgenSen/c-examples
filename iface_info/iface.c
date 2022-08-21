/*
 * iface.c - gets iface info
 *
 * This example gets a list of network interfaces and their parameters.
 * Output format look like ifconfig
 *
 * Author: Evgeniy Sennikov <sennikov.work@ya.ru>
 *
 * Licence: GPLv3
 */

#include <sys/ioctl.h>
#include <net/if.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_ADDR_STR 64

/*
 * Функция получает битовую маску флагов интерфейса на основе
 * его имени и возвращает значение состояния IFF_UP.
 * Можно использовать для получения состояния других флагов
 * из списка ниже:
 *   IFF_UP           Интерфейс активен.
 *   IFF_BROADCAST    Установлен правильный широковещательный адрес.
 *   IFF_DEBUG        Флаг внутренней отладки.
 *   IFF_LOOPBACK     Интерфейс является устройством обратной петли.
 *   IFF_POINTOPOINT  Интерфейс является point-to-point соединением.
 *   IFF_RUNNING      Ресурсы выделены.
 *   IFF_NOARP        Hет протокола arp, адрес назначения 2-ого уровня (L2) не установлен.
 *   IFF_PROMISC      Интерфейс в "смешанном" (promiscuous) режиме.
 *   IFF_NOTRAILERS   Избегать использования trailers.
 *   IFF_ALLMULTI     Принимать все многоадресные пакеты.
 *   IFF_MASTER       Мастер связки балансирования загрузкой (load balancing bundle).
 *   IFF_SLAVE        Подчиненный связки балансирования нагрузкой.
 *   IFF_MULTICAST    Поддерживает многоадресное вещание.
 *   IFF_PORTSEL      Может выбирать тип носителя с помощью ifmap.
 *   IFF_AUTOMEDIA    Запущен автоматический выбор носителя.
 *   IFF_DYNAMIC      Эти адреса потеряны, если интерфейс неактивен.
 */
int get_iface_state(char *if_name)
{
	int          fd;
	struct ifreq pIfr;
	memset(&pIfr, 0, sizeof(pIfr));

	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf("%s: socket() error\n", __func__);
		return -1;
	}

	strcpy(pIfr.ifr_name, if_name);

	if (ioctl(fd, SIOCGIFFLAGS, &pIfr) < 0)
	{
		printf("%s: get SIOCGIFFLAGS for %s error\n", __func__, if_name);
		close(fd);
		return -1;
	}

	close(fd);
	return (pIfr.ifr_flags & IFF_UP) ? 1 : 0;
}

/*
 * Функция получает IP адрес интерфейса при помощи вызова
 * SIOCGIFADDR и записывает его в структуру sockaddr_in
 */
int get_iface_ip(char *if_name, struct sockaddr_in *sin)
{
	int          fd;
	char *       ip;
	struct ifreq pIfr;

	memset(&pIfr, 0, sizeof(pIfr));

	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf("%s: socket() error\n", __func__);
		return -1;
	}

	strcpy(pIfr.ifr_name, if_name);

	if (ioctl(fd, SIOCGIFADDR, &pIfr) < 0)
	{
		// printf("%s: get SIOCGIFADDR for %s error\n", __func__, if_name);
		close(fd);
		return -1;
	}

	memcpy(sin, &(pIfr.ifr_addr), sizeof(struct sockaddr_in));
	close(fd);
	return 0;
}

/*
 * Функция получает маску интерфейса при помощи вызова
 * SIOCGIFNETMASK и записывает ее в структуру sockaddr_in
 */
int get_iface_mask(char *if_name, struct sockaddr_in *sin)
{
	int          fd;
	char *       ip;
	struct ifreq pIfr;

	memset(&pIfr, 0, sizeof(pIfr));

	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf("%s: socket() error\n", __func__);
		return -1;
	}

	strcpy(pIfr.ifr_name, if_name);

	if (ioctl(fd, SIOCGIFNETMASK, &pIfr) < 0)
	{
		// printf("%s: get SIOCGIFNETMASK for %s error\n", __func__, if_name);
		close(fd);
		return -1;
	}

	memcpy(sin, &(pIfr.ifr_addr), sizeof(struct sockaddr_in));
	close(fd);
	return 0;
}

int main(int argc, const char *argv[])
{
	int          i;
	struct ifreq pIfr;
	int          fd = socket(AF_INET, SOCK_DGRAM, 0);
	memset(&pIfr, 0, sizeof(pIfr));

	for (i = 1;; i++)
	{
		char               ip_str[MAX_ADDR_STR], mask_str[MAX_ADDR_STR];
		struct sockaddr_in sin;
		pIfr.ifr_ifindex = i;
		if (ioctl(fd, SIOCGIFNAME, &pIfr) < 0)
			break;

		if (get_iface_ip(pIfr.ifr_name, &sin) == 0)
			strcpy(ip_str, inet_ntoa(sin.sin_addr));
		else
			strcpy(ip_str, "none");

		if (get_iface_mask(pIfr.ifr_name, &sin) == 0)
			strcpy(mask_str, inet_ntoa(sin.sin_addr));
		else
			strcpy(mask_str, "none");

		printf("%-10s  Index: %d  State: %s\n", pIfr.ifr_name, pIfr.ifr_ifindex,
		       get_iface_state(pIfr.ifr_name) ? "Up" : "Down");
		printf("%-10s  Addr: %s  Mask: %s\n", " ", ip_str, mask_str);
		printf("\n");
	}

	close(fd);
	return 0;
}
