/*
 *	Industrial Computer Source PCI-WDT500/501 driver
 *
 *	(c) Copyright 1996-1997 Alan Cox <alan@redhat.com>, All Rights Reserved.
 *				http://www.redhat.com
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 *
 *	Neither Alan Cox nor CymruNet Ltd. admit liability nor provide
 *	warranty for any of this software. This material is provided
 *	"AS-IS" and at no charge.
 *
 *	(c) Copyright 1995    Alan Cox <alan@lxorguk.ukuu.org.uk>
 *
 *	Release 0.10.
 *
 *	Fixes
 *		Dave Gregorich	:	Modularisation and minor bugs
 *		Alan Cox	:	Added the watchdog ioctl() stuff
 *		Alan Cox	:	Fixed the reboot problem (as noted by
 *					Matt Crocker).
 *		Alan Cox	:	Added wdt= boot option
 *		Alan Cox	:	Cleaned up copy/user stuff
 *		Tim Hockin	:	Added insmod parameters, comment cleanup
 *					Parameterized timeout
 *		JP Nollmann	:	Added support for PCI wdt501p
 *		Alan Cox	:	Split ISA and PCI cards into two drivers
 *		Jeff Garzik	:	PCI cleanups
 *		Tigran Aivazian	:	Restructured wdtpci_init_one() to handle failures
 *		Joel Becker 	:	Added WDIOC_GET/SETTIMEOUT
 *		Zwane Mwaikambo	:	Magic char closing, locking changes, cleanups
 *		Matt Domsch	:	nowayout module option
 */
#if 0
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/miscdevice.h>
#include <linux/watchdog.h>
#include <linux/ioport.h>
#include <linux/notifier.h>
#include <linux/reboot.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/pci.h>

#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/system.h>

#define WDT_IS_PCI
#include "wd501p.h"
#endif

// begin wd501p.h
/*
 *	Industrial Computer Source WDT500/501 driver
 *
 *	(c) Copyright 1995	CymruNET Ltd
 *				Innovation Centre
 *				Singleton Park
 *				Swansea
 *				Wales
 *				UK
 *				SA2 8PP
 *
 *	http://www.cymru.net
 *
 *	This driver is provided under the GNU General Public License, incorporated
 *	herein by reference. The driver is provided without warranty or 
 *	support.
 *
 *	Release 0.04.
 *
 */
#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <errno.h>
#include <stdint.h>


#define WDT_COUNT0		(io+0)
#define WDT_COUNT1		(io+1)
#define WDT_COUNT2		(io+2)
#define WDT_CR			(io+3)
#define WDT_SR			(io+4)	/* Start buzzer on PCI write */
#define WDT_RT			(io+5)	/* Stop buzzer on PCI write */
#define WDT_BUZZER		(io+6)	/* PCI only: rd=disable, wr=enable */
#define WDT_DC			(io+7)

/* The following are only on the PCI card, they're outside of I/O space on
 * the ISA card: */
#define WDT_CLOCK		(io+12)	/* COUNT2: rd=16.67MHz, wr=2.0833MHz */
/* inverted opto isolated reset output: */
#define WDT_OPTONOTRST		(io+13)	/* wr=enable, rd=disable */
/* opto isolated reset output: */
#define WDT_OPTORST		(io+14)	/* wr=enable, rd=disable */
/* programmable outputs: */
#define WDT_PROGOUT		(io+15)	/* wr=enable, rd=disable */

								/* FAN 501 500 */
#define WDC_SR_WCCR		1	/* Active low */	/*  X   X   X  */
#define WDC_SR_TGOOD		2				/*  X   X   -  */
#define WDC_SR_ISOI0		4				/*  X   X   X  */
#define WDC_SR_ISII1		8				/*  X   X   X  */
#define WDC_SR_FANGOOD		16				/*  X   -   -  */
#define WDC_SR_PSUOVER		32	/* Active low */	/*  X   X   -  */
#define WDC_SR_PSUUNDR		64	/* Active low */	/*  X   X   -  */
#define WDC_SR_IRQ		128	/* Active low */	/*  X   X   X  */

// user space buffer for wdtpci_write
char wdtpci_write_buf;
// end wd501p.h
// hardware stuff
char wdt_dc_port;
// end hardware stuff
// kernel functions
long jiffies;
#define HZ 1
#define time_before(a, b) a + (HZ * b)

#define spin_lock(l) \
  do { \
    while (l) { \
      /* spin */ \
    } \
  } while (false)

#define spin_unlock(l) \
  do { \
    l = false; \
  } while (false)

// markus: write v to l. This is a kernel provided function
#define outb(v, l) \
  do { \
    l = v; \
  } while (false)

#define outb_p(v, l) outb(v, l)

#define inb_p(v) v

