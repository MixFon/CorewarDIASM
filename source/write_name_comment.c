
#include "../include/diasm.h"

void	check_exec_magic(char *buf)
{
	unsigned int	exec;
	int				i;

	exec = 0;
	i = -1;
	while(++i < 4)
	{
		exec = exec << 8;
		exec = exec | (unsigned char)buf[i];
	}
	if (exec != COREWAR_EXEC_MAGIC)
		sys_err("Wrong exec magic\n");
}

void	write_name(t_diasm *diasm)
{
	char	name[PROG_NAME_LENGTH + 8];

	if (read(diasm->fd_cor, name, PROG_NAME_LENGTH + 8) == -1)
		sys_err("Error read files.\n");
	ft_putstr_fd(".name \"", diasm->fd_s);
	ft_putstr_fd(name, diasm->fd_s);
	ft_putendl_fd("\"", diasm->fd_s);
}

void	write_comment(t_diasm *diasm)
{
	char	comment[COMMENT_LENGTH + 4];

	if (read(diasm->fd_cor, comment, COMMENT_LENGTH + 4) == -1)
		sys_err("Error read files.\n");
	ft_putstr_fd(".comment \"", diasm->fd_s);
	ft_putstr_fd(comment, diasm->fd_s);
	ft_putendl_fd("\"\n", diasm->fd_s);
}

void	write_name_comment(t_diasm *diasm)
{
	char buf[4];

	if (read(diasm->fd_cor, buf, 4) == -1)
		sys_err("Error read files.\n");
	check_exec_magic(buf);
	write_name(diasm);
	write_comment(diasm);
}
