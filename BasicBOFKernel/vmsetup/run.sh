#!/bin/bash

# echo "Enter link to exploit binary: "
# read link

# echo $link

# wget $link -O exploit
chmod 777 ./exploit
sleep 1
./decompress.sh
rm intramfs/exploit
cp ./exploit initramfs/
./compress.sh

qemu-system-x86_64 \
    -m 128M \
    -cpu kvm64,+smep,+smap \
    -kernel kernel/arch/x86/boot/bzImage \
    -initrd initramfs.cpio.gz \
    -snapshot \
    -nographic \
    -monitor /dev/null \
    -no-reboot \
    -append "console=ttyS0 nokaslr kpti=1 quiet panic=1"