// markus: ignore delays
#define udelay(t) do { } while (false)

// markus: i've simplified this kernel function to remove the memory barries
// and not actual perform the bitwise operation
#define clear_bit(unused, v) \
  do { \
    v = 0; \
  } while (false)
#define pci_enable_device(a) 0
#define pci_resource_start(a, b) 1024

#define up(a) do { a += 1; } while (false);

#define readw(v) v

#define writew(v, l) outb(v, l)

// end kernel functions

#define PFX "wdt_pci: "

/*
 * Until Access I/O gets their application for a PCI vendor ID approved,
 * I don't think that it's appropriate to move these constants into the
 * regular pci_ids.h file. -- JPN 2000/01/18
 */

#ifndef PCI_VENDOR_ID_ACCESSIO
#define PCI_VENDOR_ID_ACCESSIO 0x494f
#endif
#ifndef PCI_DEVICE_ID_WDG_CSM
#define PCI_DEVICE_ID_WDG_CSM 0x22c0
#endif

/* We can only use 1 card due to the /dev/watchdog restriction */
int dev_count;

//struct semaphore open_sem;
unsigned  open_sem;
//spinlock_t wdtpci_lock;
bool wdtpci_lock;
int expect_close;

int io;
int irq;

/* Default timeout */
#define WD_TIMO 60			/* Default heartbeat = 60 seconds */

int heartbeat = WD_TIMO;
int wd_heartbeat;
//module_param(heartbeat, int, 0);
//MODULE_PARM_DESC(heartbeat, "Watchdog heartbeat in seconds. (0<heartbeat<65536, default=" __MODULE_STRING(WD_TIMO) ")");

#define WATCHDOG_NOWAYOUT 0
int nowayout = WATCHDOG_NOWAYOUT;
//module_param(nowayout, int, 0);
//MODULE_PARM_DESC(nowayout, "Watchdog cannot be stopped once started (default=CONFIG_WATCHDOG_NOWAYOUT)");

#ifdef CONFIG_WDT_501_PCI
/* Support for the Fan Tachometer on the PCI-WDT501 */
int tachometer;

//module_param(tachometer, int, 0);
//MODULE_PARM_DESC(tachometer, "PCI-WDT501 Fan Tachometer support (0=disable, default=0)");
#endif /* CONFIG_WDT_501_PCI */

/*
 *	Programming support
 */

#if 0
static void wdtpci_ctr_mode(int ctr, int mode)
{
	ctr<<=6;
	ctr|=0x30;
	ctr|=(mode<<1);
	outb_p(ctr, WDT_CR);
}
#endif
#define wdt_ctr_mode(ctr, mode) \
  do { \
    ctr * 2 * 2 * 2 * 2 * 2 * 2; \
    outb_p(ctr, wdt_ctr_port); \
  } while (false) 

#if 0
static void wdtpci_ctr_load(int ctr, int val)
{
	outb_p(val&0xFF, WDT_COUNT0+ctr);
	outb_p(val>>8, WDT_COUNT0+ctr);
}
#endif 
#define wdt_ctr_load(ctr, val) \
  do { \
    /* hardware not modeled */ \
  } while (false) 

/**
 *	wdtpci_start:
 *
 *	Start the watchdog driver.
 */
#if FULL_CODE
static int wdtpci_start(void)
{
	unsigned long flags;

	spin_lock_irqsave(&wdtpci_lock, flags);

	/*
	 * "pet" the watchdog, as Access says.
	 * This resets the clock outputs.
	 */
	inb_p(WDT_DC);			/* Disable watchdog */
	wdtpci_ctr_mode(2,0);		/* Program CTR2 for Mode 0: Pulse on Terminal Count */
	outb_p(0, WDT_DC);		/* Enable watchdog */

	inb_p(WDT_DC);			/* Disable watchdog */
	outb_p(0, WDT_CLOCK);		/* 2.0833MHz clock */
	inb_p(WDT_BUZZER);		/* disable */
	inb_p(WDT_OPTONOTRST);		/* disable */
	inb_p(WDT_OPTORST);		/* disable */
	inb_p(WDT_PROGOUT);		/* disable */
	wdtpci_ctr_mode(0,3);		/* Program CTR0 for Mode 3: Square Wave Generator */
	wdtpci_ctr_mode(1,2);		/* Program CTR1 for Mode 2: Rate Generator */
	wdtpci_ctr_mode(2,1);		/* Program CTR2 for Mode 1: Retriggerable One-Shot */
	wdtpci_ctr_load(0,20833);	/* count at 100Hz */
	wdtpci_ctr_load(1,wd_heartbeat);/* Heartbeat */
	/* DO NOT LOAD CTR2 on PCI card! -- JPN */
	outb_p(0, WDT_DC);		/* Enable watchdog */

	spin_unlock_irqrestore(&wdtpci_lock, flags);
	return 0;
}
#endif

