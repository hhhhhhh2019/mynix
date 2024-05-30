{ os, base, ... }: {
	boot.loader.grub.enable = true;
	boot.loader.grub.useOSProber = true;
	boot.loader.grub.efiSupport = true;
	boot.loader.grub.efiMountPoint = "/boot/efi";

	boot.initrd.modules = [ "test_module" ];
} done
