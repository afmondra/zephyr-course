#include <zephyr/shell/shell.h>

static int cmd_channel_get_handler(const struct shell *sh, size_t argc, char **argv) 
{
    shell_info(sh, "Hello from channel get!");    
    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(our_driver_subcmd,
    SHELL_CMD_ARG(channel_get, NULL, "Get channel of my/our driver", cmd_channel_get_handler, 1, 0),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(our_driver, &our_driver_subcmd, "Our Driver set of commands", NULL);