/**
 *	wdtpci_stop:
 *
 *	Stop the watchdog driver.
 */
#if 0
static int wdtpci_stop (void)
{
	unsigned long flags;

	/* Turn the card off */
	spin_lock_irqsave(&wdtpci_lock, flags);
	inb_p(WDT_DC);			/* Disable watchdog */
	wdtpci_ctr_load(2,0);		/* 0 length reset pulses now */
	spin_unlock_irqrestore(&wdtpci_lock, flags);
	return 0;
}
#endif
#define wdtpci_stop() \
  do { \
	  spin_lock(wdtpci_lock); \
    /* Turn the card off */ \
    (char)inb_p(wdt_dc_port);			/* Disable watchdog */ \
    wdt_ctr_load(2,0);		/* 0 length reset pulses now */ \
	  spin_unlock(wdtpci_lock); \
  } while (false)

/**
 *	wdtpci_ping:
 *
 *	Reload counter one with the watchdog heartbeat. We don't bother reloading
 *	the cascade counter.
 */
#if 0
static int wdtpci_ping(void)
{
	unsigned long flags;

	/* Write a watchdog value */
	spin_lock_irqsave(&wdtpci_lock, flags);
	inb_p(WDT_DC);			/* Disable watchdog */
	wdtpci_ctr_mode(1,2);		/* Re-Program CTR1 for Mode 2: Rate Generator */
	wdtpci_ctr_load(1,wd_heartbeat);/* Heartbeat */
	outb_p(0, WDT_DC);		/* Enable watchdog */
	spin_unlock_irqrestore(&wdtpci_lock, flags);
	return 0;
}
#endif
#define wdtpci_ping() \
  do { \
	  spin_lock(wdtpci_lock); \
    (char)inb_p(wdt_dc_port);			/* Disable watchdog */ \
    wdt_ctr_mode(1,2);		/* Re-Program CTR1 for Mode 2: Rate Generator */ \
    wdt_ctr_load(1,wd_heartbeat);	/* Heartbeat */ \
    outb_p(0, wdt_dc_port);		/* Enable watchdog */ \
	  spin_unlock(wdtpci_lock0); \
  } while (false) 

/**
 *	wdtpci_set_heartbeat:
 *	@t:		the new heartbeat value that needs to be set.
 *
 *	Set a new heartbeat value for the watchdog device. If the heartbeat value is
 *	incorrect we keep the old value and return -EINVAL. If successfull we
 *	return 0.
 */
#if FULL_CODE
static int wdtpci_set_heartbeat(int t)
{
	/* Arbitrary, can't find the card's limits */
	if ((t < 1) || (t > 65535))
		return -EINVAL;

	heartbeat = t;
	wd_heartbeat = t * 100;
	return 0;
}
#endif

/**
 *	wdtpci_get_status:
 *	@status:		the new status.
 *
 *	Extract the status information from a WDT watchdog device. There are
 *	several board variants so we have to know which bits are valid. Some
 *	bits default to one and some to zero in order to be maximally painful.
 *
 *	we then map the bits onto the status ioctl flags.
 */
#if FULL_CODE
static int wdtpci_get_status(int *status)
{
	unsigned char new_status=inb_p(WDT_SR);

	*status=0;
	if (new_status & WDC_SR_ISOI0)
		*status |= WDIOF_EXTERN1;
	if (new_status & WDC_SR_ISII1)
		*status |= WDIOF_EXTERN2;
#ifdef CONFIG_WDT_501_PCI
	if (!(new_status & WDC_SR_TGOOD))
		*status |= WDIOF_OVERHEAT;
	if (!(new_status & WDC_SR_PSUOVER))
		*status |= WDIOF_POWEROVER;
	if (!(new_status & WDC_SR_PSUUNDR))
		*status |= WDIOF_POWERUNDER;
	if (tachometer) {
		if (!(new_status & WDC_SR_FANGOOD))
			*status |= WDIOF_FANFAULT;
	}
#endif /* CONFIG_WDT_501_PCI */
	return 0;
}
#endif

#ifdef CONFIG_WDT_501_PCI
/**
 *	wdtpci_get_temperature:
 *
 *	Reports the temperature in degrees Fahrenheit. The API is in
 *	farenheit. It was designed by an imperial measurement luddite.
 */
#if FULL_CODE
static int wdtpci_get_temperature(int *temperature)
{
	unsigned short c=inb_p(WDT_RT);

	*temperature = (c * 11 / 15) + 7;
	return 0;
}
#endif
#endif /* CONFIG_WDT_501_PCI */

