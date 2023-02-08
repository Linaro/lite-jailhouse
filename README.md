# lite-jailhouse
Example using Jailhouse as a hypervisor with Zephyr RTOS and QEMU on
ARM64 archutecture.

## Requirements
The sample was tested using a Intel based machine running Ubuntu 22.04 LTS,
besides that you may need some extra resources:
 
* QEMU System with ARM64 support: `$ sudo apt install qemu-system `
* QEMU ARM64 test image provided in: https://github.com/siemens/jailhouse-images
* Also you'll need to install Zephyr RTOS: https://docs.zephyrproject.org/3.2.0/develop/getting_started/index.html

## Getting started
Create an directory to be used as workspace:

```
$ mkdir jailhouse_ws && cd jailhouse_ws
```

Clone this repository and the jailhouse-images repository:

```
$ git clone git@github.com:Linaro/lite-jailhouse.git
$ git clone git@github.com:siemens/jailhouse-images.git

```

You should need to build the QEMU test image:

```
$ cd jailhouse-images
$ ./kas_container.sh
```

It will open a menu, select QEMU ARM64 as build target, save and exit, then type:

```
$ ./kas_container.sh build
```

After that you just need to map the port 5555 to the 22 in order to enable
ssh to the Virtual Machine image, add the following line below here
inside of the following file of jailhose-images repository:

https://github.com/siemens/jailhouse-images/blob/0a01dd4ca08e9e2163c76bf043a06e8a2fa88bec/start-qemu.sh#L60

Edit the line to looks like below:

```
    -device virtio-net-device,netdev=net0 \
    -netdev user,id=net0,hostfwd=tcp::5555-:22"
```

Save the file, the current test image already comes with jailhouse installed and support
to the zephyr cell, creating one from sources is outside of this document scope since the
mainline jailhouse already provides a decent support for zephyr cell.

## Starting QEMU

To start QEMU, just go back to the jailhouse-images repository (after building the image),
and invoke it:

```
$ cd jailhouse-images
$ ./start_qemu.sh arm64
```

This will starts the Virtual Machine with the built image, also will directs 
the user to the termina, the default user is: root, and the password is: root

The further acces to this VM will be done in other terminal windows through SSH
which is enabled, this one will be reserved to see the output of jailhouse console
just start to intercept its messages by:

```
# cat /dev/ttyAMA0
```

Every interaction with Jailhouse will be displayed in this terminal tab.

On another terminal, access the VM by using ssh:

```
$ ssh -p 5555 root@localhost
```

The password is root, once acessed, create the jailhouse workspace folder:

```
# mkdir jailhouse_ws
```

The folder above will be used to store zephyr images, we will call this tab
from here, as jailhouse work tab.

## Build and Deploy Zephyr APP to QEMU

In another terminal tab, let's build and deploy the Zephyr demonstration app,
it used the QEMU Cortex A53 board, currently there is support only to console
and GIC, interrupt controller, here is how to build the app, assuming Zephyr
RTOS is already set in the host machine :

```
$ cd lite-jailhouse
$ west build -p auto -bqemu_cortex_a53 zephyr_app
```

Please mind due to the Jailhouse configuration, the application should be 
loaded from the address 0x70000000, an overlay file creates this memory 
area and overrides default zephyr sram-chosen symbol to select this area instead
of the default pointed by 0x40000000.

To deploy the firmware to the target QEMU instance, assuming it is already set,
and running (with port 5555 mapped to the 22 ssh port), just type in the current
same directory:

```
$ scp -r -P 5555 build/zephyr/zephyr.bin  root@localhost://root/jailhouse_ws
```

The password is the same as mentioned before, root.

## Starting the Jailhouse Cell

Now the VM has everything needed to start the Jailhouse application in the Zephyr side,
to launch the application, go back to the jailhouse workspace tab, or otherwise ssh to 
the VM again.

Now follow the step by step below:

* First, enable the jailhouse passing the root cell configuration:

```
# jailhouse enable /etc/jailhouse/qemu-arm64.cell
```

* Then creates the Zephyr cell:

```
# jailhouse cell create /etc/jailhouse/qemu-arm64-zephyr-demo.cell
```

* Now load the firmware to the target execution area:

```
# jailhouse cell load qemu-arm64-zephyr-demo jailhouse_ws/zephyr.bin -a 0x70000000
```

* And, finally start the application:

```
# jailhouse cell start qemu-arm64-zephyr-demo
```

The application should start immediately, additionally it is possible to suspend
the cell, in case of updating the zephyr application:

```
# jailhouse cell shutdown qemu-arm64-zephyr-demo
```

With the step above, just repeat the deploy firmware step again and, after that, 
repeat the loading and start jailhouse steps to update the firmware.

## Expected results into the Jailhouse console

To see the results on console, just go back to the window where the start_qemu.sh
script was started, after the jailhouse cell was started you may see something similar
to this:

```
Cell "qemu-arm64-zephyr-demo" can be loaded
Started cell "qemu-arm64-zephyr-demo"
*** Booting Zephyr OS build v3.3.0-rc1-58-g156c7cd21759 ***
[0]: Hello Jailhouse, I'm: qemu_cortex_a53 
[510]: Hello Jailhouse, I'm: qemu_cortex_a53 
[1020]: Hello Jailhouse, I'm: qemu_cortex_a53 
[1530]: Hello Jailhouse, I'm: qemu_cortex_a53 
[2040]: Hello Jailhouse, I'm: qemu_cortex_a53 
[2550]: Hello Jailhouse, I'm: qemu_cortex_a53 
[3060]: Hello Jailhouse, I'm: qemu_cortex_a53 
[3570]: Hello Jailhouse, I'm: qemu_cortex_a53 
[4080]: Hello Jailhouse, I'm: qemu_cortex_a53 
[4590]: Hello Jailhouse, I'm: qemu_cortex_a53 
[5100]: Hello Jailhouse, I'm: qemu_cortex_a53 

```

## TODO's
* Implement the communication between Linux and Zephyr Cell (IVSHMEM, RPMsg);
* Describe steps to build jailhouse from sources adding a new configuration;
