
#include "../include/diasm.h"

void	init(t_diasm *diasm)
{
	diasm->file_name = NULL;
}

void	check_file_name(t_diasm *diasm, char *name)
{
	char *tmp;
	
	if ((tmp = ft_strstr(name, ".cor")) && (name - tmp) != 0
			&& ft_strlen(tmp) == 4)
	{
		if(!(diasm->file_name = ft_strdup(name)))
			sys_err("Error malloc.");
		ft_putendl(name);
	}
	else
		sys_err("Invalid file type.\nUse ./asm namefile.cor\n");
}

int main(int ac, char **av)
{
	t_diasm diasm;
	
	if (ac != 2)
		sys_err("Error!\nUse ./asm namefile.cor\n");
	init(&diasm);
	check_file_name(&diasm, av[1]);
	return (0);
}