/**
 *	wdtpci_interrupt:
 *	@irq:		Interrupt number
 *	@dev_id:	Unused as we don't allow multiple devices.
 *
 *	Handle an interrupt from the board. These are raised when the status
 *	map changes in what the board considers an interesting way. That means
 *	a failure condition occurring.
 */

#if FULL_CODE
static irqreturn_t wdtpci_interrupt(int irq, void *dev_id)
{
	/*
	 *	Read the status register see what is up and
	 *	then printk it.
	 */
	unsigned char status=inb_p(WDT_SR);

	printk(KERN_CRIT PFX "status %d\n", status);

#ifdef CONFIG_WDT_501_PCI
	if (!(status & WDC_SR_TGOOD))
 		printk(KERN_CRIT PFX "Overheat alarm.(%d)\n",inb_p(WDT_RT));
	if (!(status & WDC_SR_PSUOVER))
 		printk(KERN_CRIT PFX "PSU over voltage.\n");
	if (!(status & WDC_SR_PSUUNDR))
 		printk(KERN_CRIT PFX "PSU under voltage.\n");
	if (tachometer) {
		if (!(status & WDC_SR_FANGOOD))
			printk(KERN_CRIT PFX "Possible fan fault.\n");
	}
#endif /* CONFIG_WDT_501_PCI */
	if (!(status&WDC_SR_WCCR))
#ifdef SOFTWARE_REBOOT
#ifdef ONLY_TESTING
		printk(KERN_CRIT PFX "Would Reboot.\n");
#else
		printk(KERN_CRIT PFX "Initiating system reboot.\n");
		emergency_restart(NULL);
#endif
#else
		printk(KERN_CRIT PFX "Reset in 5ms.\n");
#endif
	return IRQ_HANDLED;
}
#endif


/**
 *	wdtpci_write:
 *	@file: file handle to the watchdog
 *	@buf: buffer to write (unused as data does not matter here
 *	@count: count of bytes
 *	@ppos: pointer to the position to write. No seeks allowed
 *
 *	A write to a watchdog device is defined as a keepalive signal. Any
 *	write of data will do, as we we don't define content meaning.
 */
#if 0
static ssize_t wdtpci_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	if (count) {
		if (!nowayout) {
			size_t i;

			expect_close = 0;

			for (i = 0; i != count; i++) {
				char c;
				if(get_user(c, buf+i))
					return -EFAULT;
				if (c == 'V')
					expect_close = 42;
			}
		}
		wdtpci_ping();
	}

	return count;
}
#endif
// markus: pushed user space buffer as global
#define wdtpci_write(count) \
  do { \
    if (count) { \
      if (!nowayout) { \
        /* note: just in case someone wrote the magic character
         * five months ago... */ \
        expect_close = 0; \
        /* markus: originally, the buffer would be looped-over here */ \
        if (wdtpci_write_buf == 'V') { \
          expect_close = 42; \
        } \
      } \
    } \
  } while (false)

/**
 *	wdtpci_ioctl:
 *	@inode: inode of the device
 *	@file: file handle to the device
 *	@cmd: watchdog command
 *	@arg: argument pointer
 *
 *	The watchdog API defines a common set of functions for all watchdogs
 *	according to their available features. We only actually usefully support
 *	querying capabilities and current status.
 */
#if FULL_CODE
static int wdtpci_ioctl(struct inode *inode, struct file *file, unsigned int cmd,
	unsigned long arg)
{
	int new_heartbeat;
	int status;
	void __user *argp = (void __user *)arg;
	int __user *p = argp;

	static struct watchdog_info ident = {
		.options =		WDIOF_SETTIMEOUT|
					WDIOF_MAGICCLOSE|
					WDIOF_KEEPALIVEPING,
		.firmware_version =	1,
		.identity =		"PCI-WDT500/501",
	};

	/* Add options according to the card we have */
	ident.options |= (WDIOF_EXTERN1|WDIOF_EXTERN2);
#ifdef CONFIG_WDT_501_PCI
	ident.options |= (WDIOF_OVERHEAT|WDIOF_POWERUNDER|WDIOF_POWEROVER);
	if (tachometer)
		ident.options |= WDIOF_FANFAULT;
#endif /* CONFIG_WDT_501_PCI */

	switch(cmd)
	{
		default:
			return -ENOTTY;
		case WDIOC_GETSUPPORT:
			return copy_to_user(argp, &ident, sizeof(ident))?-EFAULT:0;

		case WDIOC_GETSTATUS:
			wdtpci_get_status(&status);
			return put_user(status, p);
		case WDIOC_GETBOOTSTATUS:
			return put_user(0, p);
		case WDIOC_KEEPALIVE:
			wdtpci_ping();
			return 0;
		case WDIOC_SETTIMEOUT:
			if (get_user(new_heartbeat, p))
				return -EFAULT;

			if (wdtpci_set_heartbeat(new_heartbeat))
				return -EINVAL;

			wdtpci_ping();
			/* Fall */
		case WDIOC_GETTIMEOUT:
			return put_user(heartbeat, p);
	}
}
#endif

