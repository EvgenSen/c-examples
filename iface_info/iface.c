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
int get_if_state(char *if_name)
{
  int fd;
  struct ifreq pIfr;
  memset(&pIfr, 0, sizeof(pIfr));

  if ((fd = socket (AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    printf("%s: socket() error", __func__);
    return -1;
  }

  strcpy(pIfr.ifr_name, if_name);

  if (ioctl(fd, SIOCGIFFLAGS, &pIfr) < 0)
  {
    printf("%s: get SIOCGIFFLAGS for %s error", __func__, if_name);
    close(fd);
    return -1;
  }

  close(fd);
  return (pIfr.ifr_flags & IFF_UP) ? 1 : 0;
}

int main(int argc, const char *argv[])
{
  int i;
  struct ifreq pIfr;
  int fd = socket(AF_INET, SOCK_DGRAM, 0);
  memset(&pIfr, 0, sizeof(pIfr));

  for(i=1;;i++)
  {
    pIfr.ifr_ifindex = i;
    if (ioctl(fd, SIOCGIFNAME, &pIfr) < 0)
    {
      break;
    }
    printf("%s\tIndex: %d State: %s\n", pIfr.ifr_name, pIfr.ifr_ifindex, get_if_state(pIfr.ifr_name) ? "Up" : "Down");
    printf("\n");
  }

  close(fd);
  return 0;
}
