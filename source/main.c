
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
	ft_strcat(dot_cor, "dis");
	return (dot_cor);
}

void	create_file_s(t_diasm *diasm)
{
	diasm->name_s = dot_s(diasm->name_cor);
	if (!(diasm->fd_s = open(diasm->name_s, O_WRONLY | O_TRUNC | O_CREAT,
					S_IREAD | S_IWRITE)))
		sys_err("Error create file.\n");

}

void	printf_bit_hex(unsigned char *arr)
{
	unsigned char c;
	int i;

	i = -1;
	while (++i != 4)
		ft_printf("%02x ", (unsigned char)arr[i]);
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
		//ft_printf("{%08x}", exec);
		//ft_printf("[%x]\n", (unsigned char)buf[i]);
	}
	if (exec != COREWAR_EXEC_MAGIC)
		sys_err("Wrong exec magic\n");
	//ft_printf("exec = [%u] {%08x}\n", exec, exec);
}

void	write_name(t_diasm *diasm)
{
	char	name[PROG_NAME_LENGTH + 8];

	if (read(diasm->fd_cor, name, PROG_NAME_LENGTH + 8) == -1)
		sys_err("Error read files.\n");
	ft_putstr_fd(".name \"", diasm->fd_s);
	ft_putstr_fd(name, diasm->fd_s);
	ft_putendl_fd("\"", diasm->fd_s);
	//ft_printf("name = {%s}\n", name);
}

void	write_comment(t_diasm *diasm)
{
	char	comment[COMMENT_LENGTH + 4];

	if (read(diasm->fd_cor, comment, COMMENT_LENGTH + 4) == -1)
		sys_err("Error read files.\n");
	//ft_printf("comment = {%s}\n", comment);
	ft_putstr_fd(".comment \"", diasm->fd_s);
	ft_putstr_fd(comment, diasm->fd_s);
	ft_putendl_fd("\"", diasm->fd_s);

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

unsigned int	move_to_uint(unsigned char *buf)
{
	unsigned int	rez;
	int				i;

	rez = 0;
	i = -1;
	while(++i < 4)
	{
		rez = rez << 8;
		rez = rez | (unsigned char)buf[i];
	}
	return (rez);
}

unsigned short	move_to_ushort(unsigned char *buf)
{
	unsigned short	rez;
	int				i;

	rez = 0;
	i = -1;
	while(++i < 2)
	{
		rez = rez << 8;
		rez = rez | (unsigned char)buf[i];
	}
	return (rez);
}

void	op_live(t_diasm *diasm)
{
	unsigned char	dir[4];
	unsigned int	idir;
	char			*str_num;

	idir = 0;
	if (read(diasm->fd_cor, dir, 4) == -1)
		sys_err("Error read files.\n");
	ft_putstr_fd("\tlive %", diasm->fd_s);
	idir = move_to_uint(dir);
	str_num = ft_itoa((int)idir);
	ft_putendl_fd(str_num, diasm->fd_s);
	ft_strdel(&str_num);
	//ft_putstr_fd(comment, diasm->fd_s);
	//idir = (unsigned int*)(&dir[0]);
	//ft_printf("idir = {%04x}\n", idir);
	//printf_bit_hex(dir);
}

void	op_zjmp_fork_lfork(t_diasm *diasm, const char *op_name)
{
	unsigned char	dir[2];
	unsigned short	ushort;
	char			*str_num;

	ft_putendl(op_name);
	ushort = 0;
	if (read(diasm->fd_cor, dir, 2) == -1)
		sys_err("Error read files.\n");
	ft_putstr_fd(op_name, diasm->fd_s);
	ushort = move_to_ushort(dir);
	str_num = ft_itoa((short)ushort);
	ft_putendl_fd(str_num, diasm->fd_s);
	ft_strdel(&str_num);
}

void	woking_operation(t_diasm *diasm, unsigned char op)
{
	if (op == 0x01)
		op_live(diasm);
	else if (op == 0x09)
		op_zjmp_fork_lfork(diasm, "\tzjmp %");
	else if (op == 0x0c)
		op_zjmp_fork_lfork(diasm, "\tfork %");
	else if (op == 0x0f)
		op_zjmp_fork_lfork(diasm, "\tlfork %");

}

void	write_instruction(t_diasm *diasm)
{
	int				count;
	unsigned char	op;
	
	count = 1;	
	while (1)
	{
		if ((count = read(diasm->fd_cor, &op, 1)) == -1)
			sys_err("Error read files.\n");
		if (!count)
			break;
		woking_operation(diasm, op);
	}
}

void	clone_file(t_diasm *diasm)
{
	close(diasm->fd_cor);
	close(diasm->fd_s);
}

int		main(int ac, char **av)
{
	t_diasm diasm;
	
	if (ac != 2)
		sys_err("Error!\nUse ./asm namefile.cor\n");
	init(&diasm);
	open_file_cor(&diasm, av[1]);
	create_file_s(&diasm);
	write_name_comment(&diasm);
	write_instruction(&diasm);
	clone_file(&diasm);
	//create_file_s;
	return (0);
}