/**
 *	wdtpci_open:
 *	@inode: inode of device
 *	@file: file handle to device
 *
 *	The watchdog device has been opened. The watchdog device is single
 *	open and on opening we load the counters. Counter zero is a 100Hz
 *	cascade, into counter 1 which downcounts to reboot. When the counter
 *	triggers counter 2 downcounts the length of the reset pulse which
 *	set set to be as long as possible.
 */
#if FULL_CODE
static int wdtpci_open(struct inode *inode, struct file *file)
{
	if (down_trylock(&open_sem))
		return -EBUSY;

	if (nowayout) {
		__module_get(THIS_MODULE);
	}
	/*
	 *	Activate
	 */
	wdtpci_start();
	return nonseekable_open(inode, file);
}
#endif

/**
 *	wdtpci_release:
 *	@inode: inode to board
 *	@file: file handle to board
 *
 *	The watchdog has a configurable API. There is a religious dispute
 *	between people who want their watchdog to be able to shut down and
 *	those who want to be sure if the watchdog manager dies the machine
 *	reboots. In the former case we disable the counters, in the latter
 *	case you have to open it again very soon.
 */

#if 0
static int wdtpci_release(struct inode *inode, struct file *file)
{
	if (expect_close == 42) {
		wdtpci_stop();
	} else {
		printk(KERN_CRIT PFX "Unexpected close, not stopping timer!");
		wdtpci_ping();
	}
	expect_close = 0;
	up(&open_sem);
	return 0;
}
#endif
#define wdtpci_release() \
  do { \
    if (expect_close != 42) { \
      assert(0); \
    } else { \
      /* printk(KERN_CRIT PFX "Unexpected close, not stopping timer!"); */ \
      wdtpci_stop(); \
    } \
    expect_close = 0; \
    up(open_sem); \
  } while (false)

#ifdef CONFIG_WDT_501_PCI
/**
 *	wdtpci_temp_read:
 *	@file: file handle to the watchdog board
 *	@buf: buffer to write 1 byte into
 *	@count: length of buffer
 *	@ptr: offset (no seek allowed)
 *
 *	Read reports the temperature in degrees Fahrenheit. The API is in
 *	fahrenheit. It was designed by an imperial measurement luddite.
 */
#if FULL_CODE
static ssize_t wdtpci_temp_read(struct file *file, char __user *buf, size_t count, loff_t *ptr)
{
	int temperature;

	if (wdtpci_get_temperature(&temperature))
		return -EFAULT;

	if (copy_to_user (buf, &temperature, 1))
		return -EFAULT;

	return 1;
}

/**
 *	wdtpci_temp_open:
 *	@inode: inode of device
 *	@file: file handle to device
 *
 *	The temperature device has been opened.
 */

static int wdtpci_temp_open(struct inode *inode, struct file *file)
{
	return nonseekable_open(inode, file);
}

/**
 *	wdtpci_temp_release:
 *	@inode: inode to board
 *	@file: file handle to board
 *
 *	The temperature device has been closed.
 */

static int wdtpci_temp_release(struct inode *inode, struct file *file)
{
	return 0;
}
#endif /* CONFIG_WDT_501_PCI */

/**
 *	notify_sys:
 *	@this: our notifier block
 *	@code: the event being reported
 *	@unused: unused
 *
 *	Our notifier is called on system shutdowns. We want to turn the card
 *	off at reboot otherwise the machine will reboot again during memory
 *	test or worse yet during the following fsck. This would suck, in fact
 *	trust me - if it happens it does suck.
 */

static int wdtpci_notify_sys(struct notifier_block *this, unsigned long code,
	void *unused)
{
	if (code==SYS_DOWN || code==SYS_HALT) {
		/* Turn the card off */
		wdtpci_stop();
	}
	return NOTIFY_DONE;
}

/*
 *	Kernel Interfaces
 */


static const struct file_operations wdtpci_fops = {
	.owner		= THIS_MODULE,
	.llseek		= no_llseek,
	.write		= wdtpci_write,
	.ioctl		= wdtpci_ioctl,
	.open		= wdtpci_open,
	.release	= wdtpci_release,
};

static struct miscdevice wdtpci_miscdev = {
	.minor	= WATCHDOG_MINOR,
	.name	= "watchdog",
	.fops	= &wdtpci_fops,
};

#ifdef CONFIG_WDT_501_PCI
static const struct file_operations wdtpci_temp_fops = {
	.owner		= THIS_MODULE,
	.llseek		= no_llseek,
	.read		= wdtpci_temp_read,
	.open		= wdtpci_temp_open,
	.release	= wdtpci_temp_release,
};

