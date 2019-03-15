obj-m := kthread_counter.o
kdir := /lib/modules/`uname -r`/build
pwd := `pwd`

default:
	make -C $(kdir) M=$(pwd) modules
clean:
	make -C $(kdir) M=$(pwd) clean 
install:
	sudo dmesg --clear
	sudo insmod kthread_counter.ko
	dmesg
test:
	cat /proc/counter
uninstall:
	sudo rmmod kthread_counter.ko
	dmesg
