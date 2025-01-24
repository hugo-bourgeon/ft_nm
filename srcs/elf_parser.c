/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 17:10:51 by hubourge          #+#    #+#             */
/*   Updated: 2025/01/24 18:11:42 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include 		"nm.h"

void	*map_file(t_data *data, char *file)
{
	data->fd = open(file, O_RDONLY);
	if (data->fd < 0)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}

	if (fstat(data->fd, &(data)->statbuf) < 0)
	{
		perror("fstat");
		free_all_exit(*data, EXIT_FAILURE);
	}
	
	data->mapped_file = mmap(NULL, data->statbuf.st_size, PROT_READ, MAP_PRIVATE, data->fd, 0);
	if (data->mapped_file == MAP_FAILED)
	{
		perror("mmap");
		free_all_exit(*data, EXIT_FAILURE);
	}

	close(data->fd);
	return(data->mapped_file);
}

int	detect_valid_elf(t_data *data, void *mapped_file)
{
	// Check if the file is an ELF file
	unsigned char *e_ident = (unsigned char *)mapped_file;
    if (e_ident[EI_MAG0] != ELFMAG0 ||
        e_ident[EI_MAG1] != ELFMAG1 ||
        e_ident[EI_MAG2] != ELFMAG2 ||
        e_ident[EI_MAG3] != ELFMAG3)
	{
		ft_putstr_fd("Invalid ELF file\n", STDERR_FILENO);
		return(0);
	}

	// Detect 32 or 64 bits ELF file
	if (e_ident[EI_CLASS] == ELFCLASS64)
	{
        printf("ELF Class: 64-bit\n");
		handle_64(data);
	}
	else if(e_ident[EI_CLASS] == ELFCLASS32)
	{
		// handle_32(header);
	}
	else
	{
		ft_putstr_fd("Invalid ELF file\n", STDERR_FILENO);
		return(0);
	}

	return(1);
}

// ELF Header
// typedef struct {
// 	unsigned char e_ident[EI_NIDENT];
// 	uint16_t      e_type;
// 	uint16_t      e_machine;
// 	uint32_t      e_version;
// 	ElfN_Addr     e_entry;
// 	ElfN_Off      e_phoff;
// 	ElfN_Off      e_shoff;
// 	uint32_t      e_flags;
// 	uint16_t      e_ehsize;
// 	uint16_t      e_phentsize;
// 	uint16_t      e_phnum;
// 	uint16_t      e_shentsize;
// 	uint16_t      e_shnum;
// 	uint16_t      e_shstrndx;
// } ElfN_Ehdr;

// Section Header
// typedef struct {
// 	uint32_t   sh_name;
// 	uint32_t   sh_type;
// 	uintN_t   sh_flags;
// 	ElfN_Addr sh_addr;
// 	ElfN_Off  sh_offset;
// 	uintN_t   sh_size;
// 	uint32_t   sh_link;
// 	uint32_t   sh_info;
// 	uintN_t   sh_addralign;
// 	uintN_t   sh_entsize;
// } ElfN_Shdr;

// Section 0: 
// Section 1: .text
// Section 2: .data
// Section 3: .bss
// Section 4: .debug_info
// Section 5: .rela.debug_info
// Section 6: .debug_abbrev
// Section 7: .debug_aranges
// Section 8: .rela.debug_aranges
// Section 9: .debug_line
// Section 10: .rela.debug_line
// Section 11: .debug_str
// Section 12: .comment
// Section 13: .note.GNU-stack
// Section 14: .note.gnu.property
// Section 15: .eh_frame
// Section 16: .rela.eh_frame
// Section 17: .symtab
// Section 18: .strtab
// Section 19: .shstrtab

void	handle_64(t_data *data)
{
	// ELF Header and Section Header
	Elf64_Ehdr *header = (Elf64_Ehdr *)data->mapped_file;
	Elf64_Shdr *section = (Elf64_Shdr *)((char *)data->mapped_file + header->e_shoff);

	// Table of section names
	Elf64_Shdr *section_tab_header = &section[header->e_shstrndx];
	const char *section_tab = (const char *)data->mapped_file + section_tab_header->sh_offset;

	// Parse sections symbols;
	int found_symtab = 0;
    int found_strtab = 0;
    int found_text = 0;
    int found_data = 0;
    int found_bss = 0;

	for (uint16_t i = 0; i < header->e_shnum; i++)
	{
        const char *section_name = &section_tab[section[i].sh_name];
		
        if (!found_symtab && strcmp(section_name, ".symtab") == 0)
		{
            printf("Section: .symtab\n");
            found_symtab = 1;
        }
		else if (!found_strtab && strcmp(section_name, ".strtab") == 0)
		{
            printf("Section: .strtab\n");
            found_strtab = 1;
        }
		else if (!found_text && strcmp(section_name, ".text") == 0)
		{
            printf("Section: .text\n");
            found_text = 1;
        }
		else if (!found_data && strcmp(section_name, ".data") == 0)
		{
            printf("Section: .data\n");
            found_data = 1;
        }
		else if (!found_bss && strcmp(section_name, ".bss") == 0)
		{
            printf("Section: .bss\n");
            found_bss = 1;
        }
	}

	if (!(found_symtab && found_strtab && found_text && found_data && found_bss))
	{
		ft_putstr_fd("Invalid ELF file\n", STDERR_FILENO);
		free_all_exit(*data, EXIT_FAILURE);
	}
	
}