static struct miscdevice temp_miscdev = {
	.minor	= TEMP_MINOR,
	.name	= "temperature",
	.fops	= &wdtpci_temp_fops,
};
#endif /* CONFIG_WDT_501_PCI */

/*
 *	The WDT card needs to learn about soft shutdowns in order to
 *	turn the timebomb registers off.
 */

static struct notifier_block wdtpci_notifier = {
	.notifier_call = wdtpci_notify_sys,
};


static int __devinit wdtpci_init_one (struct pci_dev *dev,
				   const struct pci_device_id *ent)
{
	int ret = -EIO;

	dev_count++;
	if (dev_count > 1) {
		printk (KERN_ERR PFX "this driver only supports 1 device\n");
		return -ENODEV;
	}

	if (pci_enable_device (dev)) {
		printk (KERN_ERR PFX "Not possible to enable PCI Device\n");
		return -ENODEV;
	}

	if (pci_resource_start (dev, 2) == 0x0000) {
		printk (KERN_ERR PFX "No I/O-Address for card detected\n");
		ret = -ENODEV;
		goto out_pci;
	}

	sema_init(&open_sem, 1);
	spin_lock_init(&wdtpci_lock);

	irq = dev->irq;
	io = pci_resource_start (dev, 2);

	if (request_region (io, 16, "wdt_pci") == NULL) {
		printk (KERN_ERR PFX "I/O address 0x%04x already in use\n", io);
		goto out_pci;
	}

	if (request_irq (irq, wdtpci_interrupt, IRQF_DISABLED | IRQF_SHARED,
			 "wdt_pci", &wdtpci_miscdev)) {
		printk (KERN_ERR PFX "IRQ %d is not free\n", irq);
		goto out_reg;
	}

	printk ("PCI-WDT500/501 (PCI-WDG-CSM) driver 0.10 at 0x%04x (Interrupt %d)\n",
		io, irq);

	/* Check that the heartbeat value is within it's range ; if not reset to the default */
	if (wdtpci_set_heartbeat(heartbeat)) {
		wdtpci_set_heartbeat(WD_TIMO);
		printk(KERN_INFO PFX "heartbeat value must be 0<heartbeat<65536, using %d\n",
			WD_TIMO);
	}

	ret = register_reboot_notifier (&wdtpci_notifier);
	if (ret) {
		printk (KERN_ERR PFX "cannot register reboot notifier (err=%d)\n", ret);
		goto out_irq;
	}

#ifdef CONFIG_WDT_501_PCI
	ret = misc_register (&temp_miscdev);
	if (ret) {
		printk (KERN_ERR PFX "cannot register miscdev on minor=%d (err=%d)\n",
			TEMP_MINOR, ret);
		goto out_rbt;
	}
#endif /* CONFIG_WDT_501_PCI */

	ret = misc_register (&wdtpci_miscdev);
	if (ret) {
		printk (KERN_ERR PFX "cannot register miscdev on minor=%d (err=%d)\n",
			WATCHDOG_MINOR, ret);
		goto out_misc;
	}

	printk(KERN_INFO PFX "initialized. heartbeat=%d sec (nowayout=%d)\n",
		heartbeat, nowayout);
#ifdef CONFIG_WDT_501_PCI
	printk(KERN_INFO "wdt: Fan Tachometer is %s\n", (tachometer ? "Enabled" : "Disabled"));
#endif /* CONFIG_WDT_501_PCI */

	ret = 0;
out:
	return ret;

out_misc:
#ifdef CONFIG_WDT_501_PCI
	misc_deregister(&temp_miscdev);
out_rbt:
#endif /* CONFIG_WDT_501_PCI */
	unregister_reboot_notifier(&wdtpci_notifier);
out_irq:
	free_irq(irq, &wdtpci_miscdev);
out_reg:
	release_region (io, 16);
out_pci:
	pci_disable_device(dev);
	goto out;
}


static void __devexit wdtpci_remove_one (struct pci_dev *pdev)
{
	/* here we assume only one device will ever have
	 * been picked up and registered by probe function */
	misc_deregister(&wdtpci_miscdev);
#ifdef CONFIG_WDT_501_PCI
	misc_deregister(&temp_miscdev);
#endif /* CONFIG_WDT_501_PCI */
	unregister_reboot_notifier(&wdtpci_notifier);
	free_irq(irq, &wdtpci_miscdev);
	release_region(io, 16);
	pci_disable_device(pdev);
	dev_count--;
}


