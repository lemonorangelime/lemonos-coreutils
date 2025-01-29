#include <sys/utsname.h>
#include <stdio.h>
#include <argv.h>
#include <main.h>
#include <runtime.h>

void uname_show(char * value, int bit) {
	if (!bit) {
		return;
	}
	printf("%s ", value);
}

int main(int argc, char * argv[]) {
	uname_state_t state;
	state.bits.all = args_contains_long(argc, argv, "-a", "--all");
	state.bits.kernel_name = args_contains_long(argc, argv, "-s", "--kernel-name");
	state.bits.nodename = args_contains_long(argc, argv, "-n", "--nodename");
	state.bits.kernel_release = args_contains_long(argc, argv, "-r", "--kernel-release");
	state.bits.kernel_version = args_contains_long(argc, argv, "-v", "--kernel-version");
	state.bits.machine = args_contains_long(argc, argv, "-m", "--machine");
	state.bits.processor = args_contains_long(argc, argv, "-p", "--processor");
	state.bits.hardware_platform = args_contains_long(argc, argv, "-i", "--hardware-platform");
	state.bits.operating_system = args_contains_long(argc, argv, "-o", "--operating-system");
	state.bits.help = args_contains_long(argc, argv, "-h", "--help");
	state.bits.version = args_contains_long(argc, argv, "-V", "--version");
	if (state.all_bits == 0) {
		state.bits.kernel_name = 1;
	}
	if (state.bits.help || state.bits.version) {
		switch (state.bits.help) {
			case 0:
				printf("uname (LemonOS coreutils) 1.0\n");
				printf("License unlicense: <https://unlicense.org>\n");
				printf("This is public domain software: you are free to change and redistribute it.\n");
				printf("There is NO WARRANTY, to the extent permitted by law.\n");
				printf("\nWritten by Lemon\n");
				return 0;
			case 1:
				printf("Usage: uname [OPTION]...\n");
				printf("Print certain system information.  With no OPTION, same as -s.\n");
				printf("\n");
				printf("  -a, --all                print all information, in the following order,\n");
				printf("                             except omit -p and -i if unknown:\n");
				printf("  -s, --kernel-name        print the kernel name\n");
				printf("  -n, --nodename           print the network node hostname\n");
				printf("  -r, --kernel-release     print the kernel release\n");
				printf("  -v, --kernel-version     print the kernel version\n");
				printf("  -m, --machine            print the machine hardware name\n");
				printf("  -p, --processor          print the processor type\n");
				printf("  -i, --hardware-platform  print the hardware platform\n");
				printf("  -o, --operating-system   print the operating system\n");
				printf("  -h, --help     display this help and exit\n");
				printf("  -V, --version  output version information and exit\n");
				printf("\n");
				printf("Get source at <https://github.com/kitty14956590/lemonos-coreutils>\n");
		}
		return 0;
	}
	uname_bits_t * bits = &state.bits;
	char * operating_system = __is_lemonos ? "LemonOS" : "GNU/Linux";
	struct utsname name;
	uname(&name);
	uname_show(name.sysname, bits->kernel_name | bits->all);
	uname_show(name.nodename, bits->nodename | bits->all);
	uname_show(name.release, bits->kernel_release | bits->all);
	uname_show(name.version, bits->kernel_version | bits->all);
	uname_show(name.machine, bits->machine | bits->all);

	// fake this
	uname_show(name.machine, bits->processor | bits->all);
	uname_show(name.machine, bits->hardware_platform | bits->all);

	uname_show(operating_system, bits->operating_system | bits->all);
	printf("\n");
	return 0;
}
