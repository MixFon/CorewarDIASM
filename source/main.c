
#include "../include/diasm.h"

void	init(t_diasm *diasm)
{
	diasm->name_cor = NULL;
	diasm->fd_cor = -1;
	diasm->fd_s = -1;
}

void	open_file_cor(t_diasm *diasm, char *name)
{
	char *tmp;
	
	if ((tmp = ft_strstr(name, ".cor")) && (name - tmp) != 0
			&& ft_strlen(tmp) == 4)
	{
		if(!(diasm->name_cor = ft_strdup(name)))
			sys_err("Error malloc.");
		if ((diasm->fd_cor = open(name, O_RDONLY)) == -1)
			sys_err("File not opened.\n");
		ft_printf("fd_cor = [%d]\n", diasm->fd_cor);
	}
	else
		sys_err("Invalid file type.\nUse ./asm namefile.cor\n");
}

char	*dot_s(char *name)
{
	char	*dot_cor;
	size_t	len;

	len = ft_strlen(name);
	dot_cor = ft_strnew(len);
	ft_memccpy(dot_cor, name, '.', len);
	ft_strcat(dot_cor, "s");
	return (dot_cor);
}

void	create_file_s(t_diasm *diasm)
{
	diasm->name_s = dot_s(diasm->name_cor);
	if (!(diasm->fd_s = open(diasm->name_s, O_WRONLY | O_TRUNC | O_CREAT,
					S_IREAD | S_IWRITE)))
		sys_err("Error create file.\n");

}

void	printf_bit_hex(char *arr)
{
	unsigned char c;
	int i;

	i = -1;
	while (++i != 4)
		ft_printf("%x ", (unsigned char)arr[i]);
}

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
		ft_printf("{%08x}", exec);
		ft_printf("[%x]\n", (unsigned char)buf[i]);
	}
	if (exec != COREWAR_EXEC_MAGIC)
		sys_err("Wrong exec magic\n");
	ft_printf("exec = [%u] {%#x}\n", exec, exec);
}

void	write_comment(t_diasm *diasm)
{
	//int tmp;
	char buf[4];
	int s;

	//tmp = NULL;
	//tmp = 0;
	//tmp = ft_strnew(5);
	if ((s = read(diasm->fd_cor, buf, 4)) == -1)
		sys_err("Error read files.\n");
	check_exec_magic(buf);
	//ft_printf("sizeof = [%d]\n", sizeof(tmp));
	//ft_printf("s = [%d] tmp = {%d}, [%#x]\n", s, tmp, (unsigned int)tmp[2]);
	printf_bit_hex(buf);
}

int		main(int ac, char **av)
{
	t_diasm diasm;
	
	if (ac != 2)
		sys_err("Error!\nUse ./asm namefile.cor\n");
	init(&diasm);
	open_file_cor(&diasm, av[1]);
	create_file_s(&diasm);
	write_comment(&diasm);
	//create_file_s;
	return (0);
}