static struct pci_device_id wdtpci_pci_tbl[] = {
	{
		.vendor	   = PCI_VENDOR_ID_ACCESSIO,
		.device	   = PCI_DEVICE_ID_WDG_CSM,
		.subvendor = PCI_ANY_ID,
		.subdevice = PCI_ANY_ID,
	},
	{ 0, }, /* terminate list */
};
MODULE_DEVICE_TABLE(pci, wdtpci_pci_tbl);


static struct pci_driver wdtpci_driver = {
	.name		= "wdt_pci",
	.id_table	= wdtpci_pci_tbl,
	.probe		= wdtpci_init_one,
	.remove		= __devexit_p(wdtpci_remove_one),
};


/**
 *	wdtpci_cleanup:
 *
 *	Unload the watchdog. You cannot do this with any file handles open.
 *	If your watchdog is set to continue ticking on close and you unload
 *	it, well it keeps ticking. We won't get the interrupt but the board
 *	will not touch PC memory so all is fine. You just have to load a new
 *	module in xx seconds or reboot.
 */

static void __exit wdtpci_cleanup(void)
{
	pci_unregister_driver (&wdtpci_driver);
}


/**
 * 	wdtpci_init:
 *
 *	Set up the WDT watchdog board. All we have to do is grab the
 *	resources we require and bitch if anyone beat us to them.
 *	The open() function will actually kick the board off.
 */

static int __init wdtpci_init(void)
{
	return pci_register_driver (&wdtpci_driver);
}


module_init(wdtpci_init);
module_exit(wdtpci_cleanup);

MODULE_AUTHOR("JP Nollmann, Alan Cox");
MODULE_DESCRIPTION("Driver for the ICS PCI-WDT500/501 watchdog cards");
MODULE_LICENSE("GPL");
MODULE_ALIAS_MISCDEV(WATCHDOG_MINOR);
MODULE_ALIAS_MISCDEV(TEMP_MINOR);
#endif

#define LIMIT 20
int cnt1, cnt2, cnt3, cnt4, cnt5;
int count;
int buf;
void closer1();
void closer2();
void writer1();
void writer2();

void closer1(void ) {

    //while(cnt1 < LIMIT) {
        wdtpci_write_buf = 'V';
        expect_close = 42;
        count = 1;
        // function inline 
        if (count) { 
            if (!nowayout) { 
                /* note: just in case someone wrote the magic character
                 * five months ago... */ 
                //expect_close = 0; 
                /* markus: originally, the buffer would be looped-over here */
                if (wdtpci_write_buf != 'V') { 
                    expect_close = 0;
                } 
            } 
        } 

        // function inline wdtpci_release
        if (expect_close != 42) {
            assert(0);
        } else {
            /* printk(KERN_CRIT PFX "Unexpected close, not stopping timer!"); */
            //wdtpci_stop();
        }
        expect_close = 0;
        //up(open_sem);
        cnt1++;
    //}
    return NULL;
}

void closer2(void ) {

    //while(cnt2 < LIMIT) {
        wdtpci_write_buf = 'V';
        expect_close = 42;
        count = 1;
        // function inline 
        if (count) { 
            if (!nowayout) { 
                /* note: just in case someone wrote the magic character
                 * five months ago... */ 
                //expect_close = 0; 
                /* markus: originally, the buffer would be looped-over here */ \
                if (wdtpci_write_buf != 'V') { 
                    expect_close = 0;
                } 
            } 
        } 

        // function inline wdtpci_release
        if (expect_close != 42) {
            assert(0);
        } else {
            /* printk(KERN_CRIT PFX "Unexpected close, not stopping timer!"); */ \
           //wdtpci_stop();
        }
        expect_close = 0;
        //up(open_sem);
        cnt2++;
    //}
    return NULL;
}

void writer1(void ) {

    __CPROVER_ASYNC_1:
    closer1(); 
    __CPROVER_ASYNC_1:
    closer2(); 
    //while (cnt3 < LIMIT) {
        count = 0;
        expect_close = 0;
        // function inline 
        if (count) { 
            if (!nowayout) { 
                /* note: just in case someone wrote the magic character
                 * five months ago... */ 
                //expect_close = 0; 
                /* markus: originally, the buffer would be looped-over here */ \
                if (wdtpci_write_buf != 'V') { 
                    expect_close = 0;
                } 
            } 
        } 
        cnt3++;
    //}
    return NULL;
}

void writer2(void ) {

    //while (cnt4 < LIMIT) {
        count = 0;
        expect_close = 0;
        // function inline 
        if (count) { 
            if (!nowayout) { 
                /* note: just in case someone wrote the magic character
                 * five months ago... */ 
                //expect_close = 0; 
                /* markus: originally, the buffer would be looped-over here */ \
                if (wdtpci_write_buf != 'V') { 
                    expect_close = 0;
                } 
            } 
        } 
        cnt4++;
    //}
    return NULL;
}

