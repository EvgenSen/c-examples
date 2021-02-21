#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/version.h>

#include "network_mod.h"

static struct net_device *dev;

static int network_mod_open(struct net_device *dev)
{
	struct net_dev_priv *priv = netdev_priv(dev);

	strcpy(priv->data, "Data from open() func");

	printk(KERN_INFO "%s: Open %s\n", MODULE_NAME, dev->name);
	/* запуск очереди для передачи данных */
	netif_start_queue( dev );
	return 0;
}

static int network_mod_close(struct net_device *dev)
{
	struct net_dev_priv *priv = netdev_priv(dev);

	printk(KERN_INFO "%s: Close %s (Private data: \"%s\")\n", MODULE_NAME, dev->name, priv->data);
	/* остановка очереди для передачи данных */
	netif_stop_queue( dev );
	return 0;
}

static int stub_start_xmit(struct sk_buff *skb, struct net_device *dev)
{

	// int i=0;
	// printk(KERN_INFO "%s: TX len=%d data:\n", MODULE_NAME, skb->len);
	// for (i = 0; i < skb->len; i++)
	// {
	// 	printk("%02x ", skb->data[i]);
	// 	if( (i+1) % 16 == 0 )
	// 		printk("\n");
	// }
	// printk("\n");
	dev->stats.tx_dropped++;

	dev_kfree_skb( skb );
	return 0;
}

static struct net_device_ops network_mod_ndo =
{
	.ndo_open = network_mod_open,
	.ndo_stop = network_mod_close,
	.ndo_start_xmit = stub_start_xmit,
	.ndo_set_mac_address = eth_mac_addr,
};

static void network_mod_setup(struct net_device *dev)
{
	int i;

	printk(KERN_INFO "%s: Setup\n", MODULE_NAME);
	/* Set dummy MAC = 00:01:02:03:04:05 */
	for(i = 0; i < ETH_ALEN; i++)
		dev->dev_addr[i] = (char) i;
	ether_setup(dev);
	dev->netdev_ops = &network_mod_ndo;
}

static int __init network_mod_init(void)
{
	printk(KERN_INFO "%s: Init\n", MODULE_NAME);

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 17, 0))
	dev = alloc_netdev(sizeof(struct net_dev_priv), "dummy%d", network_mod_setup);
#else
	dev = alloc_netdev(sizeof(struct net_dev_priv), "dummy%d", NET_NAME_UNKNOWN, network_mod_setup);
#endif

	if(register_netdev(dev))
	{
		printk(KERN_WARNING "%s: Failed to register dummy device\n", MODULE_NAME);
		free_netdev(dev);
		return -1;
	}
	printk(KERN_INFO "%s: Init %s - Ok\n", MODULE_NAME, dev_name(&dev->dev));
	return 0;
}

static void __exit network_mod_exit(void)
{
	printk(KERN_INFO "%s: Exit\n", MODULE_NAME);
	unregister_netdev(dev);
	free_netdev(dev);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Evgeniy Sennikov <sennikov.work@ya.ru>");
MODULE_DESCRIPTION("Simple network module");
MODULE_VERSION("1.0");

module_init(network_mod_init);
module_exit(network_mod_exit);