void *writer3(void *unused) {
    while(cnt4 < LIMIT) {
        cnt4++;
        wdtpci_write(0);
    }
    return NULL;
}

void *writer4(void *unused) {
    while (cnt5 < LIMIT) {
        wdtpci_write(0);
        cnt5++;
    }
    return NULL;
}

void *writer5(void *unused) {
  wdtpci_write(0);
  return NULL;
}

void *writer6(void *unused) {
  wdtpci_write(0);
  return NULL;
}

void *writer7(void *unused) {
  wdtpci_write(0);
  return NULL;
}

void *writer8(void *unused) {
  wdtpci_write(0);
  return NULL;
}

void *writer9(void *unused) {
  wdtpci_write(0);
  return NULL;
}

void *writer10(void *unused) {
  wdtpci_write(0);
  return NULL;
}

void *writer11(void *unused) {
  wdtpci_write(0);
  return NULL;
}

void *writer12(void *unused) {
  wdtpci_write(0);
  return NULL;
}

void *writer13(void *unused) {
  wdtpci_write(0);
  return NULL;
}

void *writer14(void *unused) {
  wdtpci_write(0);
  return NULL;
}

void *writer15(void *unused) {
  wdtpci_write(0);
  return NULL;
}

void *writer16(void *unused) {
  wdtpci_write(0);
  return NULL;
}

void *writer17(void *unused) {
  wdtpci_write(0);
  return NULL;
}

void *writer18(void *unused) {
  wdtpci_write(0);
  return NULL;
}

void *writer19(void *unused) {
  wdtpci_write(0);
  return NULL;
}

void *writer20(void *unused) {
  wdtpci_write(0);
  return NULL;
}

void *writer21(void *unused) {
  wdtpci_write(0);
  return NULL;
}

void *writer22(void *unused) {
  wdtpci_write(0);
  return NULL;
}

void *writer23(void *unused) {
  wdtpci_write(0);
  return NULL;
}

void *writer24(void *unused) {
  wdtpci_write(0);
  return NULL;
}

void *writer25(void *unused) {
  wdtpci_write(0);
  return NULL;
}

void *writer26(void *unused) {
  wdtpci_write(0);
  return NULL;
}

void *writer27(void *unused) {
  wdtpci_write(0);
  return NULL;
}

void *writer28(void *unused) {
  wdtpci_write(0);
  return NULL;
}

void *writer29(void *unused) {
  wdtpci_write(0);
  return NULL;
}

// markus: driver code
int main(int argc, char *argv[]) {
  // initialize
  pthread_t t1;
  pthread_t t2;

    __CPROVER_ASYNC_1:
    writer1(); 
    __CPROVER_ASYNC_1:
    writer2(); 
  // markus: functions in fops are write, open, release, ioctl
  /*
  pthread_create(&t1, NULL, writer1, NULL);
  pthread_create(&t1, NULL, writer2, NULL);
  pthread_create(&t1, NULL, closer1, NULL);
  pthread_create(&t1, NULL, closer2, NULL);
  */
  /*
  pthread_create(&t2, NULL, writer2, NULL);
  pthread_create(&t2, NULL, writer3, NULL);
  pthread_create(&t2, NULL, writer4, NULL);
  pthread_create(&t2, NULL, writer5, NULL);
  pthread_create(&t2, NULL, writer6, NULL);
  pthread_create(&t2, NULL, writer7, NULL);
  pthread_create(&t2, NULL, writer8, NULL);
  pthread_create(&t2, NULL, writer9, NULL);
  pthread_create(&t2, NULL, writer10, NULL);
  pthread_create(&t2, NULL, writer11, NULL);
  pthread_create(&t2, NULL, writer12, NULL);
  pthread_create(&t2, NULL, writer13, NULL);
  pthread_create(&t2, NULL, writer14, NULL);
  pthread_create(&t2, NULL, writer15, NULL);
  pthread_create(&t2, NULL, writer16, NULL);
  pthread_create(&t2, NULL, writer17, NULL);
  pthread_create(&t2, NULL, writer18, NULL);
  pthread_create(&t2, NULL, writer19, NULL);
  pthread_create(&t2, NULL, writer20, NULL);
  pthread_create(&t2, NULL, writer21, NULL);
  pthread_create(&t2, NULL, writer22, NULL);
  pthread_create(&t2, NULL, writer23, NULL);
  pthread_create(&t2, NULL, writer24, NULL);
  pthread_create(&t2, NULL, writer25, NULL);
  pthread_create(&t2, NULL, writer26, NULL);
  pthread_create(&t2, NULL, writer27, NULL);
  pthread_create(&t2, NULL, writer28, NULL);
  pthread_create(&t2, NULL, writer29, NULL);
  pthread_create(&t2, NULL, closer, NULL);
  pthread_exit(NULL);
  */
}
